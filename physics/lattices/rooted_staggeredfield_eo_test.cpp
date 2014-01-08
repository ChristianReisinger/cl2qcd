/** @file
 * Unit test for the physics::lattices::Rooted_Staggeredfield_eo class
 *
 * Copyright (c) 2013 Alessandro Sciarra <sciarra@th.physik.uni-frankfurt.de>
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

#include "rooted_staggeredfield_eo.hpp"
#include "util.hpp"

// use the boost test framework
#define BOOST_TEST_DYN_LINK
#define BOOST_TEST_MODULE physics::lattice::Rooted_Staggeredfield_eo
#include <boost/test/unit_test.hpp>

#include "../../logger.hpp"
#include "../../meta/type_ops.hpp"
#include <cmath>


BOOST_AUTO_TEST_CASE(initialization)
{
	using namespace physics::lattices;

	const char * _params[] = {"foo"};
	meta::Inputparameters params(1, _params);
	hardware::System system(params);
	logger.debug() << "Devices: " << system.get_devices().size();

	Rooted_Staggeredfield_eo sf(system);
	physics::algorithms::Rational_Approximation approx(3,1,4,1e-5,1);
	Rooted_Staggeredfield_eo sf2(approx, system);
	
}


BOOST_AUTO_TEST_CASE(rescale)
{
	using namespace physics::algorithms;
	using namespace physics::lattices;
	
	Rational_Approximation approx(15,1,4,1e-5,1,false);
	
	const char * _params[] = {"foo", "--ntime=4", "--fermact=rooted_stagg"};
	meta::Inputparameters params(3, _params);
	hardware::System system(params);
	physics::PRNG prng(system);
	
	//Operator for the test
	physics::fermionmatrix::MdagM_eo matrix(system, 0.567);
	//This configuration for the Ref.Code is the same as for example dks_input_5
	Gaugefield gf(system, prng, std::string(SOURCEDIR) + "/tests/conf.00200");
	Rooted_Staggeredfield_eo sf(system);
	
	//Reference rescaled coefficients
	hmc_float a0_ref = 3.78396627036665123;
	hmc_float a_ref[15] = {-2.722986658932525683e-07, -1.2631475639728917521e-06,
	                       -4.360435755914117958e-06, -1.4160691433260296987e-05,
			       -4.5207926211912900696e-05, -0.00014352921651252466598,
			       -0.00045497933178524781715, -0.0014431546521933168083,
			       -0.0045926953908840342788, -0.014747783330565073998,
			       -0.048456946841957317107, -0.16880046472141346792,
			       -0.68431552061715394952, -4.2198332136416603078,
			       -117.03837887995429412};
	hmc_float b_ref[15] = {9.2907369101588763806e-06, 5.0627210699159516975e-05,
	                       0.00016198783558680096995, 0.00044474583457844121164,
			       0.0011563267279465730842, 0.0029447969579766914219,
			       0.007440491639209977949, 0.018751086281155571189,
			       0.047271072978110562079, 0.11959352757092110708,
			       0.30563294009891067704, 0.80189441104810432748,
			       2.2562393220051499831, 7.7746421837770229857,
			       59.252309299420609534};
	//Reference rescaled coefficients conservative
	hmc_float a0_ref_cons = 3.8304052181004228927;
	hmc_float a_ref_cons[15] = {-2.8942286130576286221e-07, -1.3425838169907005556e-06,
				    -4.6346528686679587346e-06, -1.5051222595004849162e-05,
				    -4.8050941840019680289e-05, -0.00015255541700051565286,
				    -0.00048359186633654911497, -0.0015339111096058541221,
				    -0.0048815187425726713766, -0.015675235262161982264,
				    -0.051504285410779379606, -0.17941591204330820108,
				    -0.72735044574402074602, -4.4852081772741634325,
				    -124.39863554566063897};
	hmc_float b_ref_cons[15] = {9.7552862512145591676e-06, 5.3158639325027219212e-05,
				    0.00017008744523117717558, 0.00046698372446689161283,
				    0.0012141446195419417376, 0.003092040766471044512,
				    0.007812526228236587808, 0.019688665814428581158,
				    0.04963469020409881638, 0.12557336479641045823,
				    0.32091499816392937694, 0.84199021010590602287,
				    2.3690543226274733968, 8.1633847494467222106,
				    62.215004455600926292};
	
	int ord = sf.Get_order();

	sf.Rescale_Coefficients(approx, matrix, gf, system, 1.e-3);
	BOOST_CHECK_CLOSE(sf.Get_a0(), a0_ref, 5.e-5);
	std::vector<hmc_float> a = sf.Get_a();
	std::vector<hmc_float> b = sf.Get_b();
	
	sf.Rescale_Coefficients(approx, matrix, gf, system, 1.e-3, true);
	BOOST_CHECK_CLOSE(sf.Get_a0(), a0_ref_cons, 5.e-5);
	std::vector<hmc_float> a_cons = sf.Get_a();
	std::vector<hmc_float> b_cons = sf.Get_b();
	
	//Test result: note that the precision is not so high since
	//the reference code uses a slightly different method to calculate
	//maximum and minimum eigenvalues (I tuned a bit the ref.code adapting the number
	//of loop iterations in finding the max and min eigenvalues, but not too much)
	for(int i=0; i<ord; i++){
		BOOST_CHECK_CLOSE(a[i], a_ref[i], 5.e-5);
		BOOST_CHECK_CLOSE(b[i], b_ref[i], 5.e-5);
		BOOST_CHECK_CLOSE(a_cons[i], a_ref_cons[i], 2.e-4);
		BOOST_CHECK_CLOSE(b_cons[i], b_ref_cons[i], 2.e-4);
	}
	
	logger.info() << "Test done!";
}