/*
 * Copyright 2012, 2013, 2014 Lars Zeidlewicz, Christopher Pinke,
 * Matthias Bach, Christian Schäfer, Stefano Lottini, Alessandro Sciarra
 *
 * This file is part of CL2QCD.
 *
 * CL2QCD is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * CL2QCD is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with CL2QCD.  If not, see <http://www.gnu.org/licenses/>.
 */

#define BOOST_TEST_DYN_LINK
#define BOOST_TEST_MODULE HARDWARE_CODE_MOLECULAR_DYNAMICS

#include "GaugemomentumTester.hpp"
#include "SpinorTester.hpp"
#include "molecular_dynamics.hpp"

#include "../physics/lattices/gaugefield.hpp"

class MolecularDynamicsTester : public GaugemomentumTester
{
public:
	MolecularDynamicsTester(std::string kernelName, std::string inputfileIn, int numberOfValues = 1, int typeOfComparision = 1) :
		GaugemomentumTester(kernelName, getSpecificInputfile(inputfileIn), numberOfValues, typeOfComparision)
		{
			gaugefieldCode = device->get_gaugefield_code();
			molecularDynamicsCode = device->get_molecular_dynamics_code();
			
			gaugefield = new physics::lattices::Gaugefield(*system, *prng);
		}
		
		~MolecularDynamicsTester()
		{
			molecularDynamicsCode = NULL;
			gaugefieldCode = NULL;
		}
		
protected:
	std::string getSpecificInputfile(std::string inputfileIn)
	{
		//todo: this is ugly, find a better solution.
		// The problem is that the parent class calls a similar fct.
		return "../molecularDynamics/" + inputfileIn;
	}
	
	const hardware::buffers::SU3* getGaugefieldBuffer() {
		return gaugefield->get_buffers()[0];
	}
	
	const hardware::code::Molecular_Dynamics * molecularDynamicsCode;
	const hardware::code::Gaugefield * gaugefieldCode;
	
	physics::PRNG * prng;
	physics::lattices::Gaugefield * gaugefield;
};

BOOST_AUTO_TEST_SUITE(BUILD)

	BOOST_AUTO_TEST_CASE( BUILD_1 )
	{
		MolecularDynamicsTester tester("build", "molecular_dynamics_build_input_1");
	}

BOOST_AUTO_TEST_SUITE_END()

BOOST_AUTO_TEST_SUITE( GF_UPDATE )

	class GaugefieldUpdateTester : public MolecularDynamicsTester
	{
	public:
		GaugefieldUpdateTester(std::string inputfile) : 
			MolecularDynamicsTester("md_update_gaugefield", inputfile)
			{
				code->importGaugemomentumBuffer(gaugemomentumBuffer, reinterpret_cast<ae*>( createGaugemomentum() ));
				double epsilon = parameters->get_tau();
				molecularDynamicsCode->md_update_gaugefield_device(gaugemomentumBuffer, getGaugefieldBuffer(), epsilon);
				
				const hardware::buffers::Plain<hmc_float> plaq(1, device );
				const hardware::buffers::Plain<hmc_float> splaq(1, device);
				const hardware::buffers::Plain<hmc_float> tplaq(1, device);

				gaugefieldCode->plaquette_device(getGaugefieldBuffer(), &plaq, &tplaq, &splaq);
				plaq.dump(&kernelResult[0]);
			}
	};

	BOOST_AUTO_TEST_CASE(GF_UPDATE_1 )
	{
		GaugefieldUpdateTester tester("gf_update_input_1");
	}

	BOOST_AUTO_TEST_CASE( GF_UPDATE_2 )
	{
		GaugefieldUpdateTester tester("gf_update_input_2");
	}

	BOOST_AUTO_TEST_CASE( GF_UPDATE_3 )
	{
		GaugefieldUpdateTester tester("gf_update_input_3");
	}

	BOOST_AUTO_TEST_CASE( GF_UPDATE_4 )
	{
		GaugefieldUpdateTester tester("gf_update_input_4");
	}

	BOOST_AUTO_TEST_CASE(GF_UPDATE_5 )
	{
		BOOST_MESSAGE("THIS TEST HAS TO BE INVESTIGATED!! IT COULD HINT TO AN ERROR IN THE FUNCTION!");
		GaugefieldUpdateTester tester("gf_update_input_5");
	}

	BOOST_AUTO_TEST_CASE(GF_UPDATE_6 )
	{
		GaugefieldUpdateTester tester("gf_update_input_6");
	}

BOOST_AUTO_TEST_SUITE_END()

BOOST_AUTO_TEST_SUITE( F_GAUGE )

	class FGaugeTester : public MolecularDynamicsTester
	{
	public:
		FGaugeTester(std::string inputfile) : 
			MolecularDynamicsTester("f_gauge", inputfile)
			{
				code->importGaugemomentumBuffer(gaugemomentumBuffer, reinterpret_cast<ae*>( createGaugemomentumBasedOnFilltype(zero) ));
				molecularDynamicsCode->gauge_force_device( getGaugefieldBuffer(), gaugemomentumBuffer);
				calcSquarenormAndStoreAsKernelResult(gaugemomentumBuffer);
			}
	};

	BOOST_AUTO_TEST_CASE( F_GAUGE_1 )
	{
		FGaugeTester tester("/f_gauge_input_1");
	}

	BOOST_AUTO_TEST_CASE( F_GAUGE_2 )
	{
		FGaugeTester tester("f_gauge_input_2");
	}

BOOST_AUTO_TEST_SUITE_END()

BOOST_AUTO_TEST_SUITE( F_GAUGE_TLSYM )

	class FGaugeTlsymTester : public MolecularDynamicsTester
	{
	public:
		FGaugeTlsymTester(std::string inputfile) : 
			MolecularDynamicsTester("f_gauge_tlsym", inputfile)
			{
				code->importGaugemomentumBuffer(gaugemomentumBuffer, reinterpret_cast<ae*>( createGaugemomentumBasedOnFilltype(zero) ));
				molecularDynamicsCode->gauge_force_tlsym_device( getGaugefieldBuffer(), gaugemomentumBuffer);
				calcSquarenormAndStoreAsKernelResult(gaugemomentumBuffer);
			}
	};

	BOOST_AUTO_TEST_CASE( F_GAUGE_TLSYM_1 )
	{
		FGaugeTlsymTester tester("f_gauge_tlsym_input_1");
	}

	BOOST_AUTO_TEST_CASE( F_GAUGE_TLSYM_2 )
	{
		FGaugeTlsymTester tester("f_gauge_tlsym_input_2");
	}

	BOOST_AUTO_TEST_CASE( F_GAUGE_TLSYM_3 )
	{
		FGaugeTlsymTester tester("f_gauge_tlsym_input_3");
	}

BOOST_AUTO_TEST_SUITE_END()

BOOST_AUTO_TEST_SUITE( STOUT_SMEAR_FERMION_FORCE )

	BOOST_AUTO_TEST_CASE(STOUT_SMEAR_FERMION_FORCE_1 )
	{
		BOOST_MESSAGE("NOT YET IMPLEMENTED!!");
	}

BOOST_AUTO_TEST_SUITE_END()

BOOST_AUTO_TEST_SUITE( F_FERMION )

	class FFermionTester : public MolecularDynamicsTester, public SpinorTester
	{
	public:
		FFermionTester(std::string inputfile) : 
			MolecularDynamicsTester("f_fermion", inputfile), SpinorTester(MolecularDynamicsTester::parameters, MolecularDynamicsTester::system, MolecularDynamicsTester::device)
			{
				MolecularDynamicsTester::code->importGaugemomentumBuffer(gaugemomentumBuffer, reinterpret_cast<ae*>( createGaugemomentumBasedOnFilltype(zero) ));
				const hardware::buffers::Plain<spinor> in1(SpinorTester::spinorfieldElements, MolecularDynamicsTester::device);
				const hardware::buffers::Plain<spinor> in2(SpinorTester::spinorfieldElements, MolecularDynamicsTester::device);

				in1.load(SpinorTester::createSpinorfield(SpinorTester::spinorfieldElements, 123456) );
				in2.load(SpinorTester::createSpinorfield(SpinorTester::spinorfieldElements, 789101) );
				
				molecularDynamicsCode->fermion_force_device( &in1, &in2, getGaugefieldBuffer(), gaugemomentumBuffer, MolecularDynamicsTester::parameters->get_kappa());
				MolecularDynamicsTester::calcSquarenormAndStoreAsKernelResult(gaugemomentumBuffer);
			}
	};

	BOOST_AUTO_TEST_CASE( F_FERMION_1 )
	{
		FFermionTester tester("f_fermion_input_1");
	}

	BOOST_AUTO_TEST_CASE( F_FERMION_2 )
	{
		FFermionTester tester("f_fermion_input_2");
	}

	BOOST_AUTO_TEST_CASE( F_FERMION_3 )
	{
		FFermionTester tester("f_fermion_input_3");
	}

	BOOST_AUTO_TEST_CASE( F_FERMION_4 )
	{
		FFermionTester tester("f_fermion_input_4");
	}

	BOOST_AUTO_TEST_CASE( F_FERMION_5 )
	{
		FFermionTester tester("f_fermion_input_5");
	}

	BOOST_AUTO_TEST_CASE( F_FERMION_6 )
	{
		FFermionTester tester("f_fermion_input_6");
	}

BOOST_AUTO_TEST_SUITE_END()

BOOST_AUTO_TEST_SUITE( F_FERMION_EO )

	class FFermionEvenOddTester : public MolecularDynamicsTester, public SpinorTester
	{
	public:
		FFermionEvenOddTester(std::string inputfile) : 
			MolecularDynamicsTester("f_fermion_eo", inputfile), SpinorTester(MolecularDynamicsTester::parameters, MolecularDynamicsTester::system, MolecularDynamicsTester::device)
			{
				MolecularDynamicsTester::code->importGaugemomentumBuffer(gaugemomentumBuffer, reinterpret_cast<ae*>( createGaugemomentumBasedOnFilltype(zero) ));
				const hardware::buffers::Spinor in1(SpinorTester::spinorfieldEvenOddElements, MolecularDynamicsTester::device);
				const hardware::buffers::Spinor in2(SpinorTester::spinorfieldEvenOddElements, MolecularDynamicsTester::device);
				fillTwoSpinorBuffers(&in1, &in2);
				
				int tmp = ( MolecularDynamicsTester::parameters->get_read_multiple_configs() ) ? EVEN : ODD;
				molecularDynamicsCode->fermion_force_eo_device( &in1, &in2, getGaugefieldBuffer(), gaugemomentumBuffer, tmp,  MolecularDynamicsTester::parameters->get_kappa());
				MolecularDynamicsTester::calcSquarenormAndStoreAsKernelResult(gaugemomentumBuffer);
			}
	};
	
	BOOST_AUTO_TEST_CASE( F_FERMION_EO_1 )
	{
		FFermionEvenOddTester tester("f_fermion_eo_input_1");
	}

	BOOST_AUTO_TEST_CASE( F_FERMION_EO_2 )
	{
		FFermionEvenOddTester tester("f_fermion_eo_input_2");
	}

	BOOST_AUTO_TEST_CASE( F_FERMION_EO_3 )
	{
		FFermionEvenOddTester tester("f_fermion_eo_input_3");
	}

	BOOST_AUTO_TEST_CASE( F_FERMION_EO_4 )
	{
		FFermionEvenOddTester tester("f_fermion_eo_input_4");
	}

	BOOST_AUTO_TEST_CASE( F_FERMION_EO_5 )
	{
		FFermionEvenOddTester tester("f_fermion_eo_input_5");
	}

	BOOST_AUTO_TEST_CASE( F_FERMION_EO_6 )
	{
		FFermionEvenOddTester tester("f_fermion_eo_input_6");
	}

	BOOST_AUTO_TEST_CASE( F_FERMION_EO_7 )
	{
		FFermionEvenOddTester tester("f_fermion_eo_input_7");
	}

	BOOST_AUTO_TEST_CASE( F_FERMION_EO_8 )
	{
		FFermionEvenOddTester tester("f_fermion_eo_input_8");
	}

	BOOST_AUTO_TEST_CASE( F_FERMION_EO_9 )
	{
		FFermionEvenOddTester tester("f_fermion_eo_input_9");
	}

	BOOST_AUTO_TEST_CASE( F_FERMION_EO_10 )
	{
		FFermionEvenOddTester tester("f_fermion_eo_input_10");
	}

	BOOST_AUTO_TEST_CASE( F_FERMION_EO_11)
	{
		FFermionEvenOddTester tester("f_fermion_eo_input_11");
	}

	BOOST_AUTO_TEST_CASE( F_FERMION_EO_12 )
	{
		FFermionEvenOddTester tester("f_fermion_eo_input_12");
	}

	BOOST_AUTO_TEST_CASE( F_FERMION_EO_13)
	{
		FFermionEvenOddTester tester("f_fermion_eo_input_13");
	}

	BOOST_AUTO_TEST_CASE( F_FERMION_EO_14 )
	{
		FFermionEvenOddTester tester("f_fermion_eo_input_14");
	}

	BOOST_AUTO_TEST_CASE( F_FERMION_EO_15 )
	{
		FFermionEvenOddTester tester("f_fermion_eo_input_15");
	}

	BOOST_AUTO_TEST_CASE( F_FERMION_EO_16 )
	{
		FFermionEvenOddTester tester("f_fermion_eo_input_16");
	}

	BOOST_AUTO_TEST_CASE( F_FERMION_EO_17 )
	{
		FFermionEvenOddTester tester("f_fermion_eo_input_17");
	}

	BOOST_AUTO_TEST_CASE( F_FERMION_EO_18 )
	{
		FFermionEvenOddTester tester("f_fermion_eo_input_18");
	}

	BOOST_AUTO_TEST_CASE( F_FERMION_EO_19 )
	{
		FFermionEvenOddTester tester("f_fermion_eo_input_19");
	}

	BOOST_AUTO_TEST_CASE( F_FERMION_EO_20 )
	{
		FFermionEvenOddTester tester("f_fermion_eo_input_20");
	}

BOOST_AUTO_TEST_SUITE_END()

BOOST_AUTO_TEST_SUITE( F_FERMION_COMPARE_NONEO_EO )

	class FFermionEvenOddComparator : public MolecularDynamicsTester, public SpinorTester
	{
	public:
		FFermionEvenOddComparator(std::string inputfile) : 
			MolecularDynamicsTester("f_fermion compare even-odd and non-even-odd", inputfile, 2, 3), SpinorTester(MolecularDynamicsTester::parameters, MolecularDynamicsTester::system, MolecularDynamicsTester::device)
			{
				createBuffers();
				fillBuffers();
	
				hmc_float cpu_res_eo;
				molecularDynamicsCode->fermion_force_eo_device(inEo1, inEo4, getGaugefieldBuffer(), outEo, ODD, MolecularDynamicsTester::parameters->get_kappa() );
				MolecularDynamicsTester::code->set_float_to_gaugemomentum_squarenorm_device(outEo, MolecularDynamicsTester::doubleBuffer);
				MolecularDynamicsTester::doubleBuffer->dump(&cpu_res_eo);
				logger.info() << "|force_eo (even) + force_eo (odd)|^2:";
				logger.info() << cpu_res_eo;
				
				molecularDynamicsCode->fermion_force_eo_device(inEo2, inEo3, getGaugefieldBuffer(), outEo, EVEN, MolecularDynamicsTester::parameters->get_kappa() );
				MolecularDynamicsTester::code->set_float_to_gaugemomentum_squarenorm_device(outEo, MolecularDynamicsTester::doubleBuffer);
				MolecularDynamicsTester::doubleBuffer->dump(&cpu_res_eo);
				logger.info() << "|force_eo (even) + force_eo (odd)|^2:";
				logger.info() << cpu_res_eo;
	
				MolecularDynamicsTester::calcSquarenormAndStoreAsKernelResult(outEo, 0);
				
				molecularDynamicsCode->fermion_force_device( inNonEo1, inNonEo2, getGaugefieldBuffer(), outNonEo, MolecularDynamicsTester::parameters->get_kappa());
				MolecularDynamicsTester::calcSquarenormAndStoreAsKernelResult(outNonEo, 1);
			}
			~FFermionEvenOddComparator()
			{
				delete	outNonEo;
				delete	outEo; 
				delete	inNonEo1;
				delete	inNonEo2;
				delete	inEo1;
				delete	inEo2; 
				delete	inEo3;
				delete	inEo4; 
				
				outNonEo = NULL;
				outEo = NULL;
				inNonEo1 = NULL;
				inNonEo2 = NULL;
				inEo1 = NULL;
				inEo2 = NULL;
				inEo3 = NULL;
				inEo4 = NULL;
			}
			
	private:
		void createBuffers()
		{
			outNonEo = new const hardware::buffers::Gaugemomentum(MolecularDynamicsTester::numberOfGaugemomentumElements, MolecularDynamicsTester::device);
			outEo = new const hardware::buffers::Gaugemomentum(MolecularDynamicsTester::numberOfGaugemomentumElements, MolecularDynamicsTester::device);
			inNonEo1 = new const hardware::buffers::Plain<spinor>(SpinorTester::spinorfieldElements, MolecularDynamicsTester::device);
			inNonEo2 = new const hardware::buffers::Plain<spinor>(SpinorTester::spinorfieldElements, MolecularDynamicsTester::device);
			inEo1 = new const hardware::buffers::Spinor(SpinorTester::spinorfieldEvenOddElements, MolecularDynamicsTester::device);
			inEo2 = new const hardware::buffers::Spinor(SpinorTester::spinorfieldEvenOddElements, MolecularDynamicsTester::device);
			inEo3 = new const hardware::buffers::Spinor(SpinorTester::spinorfieldEvenOddElements, MolecularDynamicsTester::device);
			inEo4 = new const hardware::buffers::Spinor(SpinorTester::spinorfieldEvenOddElements, MolecularDynamicsTester::device);
		}
		void fillBuffers()
		{
			MolecularDynamicsTester::code->importGaugemomentumBuffer(outNonEo, reinterpret_cast<ae*>( createGaugemomentumBasedOnFilltype(zero) ));
			MolecularDynamicsTester::code->importGaugemomentumBuffer(outEo, reinterpret_cast<ae*>( createGaugemomentumBasedOnFilltype(zero) ));
			
			inNonEo1->load(SpinorTester::createSpinorfield(SpinorTester::spinorfieldElements, 123456 ));
			inNonEo2->load(SpinorTester::createSpinorfield(SpinorTester::spinorfieldElements, 789101 ));
			fillTwoSpinorBuffers(inEo1, inEo2, 123456);
			fillTwoSpinorBuffers(inEo3, inEo4, 789101);
			
			//in case of rnd input, it is nontrivial to supply the same rnd vectors as eo and noneo input.
			//therefore, simply convert the eo input back to noneo
			if(SpinorTester::useRandom)
			{
				if (SpinorTester::evenOrOdd)
				{
					SpinorTester::code->convert_from_eoprec_device(inEo1, inEo2, inNonEo1);
					SpinorTester::code->convert_from_eoprec_device(inEo3, inEo4, inNonEo2);
				}
				else
				{
					SpinorTester::code->convert_to_eoprec_device(inEo1, inEo2, inNonEo1);
					SpinorTester::code->convert_to_eoprec_device(inEo3, inEo4, inNonEo2);
				}
			}
		}
			
		const hardware::buffers::Gaugemomentum * outNonEo;
		const hardware::buffers::Gaugemomentum * outEo;
		const hardware::buffers::Plain<spinor> * inNonEo1;
		const hardware::buffers::Plain<spinor> * inNonEo2;
		const hardware::buffers::Spinor * inEo1;
		const hardware::buffers::Spinor * inEo2;
		const hardware::buffers::Spinor * inEo3;
		const hardware::buffers::Spinor * inEo4;
	};

	BOOST_AUTO_TEST_CASE( F_FERMION_COMPARE_NONEO_EO_1 )
	{
		FFermionEvenOddComparator tester("f_fermion_compare_noneo_eo_input_1");
	}

	BOOST_AUTO_TEST_CASE( F_FERMION_COMPARE_NONEO_EO_2 )
	{
		FFermionEvenOddComparator tester("f_fermion_compare_noneo_eo_input_2");
	}

	BOOST_AUTO_TEST_CASE( F_FERMION_COMPARE_NONEO_EO_3 )
	{
		FFermionEvenOddComparator tester("f_fermion_compare_noneo_eo_input_3");
	}

	BOOST_AUTO_TEST_CASE( F_FERMION_COMPARE_NONEO_EO_4 )
	{
		FFermionEvenOddComparator tester("f_fermion_compare_noneo_eo_input_4");
	}

	BOOST_AUTO_TEST_CASE( F_FERMION_COMPARE_NONEO_EO_5 )
	{
		FFermionEvenOddComparator tester("f_fermion_compare_noneo_eo_input_5");
	}

	BOOST_AUTO_TEST_CASE( F_FERMION_COMPARE_NONEO_EO_6 )
	{
		FFermionEvenOddComparator tester("f_fermion_compare_noneo_eo_input_6");
	}

BOOST_AUTO_TEST_SUITE_END()

/////////////////////////////////////////////////////////////////////////
//    TESTS FOR STAGGERED FERMIONS MOLECULAR DYNAMICS RELATED TOOLS    //
/////////////////////////////////////////////////////////////////////////

#include "../../meta/util.hpp"
#include "../../physics/lattices/gaugefield.hpp"
#include "molecular_dynamics.hpp"
#include "spinors_staggered.hpp"

#include "test_util.h"
#include "test_util_staggered.h"

#include "../../physics/observables/gaugeObservables.h"

class TestMolecularDynamics {

public:
	TestMolecularDynamics(const hardware::System * system) : system(system), prng(*system), gf(*system, prng) {
		BOOST_REQUIRE_EQUAL(system->get_devices().size(), 1);
		auto inputfile = system->get_inputparameters();
		meta::print_info_hmc(inputfile);
	};

	const hardware::code::Molecular_Dynamics * get_device();
	const hardware::buffers::SU3 * get_gaugefield();

	void print_gaugeobservables() {
	  physics::gaugeObservables obs(&system->get_inputparameters() );
	  obs.measureGaugeObservables(&gf, 0);
	}

private:
	const hardware::System * const system;
	physics::PRNG prng;
	const physics::lattices::Gaugefield gf;
};

const hardware::code::Molecular_Dynamics* TestMolecularDynamics::get_device()
{
	return system->get_devices()[0]->get_molecular_dynamics_code();
}

const hardware::buffers::SU3 * TestMolecularDynamics::get_gaugefield()
{
	return gf.get_buffers().at(0);
}

void fill_sf_with_one(su3vec * sf_in, int size)
{
	for(int i = 0; i < size; ++i) {
		sf_in[i].e0 = hmc_complex_one;
		sf_in[i].e1 = hmc_complex_one;
		sf_in[i].e2 = hmc_complex_one;
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

void fill_sf_with_random(su3vec * sf_in, int size, int seed)
{
	prng_init(seed);
	for(int i = 0; i < size; ++i) {
		sf_in[i].e0.re = prng_double();
		sf_in[i].e1.re = prng_double();
		sf_in[i].e2.re = prng_double();
		
		sf_in[i].e0.im = prng_double();
		sf_in[i].e1.im = prng_double();
		sf_in[i].e2.im = prng_double();
	}
	return;
}

void fill_sf_with_random_eo(su3vec * sf_in1, su3vec * sf_in2, int size, int seed)
{
	prng_init(seed);
	for(int i = 0; i < size; ++i) {
		sf_in1[i].e0.re = prng_double();
		sf_in1[i].e1.re = prng_double();
		sf_in1[i].e2.re = prng_double();

		sf_in1[i].e0.im = prng_double();
		sf_in1[i].e1.im = prng_double();
		sf_in1[i].e2.im = prng_double();
		
		sf_in2[i].e0.re = prng_double();
		sf_in2[i].e1.re = prng_double();
		sf_in2[i].e2.re = prng_double();

		sf_in2[i].e0.im = prng_double();
		sf_in2[i].e1.im = prng_double();
		sf_in2[i].e2.im = prng_double();
	}
	return;
}

void test_f_stagg_fermion_eo(std::string inputfile)
{
	using namespace hardware::buffers;
	
	std::string kernelName = "fermion_staggered_partial_force_eo";
	printKernelInfo(kernelName);
	logger.info() << "Init device";
	meta::Inputparameters params = createParameters("molecularDynamics/" + inputfile);
	hardware::System system(params);
	TestMolecularDynamics cpu(&system);
	auto * device = cpu.get_device();
	
	su3vec * sf_in1;
	su3vec * sf_in2;
	ae * ae_out;
	
	logger.info() << "fill buffers";
	size_t NUM_ELEMENTS_SF =  hardware::code::get_eoprec_spinorfieldsize(params);
	size_t NUM_ELEMENTS_AE = meta::get_vol4d(params) * NDIM;

	sf_in1 = new su3vec[NUM_ELEMENTS_SF];
	sf_in2 = new su3vec[NUM_ELEMENTS_SF];
	ae_out = new ae[NUM_ELEMENTS_AE];

	//use the variable use_cg to switch between cold and random input sf
	if(params.get_solver() == meta::Inputparameters::cg) {
		fill_sf_with_one(sf_in1, NUM_ELEMENTS_SF);
		fill_sf_with_one(sf_in2, NUM_ELEMENTS_SF);
	} else {
		fill_sf_with_random(sf_in1, NUM_ELEMENTS_SF, 123); //With these seeds the fields are the same
		fill_sf_with_random(sf_in2, NUM_ELEMENTS_SF, 456); //as the test_sf_saxpy_staggered_eo
	}
	fill_with_zero(ae_out, NUM_ELEMENTS_AE);
	BOOST_REQUIRE(sf_in1);
	BOOST_REQUIRE(sf_in2);
	BOOST_REQUIRE(ae_out);
	
	auto spinor_code = device->get_device()->get_spinor_staggered_code();
	auto gm_code = device->get_device()->get_gaugemomentum_code();

	const SU3vec in1(NUM_ELEMENTS_SF, device->get_device());
	const SU3vec in2(NUM_ELEMENTS_SF, device->get_device());
	Gaugemomentum out(meta::get_vol4d(params) * NDIM, device->get_device());
	hardware::buffers::Plain<hmc_float> sqnorm(1, device->get_device());

	in1.load(sf_in1);
	in2.load(sf_in2);
	gm_code->importGaugemomentumBuffer(&out, ae_out);

	//The following seven lines are to be used to produce the ref_vec file needed to get the ref_value
        //---> Comment them out when the reference values have been obtained! 
	/*
        print_staggeredfield_eo_to_textfile("ref_vec_f_stagg1_eo", sf_in1, params); 
        logger.info() << "Produced the ref_vec_f_stagg1_eo text file with the staggered field for the ref. code."; 
	print_staggeredfield_eo_to_textfile("ref_vec_f_stagg2_eo", sf_in2, params); 
        logger.info() << "Produced the ref_vec_f_stagg2_eo text file with the staggered field for the ref. code. Returning...";   
        return;
	// */

	hmc_float cpu_res, cpu_back, cpu_back2;
	logger.info() << "|in_1|^2:";
	spinor_code->set_float_to_global_squarenorm_eoprec_device(&in1, &sqnorm);
	sqnorm.dump(&cpu_back);
	logger.info() << cpu_back;
	logger.info() << "|in_2|^2:";
	spinor_code->set_float_to_global_squarenorm_eoprec_device(&in2, &sqnorm);
	sqnorm.dump(&cpu_back2);
	logger.info() << cpu_back2;
	logger.info() << "Run kernel";

	//switch according to "read_multiple_configs"
	if(params.get_read_multiple_configs()) {
		device->fermion_staggered_partial_force_device(cpu.get_gaugefield(), &in1, &in2, &out, EVEN);
	} else {
		device->fermion_staggered_partial_force_device(cpu.get_gaugefield(), &in1, &in2, &out, ODD);
	}
	
	logger.info() << "|force|^2:";
	gm_code->set_float_to_gaugemomentum_squarenorm_device(&out, &sqnorm);
	sqnorm.dump(&cpu_res);
	logger.info() << cpu_res;

	logger.info() << "Clear buffers";
	delete[] sf_in1;
	delete[] sf_in2;
	delete[] ae_out;

	testFloatAgainstInputparameters(cpu_res, params);
	BOOST_MESSAGE("Test done");
}

///////////////////////////////////////////////////////////////////////////////
//    TESTS SUITE FOR STAGGERED FERMIONS MOLECULAR DYNAMICS RELATED TOOLS    //
///////////////////////////////////////////////////////////////////////////////

BOOST_AUTO_TEST_SUITE( F_STAGG_FERMION_EO )

BOOST_AUTO_TEST_CASE( F_STAGG_FERMION_EO_1 )
{
	test_f_stagg_fermion_eo("/f_staggered_fermion_partial_eo_input_1");
}

BOOST_AUTO_TEST_CASE( F_STAGG_FERMION_EO_2 )
{
	test_f_stagg_fermion_eo("/f_staggered_fermion_partial_eo_input_2");
}

BOOST_AUTO_TEST_CASE( F_STAGG_FERMION_EO_3 )
{
	test_f_stagg_fermion_eo("/f_staggered_fermion_partial_eo_input_3");
}

BOOST_AUTO_TEST_CASE( F_STAGG_FERMION_EO_4 )
{
	test_f_stagg_fermion_eo("/f_staggered_fermion_partial_eo_input_4");
}

BOOST_AUTO_TEST_CASE( F_STAGG_FERMION_EO_5 )
{
	test_f_stagg_fermion_eo("/f_staggered_fermion_partial_eo_input_5");
}

BOOST_AUTO_TEST_CASE( F_STAGG_FERMION_EO_6 )
{
	test_f_stagg_fermion_eo("/f_staggered_fermion_partial_eo_input_6");
}

BOOST_AUTO_TEST_CASE( F_STAGG_FERMION_EO_7 )
{
	test_f_stagg_fermion_eo("/f_staggered_fermion_partial_eo_input_7");
}

BOOST_AUTO_TEST_CASE( F_STAGG_FERMION_EO_8 )
{
	test_f_stagg_fermion_eo("/f_staggered_fermion_partial_eo_input_8");
}

BOOST_AUTO_TEST_CASE( F_STAGG_FERMION_EO_9 )
{
	test_f_stagg_fermion_eo("/f_staggered_fermion_partial_eo_input_9");
}

BOOST_AUTO_TEST_CASE( F_STAGG_FERMION_EO_10 )
{
	test_f_stagg_fermion_eo("/f_staggered_fermion_partial_eo_input_10");
}

BOOST_AUTO_TEST_CASE( F_STAGG_FERMION_EO_11 )
{
	test_f_stagg_fermion_eo("/f_staggered_fermion_partial_eo_input_11");
}

BOOST_AUTO_TEST_CASE( F_STAGG_FERMION_EO_12 )
{
	test_f_stagg_fermion_eo("/f_staggered_fermion_partial_eo_input_12");
}

BOOST_AUTO_TEST_CASE( F_STAGG_FERMION_EO_13 )
{
	test_f_stagg_fermion_eo("/f_staggered_fermion_partial_eo_input_13");
}

BOOST_AUTO_TEST_CASE( F_STAGG_FERMION_EO_14 )
{
	test_f_stagg_fermion_eo("/f_staggered_fermion_partial_eo_input_14");
}

BOOST_AUTO_TEST_CASE( F_STAGG_FERMION_EO_15 )
{
	test_f_stagg_fermion_eo("/f_staggered_fermion_partial_eo_input_15");
}

BOOST_AUTO_TEST_CASE( F_STAGG_FERMION_EO_16 )
{
	test_f_stagg_fermion_eo("/f_staggered_fermion_partial_eo_input_16");
}

BOOST_AUTO_TEST_CASE( F_STAGG_FERMION_EO_17 )
{
	test_f_stagg_fermion_eo("/f_staggered_fermion_partial_eo_input_17");
}

BOOST_AUTO_TEST_CASE( F_STAGG_FERMION_EO_18 )
{
	test_f_stagg_fermion_eo("/f_staggered_fermion_partial_eo_input_18");
}

BOOST_AUTO_TEST_CASE( F_STAGG_FERMION_EO_19 )
{
	test_f_stagg_fermion_eo("/f_staggered_fermion_partial_eo_input_19");
}

BOOST_AUTO_TEST_CASE( F_STAGG_FERMION_EO_20 )
{
	test_f_stagg_fermion_eo("/f_staggered_fermion_partial_eo_input_20");
}

BOOST_AUTO_TEST_CASE( F_STAGG_FERMION_EO_21 )
{
	test_f_stagg_fermion_eo("/f_staggered_fermion_partial_eo_input_21");
}

BOOST_AUTO_TEST_CASE( F_STAGG_FERMION_EO_22 )
{
	test_f_stagg_fermion_eo("/f_staggered_fermion_partial_eo_input_22");
}

BOOST_AUTO_TEST_CASE( F_STAGG_FERMION_EO_23 )
{
	test_f_stagg_fermion_eo("/f_staggered_fermion_partial_eo_input_23");
}

BOOST_AUTO_TEST_CASE( F_STAGG_FERMION_EO_24 )
{
	test_f_stagg_fermion_eo("/f_staggered_fermion_partial_eo_input_24");
}

BOOST_AUTO_TEST_CASE( F_STAGG_FERMION_EO_25 )
{
	test_f_stagg_fermion_eo("/f_staggered_fermion_partial_eo_input_25");
}

BOOST_AUTO_TEST_CASE( F_STAGG_FERMION_EO_26 )
{
	test_f_stagg_fermion_eo("/f_staggered_fermion_partial_eo_input_26");
}

BOOST_AUTO_TEST_CASE( F_STAGG_FERMION_EO_27 )
{
	test_f_stagg_fermion_eo("/f_staggered_fermion_partial_eo_input_27");
}

BOOST_AUTO_TEST_CASE( F_STAGG_FERMION_EO_28 )
{
	test_f_stagg_fermion_eo("/f_staggered_fermion_partial_eo_input_28");
}

BOOST_AUTO_TEST_CASE( F_STAGG_FERMION_EO_29 )
{
	test_f_stagg_fermion_eo("/f_staggered_fermion_partial_eo_input_29");
}

BOOST_AUTO_TEST_CASE( F_STAGG_FERMION_EO_30 )
{
	test_f_stagg_fermion_eo("/f_staggered_fermion_partial_eo_input_30");
}

BOOST_AUTO_TEST_CASE( F_STAGG_FERMION_EO_31 )
{
	test_f_stagg_fermion_eo("/f_staggered_fermion_partial_eo_input_31");
}

BOOST_AUTO_TEST_CASE( F_STAGG_FERMION_EO_32 )
{
	test_f_stagg_fermion_eo("/f_staggered_fermion_partial_eo_input_32");
}

BOOST_AUTO_TEST_SUITE_END()
