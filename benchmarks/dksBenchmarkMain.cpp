/*
 * Copyright (c) 2014,2018 Alessandro Sciarra
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

#include "../executables/dksBenchmark.hpp"
#include "../meta/util.hpp"

int main(int argc, const char* argv[])
{
    try {
        std::vector<const char*> optionsToAdd({"--enableProfiling=true", "--fermionAction=rooted_stagg"});
        std::pair<int, std::vector<const char*>> new_argc_argv = meta::addOptionsToArgv(argc, argv, optionsToAdd);
        dksBenchmark dksBenchmarkInstance(new_argc_argv.first, &(new_argc_argv.second[0]));
        dksBenchmarkInstance.benchmark();
    }  // try
    // exceptions from Opencl classes
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
