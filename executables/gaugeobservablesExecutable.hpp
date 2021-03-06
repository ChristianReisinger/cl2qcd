/** @file
 *
 * Everything required by gaugeobservable's main()
 *
 * Copyright (c) 2013,2014 Christopher Pinke
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

#ifndef GAUGEOBSERVABLESEXECUTABLE_H_
#define GAUGEOBSERVABLESEXECUTABLE_H_

#include "measurementExecutable.hpp"

class gaugeobservablesExecutable : public measurementExecutable {
  public:
    gaugeobservablesExecutable(int argc, const char* argv[]);

  protected:
    std::string filenameForGaugeobservables;

    void writeGaugeobservablesLogfile();

    void printParametersToScreenAndFile();

    /**
     * Performs measurements of gauge observables on possibly multiple gaugefield configurations.
     */
    void performApplicationSpecificMeasurements();
};

#endif /* GAUGEOBSERVABLESEXECUTABLE_H_ */
