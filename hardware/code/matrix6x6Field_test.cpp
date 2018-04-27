/*
 * Copyright (c) 2016 Francesca Cuteri
 * Copyright (c) 2016 Max Theilig
 * Copyright (c) 2018 Alessandro Sciarra
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
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with CL2QCD. If not, see <http://www.gnu.org/licenses/>.
 */

#define BOOST_TEST_DYN_LINK
#define BOOST_TEST_MODULE hardware::code::Gaugefield
#include <boost/test/unit_test.hpp>

#include "Matrix6x6FieldTester.hpp"

ReferenceValues calculateReferenceValues_CloverEvenOddInverseExplicitUpperLeftTester(const int latticeVolume, const GaugefieldFillType gaugefieldFillTypeIn, const hmc_float kappa, const hmc_float csw)
{
	if(kappa == 0. or csw == 0. or gaugefieldFillTypeIn != ascendingInTDirNonTrivialInSpatial) {
		return ReferenceValues{latticeVolume * 6.};
	}
	else if(kappa == nonTrivialParameter and csw == nonTrivialParameter and gaugefieldFillTypeIn == ascendingInTDirNonTrivialInSpatial) {
			return ReferenceValues{latticeVolume * -1.14448229330247};
	}
	return defaultReferenceValues();
}

ReferenceValues calculateReferenceValues_CloverEvenOddInverseExplicitLowerRightTester(const int latticeVolume, const GaugefieldFillType gaugefieldFillTypeIn, const hmc_float kappa, const hmc_float csw)
{
	if(kappa == 0. or csw == 0. or gaugefieldFillTypeIn != ascendingInTDirNonTrivialInSpatial) {
		return ReferenceValues{latticeVolume * 6.};
	}
	else if(kappa == nonTrivialParameter and csw == nonTrivialParameter and gaugefieldFillTypeIn == ascendingInTDirNonTrivialInSpatial) {
			return ReferenceValues{latticeVolume * -2.559858978354924};
	}
	return defaultReferenceValues();
}

ReferenceValues calculateReferenceValues_SDetTester(const int latticeVolume, const GaugefieldFillType gaugefieldFillTypeIn, const hmc_float kappa, const hmc_float csw)
{
	if(gaugefieldFillTypeIn == cold) {
		return ReferenceValues{0.0};
	}
	else if(gaugefieldFillTypeIn == ascendingInTDirNonTrivialInSpatial and kappa == nonTrivialParameter and csw == nonTrivialParameter) {
		return ReferenceValues{0.5 * latticeVolume * -3.397613813121696};
	}
	return defaultReferenceValues();
}


struct CloverEvenOddInverseExplicitUpperLeftTester : public Matrix6x6FieldTesterWithSumAsKernelResult
{
	CloverEvenOddInverseExplicitUpperLeftTester(const ParameterCollection & parameterCollection, const Matrix6x6FieldTestParameters testParams):
		Matrix6x6FieldTesterWithSumAsKernelResult("clover_eo_inverse_explicit_upper_left", parameterCollection, testParams, calculateReferenceValues_CloverEvenOddInverseExplicitUpperLeftTester(parameterCollection.hardwareParameters.getLatticeVolume(), testParams.fillType, parameterCollection.kernelParameters.getKappa(), parameterCollection.kernelParameters.getCsw()))
	{
		code->clover_eo_inverse_explicit_upper_left_device(out, gaugefieldBuffer, parameterCollection.kernelParameters.getKappa(), parameterCollection.kernelParameters.getCsw());
	}
};

struct CloverEvenOddInverseExplicitLowerRightTester : public Matrix6x6FieldTesterWithSumAsKernelResult
{
	CloverEvenOddInverseExplicitLowerRightTester(const ParameterCollection & parameterCollection, const Matrix6x6FieldTestParameters testParams):
		Matrix6x6FieldTesterWithSumAsKernelResult("clover_eo_inverse_explicit_lower_right", parameterCollection, testParams, calculateReferenceValues_CloverEvenOddInverseExplicitLowerRightTester(parameterCollection.hardwareParameters.getLatticeVolume(), testParams.fillType, parameterCollection.kernelParameters.getKappa(), parameterCollection.kernelParameters.getCsw()))
	{
		code->clover_eo_inverse_explicit_lower_right_device(out, gaugefieldBuffer, parameterCollection.kernelParameters.getKappa(), parameterCollection.kernelParameters.getCsw());
	}
};

struct SDetTester : public Matrix6x6FieldTester
{
	SDetTester(const ParameterCollection & parameterCollection, const Matrix6x6FieldTestParameters testParams):
		Matrix6x6FieldTester("S_det", parameterCollection, testParams, calculateReferenceValues_SDetTester(parameterCollection.hardwareParameters.getLatticeVolume(), testParams.fillType, parameterCollection.kernelParameters.getKappa(), parameterCollection.kernelParameters.getCsw()))
	{
		doubleBuffer = new hardware::buffers::Plain<double> (1, device);

		code = this->device->getMatrix6x6FieldCode();
		code->S_det_device(gaugefieldBuffer, doubleBuffer, parameterCollection.kernelParameters.getKappa(), parameterCollection.kernelParameters.getCsw());
		doubleBuffer->dump(&kernelResult[0]);
	}

protected:
	const hardware::buffers::Plain<hmc_float> * doubleBuffer;
};

template<typename TesterClass>
void performTest(const LatticeExtents latticeExtentsIn, const GaugefieldFillType fillType, const hmc_float kappa, const hmc_float csw)
{
	Matrix6x6FieldTestParameters parametersForThisTest {latticeExtentsIn, fillType, kappa, csw};
	hardware::HardwareParametersMockup hardwareParameters(parametersForThisTest.ns,parametersForThisTest.nt);
	hardware::code::OpenClKernelParametersMockupForCloverEvenOdd kernelParameters(parametersForThisTest.ns,parametersForThisTest.nt, parametersForThisTest.kappa, parametersForThisTest.csw);
	ParameterCollection parameterCollection(hardwareParameters, kernelParameters);
	TesterClass tester(parameterCollection, parametersForThisTest);
}

void testCloverEvenOddInverseExplicitUpperLeft(const LatticeExtents lE, const GaugefieldFillType fT, const hmc_float kappa, const hmc_float csw)
{
	performTest<CloverEvenOddInverseExplicitUpperLeftTester>(lE, fT, kappa, csw);
}

void testCloverEvenOddInverseExplicitLowerRight(const LatticeExtents lE, const GaugefieldFillType fT, const hmc_float kappa, const hmc_float csw)
{
	performTest<CloverEvenOddInverseExplicitLowerRightTester>(lE, fT, kappa, csw);
}

void testSDet(const LatticeExtents lE, const GaugefieldFillType fT, const hmc_float kappa, const hmc_float csw)
{
	performTest<SDetTester>(lE, fT, kappa, csw);
}

BOOST_AUTO_TEST_SUITE ( CLOVER_EO_INVERSE_EXPLICIT )

	BOOST_AUTO_TEST_CASE( CLOVER_EO_INVERSE_EXPLICIT_UPPER_LEFT )
	{
		testCloverEvenOddInverseExplicitUpperLeft(LatticeExtents{ns4, nt4}, GaugefieldFillType::ascendingInTDirNonTrivialInSpatial, nonTrivialParameter, nonTrivialParameter);
	}

	BOOST_AUTO_TEST_CASE( CLOVER_EO_INVERSE_EXPLICIT_LOWER_RIGHT )
	{
		testCloverEvenOddInverseExplicitLowerRight(LatticeExtents{ns4, nt4}, GaugefieldFillType::ascendingInTDirNonTrivialInSpatial, nonTrivialParameter, nonTrivialParameter);
	}

BOOST_AUTO_TEST_SUITE_END()

BOOST_AUTO_TEST_SUITE ( CLOVER_EO_LOG_DET )

	BOOST_AUTO_TEST_CASE( CLOVER_EO_LOG_DET_1 )
	{
		testSDet(LatticeExtents{ns4, nt4}, GaugefieldFillType::cold, nonTrivialParameter, nonTrivialParameter);
	}

	BOOST_AUTO_TEST_CASE( CLOVER_EO_LOG_DET_2 )
	{
		testSDet(LatticeExtents{ns4, nt4}, GaugefieldFillType::ascendingInTDirNonTrivialInSpatial, nonTrivialParameter, nonTrivialParameter);
	}

BOOST_AUTO_TEST_SUITE_END()