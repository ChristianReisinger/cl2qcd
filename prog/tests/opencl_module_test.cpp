#include "../opencl_module_hmc.h"
#include "../gaugefield_hybrid.h"
#include "../meta/util.hpp"

// use the boost test framework
#define BOOST_TEST_DYN_LINK
#define BOOST_TEST_MODULE OPENCL_MODULE
#include <boost/test/unit_test.hpp>

//some functionality
#include "test_util.h"

extern std::string const version;
std::string const version = "0.1";

class TestGaugefield : public Gaugefield_hybrid {
public:
	TestGaugefield(const hardware::System * system) : Gaugefield_hybrid(system) {
		auto inputfile = system->get_inputparameters();
		init(1, inputfile.get_use_gpu() ? CL_DEVICE_TYPE_GPU : CL_DEVICE_TYPE_CPU);
		std::string name = "test program";
		meta::print_info_hmc(name.c_str(), inputfile);
		logger.info() << "gaugeobservables: ";
		this->print_gaugeobservables_from_task(0, 0);
	};
	virtual void init_tasks() override;
	virtual void finalize_opencl() override;

	Opencl_Module * get_device();
};

void TestGaugefield::init_tasks()
{
	opencl_modules = new Opencl_Module* [get_num_tasks()];
	//here we want to test Opencl_Module
	opencl_modules[0] = new Opencl_Module(get_parameters(), get_device_for_task(0));
	opencl_modules[0]->init();
}

Opencl_Module* TestGaugefield::get_device()
{
	return static_cast<Opencl_Module*>(opencl_modules[0]);
}

void TestGaugefield::finalize_opencl()
{
	Gaugefield_hybrid::finalize_opencl();
}


void test_rectangles(std::string inputfile)
{
	std::string kernelName = "rectangles";
	printKernelInfo(kernelName);
	logger.info() << "Init device";
	meta::Inputparameters params = create_parameters(inputfile);
	hardware::System system(params);
	TestGaugefield cpu(&system);
	Opencl_Module * device = cpu.get_device();

	logger.info() << "calc rectangles value:";
	hmc_float cpu_rect;
	device->gaugeobservables_rectangles(device->get_gaugefield(), &cpu_rect);
	logger.info() << cpu_rect;

	logger.info() << "Finalize device";
	cpu.finalize();

	testFloatAgainstInputparameters(cpu_rect, params);
	BOOST_MESSAGE("Test done");
}

void test_plaquette(std::string inputfile)
{
	std::string kernelName = "plaquette";
	printKernelInfo(kernelName);
	logger.info() << "Init device";
	meta::Inputparameters params = create_parameters(inputfile);
	hardware::System system(params);
	TestGaugefield dummy(&system);

	// get reference solutions
	hmc_float ref_plaq, ref_tplaq, ref_splaq;
	ref_plaq = dummy.plaquette(&ref_tplaq, &ref_splaq);

	// get device colutions
	hmc_float dev_plaq, dev_tplaq, dev_splaq;
	hmc_complex dev_pol;
	Opencl_Module * device = dummy.get_device();
	device->gaugeobservables(&dev_plaq, &dev_tplaq, &dev_splaq, &dev_pol);

	logger.info() << "Finalize device";
	dummy.finalize();

	logger.info() << "reference value:\t" << "values obtained from host functionality";
	hmc_float prec = params.get_solver_prec();
	logger.info() << "acceptance precision: " << prec;

	// verify
	BOOST_REQUIRE_CLOSE(ref_plaq,   dev_plaq,     prec);
	BOOST_REQUIRE_CLOSE(ref_tplaq,  dev_tplaq,    prec);
	BOOST_REQUIRE_CLOSE(ref_splaq,  dev_splaq,    prec);
}

void test_polyakov(std::string inputfile)
{
	std::string kernelName = "plaquette";
	printKernelInfo(kernelName);
	logger.info() << "Init device";
	meta::Inputparameters params = create_parameters(inputfile);
	hardware::System system(params);
	TestGaugefield dummy(&system);

	// get reference solutions
	hmc_complex ref_pol = dummy.polyakov();

	// get device colutions
	hmc_float dev_plaq, dev_tplaq, dev_splaq;
	hmc_complex dev_pol;
	Opencl_Module * device = dummy.get_device();
	device->gaugeobservables(&dev_plaq, &dev_tplaq, &dev_splaq, &dev_pol);

	logger.info() << "Finalize device";
	dummy.finalize();

	logger.info() << "reference value:\t" << "values obtained from host functionality";
	hmc_float prec = params.get_solver_prec();
	logger.info() << "acceptance precision: " << prec;

	// verify
	BOOST_REQUIRE_CLOSE(ref_pol.re, dev_pol.re,   prec);
	BOOST_REQUIRE_CLOSE(ref_pol.im, dev_pol.im,   prec);
}

BOOST_AUTO_TEST_SUITE ( PLAQUETTE )

BOOST_AUTO_TEST_CASE( PLAQUETTE_1 )
{
	test_plaquette( "/plaquette_input_1" );
}

BOOST_AUTO_TEST_CASE( PLAQUETTE_2 )
{
	test_plaquette( "/plaquette_input_2" );
}

BOOST_AUTO_TEST_CASE( PLAQUETTE_3 )
{
	test_plaquette( "/plaquette_input_3" );
}

BOOST_AUTO_TEST_SUITE_END()

BOOST_AUTO_TEST_SUITE ( POLYAKOV )

BOOST_AUTO_TEST_CASE( POLYAKOV_1 )
{
	test_polyakov( "/polyakov_input_1" );
}

BOOST_AUTO_TEST_CASE( POLYAKOV_2)
{
	test_polyakov( "/polyakov_input_2");
}

BOOST_AUTO_TEST_CASE( POLYAKOV_3)
{
	test_polyakov( "/polyakov_input_3");
}

BOOST_AUTO_TEST_SUITE_END()

BOOST_AUTO_TEST_SUITE ( CONVERT_GAUGEFIELD_TO_SOA )

BOOST_AUTO_TEST_CASE( CONVERT_GAUGEFIELD_TO_SOA_1 )
{
	BOOST_MESSAGE("NOT YET IMPLEMENTED!");
}

BOOST_AUTO_TEST_SUITE_END()

BOOST_AUTO_TEST_SUITE ( CONVERT_GAUGEFIELD_FROM_SOA )

BOOST_AUTO_TEST_CASE( CONVERT_GAUGEFIELD_FROM_SOA_1 )
{
	BOOST_MESSAGE("NOT YET IMPLEMENTED!");
}

BOOST_AUTO_TEST_SUITE_END()

BOOST_AUTO_TEST_SUITE ( STOUT_SMEAR )

BOOST_AUTO_TEST_CASE( STOUT_SMEAR_1 )
{
	BOOST_MESSAGE("NOT YET IMPLEMENTED!");
}

BOOST_AUTO_TEST_SUITE_END()

BOOST_AUTO_TEST_SUITE ( RECTANGLES )

BOOST_AUTO_TEST_CASE( RECTANGLES_1 )
{
	test_rectangles("/rectangles_input_1");
}

BOOST_AUTO_TEST_CASE( RECTANGLES_2 )
{
	test_rectangles("/rectangles_input_2");
}

BOOST_AUTO_TEST_SUITE_END()

