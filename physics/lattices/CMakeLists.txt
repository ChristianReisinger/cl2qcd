# Build specifications for the contents of the hardware package
#
# Copyright (c) 2012,2013 Matthias Bach
# Copyright (c) 2013,2014,2018 Alessandro Sciarra
# Copyright (c) 2014,2015 Christopher Pinke
# Copyright (c) 2016 Francesca Cuteri
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

add_library(gaugefield
    gaugefield.cpp
)

target_link_libraries(gaugefield
    hardware_lattices
)

add_library(lattices
    spinorfield.cpp
    spinorfield_eo.cpp
    scalar_complex.cpp
    algebra_real.cpp
    swappable_spinorfield.cpp
    gaugemomenta.cpp
    staggeredfield_eo.cpp
    rooted_staggeredfield_eo.cpp
)
target_link_libraries(lattices
    hardware
    gaugefield
    ildg_io
    contractioncode_io
    host_functionality
    prng
    observables
)

#
# Definition of tests
#
add_unit_test(NAME physics/lattices/scalar_complex           LIBRARIES lattices)
add_unit_test(NAME physics/lattices/vector                   LIBRARIES lattices)
add_unit_test(NAME physics/lattices/algebra_real             LIBRARIES lattices)
add_unit_test(NAME physics/lattices/gaugefield               LIBRARIES gaugefield meta hardware gaugeObservables prng ildg_io contractioncode_io host_functionality)
add_unit_test(NAME physics/lattices/gaugemomenta             LIBRARIES lattices)
add_unit_test(NAME physics/lattices/spinorfield              LIBRARIES lattices)
add_unit_test(NAME physics/lattices/spinorfield_eo           LIBRARIES lattices)
add_unit_test(NAME physics/lattices/swappable_spinorfield    LIBRARIES lattices)
add_unit_test(NAME physics/lattices/staggeredfield_eo        LIBRARIES lattices)
add_unit_test(NAME physics/lattices/rooted_staggeredfield_eo LIBRARIES lattices)
