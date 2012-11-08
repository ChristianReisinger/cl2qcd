#include "fermions.hpp"

#include "../../logger.hpp"
#include "../../meta/util.hpp"
#include "../device.hpp"

using namespace std;

static std::string collect_build_options(hardware::Device * device, const meta::Inputparameters& params);

/**
 * What follows are functions that call opencl_fermions-class-functions.
 * This is needed to be able to pass different fermionmatrices as
 *  arguments to class-functions.
 */
void hardware::code::M::operator()(const hardware::buffers::Plain<spinor> * in, const hardware::buffers::Plain<spinor> * out, const hardware::buffers::SU3 * gf, hmc_float kappa, hmc_float mubar) const
{
	that->M(in, out, gf, kappa, mubar);
}
cl_ulong hardware::code::M::get_Flops() const
{
	switch(that->get_parameters().get_fermact()) {
		case meta::Inputparameters::wilson:
			return that->get_flop_size("M_wilson");
		case meta::Inputparameters::twistedmass:
			return that->get_flop_size("M_tm_plus");
		default:
			throw Invalid_Parameters("Unkown fermion action!", "wilson or meta::Inputparameters::twistedmass", that->get_parameters().get_fermact());
	}
}
cl_ulong hardware::code::M::get_Bytes() const
{
	switch(that->get_parameters().get_fermact()) {
		case meta::Inputparameters::wilson:
			return that->get_read_write_size("M_wilson");
		case meta::Inputparameters::twistedmass:
			return that->get_read_write_size("M_tm_plus");
		default:
			throw Invalid_Parameters("Unkown fermion action!", "wilson or meta::Inputparameters::twistedmass", that->get_parameters().get_fermact());
	}
}

void hardware::code::Qplus::operator()(const hardware::buffers::Plain<spinor> * in, const hardware::buffers::Plain<spinor> * out, const hardware::buffers::SU3 * gf, hmc_float kappa, hmc_float mubar) const
{
	that->Qplus(in, out, gf, kappa, mubar);
}
cl_ulong hardware::code::Qplus::get_Flops() const
{
	cl_ulong res;
	switch(that->get_parameters().get_fermact()) {
		case meta::Inputparameters::wilson:
			res = that->get_flop_size("M_wilson");
			break;
		case meta::Inputparameters::twistedmass:
			res = that->get_flop_size("M_tm_plus");
			break;
		default:
			throw Invalid_Parameters("Unkown fermion action!", "wilson or meta::Inputparameters::twistedmass", that->get_parameters().get_fermact());
	}
	res += that->get_flop_size("gamma5");
	return res;
}
cl_ulong hardware::code::Qplus::get_Bytes() const
{
	cl_ulong res;
	switch(that->get_parameters().get_fermact()) {
		case meta::Inputparameters::wilson:
			res = that->get_read_write_size("M_wilson");
			break;
		case meta::Inputparameters::twistedmass:
			res = that->get_read_write_size("M_tm_plus");
			break;
		default:
			throw Invalid_Parameters("Unkown fermion action!", "wilson or meta::Inputparameters::twistedmass", that->get_parameters().get_fermact());
	}
	res += that->get_read_write_size("gamma5");
	return res;
}

void hardware::code::Qminus::operator()(const hardware::buffers::Plain<spinor> * in, const hardware::buffers::Plain<spinor> * out, const hardware::buffers::SU3 * gf, hmc_float kappa, hmc_float mubar) const
{
	that->Qminus(in, out, gf, kappa, mubar);
}
cl_ulong hardware::code::Qminus::get_Flops() const
{
	cl_ulong res;
	switch(that->get_parameters().get_fermact()) {
		case meta::Inputparameters::wilson:
			res = that->get_flop_size("M_wilson");
			break;
		case meta::Inputparameters::twistedmass:
			res = that->get_flop_size("M_tm_minus");
			break;
		default:
			throw Invalid_Parameters("Unkown fermion action!", "wilson or meta::Inputparameters::twistedmass", that->get_parameters().get_fermact());
	}
	res += that->get_flop_size("gamma5");
	return res;
}
cl_ulong hardware::code::Qminus::get_Bytes() const
{
	cl_ulong res;
	switch(that->get_parameters().get_fermact()) {
		case meta::Inputparameters::wilson:
			res = that->get_read_write_size("M_wilson");
			break;
		case meta::Inputparameters::twistedmass:
			res = that->get_read_write_size("M_tm_minus");
			break;
		default:
			throw Invalid_Parameters("Unkown fermion action!", "wilson or meta::Inputparameters::twistedmass", that->get_parameters().get_fermact());
	}
	res += that->get_read_write_size("gamma5");
	return res;
}

void hardware::code::QplusQminus::operator()(const hardware::buffers::Plain<spinor> * in, const hardware::buffers::Plain<spinor> * out, const hardware::buffers::SU3 * gf, hmc_float kappa, hmc_float mubar) const
{
	that->QplusQminus(in, out, gf, kappa, mubar);
}
cl_ulong hardware::code::QplusQminus::get_Flops() const
{
	cl_ulong res;
	switch(that->get_parameters().get_fermact()) {
		case meta::Inputparameters::wilson:
			res = 2 * that->get_flop_size("M_wilson");
			break;
		case meta::Inputparameters::twistedmass:
			res = that->get_flop_size("M_tm_plus");
			res += that->get_flop_size("M_tm_minus");
			break;
		default:
			throw Invalid_Parameters("Unkown fermion action!", "wilson or meta::Inputparameters::twistedmass", that->get_parameters().get_fermact());
	}
	res += 2 * that->get_flop_size("gamma5");
	return res;
}
cl_ulong hardware::code::QplusQminus::get_Bytes() const
{
	cl_ulong res;
	switch(that->get_parameters().get_fermact()) {
		case meta::Inputparameters::wilson:
			res = 2 * that->get_read_write_size("M_wilson");
			break;
		case meta::Inputparameters::twistedmass:
			res = that->get_read_write_size("M_tm_plus");
			res += that->get_read_write_size("M_tm_minus");
			break;
		default:
			throw Invalid_Parameters("Unkown fermion action!", "wilson or meta::Inputparameters::twistedmass", that->get_parameters().get_fermact());
	}
	res += 2 * that->get_read_write_size("gamma5");
	return res;
}

void hardware::code::Aee::operator()(const hardware::buffers::Spinor * in, const hardware::buffers::Spinor * out, const hardware::buffers::SU3 * gf, hmc_float kappa, hmc_float mubar) const
{
	that->Aee(in, out, gf, kappa, mubar);
}
cl_ulong hardware::code::Aee::get_Flops() const
{
	auto spinor_code = that->get_device()->get_spinor_code();

	cl_ulong res;
	switch(that->get_parameters().get_fermact()) {
		case meta::Inputparameters::wilson:
			res = 2 * that->get_flop_size("dslash_eo");
			res += spinor_code->get_flop_size("saxpy_eoprec");
			break;
		case meta::Inputparameters::twistedmass:
			res = 2 * that->get_flop_size("dslash_eo");
			res += that->get_flop_size("M_tm_inverse_sitediagonal");
			res += that->get_flop_size("M_tm_sitediagonal");
			res += spinor_code->get_flop_size("saxpy_eoprec");
			break;
		default:
			throw Invalid_Parameters("Unkown fermion action!", "wilson or meta::Inputparameters::twistedmass", that->get_parameters().get_fermact());
	}
	return res;
}
cl_ulong hardware::code::Aee::get_Bytes() const
{
	auto spinor_code = that->get_device()->get_spinor_code();

	cl_ulong res;
	switch(that->get_parameters().get_fermact()) {
		case meta::Inputparameters::wilson:
			res = 2 * that->get_read_write_size("dslash_eo");
			res += spinor_code->get_read_write_size("saxpy_eoprec");
			break;
		case meta::Inputparameters::twistedmass:
			res = 2 * that->get_read_write_size("dslash_eo");
			res += that->get_read_write_size("M_tm_inverse_sitediagonal");
			res += that->get_read_write_size("M_tm_sitediagonal");
			res += spinor_code->get_read_write_size("saxpy_eoprec");
			break;
		default:
			throw Invalid_Parameters("Unkown fermion action!", "wilson or meta::Inputparameters::twistedmass", that->get_parameters().get_fermact());
	}
	return res;
}

void hardware::code::Qplus_eo::operator()(const hardware::buffers::Spinor * in, const hardware::buffers::Spinor * out, const hardware::buffers::SU3 * gf, hmc_float kappa, hmc_float mubar) const
{
	that->Qplus_eo(in, out, gf, kappa, mubar);
}
cl_ulong hardware::code::Qplus_eo::get_Flops() const
{
	auto spinor_code = that->get_device()->get_spinor_code();

	cl_ulong res;
	switch(that->get_parameters().get_fermact()) {
		case meta::Inputparameters::wilson:
			res = 2 * that->get_flop_size("dslash_eo");
			res += spinor_code->get_flop_size("saxpy_eoprec");
			break;
		case meta::Inputparameters::twistedmass:
			res = 2 * that->get_flop_size("dslash_eo");
			res += that->get_flop_size("M_tm_inverse_sitediagonal");
			res += that->get_flop_size("M_tm_sitediagonal");
			res += spinor_code->get_flop_size("saxpy_eoprec");
			break;
		default:
			throw Invalid_Parameters("Unkown fermion action!", "wilson or meta::Inputparameters::twistedmass", that->get_parameters().get_fermact());
	}
	res += that->get_flop_size("gamma5_eo");
	return res;
}
cl_ulong hardware::code::Qplus_eo::get_Bytes() const
{
	auto spinor_code = that->get_device()->get_spinor_code();

	cl_ulong res;
	switch(that->get_parameters().get_fermact()) {
		case meta::Inputparameters::wilson:
			res = 2 * that->get_read_write_size("dslash_eo");
			res += spinor_code->get_read_write_size("saxpy_eoprec");
			break;
		case meta::Inputparameters::twistedmass:
			res = 2 * that->get_read_write_size("dslash_eo");
			res += that->get_read_write_size("M_tm_inverse_sitediagonal");
			res += that->get_read_write_size("M_tm_sitediagonal");
			res += spinor_code->get_read_write_size("saxpy_eoprec");
			break;
		default:
			throw Invalid_Parameters("Unkown fermion action!", "wilson or meta::Inputparameters::twistedmass", that->get_parameters().get_fermact());
	}
	res += that->get_flop_size("gamma5_eo");
	return res;
}

void hardware::code::Qminus_eo::operator()(const hardware::buffers::Spinor * in, const hardware::buffers::Spinor * out, const hardware::buffers::SU3 * gf, hmc_float kappa, hmc_float mubar) const
{
	that->Qminus_eo(in, out, gf, kappa, mubar);
}
cl_ulong hardware::code::Qminus_eo::get_Flops() const
{
	auto spinor_code = that->get_device()->get_spinor_code();

	cl_ulong res;
	switch(that->get_parameters().get_fermact()) {
		case meta::Inputparameters::wilson:
			res = 2 * that->get_flop_size("dslash_eo");
			res += spinor_code->get_flop_size("saxpy_eoprec");
			break;
		case meta::Inputparameters::twistedmass:
			res = 2 * that->get_flop_size("dslash_eo");
			res += that->get_flop_size("M_tm_inverse_sitediagonal_minus");
			res += that->get_flop_size("M_tm_sitediagonal_minus");
			res += spinor_code->get_flop_size("saxpy_eoprec");
			break;
		default:
			throw Invalid_Parameters("Unkown fermion action!", "wilson or meta::Inputparameters::twistedmass", that->get_parameters().get_fermact());
	}
	res += that->get_flop_size("gamma5_eo");
	return res;
}
cl_ulong hardware::code::Qminus_eo::get_Bytes() const
{
	auto spinor_code = that->get_device()->get_spinor_code();

	cl_ulong res;
	switch(that->get_parameters().get_fermact()) {
		case meta::Inputparameters::wilson:
			res = 2 * that->get_read_write_size("dslash_eo");
			res += spinor_code->get_read_write_size("saxpy_eoprec");
			break;
		case meta::Inputparameters::twistedmass:
			res = 2 * that->get_read_write_size("dslash_eo");
			res += that->get_read_write_size("M_tm_inverse_sitediagonal_minus");
			res += that->get_read_write_size("M_tm_sitediagonal_minus");
			res += spinor_code->get_read_write_size("saxpy_eoprec");
			break;
		default:
			throw Invalid_Parameters("Unkown fermion action!", "wilson or meta::Inputparameters::twistedmass", that->get_parameters().get_fermact());
	}
	res += that->get_flop_size("gamma5_eo");
	return res;
}

void hardware::code::QplusQminus_eo::operator()(const hardware::buffers::Spinor * in, const hardware::buffers::Spinor * out, const hardware::buffers::SU3 * gf, hmc_float kappa, hmc_float mubar) const
{
	that->QplusQminus_eo(in, out, gf, kappa, mubar);
}
cl_ulong hardware::code::QplusQminus_eo::get_Flops() const
{
	auto spinor_code = that->get_device()->get_spinor_code();

	cl_ulong res;
	switch(that->get_parameters().get_fermact()) {
		case meta::Inputparameters::wilson:
			res = 2 * that->get_flop_size("dslash_eo");
			res += spinor_code->get_flop_size("saxpy_eoprec");
			res *= 2;
			break;
		case meta::Inputparameters::twistedmass:
			res = 4 * that->get_flop_size("dslash_eo");
			res += that->get_flop_size("M_tm_inverse_sitediagonal");
			res += that->get_flop_size("M_tm_sitediagonal");
			res += that->get_flop_size("M_tm_inverse_sitediagonal_minus");
			res += that->get_flop_size("M_tm_sitediagonal_minus");
			res += 2 * spinor_code->get_flop_size("saxpy_eoprec");
			break;
		default:
			throw Invalid_Parameters("Unkown fermion action!", "wilson or meta::Inputparameters::twistedmass", that->get_parameters().get_fermact());
	}
	res += 2 * that->get_flop_size("gamma5_eo");
	return res;
}
cl_ulong hardware::code::QplusQminus_eo::get_Bytes() const
{
	auto spinor_code = that->get_device()->get_spinor_code();

	cl_ulong res;
	switch(that->get_parameters().get_fermact()) {
		case meta::Inputparameters::wilson:
			res = 2 * that->get_read_write_size("dslash_eo");
			res += spinor_code->get_read_write_size("saxpy_eoprec");
			res *= 2;
			break;
		case meta::Inputparameters::twistedmass:
			res = 4 * that->get_read_write_size("dslash_eo");
			res += that->get_read_write_size("M_tm_inverse_sitediagonal");
			res += that->get_read_write_size("M_tm_sitediagonal");
			res += that->get_read_write_size("M_tm_inverse_sitediagonal_minus");
			res += that->get_read_write_size("M_tm_sitediagonal_minus");
			res += 2 * spinor_code->get_read_write_size("saxpy_eoprec");
			break;
		default:
			throw Invalid_Parameters("Unkown fermion action!", "wilson or meta::Inputparameters::twistedmass", that->get_parameters().get_fermact());
	}
	res += 2 * that->get_flop_size("gamma5_eo");
	return res;
}


static std::string collect_build_options(hardware::Device *, const meta::Inputparameters& params)
{
	std::ostringstream options;

	switch (params.get_fermact()) {
		case meta::Inputparameters::twistedmass :
			options << "-D _TWISTEDMASS_";
			break;
		case meta::Inputparameters::clover :
			options << "-D _CLOVER_";
			break;
	}

	//CP: These are the BCs in spatial and temporal direction
	hmc_float tmp_spatial = (params.get_theta_fermion_spatial() * PI) / ( (hmc_float) params.get_nspace());
	hmc_float tmp_temporal = (params.get_theta_fermion_temporal() * PI) / ( (hmc_float) params.get_ntime());
	//BC: on the corners in each direction: exp(i theta) -> on each site exp(i theta*PI /LATEXTENSION) = cos(tmp2) + isin(tmp2)
	options << " -D SPATIAL_RE=" << cos(tmp_spatial);
	options << " -D MSPATIAL_RE=" << -cos(tmp_spatial);
	options << " -D SPATIAL_IM=" << sin(tmp_spatial);
	options << " -D MSPATIAL_IM=" << -sin(tmp_spatial);

	options << " -D TEMPORAL_RE=" << cos(tmp_temporal);
	options << " -D MTEMPORAL_RE=" << -cos(tmp_temporal);
	options << " -D TEMPORAL_IM=" << sin(tmp_temporal);
	options << " -D MTEMPORAL_IM=" << -sin(tmp_temporal);

	return options.str();
}

void hardware::code::Fermions::fill_kernels()
{
	sources = get_device()->get_spinor_code()->get_sources() << ClSourcePackage(collect_build_options(get_device(), get_parameters()));

	M_wilson = 0;
	M_tm_plus = 0;
	M_tm_minus = 0;

	logger.debug() << "Create fermion kernels...";
	if(get_parameters().get_fermact() == meta::Inputparameters::wilson) {
		M_wilson = createKernel("M_wilson") << sources << "fermionmatrix.cl" << "fermionmatrix_m.cl";
	} else if(get_parameters().get_fermact() == meta::Inputparameters::twistedmass) {
		M_tm_plus = createKernel("M_tm_plus") << sources << "fermionmatrix.cl" << "fermionmatrix_m_tm_plus.cl";
		M_tm_minus = createKernel("M_tm_minus") << sources << "fermionmatrix.cl" << "fermionmatrix_m_tm_minus.cl";
	} else if(get_parameters().get_fermact() == meta::Inputparameters::clover) {
		throw Print_Error_Message("no kernels for CLOVER-discretization implemented yet, aborting... ", __FILE__, __LINE__);
	} else {
		throw Print_Error_Message("there was a problem with which fermion-discretization to use, aborting... ", __FILE__, __LINE__);
	}

	gamma5 = createKernel("gamma5") << sources << "fermionmatrix.cl" << "fermionmatrix_gamma5.cl";


	//Kernels needed if eoprec is used
	if(get_parameters().get_use_eo() == true) {
		if(get_parameters().get_fermact() == meta::Inputparameters::twistedmass) {
			M_tm_sitediagonal = createKernel("M_tm_sitediagonal") << sources << "fermionmatrix.cl" << "fermionmatrix_eo.cl" << "fermionmatrix_eo_m.cl";
			M_tm_inverse_sitediagonal = createKernel("M_tm_inverse_sitediagonal") << sources << "fermionmatrix.cl" << "fermionmatrix_eo.cl" << "fermionmatrix_eo_m.cl";
			M_tm_sitediagonal_minus = createKernel("M_tm_sitediagonal_minus") << sources << "fermionmatrix.cl" << "fermionmatrix_eo.cl" << "fermionmatrix_eo_m.cl";
			M_tm_inverse_sitediagonal_minus = createKernel("M_tm_inverse_sitediagonal_minus") << sources << "fermionmatrix.cl" << "fermionmatrix_eo.cl" << "fermionmatrix_eo_m.cl";
		}
		dslash_eo = createKernel("dslash_eo") << sources << "fermionmatrix.cl" << "fermionmatrix_eo.cl" << "fermionmatrix_eo_dslash.cl";
		gamma5_eo = createKernel("gamma5_eo") << sources << "fermionmatrix.cl" << "fermionmatrix_eo_gamma5.cl";
		//merged kernels
		if (get_parameters().get_use_merge_kernels_fermion() == true) {
			dslash_AND_gamma5_eo = createKernel("dslash_AND_gamma5_eo") << sources << "fermionmatrix.cl" << "fermionmatrix_eo.cl" << "fermionmatrix_eo_dslash_AND_gamma5.cl";
			dslash_AND_M_tm_inverse_sitediagonal_eo = createKernel("dslash_AND_M_tm_inverse_sitediagonal_eo") << sources << "fermionmatrix.cl" << "fermionmatrix_eo.cl" << "fermionmatrix_eo_dslash_AND_M_tm_inverse_sitediagonal.cl";
			dslash_AND_M_tm_inverse_sitediagonal_minus_eo = createKernel("dslash_AND_M_tm_inverse_sitediagonal_minus_eo") << sources << "fermionmatrix.cl" << "fermionmatrix_eo.cl" << "fermionmatrix_eo_dslash_AND_M_tm_inverse_sitediagonal_minus.cl";
			M_tm_sitediagonal_AND_gamma5_eo = createKernel("M_tm_sitediagonal_AND_gamma5_eo") << sources << "fermionmatrix.cl" << "fermionmatrix_eo.cl" << "fermionmatrix_eo_m_merged.cl";
			M_tm_sitediagonal_minus_AND_gamma5_eo = createKernel("M_tm_sitediagonal_minus_AND_gamma5_eo") << sources << "fermionmatrix.cl" << "fermionmatrix_eo.cl" << "fermionmatrix_eo_m_merged.cl";
		}

	}


	return;
}

void hardware::code::Fermions::clear_kernels()
{
	logger.trace() << "clearing fermion kernels...";
	cl_uint clerr = CL_SUCCESS;

	if(M_wilson) {
		clerr = clReleaseKernel(M_wilson);
		if(clerr != CL_SUCCESS) throw Opencl_Error(clerr, "clReleaseKernel", __FILE__, __LINE__);
	}
	if(M_tm_plus) {
		clerr = clReleaseKernel(M_tm_plus);
		if(clerr != CL_SUCCESS) throw Opencl_Error(clerr, "clReleaseKernel", __FILE__, __LINE__);
	}
	if(M_tm_minus) {
		clerr = clReleaseKernel(M_tm_minus);
		if(clerr != CL_SUCCESS) throw Opencl_Error(clerr, "clReleaseKernel", __FILE__, __LINE__);
	}
	if(gamma5) {
		clerr = clReleaseKernel(gamma5);
		if(clerr != CL_SUCCESS) throw Opencl_Error(clerr, "clReleaseKernel", __FILE__, __LINE__);
	}

	if(get_parameters().get_use_eo()) {
		clerr = clReleaseKernel(dslash_eo);
		if(clerr != CL_SUCCESS) throw Opencl_Error(clerr, "clReleaseKernel", __FILE__, __LINE__);
		clerr = clReleaseKernel(gamma5_eo);
		if(clerr != CL_SUCCESS) throw Opencl_Error(clerr, "clReleaseKernel", __FILE__, __LINE__);
		if(get_parameters().get_fermact() == meta::Inputparameters::twistedmass) {
			clerr = clReleaseKernel(M_tm_sitediagonal);
			if(clerr != CL_SUCCESS) throw Opencl_Error(clerr, "clReleaseKernel", __FILE__, __LINE__);
			clerr = clReleaseKernel(M_tm_inverse_sitediagonal);
			if(clerr != CL_SUCCESS) throw Opencl_Error(clerr, "clReleaseKernel", __FILE__, __LINE__);
			clerr = clReleaseKernel(M_tm_sitediagonal_minus);
			if(clerr != CL_SUCCESS) throw Opencl_Error(clerr, "clReleaseKernel", __FILE__, __LINE__);
			clerr = clReleaseKernel(M_tm_inverse_sitediagonal_minus);
			if(clerr != CL_SUCCESS) throw Opencl_Error(clerr, "clReleaseKernel", __FILE__, __LINE__);
		}
	}
}

void hardware::code::Fermions::get_work_sizes(const cl_kernel kernel, size_t * ls, size_t * gs, cl_uint * num_groups) const
{
	Opencl_Module::get_work_sizes(kernel, ls, gs, num_groups);
}


//compound fermionmatrix-functions without eoprec
void hardware::code::Fermions::M(const hardware::buffers::Plain<spinor> * in, const hardware::buffers::Plain<spinor> * out, const hardware::buffers::SU3 * gf, hmc_float kappa , hmc_float mubar )
{

	if(get_parameters().get_fermact() == meta::Inputparameters::wilson) {
		//in the pure Wilson case there is just one fermionmatrix
		M_wilson_device(in, out, gf, kappa);
	} else if(get_parameters().get_fermact() == meta::Inputparameters::twistedmass) {
		M_tm_plus_device(in, out, gf, kappa, mubar);
	}
}

void hardware::code::Fermions::Qplus(const hardware::buffers::Plain<spinor> * in, const hardware::buffers::Plain<spinor> * out, const hardware::buffers::SU3 * gf, hmc_float kappa , hmc_float mubar )
{
	if(get_parameters().get_fermact() == meta::Inputparameters::wilson) {
		//in the pure Wilson case there is just one fermionmatrix
		M_wilson_device(in, out, gf, kappa);
	} else if(get_parameters().get_fermact() == meta::Inputparameters::twistedmass) {
		M_tm_plus_device(in, out, gf, kappa, mubar);
	}
	gamma5_device(out);
}

void hardware::code::Fermions::Qminus(const hardware::buffers::Plain<spinor> * in, const hardware::buffers::Plain<spinor> * out, const hardware::buffers::SU3 * gf, hmc_float kappa , hmc_float mubar )
{
	if(get_parameters().get_fermact() == meta::Inputparameters::wilson) {
		//in the pure Wilson case there is just one fermionmatrix
		M_wilson_device(in, out, gf, kappa);
	} else if(get_parameters().get_fermact() == meta::Inputparameters::twistedmass) {
		M_tm_minus_device(in, out, gf, kappa, mubar);
	}
	gamma5_device(out);
}

void hardware::code::Fermions::QplusQminus(const hardware::buffers::Plain<spinor> * in, const hardware::buffers::Plain<spinor> * out, const hardware::buffers::SU3 * gf, hmc_float kappa , hmc_float mubar )
{
	/** @todo one could save one field here if an additional copying would be included in the end...
	 * or the field should be created in here, local */
	Qminus(in, &clmem_tmp, gf, kappa, mubar);
	Qplus(&clmem_tmp, out, gf, kappa, mubar);
}

//explicit fermionmatrix-kernel calling functions
void hardware::code::Fermions::M_wilson_device(const hardware::buffers::Plain<spinor> * in, const hardware::buffers::Plain<spinor> * out, const hardware::buffers::SU3 * gf, hmc_float kappa)
{
	//get kappa
	hmc_float kappa_tmp;
	if(kappa == ARG_DEF) kappa_tmp = get_parameters().get_kappa();
	else kappa_tmp = kappa;

	//query work-sizes for kernel
	size_t ls2, gs2;
	cl_uint num_groups;
	this->get_work_sizes(M_wilson, &ls2, &gs2, &num_groups);
	//set arguments
	int clerr = clSetKernelArg(M_wilson, 0, sizeof(cl_mem), in->get_cl_buffer());
	if(clerr != CL_SUCCESS) throw Opencl_Error(clerr, "clSetKernelArg", __FILE__, __LINE__);

	clerr = clSetKernelArg(M_wilson, 1, sizeof(cl_mem), gf->get_cl_buffer());
	if(clerr != CL_SUCCESS) throw Opencl_Error(clerr, "clSetKernelArg", __FILE__, __LINE__);

	clerr = clSetKernelArg(M_wilson, 2, sizeof(cl_mem), out->get_cl_buffer());
	if(clerr != CL_SUCCESS) throw Opencl_Error(clerr, "clSetKernelArg", __FILE__, __LINE__);

	clerr = clSetKernelArg(M_wilson, 3, sizeof(hmc_float), &kappa_tmp);
	if(clerr != CL_SUCCESS) throw Opencl_Error(clerr, "clSetKernelArg", __FILE__, __LINE__);

	get_device()->enqueue_kernel( M_wilson, gs2, ls2);
}

void hardware::code::Fermions::M_tm_plus_device(const hardware::buffers::Plain<spinor> * in, const hardware::buffers::Plain<spinor> * out, const hardware::buffers::SU3 * gf, hmc_float kappa , hmc_float mubar )
{
	//get kappa
	hmc_float kappa_tmp;
	if(kappa == ARG_DEF) kappa_tmp = get_parameters().get_kappa();
	else kappa_tmp = kappa;

	//get mu
	hmc_float mubar_tmp;
	if(mubar == ARG_DEF) mubar_tmp = meta::get_mubar(get_parameters());
	else mubar_tmp = mubar;

	//query work-sizes for kernel
	size_t ls2, gs2;
	cl_uint num_groups;
	this->get_work_sizes(M_tm_plus, &ls2, &gs2, &num_groups);
	//set arguments
	int clerr = clSetKernelArg(M_tm_plus, 0, sizeof(cl_mem), in->get_cl_buffer());
	if(clerr != CL_SUCCESS) throw Opencl_Error(clerr, "clSetKernelArg", __FILE__, __LINE__);

	clerr = clSetKernelArg(M_tm_plus, 1, sizeof(cl_mem), gf->get_cl_buffer());
	if(clerr != CL_SUCCESS) throw Opencl_Error(clerr, "clSetKernelArg", __FILE__, __LINE__);

	clerr = clSetKernelArg(M_tm_plus, 2, sizeof(cl_mem), out->get_cl_buffer());
	if(clerr != CL_SUCCESS) throw Opencl_Error(clerr, "clSetKernelArg", __FILE__, __LINE__);

	clerr = clSetKernelArg(M_tm_plus, 3, sizeof(hmc_float), &kappa_tmp);
	if(clerr != CL_SUCCESS) throw Opencl_Error(clerr, "clSetKernelArg", __FILE__, __LINE__);

	clerr = clSetKernelArg(M_tm_plus, 4, sizeof(hmc_float), &mubar_tmp);
	if(clerr != CL_SUCCESS) throw Opencl_Error(clerr, "clSetKernelArg", __FILE__, __LINE__);

	get_device()->enqueue_kernel( M_tm_plus, gs2, ls2);
}

void hardware::code::Fermions::M_tm_minus_device(const hardware::buffers::Plain<spinor> * in, const hardware::buffers::Plain<spinor> * out, const hardware::buffers::SU3 * gf, hmc_float kappa , hmc_float mubar )
{
	//get kappa
	hmc_float kappa_tmp;
	if(kappa == ARG_DEF) kappa_tmp = get_parameters().get_kappa();
	else kappa_tmp = kappa;

	//get mu
	hmc_float mubar_tmp;
	if(mubar == ARG_DEF) mubar_tmp = meta::get_mubar(get_parameters());
	else mubar_tmp = mubar;

	//query work-sizes for kernel
	size_t ls2, gs2;
	cl_uint num_groups;
	this->get_work_sizes(M_tm_minus, &ls2, &gs2, &num_groups);
	//set arguments
	int clerr = clSetKernelArg(M_tm_minus, 0, sizeof(cl_mem), in->get_cl_buffer());
	if(clerr != CL_SUCCESS) throw Opencl_Error(clerr, "clSetKernelArg", __FILE__, __LINE__);

	clerr = clSetKernelArg(M_tm_minus, 1, sizeof(cl_mem), gf->get_cl_buffer());
	if(clerr != CL_SUCCESS) throw Opencl_Error(clerr, "clSetKernelArg", __FILE__, __LINE__);

	clerr = clSetKernelArg(M_tm_minus, 2, sizeof(cl_mem), out->get_cl_buffer());
	if(clerr != CL_SUCCESS) throw Opencl_Error(clerr, "clSetKernelArg", __FILE__, __LINE__);

	clerr = clSetKernelArg(M_tm_minus, 3, sizeof(hmc_float), &kappa_tmp);
	if(clerr != CL_SUCCESS) throw Opencl_Error(clerr, "clSetKernelArg", __FILE__, __LINE__);

	clerr = clSetKernelArg(M_tm_minus, 4, sizeof(hmc_float), &mubar_tmp);
	if(clerr != CL_SUCCESS) throw Opencl_Error(clerr, "clSetKernelArg", __FILE__, __LINE__);

	get_device()->enqueue_kernel( M_tm_minus, gs2, ls2);
}

void hardware::code::Fermions::gamma5_device(const hardware::buffers::Plain<spinor> * inout)
{
	//query work-sizes for kernel
	size_t ls2, gs2;
	cl_uint num_groups;
	this->get_work_sizes(gamma5, &ls2, &gs2, &num_groups);
	//set arguments
	int clerr = clSetKernelArg(gamma5, 0, sizeof(cl_mem), inout->get_cl_buffer());
	if(clerr != CL_SUCCESS) throw Opencl_Error(clerr, "clSetKernelArg", __FILE__, __LINE__);

	get_device()->enqueue_kernel(gamma5 , gs2, ls2);
}

//compound fermionmatrix-functions with eoprec
void hardware::code::Fermions::Aee(const hardware::buffers::Spinor * in, const hardware::buffers::Spinor * out, const hardware::buffers::SU3 * gf, hmc_float kappa , hmc_float mubar )
{
	int even = EVEN;
	int odd = ODD;

	auto spinor_code = get_device()->get_spinor_code();

	/**
	 * This is the even-odd preconditioned fermion matrix with the
	 * non-trivial inversion on the even sites (see DeGran/DeTar p. 174).
	 * If one has fermionmatrix
	 *  M = R + D,
	 * then Aee is:
	 * Aee = R_e - D_eo R_o_inv D_oe
	 */
	if(get_parameters().get_fermact() == meta::Inputparameters::wilson) {
		//in this case, the diagonal matrix is just 1 and falls away.
		dslash_eo_device(in, &clmem_tmp_eo_1, gf, odd, kappa);
		dslash_eo_device(&clmem_tmp_eo_1, out, gf, even, kappa);
		spinor_code->saxpy_eoprec_device(out, in, &clmem_one, out);
	} else if(get_parameters().get_fermact() == meta::Inputparameters::twistedmass) {
		dslash_eo_device(in, &clmem_tmp_eo_1, gf, odd, kappa);
		M_tm_inverse_sitediagonal_device(&clmem_tmp_eo_1, &clmem_tmp_eo_2, mubar);
		dslash_eo_device(&clmem_tmp_eo_2, out, gf, even, kappa);
		M_tm_sitediagonal_device(in, &clmem_tmp_eo_1, mubar);
		spinor_code->saxpy_eoprec_device(out, &clmem_tmp_eo_1, &clmem_one, out);
	}
}

/**
 *  This is the equivalent of the above function, but for the lower
 *  flavour, which essentially means mu -> -mu in the tm-case and
 *  no changes in the meta::Inputparameters::wilson case.
 */
void hardware::code::Fermions::Aee_minus(const hardware::buffers::Spinor * in, const hardware::buffers::Spinor * out, const hardware::buffers::SU3 * gf, hmc_float kappa , hmc_float mubar )
{
	int even = EVEN;
	int odd = ODD;

	auto spinor_code = get_device()->get_spinor_code();

	/**
	 * This is the even-odd preconditioned fermion matrix with the
	 * non-trivial inversion on the even sites (see DeGran/DeTar p. 174).
	 * If one has fermionmatrix
	 *  M = R + D,
	 * then Aee is:
	 * Aee = R_e - D_eo R_o_inv D_oe
	 * and Aee_minus is:
	 * Aee_minus = R_e(-mu) - D_eo R_o(-mu)_inv D_oe
	 */
	if(get_parameters().get_fermact() == meta::Inputparameters::wilson) {
		//in this case, the diagonal matrix is just 1 and falls away.
		dslash_eo_device(in, &clmem_tmp_eo_1, gf, odd, kappa);
		dslash_eo_device(&clmem_tmp_eo_1, out, gf, even, kappa);
		spinor_code->saxpy_eoprec_device(out, in, &clmem_one, out);
	} else if(get_parameters().get_fermact() == meta::Inputparameters::twistedmass) {
		dslash_eo_device(in, &clmem_tmp_eo_1, gf, odd, kappa);
		M_tm_inverse_sitediagonal_minus_device(&clmem_tmp_eo_1, &clmem_tmp_eo_2, mubar);
		dslash_eo_device(&clmem_tmp_eo_2, out, gf, even, kappa);
		M_tm_sitediagonal_minus_device(in, &clmem_tmp_eo_1, mubar);
		spinor_code->saxpy_eoprec_device(out, &clmem_tmp_eo_1, &clmem_one, out);
	}
}

//merged fermionmatrix-functions with eoprec
void hardware::code::Fermions::Aee_AND_gamma5_eo(const hardware::buffers::Spinor * in, const hardware::buffers::Spinor * out, const hardware::buffers::SU3 * gf, hmc_float kappa , hmc_float mubar )
{
	int even = EVEN;
	int odd = ODD;

	auto spinor_code = get_device()->get_spinor_code();

	/**
	 * This is the even-odd preconditioned fermion matrix with the
	 * non-trivial inversion on the even sites (see DeGran/DeTar p. 174).
	 * If one has fermionmatrix
	 *  M = R + D,
	 * then Aee is:
	 * Aee = R_e - D_eo R_o_inv D_oe
	 */
	if(get_parameters().get_fermact() == meta::Inputparameters::wilson) {
		//in this case, the diagonal matrix is just 1 and falls away.
		//this case has not been adjusted for the merged kernels yet...
		logger.warn() << "No merged kernels implemented for pure Wilson case!";
		dslash_eo_device(in, &clmem_tmp_eo_1, gf, odd, kappa);
		dslash_eo_device(&clmem_tmp_eo_1, out, gf, even, kappa);
		spinor_code->saxpy_eoprec_device(out, in, &clmem_one, out);
		gamma5_eo_device(out);
	} else if(get_parameters().get_fermact() == meta::Inputparameters::twistedmass) {
		/*
		dslash_eo_device(in, &clmem_tmp_eo_1, gf, odd, kappa);
		M_tm_inverse_sitediagonal_device(&clmem_tmp_eo_1, &clmem_tmp_eo_2, mubar);
		*/
		dslash_AND_M_tm_inverse_sitediagonal_eo_device(in, &clmem_tmp_eo_2, gf, odd, kappa, mubar);
		/*
		dslash_eo_device(&clmem_tmp_eo_2, out, gf, even, kappa);
		gamma5_eo_device(out);
		*/
		dslash_AND_gamma5_eo_device(&clmem_tmp_eo_2, out, gf, even, kappa);
		/*
		M_tm_sitediagonal_device(in, &clmem_tmp_eo_1, mubar);
		gamma5_eo_device(&clmem_tmp_eo_1);
		*/
		M_tm_sitediagonal_AND_gamma5_eo_device(in, &clmem_tmp_eo_1, mubar);
		spinor_code->saxpy_eoprec_device(out, &clmem_tmp_eo_1, &clmem_one, out);
	}
}

/**
 *  This is the equivalent of the above function, but for the lower
 *  flavour, which essentially means mu -> -mu in the tm-case and
 *  no changes in the meta::Inputparameters::wilson case.
 */
void hardware::code::Fermions::Aee_minus_AND_gamma5_eo(const hardware::buffers::Spinor * in, const hardware::buffers::Spinor * out, const hardware::buffers::SU3 * gf, hmc_float kappa , hmc_float mubar )
{
	int even = EVEN;
	int odd = ODD;

	auto spinor_code = get_device()->get_spinor_code();

	/**
	 * This is the even-odd preconditioned fermion matrix with the
	 * non-trivial inversion on the even sites (see DeGran/DeTar p. 174).
	 * If one has fermionmatrix
	 *  M = R + D,
	 * then Aee is:
	 * Aee = R_e - D_eo R_o_inv D_oe
	 */
	if(get_parameters().get_fermact() == meta::Inputparameters::wilson) {
		//in this case, the diagonal matrix is just 1 and falls away.
		//this case has not been adjusted for the merged kernels yet...
		logger.warn() << "No merged kernels implemented for pure Wilson case!";
		dslash_eo_device(in, &clmem_tmp_eo_1, gf, odd, kappa);
		dslash_eo_device(&clmem_tmp_eo_1, out, gf, even, kappa);
		spinor_code->saxpy_eoprec_device(out, in, &clmem_one, out);
		gamma5_eo_device(out);
	} else if(get_parameters().get_fermact() == meta::Inputparameters::twistedmass) {
		/*
		dslash_eo_device(in, &clmem_tmp_eo_1, gf, odd, kappa);
		M_tm_inverse_sitediagonal_minus_device(&clmem_tmp_eo_1, &clmem_tmp_eo_2, mubar);
		*/
		dslash_AND_M_tm_inverse_sitediagonal_minus_eo_device(in, &clmem_tmp_eo_2, gf, odd, kappa, mubar);
		/*
		dslash_eo_device(&clmem_tmp_eo_2, out, gf, even, kappa);
		gamma5_eo_device(out);
		*/
		dslash_AND_gamma5_eo_device(&clmem_tmp_eo_2, out, gf, even, kappa);
		/*
		M_tm_sitediagonal_minus_device(in, &clmem_tmp_eo_1, mubar);
		gamma5_eo_device(&clmem_tmp_eo_1);
		*/
		M_tm_sitediagonal_minus_AND_gamma5_eo_device(in, &clmem_tmp_eo_1, mubar);
		spinor_code->saxpy_eoprec_device(out, &clmem_tmp_eo_1, &clmem_one, out);
	}
}


void hardware::code::Fermions::Qplus_eo(const hardware::buffers::Spinor * in, const hardware::buffers::Spinor * out, const hardware::buffers::SU3 * gf, hmc_float kappa , hmc_float mubar )
{
	if(get_parameters().get_use_merge_kernels_fermion() == false) {
		Aee(in, out, gf, kappa, mubar);
		gamma5_eo_device(out);
	} else {
		Aee_AND_gamma5_eo(in, out, gf, kappa, mubar);
	}
	return;
}

void hardware::code::Fermions::Qminus_eo(const hardware::buffers::Spinor * in, const hardware::buffers::Spinor * out, const hardware::buffers::SU3 * gf, hmc_float kappa , hmc_float mubar )
{
	if(get_parameters().get_use_merge_kernels_fermion() == false) {
		Aee_minus(in, out, gf, kappa, mubar);
		gamma5_eo_device(out);
	} else {
		Aee_minus_AND_gamma5_eo(in, out, gf, kappa, mubar);
	}
	return;
}

void hardware::code::Fermions::QplusQminus_eo(const hardware::buffers::Spinor * in, const hardware::buffers::Spinor * out, const hardware::buffers::SU3 * gf, hmc_float kappa , hmc_float mubar )
{
	//CP: this should be an eoprec-sized field. However, this induces problems in the CG algorithm!!!
	//MB: This is because of padding, the eoprec buffer size shoulw always be queried from Opencl_Module_Spinor
	hardware::buffers::Spinor sf_eo_tmp(in->get_elements(), get_device());

	Qminus_eo(in, &sf_eo_tmp, gf, kappa, mubar);
	Qplus_eo(&sf_eo_tmp, out, gf, kappa, mubar);
}

//explicit eoprec fermionmatrix functions
void hardware::code::Fermions::gamma5_eo_device(const hardware::buffers::Spinor * inout)
{
	//query work-sizes for kernel
	size_t ls2, gs2;
	cl_uint num_groups;
	this->get_work_sizes(gamma5_eo, &ls2, &gs2, &num_groups);
	//set arguments
	int clerr = clSetKernelArg(gamma5_eo, 0, sizeof(cl_mem), inout->get_cl_buffer());
	if(clerr != CL_SUCCESS) throw Opencl_Error(clerr, "clSetKernelArg", __FILE__, __LINE__);

	get_device()->enqueue_kernel( gamma5_eo, gs2, ls2);
}

void hardware::code::Fermions::dslash_eo_device(const hardware::buffers::Spinor * in, const hardware::buffers::Spinor * out, const hardware::buffers::SU3 * gf, int evenodd, hmc_float kappa)
{
	//get kappa
	hmc_float kappa_tmp;
	if(kappa == ARG_DEF) kappa_tmp = get_parameters().get_kappa();
	else kappa_tmp = kappa;

	cl_int eo = evenodd;
	//query work-sizes for kernel
	size_t ls2, gs2;
	cl_uint num_groups;
	this->get_work_sizes(dslash_eo, &ls2, &gs2, &num_groups);
	//set arguments
	int clerr = clSetKernelArg(dslash_eo, 0, sizeof(cl_mem), in->get_cl_buffer());
	if(clerr != CL_SUCCESS) throw Opencl_Error(clerr, "clSetKernelArg", __FILE__, __LINE__);

	clerr = clSetKernelArg(dslash_eo, 1, sizeof(cl_mem), out->get_cl_buffer());
	if(clerr != CL_SUCCESS) throw Opencl_Error(clerr, "clSetKernelArg", __FILE__, __LINE__);

	clerr = clSetKernelArg(dslash_eo, 2, sizeof(cl_mem), gf->get_cl_buffer());
	if(clerr != CL_SUCCESS) throw Opencl_Error(clerr, "clSetKernelArg", __FILE__, __LINE__);

	clerr = clSetKernelArg(dslash_eo, 3, sizeof(cl_int), &eo);
	if(clerr != CL_SUCCESS) throw Opencl_Error(clerr, "clSetKernelArg", __FILE__, __LINE__);

	clerr = clSetKernelArg(dslash_eo, 4, sizeof(hmc_float), &kappa_tmp);
	if(clerr != CL_SUCCESS) throw Opencl_Error(clerr, "clSetKernelArg", __FILE__, __LINE__);

	get_device()->enqueue_kernel(dslash_eo , gs2, ls2);
}

void hardware::code::Fermions::dslash_AND_gamma5_eo_device(const hardware::buffers::Spinor * in, const hardware::buffers::Spinor * out, const hardware::buffers::SU3 * gf, int evenodd, hmc_float kappa)
{
	//get kappa
	hmc_float kappa_tmp;
	if(kappa == ARG_DEF) kappa_tmp = get_parameters().get_kappa();
	else kappa_tmp = kappa;

	cl_int eo = evenodd;
	//query work-sizes for kernel
	size_t ls2, gs2;
	cl_uint num_groups;
	this->get_work_sizes(dslash_AND_gamma5_eo, &ls2, &gs2, &num_groups);
	//set arguments
	int clerr = clSetKernelArg(dslash_AND_gamma5_eo, 0, sizeof(cl_mem), in->get_cl_buffer());
	if(clerr != CL_SUCCESS) throw Opencl_Error(clerr, "clSetKernelArg", __FILE__, __LINE__);

	clerr = clSetKernelArg(dslash_AND_gamma5_eo, 1, sizeof(cl_mem), out->get_cl_buffer());
	if(clerr != CL_SUCCESS) throw Opencl_Error(clerr, "clSetKernelArg", __FILE__, __LINE__);

	clerr = clSetKernelArg(dslash_AND_gamma5_eo, 2, sizeof(cl_mem), gf->get_cl_buffer());
	if(clerr != CL_SUCCESS) throw Opencl_Error(clerr, "clSetKernelArg", __FILE__, __LINE__);

	clerr = clSetKernelArg(dslash_AND_gamma5_eo, 3, sizeof(cl_int), &eo);
	if(clerr != CL_SUCCESS) throw Opencl_Error(clerr, "clSetKernelArg", __FILE__, __LINE__);

	clerr = clSetKernelArg(dslash_AND_gamma5_eo, 4, sizeof(hmc_float), &kappa_tmp);
	if(clerr != CL_SUCCESS) throw Opencl_Error(clerr, "clSetKernelArg", __FILE__, __LINE__);

	get_device()->enqueue_kernel(dslash_AND_gamma5_eo , gs2, ls2);
}

void hardware::code::Fermions::dslash_AND_M_tm_inverse_sitediagonal_eo_device(const hardware::buffers::Spinor * in, const hardware::buffers::Spinor * out, const hardware::buffers::SU3 * gf, int evenodd, hmc_float kappa, hmc_float mubar)
{
	//get kappa
	hmc_float kappa_tmp, mubar_tmp;
	if(kappa == ARG_DEF) kappa_tmp = get_parameters().get_kappa();
	else kappa_tmp = kappa;
	if(mubar == ARG_DEF) mubar_tmp = meta::get_mubar(get_parameters());
	else mubar_tmp = kappa;

	cl_int eo = evenodd;
	//query work-sizes for kernel
	size_t ls2, gs2;
	cl_uint num_groups;
	this->get_work_sizes(dslash_AND_M_tm_inverse_sitediagonal_eo, &ls2, &gs2, &num_groups);
	//set arguments
	int clerr = clSetKernelArg(dslash_AND_M_tm_inverse_sitediagonal_eo, 0, sizeof(cl_mem), in->get_cl_buffer());
	if(clerr != CL_SUCCESS) throw Opencl_Error(clerr, "clSetKernelArg", __FILE__, __LINE__);

	clerr = clSetKernelArg(dslash_AND_M_tm_inverse_sitediagonal_eo, 1, sizeof(cl_mem), out->get_cl_buffer());
	if(clerr != CL_SUCCESS) throw Opencl_Error(clerr, "clSetKernelArg", __FILE__, __LINE__);

	clerr = clSetKernelArg(dslash_AND_M_tm_inverse_sitediagonal_eo, 2, sizeof(cl_mem), gf->get_cl_buffer());
	if(clerr != CL_SUCCESS) throw Opencl_Error(clerr, "clSetKernelArg", __FILE__, __LINE__);

	clerr = clSetKernelArg(dslash_AND_M_tm_inverse_sitediagonal_eo, 3, sizeof(cl_int), &eo);
	if(clerr != CL_SUCCESS) throw Opencl_Error(clerr, "clSetKernelArg", __FILE__, __LINE__);

	clerr = clSetKernelArg(dslash_AND_M_tm_inverse_sitediagonal_eo, 4, sizeof(hmc_float), &kappa_tmp);
	if(clerr != CL_SUCCESS) throw Opencl_Error(clerr, "clSetKernelArg", __FILE__, __LINE__);

	clerr = clSetKernelArg(dslash_AND_M_tm_inverse_sitediagonal_eo, 5, sizeof(hmc_float), &mubar_tmp);
	if(clerr != CL_SUCCESS) throw Opencl_Error(clerr, "clSetKernelArg", __FILE__, __LINE__);

	get_device()->enqueue_kernel(dslash_AND_M_tm_inverse_sitediagonal_eo , gs2, ls2);
}

void hardware::code::Fermions::dslash_AND_M_tm_inverse_sitediagonal_minus_eo_device(const hardware::buffers::Spinor * in, const hardware::buffers::Spinor * out, const hardware::buffers::SU3 * gf, int evenodd, hmc_float kappa, hmc_float mubar)
{
	//get kappa
	hmc_float kappa_tmp, mubar_tmp;
	if(kappa == ARG_DEF) kappa_tmp = get_parameters().get_kappa();
	else kappa_tmp = kappa;
	if(mubar == ARG_DEF) mubar_tmp = meta::get_mubar(get_parameters());
	else mubar_tmp = kappa;

	cl_int eo = evenodd;
	//query work-sizes for kernel
	size_t ls2, gs2;
	cl_uint num_groups;
	this->get_work_sizes(dslash_AND_M_tm_inverse_sitediagonal_minus_eo, &ls2, &gs2, &num_groups);
	//set arguments
	int clerr = clSetKernelArg(dslash_AND_M_tm_inverse_sitediagonal_minus_eo, 0, sizeof(cl_mem), in->get_cl_buffer());
	if(clerr != CL_SUCCESS) throw Opencl_Error(clerr, "clSetKernelArg", __FILE__, __LINE__);

	clerr = clSetKernelArg(dslash_AND_M_tm_inverse_sitediagonal_minus_eo, 1, sizeof(cl_mem), out->get_cl_buffer());
	if(clerr != CL_SUCCESS) throw Opencl_Error(clerr, "clSetKernelArg", __FILE__, __LINE__);

	clerr = clSetKernelArg(dslash_AND_M_tm_inverse_sitediagonal_minus_eo, 2, sizeof(cl_mem), gf->get_cl_buffer());
	if(clerr != CL_SUCCESS) throw Opencl_Error(clerr, "clSetKernelArg", __FILE__, __LINE__);

	clerr = clSetKernelArg(dslash_AND_M_tm_inverse_sitediagonal_minus_eo, 3, sizeof(cl_int), &eo);
	if(clerr != CL_SUCCESS) throw Opencl_Error(clerr, "clSetKernelArg", __FILE__, __LINE__);

	clerr = clSetKernelArg(dslash_AND_M_tm_inverse_sitediagonal_minus_eo, 4, sizeof(hmc_float), &kappa_tmp);
	if(clerr != CL_SUCCESS) throw Opencl_Error(clerr, "clSetKernelArg", __FILE__, __LINE__);

	clerr = clSetKernelArg(dslash_AND_M_tm_inverse_sitediagonal_minus_eo, 5, sizeof(hmc_float), &mubar_tmp);
	if(clerr != CL_SUCCESS) throw Opencl_Error(clerr, "clSetKernelArg", __FILE__, __LINE__);

	get_device()->enqueue_kernel(dslash_AND_M_tm_inverse_sitediagonal_minus_eo , gs2, ls2);
}

void hardware::code::Fermions::M_tm_inverse_sitediagonal_device(const hardware::buffers::Spinor * in, const hardware::buffers::Spinor * out, hmc_float mubar)
{
	//get mu
	hmc_float mubar_tmp;
	if(mubar == ARG_DEF) mubar_tmp = meta::get_mubar(get_parameters());
	else mubar_tmp = mubar;

	//query work-sizes for kernel
	size_t ls2, gs2;
	cl_uint num_groups;
	this->get_work_sizes(M_tm_inverse_sitediagonal, &ls2, &gs2, &num_groups);
	//set arguments
	int clerr = clSetKernelArg(M_tm_inverse_sitediagonal, 0, sizeof(cl_mem), in->get_cl_buffer());
	if(clerr != CL_SUCCESS) throw Opencl_Error(clerr, "clSetKernelArg", __FILE__, __LINE__);

	clerr = clSetKernelArg(M_tm_inverse_sitediagonal, 1, sizeof(cl_mem), out->get_cl_buffer());
	if(clerr != CL_SUCCESS) throw Opencl_Error(clerr, "clSetKernelArg", __FILE__, __LINE__);

	clerr = clSetKernelArg(M_tm_inverse_sitediagonal, 2, sizeof(hmc_float), &mubar_tmp);
	if(clerr != CL_SUCCESS) throw Opencl_Error(clerr, "clSetKernelArg", __FILE__, __LINE__);

	get_device()->enqueue_kernel( M_tm_inverse_sitediagonal, gs2, ls2);
}

void hardware::code::Fermions::M_tm_sitediagonal_device(const hardware::buffers::Spinor * in, const hardware::buffers::Spinor * out, hmc_float mubar)
{
	//get mu
	hmc_float mubar_tmp;
	if(mubar == ARG_DEF) mubar_tmp = meta::get_mubar(get_parameters());
	else mubar_tmp = mubar;

	//query work-sizes for kernel
	size_t ls2, gs2;
	cl_uint num_groups;
	this->get_work_sizes(M_tm_sitediagonal, &ls2, &gs2, &num_groups);
	//set arguments
	int clerr = clSetKernelArg(M_tm_sitediagonal, 0, sizeof(cl_mem), in->get_cl_buffer());
	if(clerr != CL_SUCCESS) throw Opencl_Error(clerr, "clSetKernelArg", __FILE__, __LINE__);

	clerr = clSetKernelArg(M_tm_sitediagonal, 1, sizeof(cl_mem), out->get_cl_buffer());
	if(clerr != CL_SUCCESS) throw Opencl_Error(clerr, "clSetKernelArg", __FILE__, __LINE__);

	clerr = clSetKernelArg(M_tm_sitediagonal, 2, sizeof(hmc_float), &mubar_tmp);
	if(clerr != CL_SUCCESS) throw Opencl_Error(clerr, "clSetKernelArg", __FILE__, __LINE__);

	get_device()->enqueue_kernel(M_tm_sitediagonal , gs2, ls2);
}

void hardware::code::Fermions::M_tm_sitediagonal_AND_gamma5_eo_device(const hardware::buffers::Spinor * in, const hardware::buffers::Spinor * out, hmc_float mubar)
{
	//get mu
	hmc_float mubar_tmp;
	if(mubar == ARG_DEF) mubar_tmp = meta::get_mubar(get_parameters());
	else mubar_tmp = mubar;

	//query work-sizes for kernel
	size_t ls2, gs2;
	cl_uint num_groups;
	this->get_work_sizes(M_tm_sitediagonal_AND_gamma5_eo, &ls2, &gs2, &num_groups);
	//set arguments
	int clerr = clSetKernelArg(M_tm_sitediagonal_AND_gamma5_eo, 0, sizeof(cl_mem), in->get_cl_buffer());
	if(clerr != CL_SUCCESS) throw Opencl_Error(clerr, "clSetKernelArg", __FILE__, __LINE__);

	clerr = clSetKernelArg(M_tm_sitediagonal_AND_gamma5_eo, 1, sizeof(cl_mem), out->get_cl_buffer());
	if(clerr != CL_SUCCESS) throw Opencl_Error(clerr, "clSetKernelArg", __FILE__, __LINE__);

	clerr = clSetKernelArg(M_tm_sitediagonal_AND_gamma5_eo, 2, sizeof(hmc_float), &mubar_tmp);
	if(clerr != CL_SUCCESS) throw Opencl_Error(clerr, "clSetKernelArg", __FILE__, __LINE__);

	get_device()->enqueue_kernel(M_tm_sitediagonal_AND_gamma5_eo , gs2, ls2);
}

void hardware::code::Fermions::M_tm_sitediagonal_minus_AND_gamma5_eo_device(const hardware::buffers::Spinor * in, const hardware::buffers::Spinor * out, hmc_float mubar)
{
	//get mu
	hmc_float mubar_tmp;
	if(mubar == ARG_DEF) mubar_tmp = meta::get_mubar(get_parameters());
	else mubar_tmp = mubar;

	//query work-sizes for kernel
	size_t ls2, gs2;
	cl_uint num_groups;
	this->get_work_sizes(M_tm_sitediagonal_minus_AND_gamma5_eo, &ls2, &gs2, &num_groups);
	//set arguments
	int clerr = clSetKernelArg(M_tm_sitediagonal_minus_AND_gamma5_eo, 0, sizeof(cl_mem), in->get_cl_buffer());
	if(clerr != CL_SUCCESS) throw Opencl_Error(clerr, "clSetKernelArg", __FILE__, __LINE__);

	clerr = clSetKernelArg(M_tm_sitediagonal_minus_AND_gamma5_eo, 1, sizeof(cl_mem), out->get_cl_buffer());
	if(clerr != CL_SUCCESS) throw Opencl_Error(clerr, "clSetKernelArg", __FILE__, __LINE__);

	clerr = clSetKernelArg(M_tm_sitediagonal_minus_AND_gamma5_eo, 2, sizeof(hmc_float), &mubar_tmp);
	if(clerr != CL_SUCCESS) throw Opencl_Error(clerr, "clSetKernelArg", __FILE__, __LINE__);

	get_device()->enqueue_kernel(M_tm_sitediagonal_minus_AND_gamma5_eo , gs2, ls2);
}

void hardware::code::Fermions::M_tm_inverse_sitediagonal_minus_device(const hardware::buffers::Spinor * in, const hardware::buffers::Spinor * out, hmc_float mubar)
{
	//get mu
	hmc_float mubar_tmp;
	if(mubar == ARG_DEF) mubar_tmp = meta::get_mubar(get_parameters());
	else mubar_tmp = mubar;

	//query work-sizes for kernel
	size_t ls2, gs2;
	cl_uint num_groups;
	this->get_work_sizes(M_tm_inverse_sitediagonal_minus, &ls2, &gs2, &num_groups);
	//set arguments
	int clerr = clSetKernelArg(M_tm_inverse_sitediagonal_minus, 0, sizeof(cl_mem), in->get_cl_buffer());
	if(clerr != CL_SUCCESS) throw Opencl_Error(clerr, "clSetKernelArg", __FILE__, __LINE__);

	clerr = clSetKernelArg(M_tm_inverse_sitediagonal_minus, 1, sizeof(cl_mem), out->get_cl_buffer());
	if(clerr != CL_SUCCESS) throw Opencl_Error(clerr, "clSetKernelArg", __FILE__, __LINE__);

	clerr = clSetKernelArg(M_tm_inverse_sitediagonal_minus, 2, sizeof(hmc_float), &mubar_tmp);
	if(clerr != CL_SUCCESS) throw Opencl_Error(clerr, "clSetKernelArg", __FILE__, __LINE__);

	get_device()->enqueue_kernel( M_tm_inverse_sitediagonal_minus, gs2, ls2);
}

void hardware::code::Fermions::M_tm_sitediagonal_minus_device(const hardware::buffers::Spinor * in, const hardware::buffers::Spinor * out, hmc_float mubar)
{
	//get mu
	hmc_float mubar_tmp;
	if(mubar == ARG_DEF) mubar_tmp = meta::get_mubar(get_parameters());
	else mubar_tmp = mubar;

	//query work-sizes for kernel
	size_t ls2, gs2;
	cl_uint num_groups;
	this->get_work_sizes(M_tm_sitediagonal_minus, &ls2, &gs2, &num_groups);
	//set arguments
	int clerr = clSetKernelArg(M_tm_sitediagonal_minus, 0, sizeof(cl_mem), in->get_cl_buffer());
	if(clerr != CL_SUCCESS) throw Opencl_Error(clerr, "clSetKernelArg", __FILE__, __LINE__);

	clerr = clSetKernelArg(M_tm_sitediagonal_minus, 1, sizeof(cl_mem), out->get_cl_buffer());
	if(clerr != CL_SUCCESS) throw Opencl_Error(clerr, "clSetKernelArg", __FILE__, __LINE__);

	clerr = clSetKernelArg(M_tm_sitediagonal_minus, 2, sizeof(hmc_float), &mubar_tmp);
	if(clerr != CL_SUCCESS) throw Opencl_Error(clerr, "clSetKernelArg", __FILE__, __LINE__);

	get_device()->enqueue_kernel(M_tm_sitediagonal_minus , gs2, ls2);
}

int hardware::code::Fermions::bicgstab(const Matrix_Function & f, const hardware::buffers::Plain<spinor> * inout, const hardware::buffers::Plain<spinor> * source, const hardware::buffers::SU3 * gf, hmc_float prec, hmc_float kappa, hmc_float mubar)
{
	using namespace hardware::buffers;

	const Plain<hmc_float> clmem_resid(1, get_device());
	const Plain<hmc_float> clmem_trueresid(1, get_device());

	auto spinor_code = get_device()->get_spinor_code();

	//"save" version, with comments. this is called if "bicgstab_save" is choosen.
	if (get_parameters().get_solver() == meta::Inputparameters::bicgstab_save) {
		hmc_float resid;
		for(int iter = 0; iter < get_parameters().get_cgmax(); iter++) {
			if(iter % get_parameters().get_iter_refresh() == 0) {
				spinor_code->set_zero_spinorfield_device(&clmem_v);
				spinor_code->set_zero_spinorfield_device(&clmem_p);
				//initial r_n
				f(inout, &clmem_rn, gf, kappa, mubar);
				spinor_code->saxpy_device(&clmem_rn, source, &clmem_one, &clmem_rn);
				//rhat = r_n
				hardware::buffers::copyData(&clmem_rhat, &clmem_rn);
				//set some constants to 1
				copyData(&clmem_alpha, &clmem_one);
				copyData(&clmem_omega, &clmem_one);
				copyData(&clmem_rho, &clmem_one);
			}
			//rho_next = (rhat, rn)
			spinor_code->set_complex_to_scalar_product_device(&clmem_rhat, &clmem_rn, &clmem_rho_next);
			//check if algorithm is stuck
			hmc_complex check;
			clmem_rho_next.dump(&check);
			//if rho is too small the algorithm will get stuck and will never converge!!
			if(abs(check.re) < 1e-25 && abs(check.im) < 1e-25 ) {
				//print the last residuum
				logger.fatal() << "\t\t\tsolver stuck at resid:\t" << resid;
				return -iter;
			}
			//tmp1 = rho_next/rho = (rhat, rn)/..
			spinor_code->set_complex_to_ratio_device(&clmem_rho_next, &clmem_rho, &clmem_tmp1);
			//rho_next = rho
			copyData(&clmem_rho, &clmem_rho_next);
			//tmp2 = alpha/omega = ...
			spinor_code->set_complex_to_ratio_device(&clmem_alpha, &clmem_omega, &clmem_tmp2);
			//beta = tmp1*tmp2
			spinor_code->set_complex_to_product_device(&clmem_tmp1, &clmem_tmp2, &clmem_beta);

			//tmp1 = beta*omega
			spinor_code->set_complex_to_product_device(&clmem_beta, &clmem_omega, &clmem_tmp1);
			//tmp2 = -tmp1
			spinor_code->set_complex_to_product_device(&clmem_minusone, &clmem_tmp1, &clmem_tmp2);
			//p = beta*p + tmp2*v + r_n = beta*p - beta*omega*v + r_n
			spinor_code->saxsbypz_device(&clmem_p, &clmem_v, &clmem_rn, &clmem_beta, &clmem_tmp2, &clmem_p);

			//v = A*p
			f(&clmem_p, &clmem_v, gf, kappa, mubar);
			//tmp1 = (rhat, v)
			spinor_code->set_complex_to_scalar_product_device(&clmem_rhat, &clmem_v, &clmem_tmp1);
			//alpha = rho/tmp1 = (..)/(rhat, v)
			spinor_code->set_complex_to_ratio_device (&clmem_rho, &clmem_tmp1, &clmem_alpha);
			//s = - alpha * v - r_n
			spinor_code->saxpy_device(&clmem_v, &clmem_rn, &clmem_alpha, &clmem_s);
			//t = A s
			f(&clmem_s, &clmem_t, gf, kappa, mubar);
			//tmp1 = (t, s)
			spinor_code->set_complex_to_scalar_product_device(&clmem_t, &clmem_s, &clmem_tmp1);
			//!!CP: this can also be global_squarenorm, but one needs a complex number here
			//tmp2 = (t,t)
			spinor_code->set_complex_to_scalar_product_device(&clmem_t, &clmem_t, &clmem_tmp2);
			//omega = tmp1/tmp2 = (t,s)/(t,t)
			spinor_code->set_complex_to_ratio_device(&clmem_tmp1, &clmem_tmp2, &clmem_omega);
			//r_n = - omega*t - s
			spinor_code->saxpy_device(&clmem_t, &clmem_s, &clmem_omega, &clmem_rn);
			//inout = alpha*p + omega * s + inout
			spinor_code->saxsbypz_device(&clmem_p, &clmem_s, inout, &clmem_alpha, &clmem_omega, inout);
			//resid = (rn,rn)
			spinor_code->set_float_to_global_squarenorm_device(&clmem_rn, &clmem_resid);
			clmem_resid.dump(&resid);

			logger.debug() << "resid: " << resid;
			//test if resid is NAN
			if(resid != resid) {
				logger.fatal() << "\tNAN occured in bicgstab!";
				return -iter;
			}
			if(resid < prec) {
				//aux = A inout
				f(inout, &clmem_aux, gf, kappa, mubar);
				//aux = -aux + source
				spinor_code->saxpy_device(&clmem_aux, source, &clmem_one, &clmem_aux);
				//trueresid = (aux, aux)
				spinor_code->set_float_to_global_squarenorm_device(&clmem_aux, &clmem_trueresid);
				hmc_float trueresid;
				clmem_trueresid.dump(&trueresid);
				logger.debug() << "\tsolver converged! true resid:\t" << trueresid;
				if(trueresid < prec)
					return iter;
			}
		}
		return -1;
	}
	//version with different structure than "save" one, similar to tmlqcd. This should be the default bicgstab.
	//  In particular this version does not perform the check if the "real" residuum is sufficiently small!
	else if (get_parameters().get_solver() == meta::Inputparameters::bicgstab) {
		hmc_float resid;
		for(int iter = 0; iter < get_parameters().get_cgmax(); iter++) {
			if(iter % get_parameters().get_iter_refresh() == 0) {
				//initial r_n, saved in p
				f(inout, &clmem_rn, gf, kappa, mubar);
				spinor_code->saxpy_device(&clmem_rn, source, &clmem_one, &clmem_p);
				//rhat = p
				hardware::buffers::copyData(&clmem_rhat, &clmem_p);
				//r_n = p
				hardware::buffers::copyData(&clmem_rn, &clmem_p);
				//rho = (rhat, rn)
				spinor_code->set_complex_to_scalar_product_device(&clmem_rhat, &clmem_rn, &clmem_rho);
			}
			//resid = (rn,rn)
			spinor_code->set_float_to_global_squarenorm_device(&clmem_rn, &clmem_resid);
			clmem_resid.dump(&resid);
			//test if resid is NAN
			if(resid != resid) {
				logger.fatal() << "\tNAN occured in bicgstab!";
				return -iter;
			}
			if(resid < prec) {
				return iter;
			}
			//v = A*p
			f(&clmem_p, &clmem_v, gf, kappa, mubar);
			//tmp1 = (rhat, v)
			spinor_code->set_complex_to_scalar_product_device(&clmem_rhat, &clmem_v, &clmem_tmp1);
			//alpha = rho/tmp1 = (rhat, rn)/(rhat, v)
			spinor_code->set_complex_to_ratio_device (&clmem_rho, &clmem_tmp1, &clmem_alpha);
			//s = - alpha * v - r_n
			spinor_code->saxpy_device(&clmem_v, &clmem_rn, &clmem_alpha, &clmem_s);
			//t = A s
			f(&clmem_s, &clmem_t, gf, kappa, mubar);
			//tmp1 = (t, s)
			spinor_code->set_complex_to_scalar_product_device(&clmem_t, &clmem_s, &clmem_tmp1);
			//!!CP: this can also be global_squarenorm, but one needs a complex number here
			//tmp2 = (t,t)
			spinor_code->set_complex_to_scalar_product_device(&clmem_t, &clmem_t, &clmem_tmp2);
			//omega = tmp1/tmp2 = (t,s)/(t,t)
			spinor_code->set_complex_to_ratio_device(&clmem_tmp1, &clmem_tmp2, &clmem_omega);
			//inout = alpha*p + omega * s + inout
			spinor_code->saxsbypz_device(&clmem_p, &clmem_s, inout, &clmem_alpha, &clmem_omega, inout);
			//r_n = - omega*t - s
			spinor_code->saxpy_device(&clmem_t, &clmem_s, &clmem_omega, &clmem_rn);
			//rho_next = (rhat, rn)
			spinor_code->set_complex_to_scalar_product_device(&clmem_rhat, &clmem_rn, &clmem_rho_next);
			//check if algorithm is stuck
			hmc_complex check;
			clmem_rho_next.dump(&check);
			//if rho is too small the algorithm will get stuck and will never converge!!
			if(abs(check.re) < 1e-25 && abs(check.im) < 1e-25 ) {
				//print the last residuum
				logger.fatal() << "\t\t\tsolver stuck at resid:\t" << resid;
				return -iter;
			}
			//tmp1 = rho_next/rho = (rhat, rn)/..
			spinor_code->set_complex_to_ratio_device(&clmem_rho_next, &clmem_rho, &clmem_tmp1);
			//tmp2 = alpha/omega = ...
			spinor_code->set_complex_to_ratio_device(&clmem_alpha, &clmem_omega, &clmem_tmp2);
			//beta = tmp1*tmp2 = alpha*rho_next / (omega*rho)
			spinor_code->set_complex_to_product_device(&clmem_tmp1, &clmem_tmp2, &clmem_beta);
			//tmp1 = beta*omega = alpha* rho_next / rho
			spinor_code->set_complex_to_product_device(&clmem_beta, &clmem_omega, &clmem_tmp1);
			//tmp2 = -tmp1
			spinor_code->set_complex_to_product_device(&clmem_minusone, &clmem_tmp1, &clmem_tmp2);
			//p = beta*p + tmp2*v + r_n = beta*p - beta*omega*v + r_n
			spinor_code->saxsbypz_device(&clmem_p, &clmem_v, &clmem_rn, &clmem_beta, &clmem_tmp2, &clmem_p);
			//rho_next = rho
			copyData(&clmem_rho, &clmem_rho_next);
		}
		return -1;
	}
	logger.fatal() << "This line should never be reached! Program logic is inconsistent!";
	throw std::logic_error("Somebody goofed up - the solver you selected is not implemented.");
}

int hardware::code::Fermions::bicgstab_eo(const Matrix_Function_eo & f, const hardware::buffers::Spinor * inout, const hardware::buffers::Spinor * source, const hardware::buffers::SU3 * gf, hmc_float prec, hmc_float kappa, hmc_float mubar)
{
	using namespace hardware::buffers;

	const Plain<hmc_float> clmem_resid(1, get_device());
	const Plain<hmc_float> clmem_trueresid(1, get_device());

	auto spinor_code = get_device()->get_spinor_code();

	//"save" version, with comments. this is called if "bicgstab_save" is choosen.
	if (get_parameters().get_solver() == meta::Inputparameters::bicgstab_save) {
		klepsydra::Monotonic timer;
		if(logger.beInfo()) {
			cl_event start_event;
			get_device()->enqueue_marker(&start_event);
			clSetEventCallback(start_event, CL_COMPLETE, resetTimerOnComplete, &timer);
			clReleaseEvent(start_event);
		}
		//CP: these have to be on the host
		hmc_float resid;
		hmc_float trueresid;
		unsigned retests = 0;
		int cgmax = get_parameters().get_cgmax();
		for(int iter = 0; iter < cgmax; iter++) {
			if(iter % get_parameters().get_iter_refresh() == 0) {
				spinor_code->set_zero_spinorfield_eoprec_device(&clmem_v_eo);
				spinor_code->set_zero_spinorfield_eoprec_device(&clmem_p_eo);

				f(inout, &clmem_rn_eo, gf, kappa, mubar);

				spinor_code->saxpy_eoprec_device(&clmem_rn_eo, source, &clmem_one, &clmem_rn_eo);

				hardware::buffers::copyData(&clmem_rhat_eo, &clmem_rn_eo);

				copyData(&clmem_alpha, &clmem_one);
				copyData(&clmem_omega, &clmem_one);
				copyData(&clmem_rho, &clmem_one);
			}
			spinor_code->set_complex_to_scalar_product_eoprec_device(&clmem_rhat_eo, &clmem_rn_eo, &clmem_rho_next);
			//check if algorithm is stuck
			hmc_complex check;
			clmem_rho_next.dump(&check);
			if(abs(check.re) < 1e-25 && abs(check.im) < 1e-25 ) {
				//print the last residuum
				logger.fatal() << "\t\t\tsolver stuck at resid:\t" << resid;
				return -iter;
			}
			spinor_code->set_complex_to_ratio_device(&clmem_rho_next, &clmem_rho, &clmem_tmp1);
			hardware::buffers::copyData(&clmem_rho, &clmem_rho_next);
			spinor_code->set_complex_to_ratio_device(&clmem_alpha, &clmem_omega, &clmem_tmp2);
			spinor_code->set_complex_to_product_device(&clmem_tmp1, &clmem_tmp2, &clmem_beta);

			spinor_code->set_complex_to_product_device(&clmem_beta, &clmem_omega, &clmem_tmp1);
			spinor_code->set_complex_to_product_device(&clmem_minusone, &clmem_tmp1, &clmem_tmp2);
			spinor_code->saxsbypz_eoprec_device(&clmem_p_eo, &clmem_v_eo, &clmem_rn_eo, &clmem_beta, &clmem_tmp2, &clmem_p_eo);

			f(&clmem_p_eo, &clmem_v_eo, gf, kappa, mubar);

			spinor_code->set_complex_to_scalar_product_eoprec_device(&clmem_rhat_eo, &clmem_v_eo, &clmem_tmp1);
			spinor_code->set_complex_to_ratio_device (&clmem_rho, &clmem_tmp1, &clmem_alpha);

			spinor_code->saxpy_eoprec_device(&clmem_v_eo, &clmem_rn_eo, &clmem_alpha, &clmem_s_eo);

			f(&clmem_s_eo, &clmem_t_eo, gf, kappa, mubar);

			spinor_code->set_complex_to_scalar_product_eoprec_device(&clmem_t_eo, &clmem_s_eo, &clmem_tmp1);
			//!!CP: can this also be global_squarenorm??
			spinor_code->set_complex_to_scalar_product_eoprec_device(&clmem_t_eo, &clmem_t_eo, &clmem_tmp2);
			spinor_code->set_complex_to_ratio_device(&clmem_tmp1, &clmem_tmp2, &clmem_omega);

			spinor_code->saxpy_eoprec_device(&clmem_t_eo, &clmem_s_eo, &clmem_omega, &clmem_rn_eo);

			spinor_code->saxsbypz_eoprec_device(&clmem_p_eo, &clmem_s_eo, inout, &clmem_alpha, &clmem_omega, inout);

			spinor_code->set_float_to_global_squarenorm_eoprec_device(&clmem_rn_eo, &clmem_resid);
			clmem_resid.dump(&resid);

			logger.debug() << "resid: " << resid;
			//test if resid is NAN
			if(resid != resid) {
				logger.fatal() << "\tNAN occured in bicgstab_eo!";
				return -iter;
			}
			if(resid < prec) {
				++retests;

				f(inout, &clmem_aux_eo, gf, kappa, mubar);
				spinor_code->saxpy_eoprec_device(&clmem_aux_eo, source, &clmem_one, &clmem_aux_eo);

				spinor_code->set_float_to_global_squarenorm_eoprec_device(&clmem_aux_eo, &clmem_trueresid);
				clmem_trueresid.dump(&trueresid);
				logger.debug() << "\ttrueresiduum:\t" << trueresid;
				if(trueresid < prec) {
					// report on performance
					if(logger.beInfo()) {
						// we are always synchroneous here, as we had to recieve the residium from the device
						uint64_t duration = timer.getTime();

						// calculate flops
						unsigned refreshs = iter / get_parameters().get_iter_refresh() + 1;
						cl_ulong mf_flops = f.get_Flops();

						cl_ulong total_flops = 4 * get_flop_size("scalar_product_eoprec") + 4 * get_flop_size("ratio") + 3 * get_flop_size("product") + 2 * spinor_code->get_flop_size("saxsbypz_eoprec") + 2 * mf_flops + 2 * spinor_code->get_flop_size("saxpy_eoprec") + get_flop_size("global_squarenorm_eoprec");
						total_flops *= iter;

						total_flops += refreshs * (mf_flops + spinor_code->get_flop_size("saxpy_eoprec"));

						total_flops += retests * (mf_flops + spinor_code->get_flop_size("saxpy_eoprec") + get_flop_size("global_squarenorm_eoprec"));

						// report performanc
						logger.info() << "BiCGstab_save completed in " << duration / 1000 << " ms @ " << (total_flops / duration / 1000.f) << " Gflops. Performed " << iter << " iterations";
					}

					// we are done here
					return iter;
				}
			}
		}
		return -1;
	} else {
		//version with different structure than "save" one, similar to tmlqcd. This should be the default bicgstab.
		//  In particular this version does not perform the check if the "real" residuum is sufficiently small!
		klepsydra::Monotonic timer;
		if(logger.beInfo()) {
			cl_event start_event;
			get_device()->enqueue_marker(&start_event);
			clSetEventCallback(start_event, CL_COMPLETE, resetTimerOnComplete, &timer);
			clReleaseEvent(start_event);
		}
		for(int iter = 0; iter < get_parameters().get_cgmax(); iter++) {
			if(iter % get_parameters().get_iter_refresh() == 0) {
				//initial r_n, saved in p
				f(inout, &clmem_rn_eo, gf, kappa, mubar);
				spinor_code->saxpy_eoprec_device(&clmem_rn_eo, source, &clmem_one, &clmem_p_eo);
				//rhat = p
				hardware::buffers::copyData(&clmem_rhat_eo, &clmem_p_eo);
				//r_n = p
				hardware::buffers::copyData(&clmem_rn_eo, &clmem_p_eo);
				//rho = (rhat, rn)
				spinor_code->set_complex_to_scalar_product_eoprec_device(&clmem_rhat_eo, &clmem_rn_eo, &clmem_rho);
			}
			//resid = (rn,rn)
			spinor_code->set_float_to_global_squarenorm_eoprec_device(&clmem_rn_eo, &clmem_resid);
			hmc_float resid;
			clmem_resid.dump(&resid);

			logger.debug() << "resid: " << resid;
			//test if resid is NAN
			if(resid != resid) {
				logger.fatal() << "\tNAN occured in bicgstab_eo!";
				return -iter;
			}
			if(resid < prec) {
				// report on performance
				if(logger.beInfo()) {
					// we are always synchroneous here, as we had to recieve the residium from the device
					uint64_t duration = timer.getTime();

					// calculate flops
					unsigned refreshs = iter / get_parameters().get_iter_refresh() + 1;
					cl_ulong mf_flops = f.get_Flops();

					cl_ulong total_flops = get_flop_size("global_squarenorm_eoprec") + 2 * mf_flops + 4 * get_flop_size("scalar_product_eoprec") + 4 * get_flop_size("ratio") + 2 * spinor_code->get_flop_size("saxpy_eoprec") + 2 * spinor_code->get_flop_size("saxsbypz_eoprec") + 3 * get_flop_size("product");
					total_flops *= iter;

					total_flops += refreshs * (mf_flops + spinor_code->get_flop_size("saxpy_eoprec") + get_flop_size("scalar_product_eoprec"));

					// report performanc
					logger.info() << "BiCGstab completed in " << duration / 1000 << " ms @ " << (total_flops / duration / 1000.f) << " Gflops. Performed " << iter << " iterations";
				}

				// we are done here
				return iter;
			}
			//v = A*p
			f(&clmem_p_eo, &clmem_v_eo, gf, kappa, mubar);
			get_device()->flush();
			//tmp1 = (rhat, v)
			spinor_code->set_complex_to_scalar_product_eoprec_device(&clmem_rhat_eo, &clmem_v_eo, &clmem_tmp1);
			//alpha = rho/tmp1 = (rhat, rn)/(rhat, v)
			spinor_code->set_complex_to_ratio_device (&clmem_rho, &clmem_tmp1, &clmem_alpha);
			//s = - alpha * v - r_n
			spinor_code->saxpy_eoprec_device(&clmem_v_eo, &clmem_rn_eo, &clmem_alpha, &clmem_s_eo);
			//t = A s
			f(&clmem_s_eo, &clmem_t_eo, gf, kappa, mubar);
			//tmp1 = (t, s)
			spinor_code->set_complex_to_scalar_product_eoprec_device(&clmem_t_eo, &clmem_s_eo, &clmem_tmp1);
			//!!CP: this can also be global_squarenorm, but one needs a complex number here
			//tmp2 = (t,t)
			spinor_code->set_complex_to_scalar_product_eoprec_device(&clmem_t_eo, &clmem_t_eo, &clmem_tmp2);
			//omega = tmp1/tmp2 = (t,s)/(t,t)
			spinor_code->set_complex_to_ratio_device(&clmem_tmp1, &clmem_tmp2, &clmem_omega);
			//inout = alpha*p + omega * s + inout
			spinor_code->saxsbypz_eoprec_device(&clmem_p_eo, &clmem_s_eo, inout, &clmem_alpha, &clmem_omega, inout);
			//r_n = - omega*t - s
			spinor_code->saxpy_eoprec_device(&clmem_t_eo, &clmem_s_eo, &clmem_omega, &clmem_rn_eo);
			//rho_next = (rhat, rn)
			spinor_code->set_complex_to_scalar_product_eoprec_device(&clmem_rhat_eo, &clmem_rn_eo, &clmem_rho_next);
			//check if algorithm is stuck
			hmc_complex check;
			clmem_rho_next.dump(&check);
			if(abs(check.re) < 1e-25 && abs(check.im) < 1e-25 ) {
				//print the last residuum
				logger.fatal() << "\t\t\tsolver stuck at resid:\t" << resid;
				return -iter;
			}

			//tmp1 = rho_next/rho = (rhat, rn)/..
			spinor_code->set_complex_to_ratio_device(&clmem_rho_next, &clmem_rho, &clmem_tmp1);
			//tmp2 = alpha/omega = ...
			spinor_code->set_complex_to_ratio_device(&clmem_alpha, &clmem_omega, &clmem_tmp2);
			//beta = tmp1*tmp2 = alpha*rho_next / (omega*rho)
			spinor_code->set_complex_to_product_device(&clmem_tmp1, &clmem_tmp2, &clmem_beta);
			//tmp1 = beta*omega = alpha* rho_next / rho
			spinor_code->set_complex_to_product_device(&clmem_beta, &clmem_omega, &clmem_tmp1);
			//tmp2 = -tmp1
			spinor_code->set_complex_to_product_device(&clmem_minusone, &clmem_tmp1, &clmem_tmp2);
			//p = beta*p + tmp2*v + r_n = beta*p - beta*omega*v + r_n
			spinor_code->saxsbypz_eoprec_device(&clmem_p_eo, &clmem_v_eo, &clmem_rn_eo, &clmem_beta, &clmem_tmp2, &clmem_p_eo);
			get_device()->flush();
			//rho_next = rho
			hardware::buffers::copyData(&clmem_rho, &clmem_rho_next);
		}
		return -1;
	}
}

int hardware::code::Fermions::cg(const Matrix_Function & f, const hardware::buffers::Plain<spinor> * inout, const hardware::buffers::Plain<spinor> * source, const hardware::buffers::SU3 * gf, hmc_float prec, hmc_float kappa, hmc_float mubar)
{
	auto spinor_code = get_device()->get_spinor_code();

	//CP: here I do not use clmem_rnhat anymore and saved one scalar_product (omega)
	//NOTE: here, most of the complex numbers may also be just hmc_floats. However, for this one would need some add. functions...
	for(int iter = 0; iter < get_parameters().get_cgmax(); iter ++) {
		if(iter % get_parameters().get_iter_refresh() == 0) {
			//rn = A*inout
			f(inout, &clmem_rn, gf, kappa, mubar);
			//rn = source - A*inout
			spinor_code->saxpy_device(&clmem_rn, source, &clmem_one, &clmem_rn);
			//p = rn
			hardware::buffers::copyData(&clmem_p, &clmem_rn);
			//omega = (rn,rn)
			spinor_code->set_complex_to_scalar_product_device(&clmem_rn, &clmem_rn, &clmem_omega);
		} else {
			//update omega
			hardware::buffers::copyData(&clmem_omega, &clmem_rho_next);
		}
		//v = A pn
		f(&clmem_p, &clmem_v, gf, kappa, mubar);
		//alpha = (rn, rn)/(pn, Apn) --> alpha = omega/rho
		spinor_code->set_complex_to_scalar_product_device(&clmem_p, &clmem_v, &clmem_rho);
		spinor_code->set_complex_to_ratio_device(&clmem_omega, &clmem_rho, &clmem_alpha);
		spinor_code->set_complex_to_product_device(&clmem_alpha, &clmem_minusone, &clmem_tmp1);

		//xn+1 = xn + alpha*p = xn - tmp1*p = xn - (-tmp1)*p
		spinor_code->saxpy_device(&clmem_p, inout, &clmem_tmp1, inout);
		//rn+1 = rn - alpha*v -> rhat
		spinor_code->saxpy_device(&clmem_v, &clmem_rn, &clmem_alpha, &clmem_rn);

		//calc residuum
		//NOTE: for beta one needs a complex number at the moment, therefore, this is done with "rho_next" instead of "resid"
		spinor_code->set_complex_to_scalar_product_device(&clmem_rn, &clmem_rn, &clmem_rho_next);
		hmc_complex tmp;
		clmem_rho_next.dump(&tmp);
		hmc_float resid = tmp.re;
		//this is the orig. call
		//set_float_to_global_squarenorm_device(clmem_rn, clmem_resid);
		//get_buffer_from_device(clmem_resid, &resid, sizeof(hmc_float));

		logger.debug() << "resid: " << resid;
		//test if resid is NAN
		if(resid != resid) {
			logger.fatal() << "\tNAN occured in cg!";
			return -iter;
		}
		if(resid < prec)
			return iter;

		//beta = (rn+1, rn+1)/(rn, rn) --> alpha = rho_next/omega
		spinor_code->set_complex_to_scalar_product_device(&clmem_rn, &clmem_rn, &clmem_rho_next);
		spinor_code->set_complex_to_ratio_device(&clmem_rho_next, &clmem_omega, &clmem_beta);

		//pn+1 = rn+1 + beta*pn
		spinor_code->set_complex_to_product_device(&clmem_beta, &clmem_minusone, &clmem_tmp2);
		spinor_code->saxpy_device(&clmem_p, &clmem_rn, &clmem_tmp2, &clmem_p);
	}
	return -1;
}

int hardware::code::Fermions::cg_eo(const Matrix_Function_eo & f, const hardware::buffers::Spinor * inout, const hardware::buffers::Spinor * source, const hardware::buffers::SU3 * gf, hmc_float prec, hmc_float kappa, hmc_float mubar)
{
	auto spinor_code = get_device()->get_spinor_code();

	//this corresponds to the above function
	//NOTE: here, most of the complex numbers may also be just hmc_floats. However, for this one would need some add. functions...
	klepsydra::Monotonic timer;
	if(logger.beInfo()) {
		cl_event start_event;
		get_device()->enqueue_marker(&start_event);
		clSetEventCallback(start_event, CL_COMPLETE, resetTimerOnComplete, &timer);
		clReleaseEvent(start_event);
	}
	for(int iter = 0; iter < get_parameters().get_cgmax(); iter ++) {
		if(iter % get_parameters().get_iter_refresh() == 0) {
			//rn = A*inout
			f(inout, &clmem_rn_eo, gf, kappa, mubar);
			//rn = source - A*inout
			spinor_code->saxpy_eoprec_device(&clmem_rn_eo, source, &clmem_one, &clmem_rn_eo);
			//p = rn
			hardware::buffers::copyData(&clmem_p_eo, &clmem_rn_eo);
			//omega = (rn,rn)
			spinor_code->set_complex_to_scalar_product_eoprec_device(&clmem_rn_eo, &clmem_rn_eo, &clmem_omega);
		} else {
			//update omega
			hardware::buffers::copyData(&clmem_omega, &clmem_rho_next);
		}
		//v = A pn
		f(&clmem_p_eo, &clmem_v_eo, gf, kappa, mubar);

		//alpha = (rn, rn)/(pn, Apn) --> alpha = omega/rho
		spinor_code->set_complex_to_scalar_product_eoprec_device(&clmem_p_eo, &clmem_v_eo, &clmem_rho);
		spinor_code->set_complex_to_ratio_device(&clmem_omega, &clmem_rho, &clmem_alpha);
		spinor_code->set_complex_to_product_device(&clmem_alpha, &clmem_minusone, &clmem_tmp1);

		//xn+1 = xn + alpha*p = xn - tmp1*p = xn - (-tmp1)*p
		spinor_code->saxpy_eoprec_device(&clmem_p_eo, inout, &clmem_tmp1, inout);
		//switch between original version and kernel merged one
		if(get_parameters().get_use_merge_kernels_spinor() == false) {
			//rn+1 = rn - alpha*v -> rhat
			spinor_code->saxpy_eoprec_device(&clmem_v_eo, &clmem_rn_eo, &clmem_alpha, &clmem_rn_eo);

			//calc residuum
			//NOTE: for beta one needs a complex number at the moment, therefore, this is done with "rho_next" instead of "resid"
			spinor_code->set_complex_to_scalar_product_eoprec_device(&clmem_rn_eo, &clmem_rn_eo, &clmem_rho_next);
		} else {
			//merge two calls:
			//rn+1 = rn - alpha*v -> rhat
			//and
			//rho_next = |rhat|^2
			//rho_next is a complex number, set its imag to zero
			spinor_code->saxpy_AND_squarenorm_eo_device(&clmem_v_eo, &clmem_rn_eo, &clmem_alpha, &clmem_rn_eo, &clmem_rho_next);
		}
		hmc_complex tmp;
		clmem_rho_next.dump(&tmp);
		hmc_float resid = tmp.re;
		//this is the orig. call
		//set_float_to_global_squarenorm_device(&clmem_rn, clmem_resid);
		//get_buffer_from_device(clmem_resid, &resid, sizeof(hmc_float));

		logger.debug() << "resid: " << resid;
		//test if resid is NAN
		if(resid != resid) {
			logger.fatal() << "\tNAN occured in cg_eo!";
			return -iter;
		}
		if(resid < prec) {
			// report on performance
			if(logger.beInfo()) {
				// we are always synchroneous here, as we had to recieve the residium from the device
				uint64_t duration = timer.getTime();

				// calculate flops
				unsigned refreshs = iter / get_parameters().get_iter_refresh() + 1;
				cl_ulong mf_flops = f.get_Flops();

				cl_ulong total_flops = mf_flops + 3 * get_flop_size("scalar_product_eoprec") + 2 * get_flop_size("ratio") + 2 * get_flop_size("product") + 3 * spinor_code->get_flop_size("saxpy_eoprec");
				total_flops *= iter;

				total_flops += refreshs * (mf_flops + spinor_code->get_flop_size("saxpy_eoprec") + get_flop_size("scalar_product_eoprec"));

				// report performanc
				logger.info() << "CG completed in " << duration / 1000 << " ms @ " << (total_flops / duration / 1000.f) << " Gflops. Performed " << iter << " iterations";
			}

			return iter;
		}

		//beta = (rn+1, rn+1)/(rn, rn) --> alpha = rho_next/omega
		spinor_code->set_complex_to_ratio_device(&clmem_rho_next, &clmem_omega, &clmem_beta);

		//pn+1 = rn+1 + beta*pn
		spinor_code->set_complex_to_product_device(&clmem_beta, &clmem_minusone, &clmem_tmp2);
		spinor_code->saxpy_eoprec_device(&clmem_p_eo, &clmem_rn_eo, &clmem_tmp2, &clmem_p_eo);
	}
	return -1;
}

const hardware::buffers::Plain<spinor> * hardware::code::Fermions::get_inout()
{
	return &clmem_inout;
}

const hardware::buffers::Plain<spinor> * hardware::code::Fermions::get_source()
{
	return &clmem_source;
}

const hardware::buffers::Plain<spinor> * hardware::code::Fermions::get_tmp()
{
	return &clmem_tmp;
}

const hardware::buffers::Spinor * hardware::code::Fermions::get_inout_eo()
{
	return &clmem_inout_eo;
}

const hardware::buffers::Spinor * hardware::code::Fermions::get_tmp_eo_1()
{
	return &clmem_tmp_eo_1;
}

const hardware::buffers::Spinor * hardware::code::Fermions::get_tmp_eo_2()
{
	return &clmem_tmp_eo_2;
}

const hardware::buffers::Spinor * hardware::code::Fermions::get_source_even()
{
	return &clmem_source_even;
}

const hardware::buffers::Spinor * hardware::code::Fermions::get_source_odd()
{
	return &clmem_source_odd;
}

const hardware::buffers::Plain<hmc_complex> * hardware::code::Fermions::get_clmem_minusone()
{
	return &clmem_minusone;
}

const hardware::buffers::Plain<hmc_complex> * hardware::code::Fermions::get_clmem_one()
{
	return &clmem_one;
}

hmc_float hardware::code::Fermions::print_info_inv_field(const hardware::buffers::Plain<spinor> * in, bool eo, std::string msg)
{
	auto spinor_code = get_device()->get_spinor_code();

	if(eo) {
		throw std::logic_error("Even-odd fields should always be stored as hardware::buffers::Spinor");
	}
	hardware::buffers::Plain<hmc_float> clmem_sqnorm_tmp(1, get_device());
	hmc_float tmp;
	spinor_code->set_float_to_global_squarenorm_device(in, &clmem_sqnorm_tmp);
	clmem_sqnorm_tmp.dump(&tmp);
	cout.precision(10);
	logger.debug() << std::scientific << msg << tmp;
	return tmp;
}

hmc_float hardware::code::Fermions::print_info_inv_field(const hardware::buffers::Spinor * in, bool eo, std::string msg)
{
	auto spinor_code = get_device()->get_spinor_code();

	if(!eo) {
		throw std::logic_error("hardware::buffers::Spinor fields are assumed to always be even-odd format");
	}
	hardware::buffers::Plain<hmc_float> clmem_sqnorm_tmp(1, get_device());
	hmc_float tmp;
	spinor_code->set_float_to_global_squarenorm_eoprec_device(in, &clmem_sqnorm_tmp);
	clmem_sqnorm_tmp.dump(&tmp);
	cout.precision(10);
	logger.debug() << std::scientific << msg << tmp;
	return tmp;
}

size_t hardware::code::Fermions::get_read_write_size(const std::string& in) const
{
	//Depending on the compile-options, one has different sizes...
	size_t D = meta::get_float_size(get_parameters());
	//this returns the number of entries in an su3-matrix
	size_t R = meta::get_mat_size(get_parameters());
	//this is the number of spinors in the system (or number of sites)
	size_t S = meta::get_spinorfieldsize(get_parameters());
	size_t Seo = meta::get_eoprec_spinorfieldsize(get_parameters());
	//factor for complex numbers
	int C = 2;
	//this is the same as in the function above
	//NOTE: 1 spinor has NC*NDIM = 12 complex entries
	if (in == "M_wilson") {
		//this kernel reads 9 spinors, 8 su3matrices and writes 1 spinor:
		return (C * 12 * (9 + 1) + C * 8 * R) * D * S;
	}
	if (in == "gamma5") {
		//this kernel reads 1 spinor and writes 1 spinor:
		return 2 * C * 12 * D * S;
	}
	if (in == "M_tm_plus") {
		//this kernel reads 9 spinors, 8 su3matrices and writes 1 spinor:
		return (C * 12 * (9 + 1) + C * 8 * R) * D * S;
	}
	if (in == "M_tm_minus") {
		//this kernel reads 9 spinors, 8 su3matrices and writes 1 spinor:
		return (C * 12 * (9 + 1) + C * 8 * R) * D * S;
	}
	if (in == "gamma5_eo") {
		//this kernel reads 1 spinor and writes 1 spinor:
		return 48 * D * Seo;
	}
	if (in == "M_tm_sitediagonal") {
		//this kernel reads 1 spinor and writes 1 spinor:
		return 48 * D * Seo;
	}
	if (in == "M_tm_inverse_sitediagonal") {
		//this kernel reads 1 spinor and writes 1 spinor:
		return 48 * D * Seo;
	}
	if (in == "M_tm_sitediagonal_minus") {
		//this kernel reads 1 spinor and writes 1 spinor:
		return 48 * D * Seo;
	}
	if (in == "M_tm_inverse_sitediagonal_minus") {
		//this kernel reads 1 spinor and writes 1 spinor:
		return 48 * D * Seo;
	}
	if (in == "dslash_eo") {
		//this kernel reads 8 spinors, 8 su3matrices and writes 1 spinor:
		const unsigned int dirs = 4;
		return (C * 12 * (2 * dirs + 1) + C * 2 * dirs * R) * D * Seo;
	}
	if (in == "dslash_AND_gamma5_eo") {
		//the dslash kernel reads 8 spinors, 8 su3matrices and writes 1 spinor:
		const unsigned int dirs = 4;
		//the gamma5 kernel reads 1 spinor and writes 1 spinor:
		//the merged kernel reads 8 spinors, 8 su3matrices and writes 1 spinor, thus it is the same as the dslash
		return  (C * 12 * (2 * dirs + 1) + C * 2 * dirs * R) * D * Seo;
	}
	if (in == "dslash_AND_M_tm_inverse_sitediagonal_eo") {
		//the dslash kernel reads 8 spinors, 8 su3matrices and writes 1 spinor:
		const unsigned int dirs = 4;
		//the diag kernel reads 1 spinor and writes 1 spinor:
		//the merged kernel reads 8 spinors, 8 su3matrices and writes 1 spinor, thus it is the same as the dslash
		return  (C * 12 * (2 * dirs + 1) + C * 2 * dirs * R) * D * Seo;
	}
	if (in == "dslash_AND_M_tm_inverse_sitediagonal_minus_eo") {
		//the dslash kernel reads 8 spinors, 8 su3matrices and writes 1 spinor:
		const unsigned int dirs = 4;
		//the diag kernel reads 1 spinor and writes 1 spinor:
		//the merged kernel reads 8 spinors, 8 su3matrices and writes 1 spinor, thus it is the same as the dslash
		return  (C * 12 * (2 * dirs + 1) + C * 2 * dirs * R) * D * Seo;
	}
	if (in == "M_tm_sitediagonal_AND_gamma5_eo") {
		//this kernel reads 1 spinor and writes 1 spinor:
		return 48 * D * Seo;
	}
	if (in == "M_tm_sitediagonal_minus_AND_gamma5_eo") {
		//this kernel reads 1 spinor and writes 1 spinor:
		return 48 * D * Seo;
	}
	return 0;
}

static int flop_dslash_per_site(const meta::Inputparameters & parameters)
{
	/** @NOTE: this is the "original" dslash without any simplifications, counting everything "full". this is a much too hight number!!
	   *  //this kernel performs for each eo site a 2*NDIM sum over (1 + gamma_mu) * su3matrix * spinor
	   *  //return  NDIM * 2 * ( get_parameters().get_flop_su3_spinor() + get_parameters().get_flop_gamma_spinor() ) ;
	  @NOTE: However, in 0911.3191 the dslash_eo is quoted to perform 1320 flop per site
	   *  If I count our implementation of the dslash-kernel, I get 1632 flop per site:
	   *  //the kernel performs 6 su3vec_acc, 2 su3_su3vec and 2 su3vec_complex in NDIM * 2 directions per site
	  */
	return NDIM * 2 * (meta::get_flop_su3_su3vec() * 2 + 6 * NC * 2 + 2 * NC * meta::get_flop_complex_mult() );
	/// I do not know for sure, but if one leaves out the 2 su3vec_complex operations in each directions one has almost 1320 flop per site. This would correspond to have the kappa in the diagonal matrix still.

}

uint64_t hardware::code::Fermions::get_flop_size(const std::string& in) const
{
	size_t S = meta::get_spinorfieldsize(get_parameters());
	size_t Seo = meta::get_eoprec_spinorfieldsize(get_parameters());
	if (in == "M_wilson") {
		//this kernel performs one dslash on each site and adds this to a spinor
		return S * (flop_dslash_per_site(get_parameters()) + NC * NDIM * meta::get_flop_complex_mult() + NC * NDIM * 2 );
	}
	if (in == "gamma5") {
		//this kernel performs ND*NC*2/2 real mults
		return S * NDIM * NC;
	}
	if (in == "M_tm_plus") {
		//this kernel performs ND*NC complex mults and one dslash on each site and adds the results
		return S * (flop_dslash_per_site(get_parameters()) + NC * NDIM * meta::get_flop_complex_mult() + NC * NDIM * 2 );
	}
	if (in == "M_tm_minus") {
		//this kernel performs ND*NC complex mults and one dslash on each site and adds the results
		return S * (flop_dslash_per_site(get_parameters()) + NC * NDIM * meta::get_flop_complex_mult() + NC * NDIM * 2 );
	}
	if (in == "gamma5_eo") {
		//this kernel performs ND*NC*2/2 real mults
		return Seo * NDIM * NC;
	}
	if (in == "M_tm_sitediagonal") {
		//this kernel performs ND*NC complex mults
		return Seo * ( NC * NDIM * meta::get_flop_complex_mult() );
	}
	if (in == "M_tm_inverse_sitediagonal") {
		//this kernel performs ND*NC complex mults and ND*NC*2 real mults
		return Seo * ( NC * NDIM * meta::get_flop_complex_mult() + NC * NDIM * 2  );
	}
	if (in == "M_tm_sitediagonal_minus") {
		//this kernel performs ND*NC complex mults
		return Seo * ( NC * NDIM * meta::get_flop_complex_mult() );
	}
	if (in == "M_tm_inverse_sitediagonal_minus") {
		//this kernel performs ND*NC complex mults and ND*NC*2 real mults
		return Seo * ( NC * NDIM * meta::get_flop_complex_mult() + NC * NDIM * 2 );
	}
	if (in == "dslash_eo") {
		return Seo * flop_dslash_per_site(get_parameters());
	}
	if (in == "dslash_AND_gamma5_eo") {
		return Seo * flop_dslash_per_site(get_parameters()) +  Seo * NDIM * NC;
	}
	if (in == "dslash_AND_M_tm_inverse_sitediagonal_eo") {
		return Seo * flop_dslash_per_site(get_parameters()) + Seo * ( NC * NDIM * meta::get_flop_complex_mult() + NC * NDIM * 2  );
	}
	if (in == "dslash_AND_M_tm_inverse_sitediagonal_minus_eo") {
		return Seo * flop_dslash_per_site(get_parameters()) + Seo * ( NC * NDIM * meta::get_flop_complex_mult() + NC * NDIM * 2  );
	}
	if (in == "M_tm_sitediagonal_AND_gamma5_eo") {
		//this kernel performs ND*NC complex mults and  ND*NC*2/2 real mults
		return Seo * ( NC * NDIM * meta::get_flop_complex_mult() ) + Seo * NDIM * NC ;
	}
	if (in == "M_tm_sitediagonal_minus_AND_gamma5_eo") {
		//this kernel performs ND*NC complex mults  ND*NC*2/2 real mults
		return Seo * ( NC * NDIM * meta::get_flop_complex_mult() ) +  Seo * NDIM * NC;
	}
	return 0;
}

void hardware::code::Fermions::print_profiling(const std::string& filename, int number) const
{
	Opencl_Module::print_profiling(filename, number);
	Opencl_Module::print_profiling(filename, M_wilson);
	Opencl_Module::print_profiling(filename, gamma5);
	Opencl_Module::print_profiling(filename, M_tm_plus);
	Opencl_Module::print_profiling(filename, M_tm_minus);
	Opencl_Module::print_profiling(filename, gamma5_eo);
	Opencl_Module::print_profiling(filename, M_tm_sitediagonal);
	Opencl_Module::print_profiling(filename, M_tm_inverse_sitediagonal);
	Opencl_Module::print_profiling(filename, M_tm_sitediagonal_minus);
	Opencl_Module::print_profiling(filename, M_tm_inverse_sitediagonal_minus);
	Opencl_Module::print_profiling(filename, dslash_eo);
	Opencl_Module::print_profiling(filename, dslash_AND_gamma5_eo);
	Opencl_Module::print_profiling(filename, dslash_AND_M_tm_inverse_sitediagonal_eo);
	Opencl_Module::print_profiling(filename, dslash_AND_M_tm_inverse_sitediagonal_minus_eo);
	Opencl_Module::print_profiling(filename, M_tm_sitediagonal_AND_gamma5_eo);
	Opencl_Module::print_profiling(filename, M_tm_sitediagonal_minus_AND_gamma5_eo);
}
hardware::code::Fermions::Fermions(const meta::Inputparameters& params, hardware::Device * device)
	: Opencl_Module(params, device),
	  M_wilson(0),
	  gamma5(0),
	  M_tm_plus(0),
	  M_tm_minus(0),
	  gamma5_eo(0),
	  M_tm_sitediagonal(0),
	  M_tm_inverse_sitediagonal(0),
	  M_tm_sitediagonal_minus(0),
	  M_tm_inverse_sitediagonal_minus(0),
	  dslash_eo(0),
	  dslash_AND_gamma5_eo(0),
	  dslash_AND_M_tm_inverse_sitediagonal_eo(0),
	  dslash_AND_M_tm_inverse_sitediagonal_minus_eo(0),
	  M_tm_sitediagonal_AND_gamma5_eo(0),
	  M_tm_sitediagonal_minus_AND_gamma5_eo(0),
	  clmem_inout(meta::get_spinorfieldsize(params), device),
	  clmem_source(meta::get_spinorfieldsize(params), device),
	  // TODO these are only used in a non-eoprec solver
	  clmem_rn(meta::get_spinorfieldsize(params), device),
	  clmem_rhat(meta::get_spinorfieldsize(params), device),
	  clmem_v(meta::get_spinorfieldsize(params), device),
	  clmem_p(meta::get_spinorfieldsize(params), device),
	  clmem_s(meta::get_spinorfieldsize(params), device),
	  clmem_t(meta::get_spinorfieldsize(params), device),
	  clmem_aux(meta::get_spinorfieldsize(params), device),
	  clmem_tmp(meta::get_spinorfieldsize(params), device),
	  //LZ only use the following if we want to apply even odd preconditioning
	  clmem_inout_eo(meta::get_eoprec_spinorfieldsize(params), device), // TODO we don't need this if no eo
	  clmem_source_even(meta::get_eoprec_spinorfieldsize(params), device), // TODO we don't need this if no eo
	  clmem_source_odd(meta::get_eoprec_spinorfieldsize(params), device), // TODO we don't need this if no eo
	  clmem_rn_eo(meta::get_eoprec_spinorfieldsize(params), device),
	  clmem_rhat_eo(meta::get_eoprec_spinorfieldsize(params), device),
	  clmem_v_eo(meta::get_eoprec_spinorfieldsize(params), device),
	  clmem_p_eo(meta::get_eoprec_spinorfieldsize(params), device),
	  clmem_s_eo(meta::get_eoprec_spinorfieldsize(params), device),
	  clmem_t_eo(meta::get_eoprec_spinorfieldsize(params), device),
	  clmem_aux_eo(meta::get_eoprec_spinorfieldsize(params), device),
	  clmem_tmp_eo_1(meta::get_eoprec_spinorfieldsize(params), device), // TODO we don't need this if no eo
	  clmem_tmp_eo_2(meta::get_eoprec_spinorfieldsize(params), device), // TODO we don't need this if no eo or no Twistedmass
	  clmem_rho(1, device),
	  clmem_rho_next(1, device),
	  clmem_alpha(1, device),
	  clmem_omega(1, device),
	  clmem_beta(1, device),
	  clmem_tmp1(1, device),
	  clmem_tmp2(1, device),
	  clmem_one(1, device),
	  clmem_minusone(1, device)
{
	//fill buffers with one and minusone
	hmc_complex one = hmc_complex_one;
	hmc_complex minusone = hmc_complex_minusone;
	clmem_one.load(&one);
	clmem_minusone.load(&minusone);

	fill_kernels();
};

hardware::code::Fermions::~Fermions()
{
	clear_kernels();
}

ClSourcePackage hardware::code::Fermions::get_sources() const noexcept
{
	return sources;
}