#include "opencl_module_correlator.h"

#include "logger.hpp"
#include "meta/util.hpp"
#include "hardware/device.hpp"

using namespace std;

static std::string collect_build_options(hardware::Device * device, const meta::Inputparameters& params);

static std::string collect_build_options(hardware::Device *, const meta::Inputparameters& params)
{
	std::ostringstream options;

	//CP: give kappa and its negative value
	hmc_float kappa_tmp = params.get_kappa();
	options << "-D KAPPA=" << kappa_tmp;
	options << " -D MKAPPA=" << -kappa_tmp;

	options << " -D NUM_SOURCES=" << params.get_num_sources();

	//CP: give content of sources as compile parameters
	options << " -D SOURCE_CONTENT=" << params.get_sourcecontent();

	return options.str();
}


void Opencl_Module_Correlator::fill_kernels()
{
	basic_correlator_code = get_device()->get_spinor_code()->get_sources() << ClSourcePackage(collect_build_options(get_device(), get_parameters()));
	ClSourcePackage prng_code = get_device()->get_prng_code()->get_sources();

	logger.debug() << "Create correlator kernels...";

	if(get_parameters().get_sourcetype() == meta::Inputparameters::point)
	  create_point_source = createKernel("create_point_source") << basic_correlator_code << prng_code << "spinorfield_point_source.cl";
	else if (get_parameters().get_sourcetype() == meta::Inputparameters::volume)
	  create_volume_source = createKernel("create_volume_source") << basic_correlator_code << prng_code << "spinorfield_volume_source.cl";
	else if (get_parameters().get_sourcetype() == meta::Inputparameters::timeslice)
	  create_timeslice_source = createKernel("create_timeslice_source") << basic_correlator_code << prng_code << "spinorfield_timeslice_source.cl";
	else if (get_parameters().get_sourcetype() == meta::Inputparameters::zslice)
	  create_zslice_source = createKernel("create_zslice_source") << basic_correlator_code << prng_code << "spinorfield_zslice_source.cl";

	if(get_parameters().get_measure_correlators() ){
	  //CP: If a pointsource is chosen, the correlators have a particular simple form. 
	  if(get_parameters().get_sourcetype() == meta::Inputparameters::point){
	    std::string filename_tmp =  "fermionobservables_correlators_point.cl";
	    switch (get_parameters().get_corr_dir()) {
	    case 0 :
	      correlator_ps = createKernel("correlator_ps_t") << basic_correlator_code << filename_tmp;
	      correlator_sc = createKernel("correlator_sc_t") << basic_correlator_code << filename_tmp;
	      correlator_vx = createKernel("correlator_vx_t") << basic_correlator_code << filename_tmp;
	      correlator_vy = createKernel("correlator_vy_t") << basic_correlator_code << filename_tmp;
	      correlator_vz = createKernel("correlator_vz_t") << basic_correlator_code << filename_tmp;
	      correlator_ax = createKernel("correlator_ax_t") << basic_correlator_code << filename_tmp;
	      correlator_ay = createKernel("correlator_ay_t") << basic_correlator_code << filename_tmp;
	      correlator_az = createKernel("correlator_az_t") << basic_correlator_code << filename_tmp;
	      break;
	    case 3 :
	      correlator_ps = createKernel("correlator_ps_z") << basic_correlator_code << filename_tmp;
	      correlator_sc = createKernel("correlator_sc_z") << basic_correlator_code << filename_tmp;
	      correlator_vx = createKernel("correlator_vx_z") << basic_correlator_code << filename_tmp;
	      correlator_vy = createKernel("correlator_vy_z") << basic_correlator_code << filename_tmp;
	      correlator_vz = createKernel("correlator_vz_z") << basic_correlator_code << filename_tmp;
	      correlator_ax = createKernel("correlator_ax_z") << basic_correlator_code << filename_tmp;
	      correlator_ay = createKernel("correlator_ay_z") << basic_correlator_code << filename_tmp;
	      correlator_az = createKernel("correlator_az_z") << basic_correlator_code << filename_tmp;
	      break;
	    default:
	      stringstream errmsg;
	      errmsg << "Could not create correlator kernel as correlator direction " << get_parameters().get_corr_dir() << " has not been implemented.";
	      throw Print_Error_Message(errmsg.str());
	    }
	  } else{
	    std::string filename_tmp =  "fermionobservables_correlators_stochastic.cl";
	    switch (get_parameters().get_corr_dir()) {
	    case 0 :
	      correlator_ps = createKernel("correlator_ps_t") << basic_correlator_code << filename_tmp;
	      correlator_sc = createKernel("correlator_sc_t") << basic_correlator_code << filename_tmp;
	      correlator_vx = createKernel("correlator_vx_t") << basic_correlator_code << filename_tmp;
	      correlator_vy = createKernel("correlator_vy_t") << basic_correlator_code << filename_tmp;
	      correlator_vz = createKernel("correlator_vz_t") << basic_correlator_code << filename_tmp;
	      correlator_ax = createKernel("correlator_ax_t") << basic_correlator_code << filename_tmp;
	      correlator_ay = createKernel("correlator_ay_t") << basic_correlator_code << filename_tmp;
	      correlator_az = createKernel("correlator_az_t") << basic_correlator_code << filename_tmp;
	      break;
	    case 3 :
	      correlator_ps = createKernel("correlator_ps_z") << basic_correlator_code << filename_tmp;
	      correlator_sc = createKernel("correlator_sc_z") << basic_correlator_code << filename_tmp;
	      correlator_vx = createKernel("correlator_vx_z") << basic_correlator_code << filename_tmp;
	      correlator_vy = createKernel("correlator_vy_z") << basic_correlator_code << filename_tmp;
	      correlator_vz = createKernel("correlator_vz_z") << basic_correlator_code << filename_tmp;
	      correlator_ax = createKernel("correlator_ax_z") << basic_correlator_code << filename_tmp;
	      correlator_ay = createKernel("correlator_ay_z") << basic_correlator_code << filename_tmp;
	      correlator_az = createKernel("correlator_az_z") << basic_correlator_code << filename_tmp;
	      break;
	    default:
	      stringstream errmsg;
	      errmsg << "Could not create correlator kernel as correlator direction " << get_parameters().get_corr_dir() << " has not been implemented.";
	      throw Print_Error_Message(errmsg.str());
	    }
	  }
	}
	if(get_parameters().get_measure_pbp() ){
	  pbp_std = createKernel("pbp_std") << basic_correlator_code << "fermionobservables_pbp.cl";
	  pbp_tm_one_end = createKernel("pbp_tm_one_end") << basic_correlator_code << "fermionobservables_pbp.cl";
	}
}

void Opencl_Module_Correlator::clear_kernels()
{
	int clerr = CL_SUCCESS;
	clerr = clReleaseKernel(correlator_ps);
	if(clerr != CL_SUCCESS) throw Opencl_Error(clerr, "clReleaseKernel", __FILE__, __LINE__);
	clerr = clReleaseKernel(correlator_sc);
	if(clerr != CL_SUCCESS) throw Opencl_Error(clerr, "clReleaseKernel", __FILE__, __LINE__);
	clerr = clReleaseKernel(correlator_vx);
	if(clerr != CL_SUCCESS) throw Opencl_Error(clerr, "clReleaseKernel", __FILE__, __LINE__);
	clerr = clReleaseKernel(correlator_vy);
	if(clerr != CL_SUCCESS) throw Opencl_Error(clerr, "clReleaseKernel", __FILE__, __LINE__);
	clerr = clReleaseKernel(correlator_vz);
	if(clerr != CL_SUCCESS) throw Opencl_Error(clerr, "clReleaseKernel", __FILE__, __LINE__);
	clerr = clReleaseKernel(correlator_ax);
	if(clerr != CL_SUCCESS) throw Opencl_Error(clerr, "clReleaseKernel", __FILE__, __LINE__);
	clerr = clReleaseKernel(correlator_ay);
	if(clerr != CL_SUCCESS) throw Opencl_Error(clerr, "clReleaseKernel", __FILE__, __LINE__);
	clerr = clReleaseKernel(correlator_az);
	if(clerr != CL_SUCCESS) throw Opencl_Error(clerr, "clReleaseKernel", __FILE__, __LINE__);
	if(create_point_source) {
		clerr = clReleaseKernel(create_point_source);
		if(clerr != CL_SUCCESS) throw Opencl_Error(clerr, "clReleaseKernel", __FILE__, __LINE__);
	}
	if(create_volume_source) {
		clerr = clReleaseKernel(create_volume_source);
		if(clerr != CL_SUCCESS) throw Opencl_Error(clerr, "clReleaseKernel", __FILE__, __LINE__);
	}
	if(create_timeslice_source) {
		clerr = clReleaseKernel(create_timeslice_source);
		if(clerr != CL_SUCCESS) throw Opencl_Error(clerr, "clReleaseKernel", __FILE__, __LINE__);
	}
	if(create_zslice_source) {
		clerr = clReleaseKernel(create_zslice_source);
		if(clerr != CL_SUCCESS) throw Opencl_Error(clerr, "clReleaseKernel", __FILE__, __LINE__);
	}
	if(pbp_std) {
		clerr = clReleaseKernel(pbp_std);
		if(clerr != CL_SUCCESS) throw Opencl_Error(clerr, "clReleaseKernel", __FILE__, __LINE__);
	}
	if(pbp_tm_one_end) {
		clerr = clReleaseKernel(pbp_tm_one_end);
		if(clerr != CL_SUCCESS) throw Opencl_Error(clerr, "clReleaseKernel", __FILE__, __LINE__);
	}
}

void Opencl_Module_Correlator::get_work_sizes(const cl_kernel kernel, size_t * ls, size_t * gs, cl_uint * num_groups) const
{
	Opencl_Module::get_work_sizes(kernel, ls, gs, num_groups);

	//LZ: should be valid for all kernels for correlators, i.e. for names that look like correlator_??_?
	string kernelname = get_kernel_name(kernel);
	if( kernelname.find("correlator") == 0 ) {
		if(get_device()->get_device_type() == CL_DEVICE_TYPE_GPU) {
			*ls = get_parameters().get_nspace();
			*gs = *ls;
			*num_groups = 1;
		} else {
			*ls = 1;
			*gs = *ls;
			*num_groups = 1;
		}
	}

	return;
}

cl_kernel Opencl_Module_Correlator::get_correlator_kernel(string which)
{
	if( which.compare("ps") == 0 ) {
		return correlator_ps;
	}
	if( which.compare("sc") == 0 ) {
		return correlator_sc;
	}
	if( which.compare("vx") == 0 ) {
		return correlator_vx;
	}
	if( which.compare("vy") == 0 ) {
		return correlator_vy;
	}
	if( which.compare("vz") == 0 ) {
		return correlator_vz;
	}
	if( which.compare("ax") == 0 ) {
		return correlator_ax;
	}
	if( which.compare("ay") == 0 ) {
		return correlator_ay;
	}
	if( which.compare("az") == 0 ) {
		return correlator_az;
	}
	throw Print_Error_Message("get_correlator_kernel failed, no appropriate kernel found");
	return 0;
}

void Opencl_Module_Correlator::create_point_source_device(const hardware::buffers::Plain<spinor> * inout, int i, int spacepos, int timepos)
{
	get_device()->get_spinor_code()->set_zero_spinorfield_device(inout);
	//query work-sizes for kernel
	size_t ls2, gs2;
	cl_uint num_groups;
	this->get_work_sizes(create_point_source, &ls2, &gs2, &num_groups);
	//set arguments
	int clerr = clSetKernelArg(create_point_source, 0, sizeof(cl_mem), inout->get_cl_buffer());
	if(clerr != CL_SUCCESS) throw Opencl_Error(clerr, "clSetKernelArg", __FILE__, __LINE__);

	clerr = clSetKernelArg(create_point_source, 1, sizeof(int), &i);
	if(clerr != CL_SUCCESS) throw Opencl_Error(clerr, "clSetKernelArg", __FILE__, __LINE__);

	clerr = clSetKernelArg(create_point_source, 2, sizeof(int), &spacepos);
	if(clerr != CL_SUCCESS) throw Opencl_Error(clerr, "clSetKernelArg", __FILE__, __LINE__);

	clerr = clSetKernelArg(create_point_source, 3, sizeof(int), &timepos);
	if(clerr != CL_SUCCESS) throw Opencl_Error(clerr, "clSetKernelArg", __FILE__, __LINE__);

	get_device()->enqueue_kernel( create_point_source, gs2, ls2);

	if(logger.beDebug()) {
	  hardware::buffers::Plain<hmc_float> sqn_tmp(1, get_device());
	  hmc_float sqn;
	  get_device()->get_spinor_code()->set_float_to_global_squarenorm_device(inout, &sqn_tmp);
	  sqn_tmp.dump(&sqn);
	  logger.debug() <<  "\t|source|^2:\t" << sqn;
	  if(sqn != sqn) {
	    throw Print_Error_Message("calculation of source gave nan! Aborting...", __FILE__, __LINE__);
	  }
	}

}

void Opencl_Module_Correlator::create_volume_source_device(const hardware::buffers::Plain<spinor> * inout, const hardware::buffers::PRNGBuffer * prng)
{
  get_device()->get_spinor_code()->set_zero_spinorfield_device(inout);
	//query work-sizes for kernel
	size_t ls2, gs2;
	cl_uint num_groups;
	this->get_work_sizes(create_volume_source, &ls2, &gs2, &num_groups);
	//set arguments
	int clerr = clSetKernelArg(create_volume_source, 0, sizeof(cl_mem), inout->get_cl_buffer());
	if(clerr != CL_SUCCESS) throw Opencl_Error(clerr, "clSetKernelArg", __FILE__, __LINE__);

	clerr = clSetKernelArg(create_volume_source, 1, sizeof(cl_mem), prng->get_cl_buffer());
	if(clerr != CL_SUCCESS) throw Opencl_Error(clerr, "clSetKernelArg", __FILE__, __LINE__);

	get_device()->enqueue_kernel( create_volume_source, gs2, ls2);

	if(logger.beDebug()) {
	  hardware::buffers::Plain<hmc_float> sqn_tmp(1, get_device());
	  hmc_float sqn;
	  get_device()->get_spinor_code()->set_float_to_global_squarenorm_device(inout, &sqn_tmp);
	  sqn_tmp.dump(&sqn);
	  logger.debug() <<  "\t|source|^2:\t" << sqn;
	  if(sqn != sqn) {
	    throw Print_Error_Message("calculation of source gave nan! Aborting...", __FILE__, __LINE__);
	  }
	}
}

void Opencl_Module_Correlator::create_timeslice_source_device(const hardware::buffers::Plain<spinor> * inout, const hardware::buffers::PRNGBuffer * prng, const int timeslice)
{
  get_device()->get_spinor_code()->set_zero_spinorfield_device(inout);

	//query work-sizes for kernel
	size_t ls2, gs2;
	cl_uint num_groups;
	this->get_work_sizes(create_timeslice_source, &ls2, &gs2, &num_groups);
	//set arguments
	int clerr = clSetKernelArg(create_timeslice_source, 0, sizeof(cl_mem), inout->get_cl_buffer());
	if(clerr != CL_SUCCESS) throw Opencl_Error(clerr, "clSetKernelArg", __FILE__, __LINE__);

	clerr = clSetKernelArg(create_timeslice_source, 1, sizeof(cl_mem), prng->get_cl_buffer());
	if(clerr != CL_SUCCESS) throw Opencl_Error(clerr, "clSetKernelArg", __FILE__, __LINE__);

	int tmp = timeslice;
	clerr = clSetKernelArg(create_timeslice_source, 2, sizeof(int), &tmp);
	if(clerr != CL_SUCCESS) throw Opencl_Error(clerr, "clSetKernelArg", __FILE__, __LINE__);

	get_device()->enqueue_kernel( create_timeslice_source, gs2, ls2);

	if(logger.beDebug()) {
	  hardware::buffers::Plain<hmc_float> sqn_tmp(1, get_device());
	  hmc_float sqn;
	  get_device()->get_spinor_code()->set_float_to_global_squarenorm_device(inout, &sqn_tmp);
	  sqn_tmp.dump(&sqn);
	  logger.debug() <<  "\t|source|^2:\t" << sqn;
	  if(sqn != sqn) {
	    throw Print_Error_Message("calculation of source gave nan! Aborting...", __FILE__, __LINE__);
	  }
	}
}

void Opencl_Module_Correlator::create_zslice_source_device(const hardware::buffers::Plain<spinor> * inout, const hardware::buffers::PRNGBuffer * prng, const int zslice)
{
  get_device()->get_spinor_code()->set_zero_spinorfield_device(inout);

	//query work-sizes for kernel
	size_t ls2, gs2;
	cl_uint num_groups;
	this->get_work_sizes(create_zslice_source, &ls2, &gs2, &num_groups);
	//set arguments

	int clerr = clSetKernelArg(create_zslice_source, 0, sizeof(cl_mem), inout->get_cl_buffer());
	if(clerr != CL_SUCCESS) throw Opencl_Error(clerr, "clSetKernelArg", __FILE__, __LINE__);

	clerr = clSetKernelArg(create_zslice_source, 1, sizeof(cl_mem), prng->get_cl_buffer());
	if(clerr != CL_SUCCESS) throw Opencl_Error(clerr, "clSetKernelArg", __FILE__, __LINE__);

	int tmp = zslice;
	clerr = clSetKernelArg(create_zslice_source, 2, sizeof(int), &tmp);
	if(clerr != CL_SUCCESS) throw Opencl_Error(clerr, "clSetKernelArg", __FILE__, __LINE__);

	get_device()->enqueue_kernel( create_zslice_source, gs2, ls2);

	if(logger.beDebug()) {
	  hardware::buffers::Plain<hmc_float> sqn_tmp(1, get_device());
	  hmc_float sqn;
	  get_device()->get_spinor_code()->set_float_to_global_squarenorm_device(inout, &sqn_tmp);
	  sqn_tmp.dump(&sqn);
	  logger.debug() <<  "\t|source|^2:\t" << sqn;
	  if(sqn != sqn) {
	    throw Print_Error_Message("calculation of source gave nan! Aborting...", __FILE__, __LINE__);
	  }
	}
}

void Opencl_Module_Correlator::correlator_device(const cl_kernel correlator_kernel, const hardware::buffers::Plain<spinor> * in, const hardware::buffers::Plain<hmc_float> * correlator)
{
	//query work-sizes for kernel
	size_t ls2, gs2;
	cl_uint num_groups;
	this->get_work_sizes(correlator_kernel, &ls2, &gs2, &num_groups);
	//set arguments
	int clerr = clSetKernelArg(correlator_kernel, 0, sizeof(cl_mem), in->get_cl_buffer());
	if(clerr != CL_SUCCESS) throw Opencl_Error(clerr, "clSetKernelArg", __FILE__, __LINE__);
	clerr = clSetKernelArg(correlator_kernel, 1, sizeof(cl_mem), correlator->get_cl_buffer());
	if(clerr != CL_SUCCESS) throw Opencl_Error(clerr, "clSetKernelArg", __FILE__, __LINE__);

	get_device()->enqueue_kernel(correlator_kernel , gs2, ls2);
}

void Opencl_Module_Correlator::correlator_device(const cl_kernel correlator_kernel, const hardware::buffers::Plain<spinor> * in, const hardware::buffers::Plain<spinor> * source, const hardware::buffers::Plain<hmc_float> * correlator)
{
	//query work-sizes for kernel
	size_t ls2, gs2;
	cl_uint num_groups;
	this->get_work_sizes(correlator_kernel, &ls2, &gs2, &num_groups);
	//set arguments
	int clerr = clSetKernelArg(correlator_kernel, 0, sizeof(cl_mem), in->get_cl_buffer());
	if(clerr != CL_SUCCESS) throw Opencl_Error(clerr, "clSetKernelArg", __FILE__, __LINE__);
	clerr = clSetKernelArg(correlator_kernel, 1, sizeof(cl_mem), source->get_cl_buffer());
	if(clerr != CL_SUCCESS) throw Opencl_Error(clerr, "clSetKernelArg", __FILE__, __LINE__);
	clerr = clSetKernelArg(correlator_kernel, 2, sizeof(cl_mem), correlator->get_cl_buffer());
	if(clerr != CL_SUCCESS) throw Opencl_Error(clerr, "clSetKernelArg", __FILE__, __LINE__);

	get_device()->enqueue_kernel(correlator_kernel , gs2, ls2);
}

void Opencl_Module_Correlator::pbp_std_device(const hardware::buffers::Plain<spinor> * in, const hardware::buffers::Plain<spinor> * source, const hardware::buffers::Plain<hmc_float> * correlator)
{
	//query work-sizes for kernel
	size_t ls2, gs2;
	cl_uint num_groups;
	this->get_work_sizes(pbp_std, &ls2, &gs2, &num_groups);
	//set arguments
	int clerr = clSetKernelArg(pbp_std, 0, sizeof(cl_mem), in->get_cl_buffer());
	if(clerr != CL_SUCCESS) throw Opencl_Error(clerr, "clSetKernelArg", __FILE__, __LINE__);
	clerr = clSetKernelArg(pbp_std, 1, sizeof(cl_mem), source->get_cl_buffer());
	if(clerr != CL_SUCCESS) throw Opencl_Error(clerr, "clSetKernelArg", __FILE__, __LINE__);
	clerr = clSetKernelArg(pbp_std, 2, sizeof(cl_mem), correlator->get_cl_buffer());
	if(clerr != CL_SUCCESS) throw Opencl_Error(clerr, "clSetKernelArg", __FILE__, __LINE__);

	get_device()->enqueue_kernel(pbp_std , gs2, ls2);
}

void Opencl_Module_Correlator::pbp_tm_one_end_trick_device(const hardware::buffers::Plain<spinor> * in, const hardware::buffers::Plain<spinor> * source, const hardware::buffers::Plain<hmc_float> * correlator)
{
	//query work-sizes for kernel
	size_t ls2, gs2;
	cl_uint num_groups;
	this->get_work_sizes(pbp_tm_one_end, &ls2, &gs2, &num_groups);
	//set arguments
	int clerr = clSetKernelArg(pbp_tm_one_end, 0, sizeof(cl_mem), in->get_cl_buffer());
	if(clerr != CL_SUCCESS) throw Opencl_Error(clerr, "clSetKernelArg", __FILE__, __LINE__);
	clerr = clSetKernelArg(pbp_tm_one_end, 1, sizeof(cl_mem), source->get_cl_buffer());
	if(clerr != CL_SUCCESS) throw Opencl_Error(clerr, "clSetKernelArg", __FILE__, __LINE__);
	clerr = clSetKernelArg(pbp_tm_one_end, 2, sizeof(cl_mem), correlator->get_cl_buffer());
	if(clerr != CL_SUCCESS) throw Opencl_Error(clerr, "clSetKernelArg", __FILE__, __LINE__);

	get_device()->enqueue_kernel(pbp_tm_one_end , gs2, ls2);
}

size_t Opencl_Module_Correlator::get_read_write_size(const std::string& in) const
{
	//Depending on the compile-options, one has different sizes...
	size_t D = meta::get_float_size(get_parameters());
	//this returns the number of entries in an su3-matrix
	size_t R = meta::get_mat_size(get_parameters());
	size_t S = meta::get_spinorfieldsize(get_parameters());
	size_t Seo = meta::get_eoprec_spinorfieldsize(get_parameters());
	//factor for complex numbers
	int C = 2;
	//this is the same as in the function above
	//NOTE: 1 spinor has NC*NDIM = 12 complex entries
	if (in == "create_point_source") {
		return 1000000000000000000000000;
	}
	if (in == "create_volume_source") {
		return 1000000000000000000000000;
	}
	if (in == "create_timeslice_source") {
		return 1000000000000000000000000;
	}
	if (in == "create_zslice_source") {
		return 1000000000000000000000000;
	}
	if (in == "pbp_std") {
		return 1000000000000000000000000;
	}
	if (in == "pbp_tm_one_end") {
		return 1000000000000000000000000;
	}
	if (in == "correlator_ps_z" ) {
		//this kernel reads NUM_SOURCES spinors and writes NSPACE/NTIME real numbers
		int size_buffer = 0;
		int num_sources = get_parameters().get_num_sources();
		if(get_parameters().get_corr_dir() == 3) size_buffer = get_parameters().get_nspace();
		if(get_parameters().get_corr_dir() == 0) size_buffer = get_parameters().get_ntime();
		return num_sources * S * D * 12 * C + size_buffer * D;
	}
	if (in == "correlator_sc_z") {
		//this kernel reads NUM_SOURCES spinors and writes NSPACE/NTIME real numbers
		int size_buffer = 0;
		int num_sources = get_parameters().get_num_sources();
		if(get_parameters().get_corr_dir() == 3) size_buffer = get_parameters().get_nspace();
		if(get_parameters().get_corr_dir() == 0) size_buffer = get_parameters().get_ntime();
		return num_sources * S * D * 12 * C + size_buffer * D;
	}
	if (in == "correlator_vx_z") {
		//this kernel reads NUM_SOURCES spinors and writes NSPACE/NTIME real numbers
		int size_buffer = 0;
		int num_sources = get_parameters().get_num_sources();
		if(get_parameters().get_corr_dir() == 3) size_buffer = get_parameters().get_nspace();
		if(get_parameters().get_corr_dir() == 0) size_buffer = get_parameters().get_ntime();
		return num_sources * S * D * 12 * C + size_buffer * D;
	}
	if (in == "correlator_vy_z") {
		//this kernel reads NUM_SOURCES spinors and writes NSPACE/NTIME real numbers
		int size_buffer = 0;
		int num_sources = get_parameters().get_num_sources();
		if(get_parameters().get_corr_dir() == 3) size_buffer = get_parameters().get_nspace();
		if(get_parameters().get_corr_dir() == 0) size_buffer = get_parameters().get_ntime();
		return num_sources * S * D * 12 * C + size_buffer * D;
	}
	if (in == "correlator_vz_z") {
		//this kernel reads NUM_SOURCES spinors and writes NSPACE/NTIME real numbers
		int size_buffer = 0;
		int num_sources = get_parameters().get_num_sources();
		if(get_parameters().get_corr_dir() == 3) size_buffer = get_parameters().get_nspace();
		if(get_parameters().get_corr_dir() == 0) size_buffer = get_parameters().get_ntime();
		return num_sources * S * D * 12 * C + size_buffer * D;
	}
	if (in == "correlator_ax_z") {
		//this kernel reads NUM_SOURCES spinors and writes NSPACE/NTIME real numbers
		int size_buffer = 0;
		int num_sources = get_parameters().get_num_sources();
		if(get_parameters().get_corr_dir() == 3) size_buffer = get_parameters().get_nspace();
		if(get_parameters().get_corr_dir() == 0) size_buffer = get_parameters().get_ntime();
		return num_sources * S * D * 12 * C + size_buffer * D;
	}
	if (in == "correlator_ay_z") {
		//this kernel reads NUM_SOURCES spinors and writes NSPACE/NTIME real numbers
		int size_buffer = 0;
		int num_sources = get_parameters().get_num_sources();
		if(get_parameters().get_corr_dir() == 3) size_buffer = get_parameters().get_nspace();
		if(get_parameters().get_corr_dir() == 0) size_buffer = get_parameters().get_ntime();
		return num_sources * S * D * 12 * C + size_buffer * D;
	}
	if (in == "correlator_az_z") {
		//this kernel reads NUM_SOURCES spinors and writes NSPACE/NTIME real numbers
		int size_buffer = 0;
		int num_sources = get_parameters().get_num_sources();
		if(get_parameters().get_corr_dir() == 3) size_buffer = get_parameters().get_nspace();
		if(get_parameters().get_corr_dir() == 0) size_buffer = get_parameters().get_ntime();
		return num_sources * S * D * 12 * C + size_buffer * D;
	}

	return 0;
}

uint64_t Opencl_Module_Correlator::get_flop_size(const std::string& in) const
{
	size_t S = meta::get_spinorfieldsize(get_parameters());
	size_t Seo = meta::get_eoprec_spinorfieldsize(get_parameters());
	//this is the same as in the function above
	if (in == "create_point_source") {
		return 1000000000000000000000000;
	}
	if (in == "create_volume_source") {
		return 1000000000000000000000000;
	}
	if (in == "create_timeslice_source") {
		return 1000000000000000000000000;
	}
	if (in == "create_zslice_source") {
		return 1000000000000000000000000;
	}
	if (in == "correlator_ps_z" ) {
		return 1000000000000000000000000;
	}
	if (in == "correlator_sc_z") {
		return 1000000000000000000000000;
	}
	if (in == "correlator_vx_z") {
		return 1000000000000000000000000;
	}
	if (in == "correlator_vy_z") {
		return 1000000000000000000000000;
	}
	if (in == "correlator_vz_z") {
		return 1000000000000000000000000;
	}
	if (in == "correlator_ax_z") {
		return 1000000000000000000000000;
	}
	if (in == "correlator_ay_z") {
		return 1000000000000000000000000;
	}
	if (in == "correlator_az_z") {
		return 1000000000000000000000000;
	}
	if (in == "pbp_std") {
	  return 1000000000000000000000000000;
	}
	if (in == "pbp_tm_one_end") {
	  return 1000000000000000000000000000;
	}

	return 0;
}

void Opencl_Module_Correlator::print_profiling(const std::string& filename, int number) const
{
	Opencl_Module::print_profiling(filename, number);
	if(create_point_source) {
		Opencl_Module::print_profiling(filename, create_point_source);
	}
	if(create_volume_source) {
		Opencl_Module::print_profiling(filename, create_volume_source);
	}
	if(create_timeslice_source) {
		Opencl_Module::print_profiling(filename, create_timeslice_source);
	}
	if(create_zslice_source) {
		Opencl_Module::print_profiling(filename, create_zslice_source);
	}
	if(correlator_ps)
	  Opencl_Module::print_profiling(filename, correlator_ps);
	if(correlator_sc)
	  Opencl_Module::print_profiling(filename, correlator_sc);
	if(correlator_vx)
	  Opencl_Module::print_profiling(filename, correlator_vx);
	if(correlator_vy)
	  Opencl_Module::print_profiling(filename, correlator_vy);
	if(correlator_vz)
	  Opencl_Module::print_profiling(filename, correlator_vz);
	if(correlator_ax)
	  Opencl_Module::print_profiling(filename, correlator_ax);
	if(correlator_ay)
	  Opencl_Module::print_profiling(filename, correlator_ay);
	if(correlator_az)
	  Opencl_Module::print_profiling(filename, correlator_az);
	if(pbp_std)
	  Opencl_Module::print_profiling(filename, pbp_std);
	if(pbp_tm_one_end)
	  Opencl_Module::print_profiling(filename, pbp_tm_one_end);
}

Opencl_Module_Correlator::Opencl_Module_Correlator(const meta::Inputparameters& params, hardware::Device * device)
	: Opencl_Module(params, device),
	  create_point_source(0), create_volume_source(0), create_timeslice_source(0), create_zslice_source(0),
	  correlator_ps(0), correlator_sc(0), correlator_vx(0), correlator_vy(0), correlator_vz(0), correlator_ax(0), correlator_ay(0), correlator_az(0), pbp_std(0), pbp_tm_one_end(0)
{
	fill_kernels();
}

Opencl_Module_Correlator::~Opencl_Module_Correlator()
{
	clear_kernels();
}
