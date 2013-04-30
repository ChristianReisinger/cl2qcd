/** @file
 * Tests of the fermionmatrix implementations
 */

#include "fermionmatrix.hpp"

#include "../lattices/util.hpp"
#include <boost/type_traits.hpp>
#include <boost/utility.hpp>

// use the boost test framework
#define BOOST_TEST_DYN_LINK
#define BOOST_TEST_MODULE physics::fermionmatrix::<fermionmatrix>
#include <boost/test/unit_test.hpp>

//template<class FERMIONMATRIX> void test_fermionmatrix(const hmc_float refs[4], const int seed);
template<class FERMIONMATRIX>
typename boost::enable_if<boost::is_base_of<physics::fermionmatrix::Fermionmatrix, FERMIONMATRIX>, void>::type
test_fermionmatrix(const hmc_float refs[4], const int seed);
template<class FERMIONMATRIX>
typename boost::enable_if<boost::is_base_of<physics::fermionmatrix::Fermionmatrix_eo, FERMIONMATRIX>, void>::type
test_fermionmatrix(const hmc_float refs[4], const int seed);

BOOST_AUTO_TEST_CASE(M)
{
	const hmc_float refs[4] = {2610.3804893063798, 4356.332327032359, 2614.2685771909237, 4364.1408252701831};
	test_fermionmatrix<physics::fermionmatrix::M>(refs, 1);
}
BOOST_AUTO_TEST_CASE(Qplus)
{
	const hmc_float refs[4] = {2572.0832640524568, 4319.4350920404331, 2536.3229189621006, 4253.3268746160957};
	test_fermionmatrix<physics::fermionmatrix::Qplus>(refs, 2);
}
BOOST_AUTO_TEST_CASE(Qminus)
{
	const hmc_float refs[4] = {2573.5157540104783, 4301.7125032523254, 2614.8228264606378, 4430.2973098680795};
	test_fermionmatrix<physics::fermionmatrix::Qminus>(refs, 3);
}
BOOST_AUTO_TEST_CASE(QplusQminus)
{
	const hmc_float refs[4] = {4386.7804813376488, 20922.555314190751, 4503.5782668210904, 20604.833689935829};
	test_fermionmatrix<physics::fermionmatrix::QplusQminus>(refs, 4);
}

BOOST_AUTO_TEST_CASE(Aee)
{
	const hmc_float refs[4] = {1111.6772283004893, 1290.5580533222192, 1114.183875802898, 1304.0219632505139};
	test_fermionmatrix<physics::fermionmatrix::Aee>(refs, 5);
}
BOOST_AUTO_TEST_CASE(Aee_minus)
{
	const hmc_float refs[4] = {1115.2304524177298, 1291.4471557813199, 1125.22036086961, 1315.8354179702137};
	test_fermionmatrix<physics::fermionmatrix::Aee_minus>(refs, 6);
}
BOOST_AUTO_TEST_CASE(Qplus_eo)
{
	const hmc_float refs[4] = {1120.7679957612427, 1303.6316057267845, 1095.6614513886482, 1285.3471211115243};
	test_fermionmatrix<physics::fermionmatrix::Qplus_eo>(refs, 7);
}
BOOST_AUTO_TEST_CASE(Qminus_eo)
{
	const hmc_float refs[4] = {1100.7016006189097, 1282.8216593368254, 1111.2138438131915, 1309.970927161924};
	test_fermionmatrix<physics::fermionmatrix::Qminus_eo>(refs, 8);
}
BOOST_AUTO_TEST_CASE(QplusQminus_eo)
{
	const hmc_float refs[4] = {1286.5408969350369, 1931.6496938488942, 1350.5870046327818, 2182.2775949059878};
	test_fermionmatrix<physics::fermionmatrix::QplusQminus_eo>(refs, 9);
}

template<class FERMIONMATRIX>
typename boost::enable_if<boost::is_base_of<physics::fermionmatrix::Fermionmatrix_eo, FERMIONMATRIX>, void>::type
test_fermionmatrix(const hmc_float refs[4], const int seed)
{
	{
		using namespace physics::lattices;
		const char * _params[] = {"foo", "--ntime=16"};
		meta::Inputparameters params(2, _params);
		hardware::System system(params);
		physics::PRNG prng(system);
		FERMIONMATRIX matrix(ARG_DEF, ARG_DEF, system);

		Gaugefield gf(system, prng, false);
		Spinorfield src(system);
		Spinorfield_eo sf1(system);
		Spinorfield_eo sf2(system);

		pseudo_randomize<Spinorfield, spinor>(&src, seed);
		convert_to_eoprec(&sf1, &sf2, src);

		matrix(&sf2, gf, sf1);
		BOOST_CHECK_CLOSE(squarenorm(sf2), refs[0], 0.01);
		matrix(&sf1, gf, sf2);
		BOOST_CHECK_CLOSE(squarenorm(sf1), refs[1], 0.01);
	}

	{
		using namespace physics::lattices;
		const char * _params[] = {"foo", "--ntime=4"};
		meta::Inputparameters params(2, _params);
		hardware::System system(params);
		physics::PRNG prng(system);
		FERMIONMATRIX matrix(ARG_DEF, ARG_DEF, system);

		Gaugefield gf(system, prng, std::string(SOURCEDIR) + "/tests/conf.00200");
		Spinorfield src(system);
		Spinorfield_eo sf1(system);
		Spinorfield_eo sf2(system);

		pseudo_randomize<Spinorfield, spinor>(&src, seed);
		convert_to_eoprec(&sf1, &sf2, src);

		matrix(&sf2, gf, sf1);
		BOOST_CHECK_CLOSE(squarenorm(sf2), refs[2], 0.01);
		matrix(&sf1, gf, sf2);
		BOOST_CHECK_CLOSE(squarenorm(sf1), refs[3], 0.01);
	}
}

template<class FERMIONMATRIX>
typename boost::enable_if<boost::is_base_of<physics::fermionmatrix::Fermionmatrix, FERMIONMATRIX>, void>::type
test_fermionmatrix(const hmc_float refs[4], const int seed)
{
	{
		using namespace physics::lattices;
		const char * _params[] = {"foo", "--ntime=16"};
		meta::Inputparameters params(2, _params);
		hardware::System system(params);
		physics::PRNG prng(system);
		FERMIONMATRIX matrix(ARG_DEF, ARG_DEF, system);

		Gaugefield gf(system, prng, false);
		Spinorfield sf1(system);
		Spinorfield sf2(system);

		pseudo_randomize<Spinorfield, spinor>(&sf1, seed);
		pseudo_randomize<Spinorfield, spinor>(&sf2, seed + 1);

		matrix(&sf2, gf, sf1);
		BOOST_CHECK_CLOSE(squarenorm(sf2), refs[0], 0.01);
		matrix(&sf1, gf, sf2);
		BOOST_CHECK_CLOSE(squarenorm(sf1), refs[1], 0.01);
	}

	{
		using namespace physics::lattices;
		const char * _params[] = {"foo", "--ntime=4"};
		meta::Inputparameters params(2, _params);
		hardware::System system(params);
		physics::PRNG prng(system);
		FERMIONMATRIX matrix(ARG_DEF, ARG_DEF, system);

		Gaugefield gf(system, prng, std::string(SOURCEDIR) + "/tests/conf.00200");
		Spinorfield sf1(system);
		Spinorfield sf2(system);

		pseudo_randomize<Spinorfield, spinor>(&sf1, seed + 2);
		pseudo_randomize<Spinorfield, spinor>(&sf2, seed + 3);

		matrix(&sf2, gf, sf1);
		BOOST_CHECK_CLOSE(squarenorm(sf2), refs[2], 0.01);
		matrix(&sf1, gf, sf2);
		BOOST_CHECK_CLOSE(squarenorm(sf1), refs[3], 0.01);
	}
}