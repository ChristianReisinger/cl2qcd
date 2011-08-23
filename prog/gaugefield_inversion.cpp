#include "gaugefield_inversion.h"


hmc_error Gaugefield_inversion::init_devices(cl_device_type* devicetypes)
{
	if(get_num_ocl_devices() != 1) {
		//LZ: so far, we only use !!! 1 !!! device
		//this needs generalisation to several devices and subsets!!!!!
		cerr << "only 1 device possible..." << endl;
	}

	if(get_num_ocl_devices() > 0) {
		Opencl_fermions* dev_tmp = new Opencl_fermions[get_num_ocl_devices()];
		alloc_devicetypes();
		set_devices(dev_tmp);
	}

	for(int n = 0; n < get_num_ocl_devices(); n++) {
		logger.trace() << "init device #" << n ;
		get_devices_fermions()[n].init(devicetypes[n], get_parameters());
	}
	return HMC_SUCCESS;
}

hmc_error Gaugefield_inversion::finalize()
{
	hmc_error err = HMC_SUCCESS;
	err |= Gaugefield::finalize();
	for(int n = 0; n < get_num_ocl_devices(); n++)
		err |= get_devices_fermions()[n].finalize_fermions();
	return err;
}

hmc_error Gaugefield_inversion::free_devices()
{
	if(get_num_ocl_devices() > 0)
		delete [] get_devices_fermions();
	return HMC_SUCCESS;
}

Opencl_fermions * Gaugefield_inversion::get_devices_fermions ()
{
	return  (Opencl_fermions*)get_devices();
}

hmc_error Gaugefield_inversion::perform_inversion_pointsource_ps_corr_devices(usetimer* copytimer, usetimer* singletimer, usetimer* solvertimer){
	//this uses a BiCGStab inverter on device

	size_t ls;
	size_t gs;
	cl_uint num_groups;
	//CP: This has no effect yet!!
	char * kernelname = "dummy";
	get_devices_fermions()[0].get_work_sizes(&ls, &gs, &num_groups, get_devices_fermions()[0].get_device_type(), kernelname);	

  /** @todo here one has to introduce more timer instead of noop*/
  usetimer noop;

	int use_eo = get_parameters()->get_use_eo();

  if(use_eo==false){
    get_devices_fermions()[0].set_spinorfield_cold_device(get_devices_fermions()[0].get_clmem_inout(), ls, gs);
  }
  else{
    get_devices_fermions()[0].set_eoprec_spinorfield_cold_device(get_devices_fermions()[0].get_clmem_inout_eoprec(), ls, gs);		
  }

  get_devices_fermions()[0].set_zero_spinorfield_device(get_devices_fermions()[0].get_clmem_corr(), ls, gs);

  for(int k=0; k<12; k++) {
    if(use_eo == false){
      get_devices_fermions()[0].create_point_source_device(get_devices_fermions()[0].get_clmem_source(), k,0,0,ls, gs);
      get_devices_fermions()[0].solver_device(M_call, get_devices_fermions()[0].get_clmem_inout(), get_devices_fermions()[0].get_clmem_source(), get_devices_fermions()[0].get_clmem_gaugefield(), copytimer, singletimer, solvertimer, ls, gs, get_parameters()->get_cgmax());
      //CP: add solution to former ones...
			get_devices_fermions()[0].saxpy_device(get_devices_fermions()[0].get_clmem_inout(), get_devices_fermions()[0].get_clmem_corr(), get_devices_fermions()[0].get_clmem_minusone(), get_devices_fermions()[0].get_clmem_corr(), ls, gs);
    }
    else{
      get_devices_fermions()[0].create_point_source_eoprec_device(get_devices_fermions()[0].get_clmem_source_even(), get_devices_fermions()[0].get_clmem_source_odd(), get_devices_fermions()[0].get_clmem_gaugefield(), k,0,0,ls, gs);
      get_devices_fermions()[0].solver_eoprec_device(Aee_call, get_devices_fermions()[0].get_clmem_inout(), get_devices_fermions()[0].get_clmem_inout_eoprec(), get_devices_fermions()[0].get_clmem_source_even(), get_devices_fermions()[0].get_clmem_source_odd(), get_devices_fermions()[0].get_clmem_gaugefield(), copytimer, singletimer, solvertimer, ls, gs, get_parameters()->get_cgmax());
      //CP: add solution to former ones... This is the same call as without eoprec since the eoprec solver saves the normal field also in clmem_inout!!
      get_devices_fermions()[0].saxpy_device(get_devices_fermions()[0].get_clmem_inout(), get_devices_fermions()[0].get_clmem_corr(), get_devices_fermions()[0].get_clmem_minusone(), get_devices_fermions()[0].get_clmem_corr(), ls, gs);
    }
  }
	
	//CP: this should be called from outside...
  //get_devices_fermions()[0].ps_correlator_device(ls, gs, &noop);

  return HMC_SUCCESS;
}
