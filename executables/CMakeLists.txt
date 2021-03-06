# Build specifications for the contents of the physics package
#
# Copyright (c) 2013,2014 Christopher Pinke
# Copyright (c) 2014,2018 Alessandro Sciarra
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

add_library(exceptions
    exceptions.cpp
)

add_library(executables
    generalExecutable.cpp
    generationExecutable.cpp
    measurementExecutable.cpp
    benchmarkExecutable.cpp
    su3heatbathExecutable.cpp
    gaugeobservablesExecutable.cpp
    inverterExecutable.cpp
    hmcExecutable.cpp
    rhmcExecutable.cpp
    dslashBenchmark.cpp
    dksBenchmark.cpp
    su3heatbathBenchmark.cpp
)

target_link_libraries(executables
    exceptions
    physics
)

add_subdirectory(tests)
