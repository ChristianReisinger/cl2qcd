/** @FILE
 *
 * COPYRIGHT (C) 2012-2013 Christopher Pinke <pinke@th.uni-frankfurt.de>
 * Copyright (c) 2014 Christopher Pinke
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

#include "exceptions.hpp"

#include <sstream>

static std::string create_solver_stuck_message(int iterations);
physics::algorithms::solvers::SolverStuck::SolverStuck(int iterations, std::string filename, int linenumber)
    : SolverException(create_solver_stuck_message(iterations), iterations, filename, linenumber)
{
}

static std::string create_solver_stuck_message(int iterations)
{
    std::ostringstream tmp;
    tmp << "Solver got stuck after " << iterations << " iterations";
    return tmp.str();
}
