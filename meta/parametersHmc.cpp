/** @file
 *
 * Copyright (c) 2014 Christopher Pinke
 * Copyright (c) 2014 Matthias Bach
 * Copyright (c) 2015,2018 Francesca Cuteri
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

#include "parametersHmc.hpp"

#include "../host_functionality/logger.hpp"

#include <stdexcept>

using namespace meta;

double ParametersHmc::get_tau() const noexcept
{
    return tau;
}
bool ParametersHmc::get_reversibility_check() const noexcept
{
    return reversibility_check;
}
int ParametersHmc::get_integrationsteps(size_t timescale) const noexcept
{
    switch (timescale) {
        case 0:
            return integrationsteps0;
        case 1:
            return integrationsteps1;
        case 2:
            return integrationsteps2;
        default:
            throw std::out_of_range("No such timescale");
    }
}
int ParametersHmc::get_hmcsteps() const noexcept
{
    return hmcsteps;
}
int ParametersHmc::get_num_timescales() const noexcept
{
    return num_timescales;
}
common::integrator ParametersHmc::get_integrator(size_t timescale) const noexcept
{
    switch (timescale) {
        case 0:
            return integrator0;
        case 1:
            return integrator1;
        case 2:
            return integrator2;
        default:
            throw std::out_of_range("No such timescale");
    }
}
double ParametersHmc::get_lambda(size_t timescale) const noexcept
{
    switch (timescale) {
        case 0:
            return lambda0;
        case 1:
            return lambda1;
        case 2:
            return lambda2;
        default:
            throw std::out_of_range("No such timescale");
    }
}

ParametersHmc::ParametersHmc() : options("HMC options")
{
    // clang-format off
	options.add_options()
	("tau", po::value<double>(&tau)->default_value(0.5),"The total time of integration.")
	("reversibilityCheck", po::value<bool>(&reversibility_check)->default_value(false),"Whether to check reversibility violation in the integrator by integrating back in time.")
	("integrationSteps0", po::value<int>(&integrationsteps0)->default_value(10),"The number of integration steps for timescale 0.")
	("integrationSteps1", po::value<int>(&integrationsteps1)->default_value(10),"The number of integration steps for timescale 1.")
	("integrationSteps2", po::value<int>(&integrationsteps2)->default_value(10),"The number of integration steps for timescale 2.")
	("nHmcSteps", po::value<int>(&hmcsteps)->default_value(10),"The number of hmc steps (i.e. the number of configuration updates in the Markov chain).")
	("nTimescales", po::value<int>(&num_timescales)->default_value(1),"The number of time scales (timescale0 for the gauge-part, timescale1 for the fermion, timescale2 for mass preconditioning). Consider that different timescales must use the same integrator.")
	("integrator0", po::value<std::string>()->default_value("leapfrog"),"The integration scheme for timescale 0 (one among leapfrog and twomn).")
	("integrator1", po::value<std::string>()->default_value("leapfrog"),"The integration scheme for timescale 1 (one among leapfrog and twomn).")
	("integrator2", po::value<std::string>()->default_value("leapfrog"),"The integration scheme for timescale 2 (one among leapfrog and twomn).")
	// this is the optimal value...
	("lambda0", po::value<double>(&lambda0)->default_value(0.1931833275037836),"The lambda parameter for timescale 0 (for the twomn integrator).")
	("lambda1", po::value<double>(&lambda1)->default_value(0.1931833275037836),"The lambda parameter for timescale 1 (for the twomn integrator).")
	("lambda2", po::value<double>(&lambda2)->default_value(0.1931833275037836),"The lambda parameter for timescale 2 (for the twomn integrator).")
	("useGaugeOnly", po::value<bool>(&use_gauge_only)->default_value(false),"Whether to simulate pure gauge theory with HMC. In this case 'nTimescales' has to be set to 1.")
	("useMP", po::value<bool>(&use_mp)->default_value(false),"Whether to use the Mass Preconditioning trick.");
    // clang-format on
}

bool ParametersHmc::get_use_gauge_only() const noexcept
{
    return use_gauge_only;
}

bool ParametersHmc::get_use_mp() const noexcept
{
    return use_mp;
}
