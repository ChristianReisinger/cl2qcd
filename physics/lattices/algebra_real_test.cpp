/** @file
 * Unit test for the real algebra operations in physics::lattices
 *
 * Copyright 2012, 2013 Lars Zeidlewicz, Christopher Pinke,
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

#include "algebra_real.hpp"
#include "../../host_functionality/logger.hpp"
#include "../../meta/type_ops.hpp"


// use the boost test framework
#define BOOST_TEST_DYN_LINK
#define BOOST_TEST_MODULE physics::lattices::algebra_real
#include <boost/test/unit_test.hpp>
#include <boost/test/floating_point_comparison.hpp>


void test_cgm_update(int switcher)
{
	//switch is to select among alpha, beta and zeta
	using namespace physics::lattices;

	const char * _params[] = {"foo"};
	meta::Inputparameters params(1, _params);
	hardware::System system(params);
	logger.debug() << "Devices: " << system.get_devices().size();
	
	const int numeq = 16;
	Vector<hmc_float> out(numeq, system);
	Vector<hmc_float> v1(numeq, system);
	Vector<hmc_float> v2(numeq, system);
	Vector<hmc_float> v3(numeq, system);
	Scalar<hmc_float> s1(system);
	Scalar<hmc_float> s2(system);
	Scalar<hmc_float> s3(system);
	
	std::vector<hmc_float> ref_v1(16, 1.);
	std::vector<hmc_float> ref_v2(16, 2.);
	std::vector<hmc_float> ref_v3(16, 3.);
	std::vector<hmc_float> out_got(16);
	hmc_float scalar = 3.14;
	v1.store(ref_v1);
	v2.store(ref_v2);
	v3.store(ref_v3);
	s1.store(scalar);
	s2.store(scalar);
	s3.store(scalar);
	
	if(switcher == 0){ //zeta
	  update_zeta_cgm(&out, v1, v2, s1, s2, s3, v3, numeq);
	  out_got = out.get();
	  for(int i=0; i<numeq; i++)
	    BOOST_REQUIRE_CLOSE(out_got[i], -0.145985401460, 1.e-8);
	}
	if(switcher == 1){ //beta
	  update_beta_cgm(&out, s1, v1, v2, numeq);
	  out_got = out.get();
	  for(int i=0; i<numeq; i++)
	    BOOST_REQUIRE_CLOSE(out_got[i], 1.57, 1.e-8);
	}
	if(switcher == 2){ //alpha
	  update_alpha_cgm(&out, s1, v1, v2, v3, s2, numeq);
	  out_got = out.get();
	  for(int i=0; i<numeq; i++)
	    BOOST_REQUIRE_CLOSE(out_got[i], 0.666666666667, 1.e-8);
	}
	
}


BOOST_AUTO_TEST_CASE(update_zeta)
{
	test_cgm_update(0);
}

BOOST_AUTO_TEST_CASE(update_beta)
{
	test_cgm_update(1);
}

BOOST_AUTO_TEST_CASE(update_alpha)
{
	test_cgm_update(2);
}