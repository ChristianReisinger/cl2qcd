# Build specifications for the contents of the Meta package
#
# Copyright (c) 2012,2013 Matthias Bach
# Copyright (c) 2013,2014 Christopher Pinke
# Copyright (c) 2016,2018 Alessandro Sciarra
# Copyright (c) 2018 Francesca Cuteri
#
# This file is part of CL2QCD.
#
# CL2QCD is free software: you can redistribute it and/or modify
# it under the terms of the GNU General Public License as published by
# the Free Software Foundation, either version 3 of the License, or
# (at your option) any later version.
#
# CL2QCD is distributed in the hope that it will be useful,
# but WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
# GNU General Public License for more details.
#
# You should have received a copy of the GNU General Public License
# along with CL2QCD. If not, see <http://www.gnu.org/licenses/>.

add_library(meta
    parametersBasic.cpp
    parametersConfig.cpp
    parametersIo.cpp
    parametersGauge.cpp
    parametersFermion.cpp
    parametersSources.cpp
    parametersSolver.cpp
    parametersObs.cpp
    parametersIntegrator.cpp
    parametersRationalApproximation.cpp
    parametersMonteCarlo.cpp
    parametersTest.cpp
    inputparameters.cpp
    util.cpp
    util_io.cpp
    counter.cpp
    config_file_normalizer.cpp
    type_ops.cpp
)

add_dependencies(meta git_commit_id_header)
target_link_libraries(meta
    exceptions
    logger
    ${Boost_LIBRARIES}
)

#
# Definition of tests
#
configure_file(test_input_1 test_input_1 COPYONLY)
configure_file(test_input_2 test_input_2 COPYONLY)
configure_file(test_input_aliases test_input_aliases COPYONLY)

add_unit_test(NAME meta/inputparameters        LIBRARIES meta)
add_unit_test(NAME meta/config_file_normalizer LIBRARIES meta)
add_unit_test(NAME meta/type_ops               LIBRARIES meta)
