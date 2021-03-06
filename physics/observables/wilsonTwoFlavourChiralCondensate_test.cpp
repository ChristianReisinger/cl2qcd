/** @file
 * Unit tests for the physics::observables::wilson::TwoFlavourChiralCondensate class
 *
 * Copyright (c) 2014,2015 Christopher Pinke
 * Copyright (c) 2015 Christopher Czaban
 * Copyright (c) 2016,2018 Alessandro Sciarra
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

#include "wilsonTwoFlavourChiralCondensate.hpp"

#define BOOST_TEST_DYN_LINK
#define BOOST_TEST_MODULE physics::observables::wilson::TwoFlavourChiralCondensate
#include "../../interfaceImplementations/hardwareParameters.hpp"
#include "../../interfaceImplementations/interfacesHandler.hpp"
#include "../../interfaceImplementations/openClKernelParameters.hpp"
#include "../interfacesHandler.hpp"

#include <boost/lexical_cast.hpp>
#include <boost/test/unit_test.hpp>

BOOST_AUTO_TEST_SUITE(BUILD)

    void testLogicError(const char* _params[], int length)
    {
        meta::Inputparameters params(length, _params);
        physics::InterfacesHandlerImplementation interfacesHandler{params};
        hardware::HardwareParametersImplementation hP(&params);
        hardware::code::OpenClKernelParametersImplementation kP(params);
        hardware::System system(hP, kP);
        physics::PrngParametersImplementation prngParameters{params};
        const physics::PRNG prng{system, &prngParameters};
        const physics::lattices::Gaugefield gaugefield(system,
                                                       &interfacesHandler.getInterface<physics::lattices::Gaugefield>(),
                                                       prng);

        BOOST_REQUIRE_THROW(physics::observables::wilson::
                                measureTwoFlavourChiralCondensateAndWriteToFile(&gaugefield, 0, interfacesHandler),
                            std::logic_error);
    }

    BOOST_AUTO_TEST_CASE(INV_ARGUMENT_1)
    {
        const char* _params[] = {"foo", "--measurePbp=false"};
        testLogicError(_params, 2);
    }

    void testInvalidFermionActionAndVersion(std::string actionName, std::string version = "--pbpVersion=std")
    {
        logger.info() << "Testing fermion action \"" + actionName + "\" and chiral condensate version \"" + version +
                             "\" for logic error";
        const char* standardParameters[]    = {"foo", "--measurePbp=true"};
        const char* commandLineParameters[] = {standardParameters[0], standardParameters[1], actionName.c_str(),
                                               version.c_str()};

        meta::Inputparameters params(4, commandLineParameters);
        hardware::HardwareParametersImplementation hP(&params);
        hardware::code::OpenClKernelParametersImplementation kP(params);
        hardware::System system(hP, kP);
        physics::InterfacesHandlerImplementation interfacesHandler{params};
        physics::PrngParametersImplementation prngParameters{params};
        const physics::PRNG prng{system, &prngParameters};
        const physics::lattices::Gaugefield gaugefield(system,
                                                       &interfacesHandler.getInterface<physics::lattices::Gaugefield>(),
                                                       prng);

        BOOST_REQUIRE_THROW(physics::observables::wilson::
                                measureTwoFlavourChiralCondensateAndWriteToFile(&gaugefield, 0, interfacesHandler),
                            std::logic_error);
    }

    std::vector<std::string> actionNames = {"clover", "rooted_stagg"};

    BOOST_AUTO_TEST_CASE(INV_ARGUMENT_FERMION_ACTION)
    {
        for (int i = 0; i < (int)actionNames.size(); i++) {
            testInvalidFermionActionAndVersion("--fermionAction=" + actionNames[i]);
        }
    }

    BOOST_AUTO_TEST_CASE(INV_ARGUMENT_CHIRAL_CONDENSATE_VERSION)
    {
        actionNames.push_back("wilson");
        for (int i = 0; i < (int)actionNames.size(); i++) {
            testInvalidFermionActionAndVersion("--fermionAction=" + actionNames[i], "--pbpVersion=tm_one_end_trick");
        }
    }

BOOST_AUTO_TEST_SUITE_END()

BOOST_AUTO_TEST_SUITE(MEASURE)

    void testMeasurement(std::vector<double> referenceValues, int numberOfSources, std::string fermactOptionIn,
                         std::string sourceTypeOptionIn, std::string pbpVersionOptionIn,
                         std::string eoOptionIn = "false", std::string startconditionOptionIn = "cold",
                         std::string sourcefileOptionIn = "conf.00200")
    {
        std::vector<std::string> parameters{{"foo", "--nTime=4", "--nSpace=4", "--kappa=0.15", "--mu=4.",
                                             "--measurePbp=true", "--nSources=" + std::to_string(numberOfSources),
                                             "--fermionAction=" + fermactOptionIn, "--sourceType=" + sourceTypeOptionIn,
                                             "--sourceContent=one", "--pbpVersion=" + pbpVersionOptionIn,
                                             "--startCondition=" + startconditionOptionIn,
                                             "--initialConf=" + sourcefileOptionIn, "--useEO=" + eoOptionIn}};
        std::vector<const char*> c_parameters{};
        for (auto& string : parameters)
            c_parameters.push_back(string.c_str());

        for (int i = 1; i < boost::unit_test::framework::master_test_suite().argc; i++) {
            c_parameters.push_back(boost::unit_test::framework::master_test_suite().argv[i]);
            if (strcmp(c_parameters.back(), "--") == 0)
                c_parameters.pop_back();
        }

        meta::Inputparameters params(c_parameters.size(), c_parameters.data());
        hardware::HardwareParametersImplementation hP(&params);
        hardware::code::OpenClKernelParametersImplementation kP(params);
        hardware::System system(hP, kP);
        physics::InterfacesHandlerImplementation interfacesHandler{params};
        physics::PrngParametersImplementation prngParameters{params};
        const physics::PRNG prng{system, &prngParameters};
        const physics::lattices::Gaugefield gaugefield(system,
                                                       &interfacesHandler.getInterface<physics::lattices::Gaugefield>(),
                                                       prng);

        std::vector<double> results;
        results = physics::observables::wilson::measureTwoFlavourChiralCondensateAndWriteToFile(&gaugefield,
                                                                                                "conf.test",
                                                                                                interfacesHandler);

        BOOST_REQUIRE_EQUAL(numberOfSources, (int)results.size());

        double testPrecision = 1e-8;
        for (int i = 0; i < (int)referenceValues.size(); i++) {
            BOOST_REQUIRE_CLOSE(results[i], referenceValues[i], testPrecision);
        }
    }

    BOOST_AUTO_TEST_CASE(MEASURE_NONEO)
    {
        int numberOfSources = 9;
        std::vector<double> referenceValues(numberOfSources, 4.86486486486488e-01);

        testMeasurement(referenceValues, numberOfSources, "twistedmass", "volume", "std", "false");
    }

    BOOST_AUTO_TEST_CASE(MEASURE_EO)
    {
        int numberOfSources = 9;
        std::vector<double> referenceValues(numberOfSources, 4.86486486486488e-01);

        testMeasurement(referenceValues, numberOfSources, "twistedmass", "volume", "std", "true");
    }

    BOOST_AUTO_TEST_CASE(MEASURE_ONE_END_TRICK)
    {
        int numberOfSources = 9;
        std::vector<double> referenceValues(numberOfSources, 4.86486486486488e-01);

        testMeasurement(referenceValues, numberOfSources, "twistedmass", "volume", "tm_one_end_trick");
    }

    BOOST_AUTO_TEST_CASE(MEASURE_Z_SLICE)
    {
        int numberOfSources = 9;
        std::vector<double> referenceValues(numberOfSources, 1.16971963846964e-01);

        testMeasurement(referenceValues, numberOfSources, "twistedmass", "zslice", "std");
    }

    BOOST_AUTO_TEST_CASE(MEASURE_CONFIGURATION_1)
    {
        int numberOfSources = 9;
        std::vector<double> referenceValues(numberOfSources, 6.86307941352032e-02);

        testMeasurement(referenceValues, numberOfSources, "twistedmass", "zslice", "std", "false", "continue");
    }

    BOOST_AUTO_TEST_CASE(MEASURE_CONFIGURATION_2)
    {
        int numberOfSources = 9;
        std::vector<double> referenceValues(numberOfSources, 1.35706953188924e-01);

        testMeasurement(referenceValues, numberOfSources, "wilson", "zslice", "std", "false", "continue");
    }

BOOST_AUTO_TEST_SUITE_END()
