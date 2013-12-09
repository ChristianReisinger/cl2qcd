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

#include "gaugeobservablesExecutable.h"

gaugeobservablesExecutable::gaugeobservablesExecutable(int argc, const char* argv[]) : measurementExecutable(argc, argv)
{
  	printParametersToScreenAndFile();
}

inline void gaugeobservablesExecutable::writeGaugeobservablesLogfile()
{
  outputToFile.open(filenameForLogfile,
		    std::ios::out | std::ios::app);
	if (outputToFile.is_open()) {
		meta::print_info_observables_gauge_io(&outputToFile, parameters);
		outputToFile.close();
	} else {
		throw File_Exception(filenameForLogfile);
	}
}

void gaugeobservablesExecutable::printParametersToScreenAndFile()
{
	meta::print_info_observables_gauge_io(parameters);
	writeGaugeobservablesLogfile();
}

inline void gaugeobservablesExecutable::performApplicationSpecificMeasurements()
{
  gaugeObservablesInstance.measureGaugeObservables(*gaugefield, gaugefield->get_parameters_source().trajectorynr_source, parameters);
}

int main(int argc, const char* argv[])
{
	try{
		gaugeobservablesExecutable gaugeobservablesInstance(argc, argv);
		gaugeobservablesInstance.performMeasurements();
	} //try
	//exceptions from Opencl classes
	catch (Opencl_Error& e) {
		logger.fatal() << e.what();
		exit(1);
	} catch (File_Exception& fe) {
		logger.fatal() << "Could not open file: " << fe.get_filename();
		logger.fatal() << "Aborting.";
		exit(1);
	} catch (Print_Error_Message& em) {
		logger.fatal() << em.what();
		exit(1);
	} catch (Invalid_Parameters& es) {
		logger.fatal() << es.what();
		exit(1);
	}

	return 0;
}

