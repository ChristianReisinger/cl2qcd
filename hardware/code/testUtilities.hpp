/*
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

#ifndef _TESTUTILITIES_HPP_
#define _TESTUTILITIES_HPP_

#include <memory>

#include "../../meta/inputparameters.hpp"

std::unique_ptr<meta::Inputparameters> createParameters(std::string inputfile);
std::unique_ptr<meta::Inputparameters> createParameters(std::vector<std::string> parameterStrings);
void printKernelInformation(std::string name);

const int ns4 = 4;
const int nt4 = 4;
const int ns8 = 8;
const int nt8 = 8;
const int ns12 = 12;
const int nt12 = 12;
const int ns16 = 16;
const int nt16 = 16;

static hmc_float sumOfIntegers(const int start, const int end, const int increment) noexcept
{
	// One could also implement some variant of Faulhabers Formula here to save the loop
	hmc_float sum = 0.;
	for(int iteration = start; iteration <= end; iteration += increment)
	{
		sum += iteration;
	}
	return sum;
}

static hmc_float sumOfIntegersSquared(const int end) noexcept
{
	return (2*end*end*end + 3*end*end + end) / 6.; // Faulhaber`s formula
}

#endif // _TESTUTILITIES_HPP_
