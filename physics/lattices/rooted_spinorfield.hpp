/** @file
 * Declaration of the physics::lattices::wilson:Rooted_Spinorfield class
 *
 * Copyright (c) 2016 Christopher Czaban <czaban@th.physik.uni-frankfurt.de>
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

#ifndef _PHYSICS_LATTICES_ROOTED_SPINORFIELD_
#define _PHYSICS_LATTICES_ROOTED_SPINORFIELD_

#include "../../hardware/system.hpp"
#include "../algorithms/rational_approximation.hpp"
#include "spinorfield.hpp"
#include "util.hpp" //This is to make the template pseudo_randomize friend of this class

namespace physics {
	namespace lattices {
		namespace wilson {

			//Representation of a rooted spinorfield without eo preconditioning
		    class Rooted_Spinorfield {

				public:
				Rooted_Spinorfield(const hardware::System&, const RootedSpinorfieldParametersInterface&);
				Rooted_Spinorfield(const hardware::System&, const RootedSpinorfieldParametersInterface&, const physics::algorithms::Rational_Approximation& approx);

				~Rooted_Spinorfield(){};

				//Rescale coefficients on the basis of a Rational_Approximation objects
				void Rescale_Coefficients(const physics::algorithms::Rational_Approximation& approx, const hmc_float minEigenvalue, const hmc_float maxEigenvalue);

				Rooted_Spinorfield& operator=(const Rooted_Spinorfield&) = delete;
				Rooted_Spinorfield(const Rooted_Spinorfield&) = delete;
				Rooted_Spinorfield() = delete;

                unsigned int getOrder() const;
                hmc_float get_a0() const;
                std::vector<hmc_float> get_a() const;
                std::vector<hmc_float> get_b() const;
                /**
                 * This method returns the asked pseudofermion field
                 */
                const std::unique_ptr<physics::lattices::Spinorfield>& operator[](unsigned int) const;

                /*
                 * Methods to be able to make range based for loops on Rooted_Staggeredfield_eo objects
                 */
                std::vector<std::unique_ptr<physics::lattices::Spinorfield> >::iterator begin();
                std::vector<std::unique_ptr<physics::lattices::Spinorfield> >::const_iterator begin() const;
                std::vector<std::unique_ptr<physics::lattices::Spinorfield> >::iterator end();
                std::vector<std::unique_ptr<physics::lattices::Spinorfield> >::const_iterator end() const;

				private:
                std::vector<std::unique_ptr<physics::lattices::Spinorfield> > pseudofermions;
				physics::algorithms::Rational_Coefficients rationalCoefficients;

				friend void pseudo_randomize<Rooted_Spinorfield, spinor>(const Rooted_Spinorfield* to, int seed);
			};
		}
	}
}

#endif /*_PHYSICS_LATTICES_ROOTED_SPINORFIELD_ */
