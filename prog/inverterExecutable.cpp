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

inline inverterExecutable::inverterExecutable(int argc, const char* argv[]) : generalExecutable(argc, argv)
{
	initializationTimer.reset();
	prng = new physics::PRNG(*system);
	meta::print_info_inverter(ownName, parameters);
	writeInverterLogfile();
	setIterationVariables();
	initializationTimer.add();
}

inline inverterExecutable::~inverterExecutable()
{
	if (parameters.get_profile_solver()) {
		writeProfilingDataToScreenAndFile();
	}
}

inline void inverterExecutable::performMeasurements()
{
	performanceTimer.reset();
	logger.trace() << "Perform inversion(s) on device..";

	int iteration = 0;
	for (iteration = iterationStart; iteration < iterationEnd; iteration += iterationIncrement)
	{
		performMeasurementsForSpecificIteration(iteration);
	}
	logger.trace() << "Inversion(s) done";
	performanceTimer.add();
}

inline void inverterExecutable::setIterationVariables()
{
	iterationStart =		(parameters.get_read_multiple_configs()) ? parameters.get_config_read_start() : 0;
	iterationEnd = 			(parameters.get_read_multiple_configs()) ? parameters.get_config_read_end() + 1 : 1;
	iterationIncrement =	(parameters.get_read_multiple_configs()) ? parameters.get_config_read_incr() : 1;
}

inline void inverterExecutable::saveCurrentPrngStateToFile()
{
	logger.info() << "saving current prng state to \"" << filenameForCurrentPrngState << "\"";
	prng->store(filenameForCurrentPrngState);
}

inline void inverterExecutable::initializeGaugefieldAccordingToIterationVariable(int iteration)
{
	currentConfigurationName = meta::create_configuration_name(parameters, iteration);
	gaugefield = new physics::lattices::Gaugefield(*system, *prng, currentConfigurationName);
}

inline void inverterExecutable::initializeGaugefieldAccordingToConfigurationGivenInSourcefileParameter()
{
	currentConfigurationName = parameters.get_sourcefile();
	gaugefield = new physics::lattices::Gaugefield(*system, *prng);
}

inline void inverterExecutable::initializeGaugefield(int iteration)
{
	initializationTimer.reset();
	if (parameters.get_read_multiple_configs()) {
		initializeGaugefieldAccordingToIterationVariable(iteration);
	} else {
		initializeGaugefieldAccordingToConfigurationGivenInSourcefileParameter();
	}
	initializationTimer.add();
}

inline void inverterExecutable::performMeasurementsForSpecificIteration(int iteration)
{
	initializeGaugefield(iteration);
	measureFermionicObservablesOnGaugefield();
	saveCurrentPrngStateToFile();
	delete gaugefield;
}

inline void inverterExecutable::writeInverterLogfile()
{
	outputToFile.open(filenameForInverterLogfile);
	if (outputToFile.is_open()) {
		meta::print_info_inverter(ownName, &outputToFile, parameters);
		outputToFile.close();
	} else {
		logger.warn() << "Could not open log file for inverter.";
	}
}

inline void inverterExecutable::writeProfilingDataToScreenAndFile()
{
	uint64_t avg_time = 0.;
	uint64_t time_total = 0;
	int calls_total = 0;
	getSolverStatistics(calls_total, time_total, avg_time);
	writeProfilingDataToScreen(time_total, calls_total, avg_time);
	writeProfilingDataToFile(time_total, calls_total, avg_time);
}

inline void inverterExecutable::getSolverStatistics(int& totalSolverCalls, uint64_t& totalSolverTime, uint64_t& averageSolverTime)
{
	totalSolverCalls = solverTimer.getNumMeas();
	totalSolverTime = solverTimer.getTime();
	if (totalSolverCalls != 0 && totalSolverTime != 0) {
		averageSolverTime = (uint64_t)(
				((float) (((((((((totalSolverTime))))))))))
						/ ((float) (((((((((totalSolverCalls)))))))))));
	}
}

inline void inverterExecutable::writeProfilingDataToScreen( uint64_t totalSolverTime, int totalSolverCalls, uint64_t averageSolverTime)
{
	logger.info() << "## **********************************************************";
	logger.info() << "## Solver Times [mus]:\ttime\tcalls\tavg";
	logger.info() << "##\t" << totalSolverTime << "\t" << totalSolverCalls << "\t" << averageSolverTime;
	logger.info() << "## **********************************************************";
}

inline void inverterExecutable::writeProfilingDataToFile( uint64_t totalSolverTime, int totalSolverCalls, uint64_t averageSolverTime)
{
	outputStreamForProfilingData.open(filenameForProfilingData.c_str(), std::ios::out | std::ios::app);
	if (outputStreamForProfilingData.is_open()) {
		writeProfilingDataToFileUsingOpenOutputStream(totalSolverTime, totalSolverCalls, averageSolverTime);
		outputStreamForProfilingData.close();
	} else {
		logger.warn() << "Could not open " << filenameForProfilingData;
		File_Exception(filenameForProfilingData.c_str());
	}
}

inline void inverterExecutable::writeProfilingDataToFileUsingOpenOutputStream( uint64_t totalSolverTime, int totalSolverCalls, uint64_t averageSolverTime)
{
	meta::print_info_inverter(ownName, &outputStreamForProfilingData, parameters);
	outputStreamForProfilingData.width(32);
	outputStreamForProfilingData.precision(15);
	outputStreamForProfilingData << "## **********************************************************" << std::endl;
	outputStreamForProfilingData << "## Solver Times [mus]:\ttime\tcalls\tavg" << std::endl;
	outputStreamForProfilingData << "\t" << totalSolverTime << "\t" << totalSolverCalls << "\t" << averageSolverTime << std::endl;
	outputStreamForProfilingData << "## **********************************************************" << std::endl;
	return;
}

inline void inverterExecutable::measureTwoFlavourDoubletCorrelatorsOnGaugefield() {
	filenameForTwoFlavourDoubletCorrelatorData = meta::get_ferm_obs_corr_file_name(parameters, currentConfigurationName);
	// for the correlator calculation, all sources are needed on the device
	const std::vector<physics::lattices::Spinorfield*> sources = physics::create_swappable_sources(*system, *prng, parameters.get_num_sources());
	const std::vector<physics::lattices::Spinorfield*> result = physics::lattices::create_swappable_spinorfields(*system, sources.size(), parameters.get_place_sources_on_host());
	swap_out(sources);
	swap_out(result);
	physics::algorithms::perform_inversion(&result, gaugefield, sources, *system);
	logger.info() << "Finished inversion. Starting measurements.";
	swap_in(sources);
	swap_in(result);
	physics::algorithms::flavour_doublet_correlators(result, sources, filenameForTwoFlavourDoubletCorrelatorData, *system);
	release_spinorfields(result);
	release_spinorfields(sources);
}

inline void inverterExecutable::measureTwoFlavourDoubletChiralCondensateOnGaugefield() {
	filenameForTwoFlavourDoubletChiralCondensateData = meta::get_ferm_obs_pbp_file_name(parameters, currentConfigurationName);
	int sourceNumber = 0;
	for (; sourceNumber < parameters.get_num_sources(); sourceNumber++) {
		auto sources = physics::create_sources(*system, *prng, 1);
		auto result = physics::lattices::create_spinorfields(*system, sources.size());
		physics::algorithms::perform_inversion(&result, gaugefield, sources, *system);
		physics::algorithms::flavour_doublet_chiral_condensate(result, sources, filenameForTwoFlavourDoubletChiralCondensateData, gaugefield->get_parameters_source().trajectorynr_source, *system);
		release_spinorfields(result);
		release_spinorfields(sources);
	}
}

inline void inverterExecutable::measureFermionicObservablesOnGaugefield() {
	logger.info() << "Measure fermionic observables on configuration: " << currentConfigurationName;
	if (parameters.get_print_to_screen()) {
		print_gaugeobservables(*gaugefield, gaugefield->get_parameters_source().trajectorynr_source);
	}
	if (parameters.get_measure_correlators()) {
		measureTwoFlavourDoubletCorrelatorsOnGaugefield();
	}
	if (parameters.get_measure_pbp()) {
		measureTwoFlavourDoubletChiralCondensateOnGaugefield();
	}
}

int main(int argc, const char* argv[])
{
	try{
		inverterExecutable inverterInstance(argc, argv);
		inverterInstance.performMeasurements();
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

