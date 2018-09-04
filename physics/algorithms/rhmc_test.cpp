/** @file
 * Tests of the perform_rhmc_step template function inverter algorithm
 *
 * Copyright (c) 2016,2017 Christopher Czaban
 * Copyright (c) 2017 Francesca Cuteri
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

#include "rhmc.hpp"
//#include "../lattices/gaugefield.hpp"
#include "../../host_functionality/logger.hpp"
#include "../../interfaceImplementations/hardwareParameters.hpp"
#include "../../interfaceImplementations/interfacesHandler.hpp"
#include "../../interfaceImplementations/openClKernelParameters.hpp"
#include "rational_approximation.hpp"

// use the boost test framework
#define BOOST_TEST_DYN_LINK
#define BOOST_TEST_MODULE physics::algorithms::rhmc
#include <boost/test/unit_test.hpp>

BOOST_AUTO_TEST_CASE(perform_staggered_rhmc_step)
{
    using namespace physics::lattices;
    using namespace physics::algorithms;

    const char* _params[] = {"foo", "--nTime=4", "--fermionAction=rooted_stagg", "--mass=0.567", "--nDevices=1"};
    meta::Inputparameters parameters(5, _params);

    hardware::HardwareParametersImplementation hP(&parameters);
    hardware::code::OpenClKernelParametersImplementation kP(parameters);
    hardware::System system(hP, kP);
    physics::InterfacesHandlerImplementation interfacesHandler{parameters};
    physics::PrngParametersImplementation prngParameters{parameters};
    physics::PRNG prng{system, &prngParameters};

    physics::algorithms::Rational_Approximation approx_hb(parameters.get_metro_approx_ord(), 6, 8,
                                                          parameters.get_approx_lower(), parameters.get_approx_upper(),
                                                          false);
    physics::algorithms::Rational_Approximation approx_md(parameters.get_metro_approx_ord(), 6, 4,
                                                          parameters.get_approx_lower(), parameters.get_approx_upper(),
                                                          true);
    physics::algorithms::Rational_Approximation approx_met(parameters.get_metro_approx_ord(), 6, 4,
                                                           parameters.get_approx_lower(), parameters.get_approx_upper(),
                                                           true);

    Gaugefield gf(system, &interfacesHandler.getInterface<physics::lattices::Gaugefield>(), prng,
                  std::string(SOURCEDIR) + "/ildg_io/conf.00200");
    int iteration             = 1;
    const double randomNumber = prng.get_double();
    physics::algorithms::perform_rhmc_step(approx_hb, approx_md, approx_met, &gf, iteration, randomNumber, prng, system,
                                           interfacesHandler);
}

BOOST_AUTO_TEST_CASE(perform_wilson_rhmc_step)
{
    using namespace physics::lattices;
    using namespace physics::algorithms;

    const char* _params[] = {"foo",
                             "--nTime=4",
                             "--nSpace=12",
                             "--useEO=false",
                             "--nDevices=1",
                             "--nTimeScales=2",
                             "--integrator0=twomn",
                             "--integrationSteps0=10",
                             "--integrator1=twomn",
                             "--integrationSteps1=10"};
    meta::Inputparameters parameters(10, _params);

    hardware::HardwareParametersImplementation hP(&parameters);
    hardware::code::OpenClKernelParametersImplementation kP(parameters);
    hardware::System system(hP, kP);
    physics::InterfacesHandlerImplementation interfacesHandler{parameters};
    physics::PrngParametersImplementation prngParameters{parameters};
    physics::PRNG prng{system, &prngParameters};

    physics::algorithms::Rational_Approximation approx_hb(parameters.get_metro_approx_ord(), 3, 4,
                                                          parameters.get_approx_lower(), parameters.get_approx_upper(),
                                                          false);  // N_f=3 , positive rational exponent
    physics::algorithms::Rational_Approximation approx_md(parameters.get_metro_approx_ord(), 3, 2,
                                                          parameters.get_approx_lower(), parameters.get_approx_upper(),
                                                          true);  // N_f=3 , negative rational exponent
    physics::algorithms::Rational_Approximation approx_met(parameters.get_metro_approx_ord(), 3, 2,
                                                           parameters.get_approx_lower(), parameters.get_approx_upper(),
                                                           true);  // N_f=3 , negative rational exponent

    Gaugefield gf(system, &interfacesHandler.getInterface<physics::lattices::Gaugefield>(), prng, true);
    int iteration             = 3;
    const double randomNumber = prng.get_double();
    physics::algorithms::perform_wilson_rhmc_step(approx_hb, approx_md, approx_met, &gf, iteration, randomNumber, prng,
                                                  system, interfacesHandler);
}

BOOST_AUTO_TEST_CASE(perform_wilson_eo_rhmc_step)
{
    using namespace physics::lattices;
    using namespace physics::algorithms;

    const char* _params[] = {"foo",
                             "--nTime=4",
                             "--nSpace=12",
                             "--useEO=true",
                             "--nDevices=1",
                             "--nTimeScales=2",
                             "--integrator0=twomn",
                             "--integrationSteps0=10",
                             "--integrator1=twomn",
                             "--integrationSteps1=10"};
    meta::Inputparameters parameters(10, _params);

    hardware::HardwareParametersImplementation hP(&parameters);
    hardware::code::OpenClKernelParametersImplementation kP(parameters);
    hardware::System system(hP, kP);
    physics::InterfacesHandlerImplementation interfacesHandler{parameters};
    physics::PrngParametersImplementation prngParameters{parameters};
    physics::PRNG prng{system, &prngParameters};

    physics::algorithms::Rational_Approximation approx_hb(parameters.get_metro_approx_ord(), 3, 4,
                                                          parameters.get_approx_lower(), parameters.get_approx_upper(),
                                                          false);  // N_f=3 , positive rational exponent
    physics::algorithms::Rational_Approximation approx_md(parameters.get_metro_approx_ord(), 3, 2,
                                                          parameters.get_approx_lower(), parameters.get_approx_upper(),
                                                          true);  // N_f=3 , negative rational exponent
    physics::algorithms::Rational_Approximation approx_met(parameters.get_metro_approx_ord(), 3, 2,
                                                           parameters.get_approx_lower(), parameters.get_approx_upper(),
                                                           true);  // N_f=3 , negative rational exponent

    Gaugefield gf(system, &interfacesHandler.getInterface<physics::lattices::Gaugefield>(), prng, true);
    int iteration             = 3;
    const double randomNumber = prng.get_double();
    physics::algorithms::perform_wilson_rhmc_step(approx_hb, approx_md, approx_met, &gf, iteration, randomNumber, prng,
                                                  system, interfacesHandler);
}
