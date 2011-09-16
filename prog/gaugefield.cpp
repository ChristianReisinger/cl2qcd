#include "gaugefield.h"

void Gaugefield::init(int numdevs, cl_device_type* devicetypes, inputparameters* input_parameters)
{

	int n_devs[1] = {numdevs};
	init(n_devs, 1, devicetypes, input_parameters);

	return;

}

void Gaugefield::init(int* numdevs, int numdevtypes, cl_device_type* devicetypes, inputparameters* input_parameters)
{
	if(input_parameters->get_use_gpu()) {
		numrndstates = 5120;
	} else {
		numrndstates = 64;
	}
	sizeof_rndarray = sizeof(hmc_ocl_ran) * numrndstates;

	rndarray = new hmc_ocl_ran[numrndstates];


	set_num_device_types(numdevtypes);

	//LZ: for now assume that there is only one num_ocl_devices that is the same for all device types
	//    to be generalized later
	set_num_ocl_devices(numdevs[0]);


	//allocate memory for private gaugefield
	Matrixsu3 * gftmp = new Matrixsu3[NDIM * VOLSPACE * NTIME];
	set_sgf(gftmp);

	set_parameters(input_parameters);

	init_gaugefield();

	this->init_devicetypes_array(devicetypes, input_parameters);

	this->init_devices(devicetypes);

	return;
}

void Gaugefield::init_devicetypes_array(cl_device_type* devicetypes, inputparameters* parameters)
{
	/** @todo work this out! */
	//Check if only one device should be used
	if((*parameters).get_num_dev() == 1) {
		if((*parameters).get_use_gpu() == 1)
			devicetypes[0] = CL_DEVICE_TYPE_GPU;
		else
			devicetypes[0] = CL_DEVICE_TYPE_CPU;
	} else if((*parameters).get_num_dev() == 2) {
		devicetypes[0] = CL_DEVICE_TYPE_GPU;
		devicetypes[1] = CL_DEVICE_TYPE_CPU;
	}
	//So far, more than 3 devices are not supported
	else throw Print_Error_Message("Number of devices too big, aborting...", __FILE__, __LINE__);

	return;
}

void Gaugefield::init_devices(cl_device_type* devicetypes)
{
//  if(get_num_ocl_devices() != 1) {
//    //LZ: so far, we only use !!! 1 !!! device
//    //this needs generalisation to several devices and subsets!!!!!
//    logger.error() << "only 1 device possible...";
//  }

	if(get_num_ocl_devices() > 0) {
		alloc_devicetypes();
		for(int n = 0; n < get_num_device_types(); n++) {
			Opencl* devtmp = new Opencl[get_num_ocl_devices()];
			set_devices(devtmp, n);
		}
	}

	for(int n = 0; n < num_ocl_devices; n++) {
		logger.debug() << "init device #" << n;
		get_devices()[n].init(devicetypes[n], get_parameters(), numrndstates);
	}
	return;
}


void Gaugefield::init_gaugefield()
{

	sourcefileparameters parameters_source;
	if((get_parameters())->get_startcondition() == START_FROM_SOURCE) {
		//hmc_gaugefield for filetransfer, initialize here, because otherwise it is not needed
		hmc_complex* gftmp = new hmc_complex[get_num_hmc_gaugefield_elems()];
		//tmp gauge field
		hmc_float * gaugefield_tmp;
		gaugefield_tmp = (hmc_float*) malloc(sizeof(hmc_float) * NDIM * NC * NC * NTIME * VOLSPACE);
		parameters_source.readsourcefile(&(get_parameters()->sourcefile)[0], get_parameters()->get_prec(), &gaugefield_tmp);
		copy_gaugefield_from_ildg_format(gftmp, gaugefield_tmp, parameters_source.num_entries_source, parameters);
		copy_gaugefield_to_s_gaugefield (get_sgf(), gftmp);
		free(gaugefield_tmp);
		delete[] gftmp;
	}
	if(get_parameters()->get_startcondition() == COLD_START) {
		set_gaugefield_cold_new(get_sgf());
	}
	if(get_parameters()->get_startcondition() == HOT_START) {
		set_gaugefield_hot_new(get_sgf());
	}

	return;
}

void Gaugefield::copy_gaugefield_to_s_gaugefield (Matrixsu3 * sgfo, hmc_complex * gf)
{
	for (int d = 0; d < NDIM; d++) {
		for (int n = 0; n < VOLSPACE; n++) {
			for (int t = 0; t < NTIME; t++) {
				hmc_su3matrix srcElem;
				get_su3matrix(&srcElem, gf, n, t, d);

				Matrixsu3 destElem;
#ifdef _RECONSTRUCT_TWELVE_
				destElem.e00 = srcElem[0];
				destElem.e01 = srcElem[2];
				destElem.e02 = srcElem[4];
				destElem.e10 = srcElem[1];
				destElem.e11 = srcElem[3];
				destElem.e12 = srcElem[5];
#else
				destElem.e00 = srcElem[0][0];
				destElem.e01 = srcElem[0][1];
				destElem.e02 = srcElem[0][2];
				destElem.e10 = srcElem[1][0];
				destElem.e11 = srcElem[1][1];
				destElem.e12 = srcElem[1][2];
				destElem.e20 = srcElem[2][0];
				destElem.e21 = srcElem[2][1];
				destElem.e22 = srcElem[2][2];
#endif
				set_to_gaugefield(sgfo, d, n, t, destElem);
			}
		}
	}
	return;
}

void Gaugefield::copy_s_gaugefield_to_gaugefield(hmc_complex * gf, Matrixsu3 * sgfo)
{
	for (int d = 0; d < NDIM; d++) {
		for (int n = 0; n < VOLSPACE; n++) {
			for (int t = 0; t < NTIME; t++) {
				hmc_su3matrix destElem;
				Matrixsu3 srcElem = get_from_gaugefield(sgfo, d, n, t);
#ifdef _RECONSTRUCT_TWELVE_
				destElem[0] = srcElem.e00;
				destElem[2] = srcElem.e01;
				destElem[4] = srcElem.e02;
				destElem[1] = srcElem.e10;
				destElem[3] = srcElem.e11;
				destElem[5] = srcElem.e12;
#else
				destElem[0][0] = srcElem.e00;
				destElem[0][1] = srcElem.e01;
				destElem[0][2] = srcElem.e02;
				destElem[1][0] = srcElem.e10;
				destElem[1][1] = srcElem.e11;
				destElem[1][2] = srcElem.e12;
				destElem[2][0] = srcElem.e20;
				destElem[2][1] = srcElem.e21;
				destElem[2][2] = srcElem.e22;
#endif
				put_su3matrix(gf, &destElem, n, t, d);
			}
		}
	}
	return;
}

void Gaugefield::set_gaugefield_cold_new (Matrixsu3 * field)
{
	for(int t = 0; t < NTIME; t++) {
		for(int n = 0; n < VOLSPACE; n++) {
			for(int mu = 0; mu < NDIM; mu++) {
				const Matrixsu3 tmp = unit_matrixsu3();
				set_to_gaugefield(field, mu, n, t, tmp);
			}
		}
	}
	return;
}


//Implement this
void Gaugefield::set_gaugefield_hot_new(Matrixsu3 * field)
{
	logger.warn() << "Hot initialization not impelmented – initializing cold.";
	set_gaugefield_cold_new(field);
	return;
}

void Gaugefield::copy_gaugefield_to_devices()
{
	copy_to.reset();
	//LZ: so far, we only use !!! 1 !!! device
	// this function needs to be generalised to several devices and definition of subsets...
	get_devices()[0].copy_gaugefield_to_device(get_sgf());

	copy_to.add();
	return;
}

void Gaugefield::sync_gaugefield()
{
	copy_to.reset();

	//LZ: so far, we only use !!! 1 !!! device
	// this function needs to be generalised to several devices and definition of subsets...
	get_devices()[0].get_gaugefield_from_device(get_sgf());

	copy_to.add();
	return;
}

void Gaugefield::copy_rndarray_to_devices()
{
	copy_to.reset();

	//LZ: so far, we only use !!! 1 !!! device
	// this function needs to be generalised to several devices and definition of subsets...
	get_devices()[0].copy_rndarray_to_device(this->get_rndarray());

	copy_to.add();
	return;
}

void Gaugefield::copy_rndarray_from_devices()
{
	copy_to.reset();
	//LZ: so far, we only use !!! 1 !!! device
	// this function needs to be generalised to several devices and definition of subsets...
	get_devices()[0].copy_rndarray_from_device(this->get_rndarray());

	copy_to.add();
	return;
}

void Gaugefield::print_info_source(sourcefileparameters* params)
{
	logger.info() << "**********************************************************";
	logger.info() << "Sourcefile parameters: (list not complete)";
	logger.info() << "field:   " << params->field_source;
	logger.info() << "LX:      " << params->lx_source;
	logger.info() << "LY:      " << params->ly_source;
	logger.info() << "LZ:      " << params->lz_source;
	logger.info() << "LT:      " << params->lt_source;
	logger.info() << "entries: " << params->num_entries_source;
	logger.info() << "beta:    " << params->beta_source;
	logger.info() << "mu:      " << params->mu_source;
	logger.info() << "kappa:   " << params->kappa_source;
	logger.info() << "mubar:   " << params->mubar_source;
	logger.info() << "plaq:    " << params->plaquettevalue_source;
	logger.info() << "**********************************************************";
	return;
}


void Gaugefield::save(int number)
{
	const int gaugefield_buf_size = 2 * NC * NC * NDIM * VOLSPACE * NTIME;
	hmc_float * gaugefield_buf = new hmc_float[gaugefield_buf_size];

	//these are not yet used...
	hmc_float c2_rec = 0, epsilonbar = 0, mubar = 0;
	//LZ: generalize the following to larger numbers, if necessary...
	stringstream strnumber;
	strnumber.fill('0');
	strnumber.width(5);
	strnumber << right << number;
	stringstream outfilename;
	outfilename << "conf." << strnumber.str();
	string outputfile = outfilename.str();

	hmc_complex* gftmp = new hmc_complex[get_num_hmc_gaugefield_elems()];
	copy_s_gaugefield_to_gaugefield(gftmp, get_sgf());
	copy_gaugefield_to_ildg_format(gaugefield_buf, gftmp);

	hmc_float plaq = plaquette();

	write_gaugefield ( gaugefield_buf, gaugefield_buf_size , NSPACE, NSPACE, NSPACE, NTIME, get_parameters()->get_prec(), number, plaq, get_parameters()->get_beta(), get_parameters()->get_kappa(), get_parameters()->get_mu(), c2_rec, epsilonbar, mubar, version.c_str(), outputfile.c_str());

	delete[] gaugefield_buf;
	delete[] gftmp;

	return;
}
void Gaugefield::print_gaugeobservables(usetimer * timer, usetimer * timer2)
{
	timer->reset();
	hmc_float tplaq = 0;
	hmc_float splaq = 0;
	printf("calling plaquette function...\n");
	hmc_float plaq = plaquette(&tplaq, &splaq);
	timer->add();
	timer2->reset();
	hmc_complex pol = polyakov();
	logger.info() << plaq << '\t' << tplaq << '\t' << splaq << '\t' << pol.re << '\t' << pol.im << '\t' << sqrt(pol.re * pol.re + pol.im * pol.im);
	timer2->add();
	return;
}

void Gaugefield::print_gaugeobservables(usetimer * timer, usetimer * timer2, int iter)
{
	timer->reset();
	hmc_float tplaq = 0;
	hmc_float splaq = 0;
	hmc_float plaq = plaquette(&tplaq, &splaq);
	timer->add();
	timer2->reset();
	hmc_complex pol = polyakov();
	logger.info() << iter << '\t' << plaq << '\t' << tplaq << '\t' << splaq << '\t' << pol.re << '\t' << pol.im << '\t' << sqrt(pol.re * pol.re + pol.im * pol.im);
	timer2->add();
	return;
}

void Gaugefield::print_gaugeobservables(usetimer * timer, usetimer * timer2, int iter, std::string filename)
{
	timer->reset();
	hmc_float tplaq = 0;
	hmc_float splaq = 0;
	hmc_float plaq = plaquette(&tplaq, &splaq);
	timer->add();
	timer2->reset();
	hmc_complex pol = polyakov();
	timer2->add();
	//printf("%d\t%f\t%f\t%f\t%f\t%f\n",iter,plaq,tplaq,splaq,pol.re,pol.im);
	std::fstream gaugeout;
	gaugeout.open(filename.c_str(), std::ios::out | std::ios::app);
	if(!gaugeout.is_open()) throw File_Exception(filename);
	gaugeout.width(8);
	gaugeout << iter;
	gaugeout << "\t";
	gaugeout.precision(15);
	gaugeout << plaq << "\t" << tplaq << "\t" << splaq << "\t" << pol.re << "\t" << pol.im << "\t" << sqrt(pol.re * pol.re + pol.im * pol.im) << std::endl;
	gaugeout.close();
	return;
}

void Gaugefield::print_gaugeobservables(hmc_float plaq, hmc_float tplaq, hmc_float splaq, hmc_complex pol, int iter, std::string filename)
{
	//printf("%d\t%f\t%f\t%f\t%f\t%f\n",iter,plaq,tplaq,splaq,pol.re,pol.im);
	std::fstream gaugeout;
	gaugeout.open(filename.c_str(), std::ios::out | std::ios::app);
	if(!gaugeout.is_open()) throw File_Exception(filename);
	gaugeout.width(8);
	gaugeout << iter;
	gaugeout << "\t";
	gaugeout.precision(15);
	gaugeout << plaq << "\t" << tplaq << "\t" << splaq << "\t" << pol.re << "\t" << pol.im << "\t" << sqrt(pol.re * pol.re + pol.im * pol.im) << std::endl;
	gaugeout.close();
	return;
}

//print to stdout
void Gaugefield::print_gaugeobservables(hmc_float plaq, hmc_float tplaq, hmc_float splaq, hmc_complex pol, int iter)
{
	//printf("%d\t%f\t%f\t%f\t%f\t%f\n",iter,plaq,tplaq,splaq,pol.re,pol.im);
	std::cout.width(8);
	std::cout << iter;
	std::cout << "\t";
	std::cout.precision(15);
	std::cout << plaq << "\t" << tplaq << "\t" << splaq << "\t" << pol.re << "\t" << pol.im << "\t" << sqrt(pol.re * pol.re + pol.im * pol.im) << std::endl;
	return;
}

void Gaugefield::print_gaugeobservables_from_devices(hmc_float * const plaq, hmc_float * const tplaq, hmc_float * const splaq, hmc_complex * const pol, const int i, const string gaugeoutname, int stdout)
{
	//LZ: so far, we only use !!! 1 !!! device
	// this function needs to be generalised to several devices and definition of subsets...

	get_devices()[0].gaugeobservables (get_devices()[0].get_clmem_gaugefield(), plaq, tplaq, splaq, pol);
	print_gaugeobservables(*plaq, *tplaq, *splaq, *pol, i, gaugeoutname);

	//if wanted, this prints the results to the screen
	if(stdout)
		print_gaugeobservables(*plaq, *tplaq, *splaq, *pol, i);
	return;
}

void Gaugefield::print_gaugeobservables_from_devices(const int i, const string gaugeoutname, int stdout)
{
	hmc_float plaq, tplaq, splaq;
	hmc_complex pol;

	print_gaugeobservables_from_devices(&plaq, &tplaq, &splaq, &pol, i, gaugeoutname, stdout);

	return;
}


hmc_float Gaugefield::plaquette()
{
	hmc_float tdummy;
	hmc_float sdummy;
	//LZ: calculation of tdummy and sdummy is unnecessary for this function, chance to speed up a little bit...
	return plaquette(&tdummy, &sdummy);
}

hmc_float Gaugefield::plaquette(hmc_float* tplaq, hmc_float* splaq)
{
	hmc_float plaq = 0;
	*tplaq = 0;
	*splaq = 0;

	//CP: new method that is not working right now since elementary matrix-function using structs are missing on the host
	/*
	Matrixsu3 prod;

	for(int t = 0; t < NTIME; t++) {
	  for(int n = 0; n < VOLSPACE; n++) {
	    for(int mu = 0; mu < NDIM; mu++) {
	  for(int nu = 0; nu < mu; nu++) {
	    prod = local_plaquette(get_sgf(), pos, t, mu, nu );
	    hmc_float tmpfloat = trace_matrixsu3(prod).re;
	    plaq += tmpfloat;
	    if(mu == 0 || nu == 0) {
	          tplaq += tmpfloat;
	    } else {
	    splaq += tmpfloat;
	    }
	  }
	      }
	  }
	}
	*/
	//CP: old method, this should be replaced!!
	//LZ: for now it works because I have inserted the copy_to/from routines...
	//LZ: eventually, someone should implement the "structured operations" for the host

	hmc_complex* gftmp = new hmc_complex[get_num_hmc_gaugefield_elems()];
	copy_s_gaugefield_to_gaugefield(gftmp, get_sgf());


	for(int t = 0; t < NTIME; t++) {
		for(int n = 0; n < VOLSPACE; n++) {
			for(int mu = 0; mu < NDIM; mu++) {
				for(int nu = 0; nu < mu; nu++) {
					hmc_su3matrix prod;
					local_plaquette(gftmp, &prod, n, t, mu, nu );
					hmc_float tmpfloat = trace_su3matrix(&prod).re;
					plaq += tmpfloat;
					if(mu == 0 || nu == 0) {
						*tplaq += tmpfloat;
					} else {
						*splaq += tmpfloat;
					}
				}
			}
		}
	}

	delete[] gftmp;

	*tplaq /= static_cast<hmc_float>(get_parameters()->get_vol4d() * NC * (NDIM - 1));
	*splaq /= static_cast<hmc_float>(get_parameters()->get_vol4d() * NC * (NDIM - 1) * (NDIM - 2)) / 2. ;
	return plaq * 2.0 / static_cast<hmc_float>(get_parameters()->get_vol4d() * NDIM * (NDIM - 1) * NC);
}


hmc_complex Gaugefield::polyakov()
{

	hmc_complex* gftmp = new hmc_complex[get_num_hmc_gaugefield_elems()];
	copy_s_gaugefield_to_gaugefield(gftmp, get_sgf());

	hmc_complex res;
	res.re = 0;
	res.im = 0;
	for(int n = 0; n < VOLSPACE; n++) {
		hmc_su3matrix prod;
		local_polyakov(gftmp, &prod, n);
		hmc_complex tmpcomplex = trace_su3matrix(&prod);
		complexaccumulate(&res, &tmpcomplex);
	}

	delete[] gftmp;

	res.re /= static_cast<hmc_float>(NC * VOLSPACE);
	res.im /= static_cast<hmc_float>(NC * VOLSPACE);
	return res;
}


hmc_complex Gaugefield::spatial_polyakov(int dir)
{

	hmc_complex* gftmp = new hmc_complex[get_num_hmc_gaugefield_elems()];
	copy_s_gaugefield_to_gaugefield(gftmp, get_sgf());

	//assuming dir=1,2, or 3
	hmc_complex res;
	res.re = 0;
	res.im = 0;
	for(int x1 = 0; x1 < NSPACE; x1++) {
		for(int x2 = 0; x2 < NSPACE; x2++) {
			for(int t = 0; t < NTIME; t++) {
				hmc_su3matrix prod;
				unit_su3matrix(&prod);
				for(int xpol = 0; xpol < NSPACE; xpol++) {
					hmc_su3matrix tmp;
					int coord[NDIM];
					coord[0] = t;
					coord[dir] = xpol;
					int next = (dir % (NDIM - 1)) + 1;
					coord[next] = x1;
					int nnext = (next % (NDIM - 1)) + 1;
					coord[nnext] = x2;
					int pos = get_nspace(coord);
					get_su3matrix(&tmp, gftmp, pos, t, dir);
					accumulate_su3matrix_prod(&prod, &tmp);
				}
				hmc_complex tmpcomplex = trace_su3matrix(&prod);
				complexaccumulate(&res, &tmpcomplex);
			}
		}
	}

	delete[] gftmp;

	res.re /= static_cast<hmc_float>(NC * NSPACE * NSPACE * NTIME);
	res.im /= static_cast<hmc_float>(NC * NSPACE * NSPACE * NTIME);
	return res;
}


void Gaugefield::finalize()
{
	delete[] get_sgf();
	delete[] get_rndarray();
	this->free_devices();
	return;
}

void Gaugefield::free_devices()
{
	if(get_num_device_types() == 1) {
		if(get_num_ocl_devices() > 0)
			delete [] get_devices();
	} else {
		for(int i = 0; i < get_num_device_types(); i++) {
			if(get_num_ocl_devices() > 0)
				delete [] get_devices(i);
		}
		delete [] devices;
	}
	return;
}

void Gaugefield::set_devices (Opencl * devices_val)
{
	set_devices(devices_val, 0);
	return;
}

void Gaugefield::set_devices (Opencl * devices_val, int i)
{
	if(get_num_device_types() == 1) {
		devices[0] = devices_val;
	} else {
		devices[i] = devices_val;
	}
	return;
}

Opencl * Gaugefield::get_devices ()
{
	return get_devices(0);
}

Opencl * Gaugefield::get_devices (int i)
{
	if(i >= get_num_device_types() || i < 0) {
		logger.warn() << "get_devices: no pointer to that device number";
		return NULL;
	}
	return devices[i];
}


void Gaugefield::set_num_ocl_devices (int num)
{
	num_ocl_devices = num;
	return;
}

int Gaugefield::get_num_ocl_devices ()
{
	return num_ocl_devices;
}


void Gaugefield::set_num_device_types (int num)
{
	num_device_types = num;
	return;
}

int Gaugefield::get_num_device_types ()
{
	return num_device_types;
}


void Gaugefield::set_parameters (inputparameters * parameters_val)
{
	parameters = parameters_val;
	return;
}

inputparameters * Gaugefield::get_parameters ()
{
	return  parameters;
}



Matrixsu3 * Gaugefield::get_sgf ()
{
	return sgf;
}

void Gaugefield::set_sgf (Matrixsu3 * sgf_val)
{
	sgf = sgf_val;
	return;
}


void Gaugefield::alloc_devicetypes()
{
	devices = new Opencl* [this->get_num_device_types()];
	return;
}


hmc_ocl_ran* Gaugefield::get_rndarray()
{
	return rndarray;
}

size_t Gaugefield::get_numrndstates()
{
	return numrndstates;
}

usetimer * Gaugefield::get_copy_on()
{
	return &copy_on;
}

usetimer * Gaugefield::get_copy_to()
{
	return &copy_to;
}

size_t Gaugefield::get_num_hmc_gaugefield_elems()
{
#ifdef _RECONSTRUCT_TWELVE_
	return NC * (NC - 1) * NDIM * VOLSPACE * NTIME;
#else
	return NC * NC * NDIM * VOLSPACE * NTIME;
#endif
}

void Gaugefield::set_to_gaugefield(Matrixsu3 * field, const size_t mu, const size_t x, const size_t t, const Matrixsu3 val)
{
	field[get_global_link_pos(mu, x, t)] = val;
}

Matrixsu3 Gaugefield::get_from_gaugefield(const Matrixsu3 * field, const size_t mu, const size_t x, const size_t t) const
{
	return field[get_global_link_pos(mu, x, t)];
}
