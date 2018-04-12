/** @file
 * Implementation of the physics::lattices::Rooted_Spinorfield_eo class
 *
 * Copyright (c) 2017 Christopher Czaban
 * Copyright (c) 2017 Francesca Cuteri
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

#include "rooted_spinorfield_eo.hpp"

physics::lattices::wilson::Rooted_Spinorfield_eo::Rooted_Spinorfield_eo(const hardware::System& system
																	, const RootedSpinorfieldEoParametersInterface& rootedSpinorfieldEoParametersInterface)
	: rationalCoefficients(std::max(rootedSpinorfieldEoParametersInterface.getMetropolisRationalApproximationOrder()
		, rootedSpinorfieldEoParametersInterface.getMolecularDynamicsRationalApproximationOrder()))
{
    const unsigned int numberOfPseudofermions = rootedSpinorfieldEoParametersInterface.getNumberOfPseudofermions();
    pseudofermions.reserve(numberOfPseudofermions);
    for(unsigned int j=0; j<numberOfPseudofermions; j++){
        pseudofermions.emplace_back(new physics::lattices::Spinorfield_eo(system, rootedSpinorfieldEoParametersInterface));
    }
}

physics::lattices::wilson::Rooted_Spinorfield_eo::Rooted_Spinorfield_eo(const hardware::System& system
																	, const RootedSpinorfieldEoParametersInterface& rootedSpinorfieldEoParametersInterface
																	, const physics::algorithms::Rational_Approximation& approx)
	: rationalCoefficients(approx.Get_order(), approx.Get_a0(), approx.Get_a(), approx.Get_b())
{
    const unsigned int numberOfPseudofermions = rootedSpinorfieldEoParametersInterface.getNumberOfPseudofermions();
    pseudofermions.reserve(numberOfPseudofermions);
    for(unsigned int j=0; j<numberOfPseudofermions; j++){
        pseudofermions.emplace_back(new physics::lattices::Spinorfield_eo(system, rootedSpinorfieldEoParametersInterface));
    }
}

void physics::lattices::wilson::Rooted_Spinorfield_eo::Rescale_Coefficients(const physics::algorithms::Rational_Approximation& approx, const hmc_float minEigenvalue, const hmc_float maxEigenvalue)
{
//	physics::algorithms::Rational_Coefficients aux = ;
//	rationalCoefficients.Set_coeff(aux.Get_a0(), aux.Get_a(), aux.Get_b());
	rationalCoefficients = std::move(approx.Rescale_Coefficients(minEigenvalue, maxEigenvalue));
}

unsigned int physics::lattices::wilson::Rooted_Spinorfield_eo::getOrder() const
{
	return rationalCoefficients.Get_order();
}

hmc_float physics::lattices::wilson::Rooted_Spinorfield_eo::get_a0() const
{
	return rationalCoefficients.Get_a0();
}

std::vector<hmc_float> physics::lattices::wilson::Rooted_Spinorfield_eo::get_a() const
{
	return rationalCoefficients.Get_a();
}

std::vector<hmc_float> physics::lattices::wilson::Rooted_Spinorfield_eo::get_b() const
{
	return rationalCoefficients.Get_b();
}

const std::unique_ptr<physics::lattices::Spinorfield_eo>& physics::lattices::wilson::Rooted_Spinorfield_eo::operator[](unsigned int index) const
{
    return pseudofermions[index];
}

std::vector<std::unique_ptr<physics::lattices::Spinorfield_eo> >::iterator physics::lattices::wilson::Rooted_Spinorfield_eo::begin()
{
    return pseudofermions.begin();
}

std::vector<std::unique_ptr<physics::lattices::Spinorfield_eo> >::const_iterator physics::lattices::wilson::Rooted_Spinorfield_eo::begin() const
{
    return pseudofermions.begin();
}

std::vector<std::unique_ptr<physics::lattices::Spinorfield_eo> >::iterator physics::lattices::wilson::Rooted_Spinorfield_eo::end()
{
    return pseudofermions.end();
}

std::vector<std::unique_ptr<physics::lattices::Spinorfield_eo> >::const_iterator physics::lattices::wilson::Rooted_Spinorfield_eo::end() const
{
    return pseudofermions.end();
}
