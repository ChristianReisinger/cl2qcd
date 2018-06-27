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

#ifndef _META_PARAMETERS_BASIC_HPP_
#define _META_PARAMETERS_BASIC_HPP_

#include "../common_header_files/types.hpp"

#include <boost/program_options.hpp>
#include <string>
#include <vector>
namespace po = boost::program_options;

namespace meta {

    class InputparametersOptions : public po::options_description {
      public:
        InputparametersOptions(std::string optionsDescription);
        void printOptionsInCustomizedWay(std::ostream& stream) const;

      private:
        InputparametersOptions(std::string optionsDescriptionIn, unsigned int lineLengthIn,
                               unsigned int minimumDescriptionLengthIn);
        unsigned int lineLength;  // Store even if privately present in parent
    };

}  // namespace meta

#endif
