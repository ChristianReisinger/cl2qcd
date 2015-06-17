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

#include "inverterExecutable.h"

#include "../physics/observables/wilsonTwoFlavourChiralCondensate.hpp"
#include "../physics/observables/wilsonTwoFlavourCorrelators.hpp"
#include "../physics/observables/staggeredChiralCondensate.hpp"

inverterExecutable::inverterExecutable(int argc, const char* argv[]) : measurementExecutable(argc, argv, "inverter")
{
	initializationTimer.reset();
	printParametersToScreenAndFile();
	initializationTimer.add();
}

void inverterExecutable::writeInverterLogfile()
{
  outputToFile.open(filenameForLogfile,
		    std::ios::out | std::ios::app);
	if (outputToFile.is_open()) {
		meta::print_info_inverter(&outputToFile, parameters);
		outputToFile.close();
	} else {
		throw File_Exception(filenameForLogfile);
	}
}

void inverterExecutable::printParametersToScreenAndFile()
{
	meta::print_info_inverter(parameters);
	writeInverterLogfile();
}

void inverterExecutable::performApplicationSpecificMeasurements() {
    logger.info() << "Measure fermionic observables on configuration: " << currentConfigurationName;
    physics::observables::measureGaugeObservablesAndWriteToFile(gaugefield, gaugefield->get_trajectoryNumberAtInit() );
    if (parameters.get_fermact() == meta::action::rooted_stagg) {
        if (parameters.get_measure_pbp()) {
            //NOTE: if parameters.get_read_multiple_configs()==1 maybe here the iteration number is not correct set as it is now
            physics::observables::staggered::measureChiralCondensateAndWriteToFile(*gaugefield, gaugefield->get_trajectoryNumberAtInit());
        }
        if (parameters.get_measure_correlators()) {
            logger.warn() << "Correlators not yet implemented in the staggered case! Required measurement will not be done!";
        }
    } else {
        if (parameters.get_measure_correlators()) {
            physics::observables::wilson::measureTwoFlavourDoubletCorrelatorsOnGaugefield(gaugefield, currentConfigurationName);
        }
        if (parameters.get_measure_pbp()) {
            physics::observables::wilson::measureTwoFlavourChiralCondensateAndWriteToFile(gaugefield, currentConfigurationName);
        }
    }
}

