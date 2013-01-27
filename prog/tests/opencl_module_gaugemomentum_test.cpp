#include "../gaugefield_hybrid.h"

#include "../meta/util.hpp"
#include "../host_random.h"

// use the boost test framework
#define BOOST_TEST_DYN_LINK
#define BOOST_TEST_MODULE OPENCL_MODULE_GAUGEMOMENTUM
#include <boost/test/unit_test.hpp>

#include "test_util.h"

extern std::string const version;
std::string const version = "0.1";

class TestGaugefield : public Gaugefield_hybrid {

public:
	TestGaugefield(const hardware::System * system) : Gaugefield_hybrid(system), prng(*system) {
		auto inputfile = system->get_inputparameters();
		init(1, inputfile.get_use_gpu() ? CL_DEVICE_TYPE_GPU : CL_DEVICE_TYPE_CPU, prng);
		meta::print_info_hmc("test program", inputfile);
		print_gaugeobservables(0);
	};

	virtual void init_tasks();
	virtual void finalize_opencl();

	hardware::code::Gaugemomentum * get_device();
  physics::PRNG* get_prng();
private:
	physics::PRNG prng;
};

void TestGaugefield::init_tasks()
{
	opencl_modules = new hardware::code::Opencl_Module* [get_num_tasks()];
	opencl_modules[0] = get_device_for_task(0)->get_gaugemomentum_code();
}

void TestGaugefield::finalize_opencl()
{
	Gaugefield_hybrid::finalize_opencl();
}

hardware::code::Gaugemomentum* TestGaugefield::get_device()
{
	return static_cast<hardware::code::Gaugemomentum*>(opencl_modules[0]);
}

void fill_with_one(hmc_float * sf_in, int size)
{
	for(int i = 0; i < size; ++i) {
		sf_in[i] = 1.;
	}
	return;
}

void fill_with_random(hmc_float * sf_in, int size, int seed)
{
	prng_init(seed);
	for(int i = 0; i < size; ++i) {
		sf_in[i] = prng_double();
	}
	return;
}

ae make_ae(hmc_float e1, hmc_float e2, hmc_float e3, hmc_float e4,
           hmc_float e5, hmc_float e6, hmc_float e7, hmc_float e8)
{
	ae tmp = {e1, e2, e3, e4, e5, e6, e7, e8};
	return tmp;
}

void fill_with_zero(ae * ae, int size)
{
	for(int i = 0; i < size; ++i) {
		ae[i] = make_ae(0., 0., 0., 0., 0., 0., 0., 0.);
	}
	return;
}

hmc_float count_gm(ae * ae_in, int size)
{
  hmc_float sum = 0.;
  for (int i = 0; i<size;i++){
    sum +=
       ae_in[i].e0
      + ae_in[i].e1
      + ae_in[i].e2
      + ae_in[i].e3
      + ae_in[i].e4
      + ae_in[i].e5
      + ae_in[i].e6
      + ae_in[i].e7;
  }
  return sum;
}

hmc_float calc_var(hmc_float in, hmc_float mean){
  return (in - mean) * (in - mean);
}

hmc_float calc_var_gm(ae * ae_in, int size, hmc_float sum){
  hmc_float var = 0.;
  for(int k = 0; k<size; k++){
    var +=
      calc_var(   ae_in[k].e0 , sum) 
      + calc_var( ae_in[k].e1 , sum) 
      + calc_var( ae_in[k].e2 , sum)
      + calc_var( ae_in[k].e3 , sum) 
      + calc_var( ae_in[k].e4 , sum) 
      + calc_var( ae_in[k].e5 , sum) 
      + calc_var( ae_in[k].e6 , sum) 
      + calc_var( ae_in[k].e7 , sum) ;
  }
  return var;
}

physics::PRNG* TestGaugefield::get_prng()
{
	return &prng;
}

void test_build(std::string inputfile)
{
	logger.info() << "build opencl_module_gaugemomentum";
	logger.info() << "Init device";
	meta::Inputparameters params = create_parameters(inputfile);
	hardware::System system(params);
	TestGaugefield cpu(&system);
	logger.info() << "Finalize device";
	cpu.finalize();
	BOOST_MESSAGE("Test done");
}

void test_generate_gaussian_gaugemomenta(std::string inputfile)
{
	using namespace hardware::buffers;

	std::string kernelName;
	kernelName = "generate_gaussian_gaugemomentum";
	printKernelInfo(kernelName);
	logger.info() << "Init device";
	meta::Inputparameters params = create_parameters(inputfile);
	hardware::System system(params);
	TestGaugefield cpu(&system);

	physics::PRNG * prng = cpu.get_prng();
	cl_int err = CL_SUCCESS;
	hardware::code::Gaugemomentum * device = cpu.get_device();

	logger.info() << "Fill buffers...";
	size_t NUM_ELEMENTS_AE = meta::get_vol4d(params) * NDIM * meta::get_su3algebrasize();
	size_t NUM_ELEMENTS_GM = meta::get_vol4d(params) * NDIM;
	hardware::buffers::Gaugemomentum out(meta::get_vol4d(params) * NDIM, device->get_device());
	hardware::buffers::Plain<hmc_float> sqnorm(1, device->get_device());

	BOOST_REQUIRE_EQUAL(err, CL_SUCCESS);

	//CP: run the kernel a couple of times times
	int iterations = params.get_integrationsteps(0);

	ae * gm_out;
	gm_out = new ae[NUM_ELEMENTS_GM * iterations];
	BOOST_REQUIRE(gm_out);

	auto gm_code = device->get_device()->get_gaugemomentum_code();
	auto prng_buf = prng->get_buffers().at(0);

	hmc_float sum = 0;
	for (int i = 0; i< iterations; i++){
	  logger.info() << "Run kernel";
	  device->generate_gaussian_gaugemomenta_device(&out, prng_buf);
	  out.dump(&gm_out[i*NUM_ELEMENTS_GM]);
	  sum += count_gm(&gm_out[i*NUM_ELEMENTS_GM], NUM_ELEMENTS_GM);
	}
	logger.info() << "result: mean";
	hmc_float cpu_res = 0.;
	sum = sum/iterations/NUM_ELEMENTS_GM/8;	
	cpu_res= sum;
	logger.info() << cpu_res;

	if(params.get_read_multiple_configs()  == false){
	  //CP: calc std derivation
	  hmc_float var=0.;
	  for (int i=0; i<iterations; i++){
	    var += calc_var_gm(&gm_out[i*NUM_ELEMENTS_GM], NUM_ELEMENTS_GM, sum);
	  }
	  var=var/iterations/NUM_ELEMENTS_GM/8;
	  
	  cpu_res = sqrt(var);
	  logger.info() << "result: variance";
	  logger.info() << cpu_res;
	}

	logger.info() << "Finalize device";
	cpu.finalize();

	testFloatSizeAgainstInputparameters(cpu_res, params);
	BOOST_MESSAGE("Test done");
}

void test_set_zero_gm(std::string inputfile)
{
	std::string kernelName = "set_zero_gm";
	printKernelInfo(kernelName);

	logger.info() << "Init device";
	meta::Inputparameters params = create_parameters(inputfile);
	hardware::System system(params);
	TestGaugefield cpu(&system);
	hardware::code::Gaugemomentum * device = cpu.get_device();
	hmc_float * gm_in;

	logger.info() << "create buffers";
	size_t NUM_ELEMENTS_AE = meta::get_vol4d(params) * NDIM * meta::get_su3algebrasize();
	gm_in = new hmc_float[NUM_ELEMENTS_AE];
	fill_with_random(gm_in, NUM_ELEMENTS_AE, 123456);
	BOOST_REQUIRE(gm_in);

	hardware::buffers::Gaugemomentum in(meta::get_vol4d(params) * NDIM, device->get_device());
	device->importGaugemomentumBuffer(&in, reinterpret_cast<ae*>(gm_in));
	hardware::buffers::Plain<hmc_float> sqnorm(1, device->get_device());

	logger.info() << "|in|^2:";
	device->set_float_to_gaugemomentum_squarenorm_device(&in, &sqnorm);
	hmc_float cpu_back;
	sqnorm.dump(&cpu_back);
	logger.info() << cpu_back;

	logger.info() << "Run kernel";
	device->set_zero_gaugemomentum(&in);

	logger.info() << "|out|^2:";
	device->set_float_to_gaugemomentum_squarenorm_device(&in, &sqnorm);
	hmc_float cpu_back2;
	sqnorm.dump(&cpu_back2);
	logger.info() << cpu_back2;

	logger.info() << "Finalize device";
	cpu.finalize();

	logger.info() << "Free buffers";
	delete[] gm_in;

	testFloatAgainstInputparameters(cpu_back2, params);
	BOOST_MESSAGE("Test done");

}

void test_gm_squarenorm(std::string inputfile)
{
	std::string kernelName = "gaugemomenta squarenorm";
	printKernelInfo(kernelName);

	logger.info() << "Init device";
	meta::Inputparameters params = create_parameters(inputfile);
	hardware::System system(params);
	TestGaugefield cpu(&system);
	hardware::code::Gaugemomentum * device = cpu.get_device();
	hmc_float * gm_in;

	logger.info() << "create buffers";
	size_t NUM_ELEMENTS_AE = meta::get_vol4d(params) * NDIM * meta::get_su3algebrasize();
	gm_in = new hmc_float[NUM_ELEMENTS_AE];

	//use the variable use_cg to switch between cold and random input sf
	if(params.get_solver() == meta::Inputparameters::cg) {
		fill_with_one(gm_in, NUM_ELEMENTS_AE);
	} else {
		fill_with_random(gm_in, NUM_ELEMENTS_AE, 123456);
	}
	BOOST_REQUIRE(gm_in);

	hardware::buffers::Gaugemomentum in(meta::get_vol4d(params) * NDIM, device->get_device());
	device->importGaugemomentumBuffer(&in, reinterpret_cast<ae*>(gm_in));
	hardware::buffers::Plain<hmc_float> sqnorm(1, device->get_device());

	logger.info() << "Run kernel";
	logger.info() << "|in|^2:";
	device->set_float_to_gaugemomentum_squarenorm_device(&in, &sqnorm);
	hmc_float cpu_back;
	sqnorm.dump(&cpu_back);
	logger.info() << cpu_back;

	logger.info() << "Finalize device";
	cpu.finalize();

	logger.info() << "Free buffers";
	delete[] gm_in;

	testFloatAgainstInputparameters(cpu_back, params);
	BOOST_MESSAGE("Test done");
}

void test_gm_convert_to_soa(std::string inputfile)
{

}

void test_gm_convert_from_soa(std::string inputfile)
{

}

BOOST_AUTO_TEST_SUITE(BUILD)

BOOST_AUTO_TEST_CASE( BUILD_1 )
{
	test_build("/opencl_module_gaugemomentum_build_input_1");
}

BOOST_AUTO_TEST_SUITE_END()

BOOST_AUTO_TEST_SUITE(GENERATE_GAUSSIAN_GAUGEMOMENTA  )

BOOST_AUTO_TEST_CASE(GENERATE_GAUSSIAN_GAUGEMOMENTA_1 )
{
	test_generate_gaussian_gaugemomenta("/gm_gaussian_input_1");
}

BOOST_AUTO_TEST_CASE(GENERATE_GAUSSIAN_GAUGEMOMENTA_2 )
{
	test_generate_gaussian_gaugemomenta("/gm_gaussian_input_2");
}

BOOST_AUTO_TEST_SUITE_END()

BOOST_AUTO_TEST_SUITE( GM_CONVERT_TO_SOA )

BOOST_AUTO_TEST_CASE( GM_CONVERT_TO_SOA_1 )
{
	BOOST_MESSAGE("NOT YET IMPLEMENTED!!");
	test_gm_convert_to_soa("/gm_convert_to_soa_input_1");
}

BOOST_AUTO_TEST_SUITE_END()

BOOST_AUTO_TEST_SUITE( GM_CONVERT_FROM_SOA )

BOOST_AUTO_TEST_CASE( GM_CONVERT_FROM_SOA_1 )
{
	BOOST_MESSAGE("NOT YET IMPLEMENTED!!");
	test_gm_convert_from_soa("/gm_convert_from_soa_input_1");
}

BOOST_AUTO_TEST_SUITE_END()

BOOST_AUTO_TEST_SUITE( GM_SQUARENORM )

BOOST_AUTO_TEST_CASE(GM_SQUARENORM_1  )
{
	test_gm_squarenorm("/gm_squarenorm_input_1");
}

BOOST_AUTO_TEST_CASE(GM_SQUARENORM_2  )
{
	test_gm_squarenorm("/gm_squarenorm_input_2");
}

BOOST_AUTO_TEST_SUITE_END()

BOOST_AUTO_TEST_SUITE( SET_ZERO_GM )

BOOST_AUTO_TEST_CASE( SET_ZERO_GM_1 )
{
	test_set_zero_gm("/gm_set_zero_input_1");
}

BOOST_AUTO_TEST_SUITE_END()

