/** @file
 * Implementation of the su3heatbath algorithm
 *
 * Copyright (c) 2012,2013 Matthias Bach
 * Copyright (c) 2014 Christopher Pinke
 * Copyright (c) 2015 Francesca Cuteri
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

#include "su3heatbath.hpp"

#include "../../hardware/code/heatbath.hpp"
#include "../../hardware/device.hpp"

#include <set>
#include <vector>
#include <cassert>
#include <exception>

void physics::algorithms::su3heatbath(physics::lattices::Gaugefield& gf, physics::PRNG& prng, int overrelax,
		const std::set<int>& fixed_timeslices)
{
    assert(overrelax >= 0);
    assert(gf.get_buffers().size() == 1);

    // run su3heatbath
    auto gf_dev   = gf.get_buffers()[0];
    auto prng_dev = prng.get_buffers()[0];
    auto code     = gf_dev->get_device()->getHeatbathCode();

    std::vector<int> fixed_timeslices_vec(fixed_timeslices.begin(), fixed_timeslices.end());
    if(fixed_timeslices_vec.empty())
        fixed_timeslices_vec.push_back(-1);
    hardware::buffers::Plain<int> fixed_timeslices_buf(fixed_timeslices_vec.size(), gf_dev->get_device());
    fixed_timeslices_buf.load(fixed_timeslices_vec.data());

    code->run_heatbath(gf_dev, prng_dev, fixed_timeslices_buf);

    // add overrelaxation
    if (overrelax > 0) {
	if(!fixed_timeslices.empty())
		throw std::logic_error("Overrelaxation with fixed timeslices is not implemented");
        physics::algorithms::overrelax(gf, prng, overrelax);
    }
}

void physics::algorithms::overrelax(physics::lattices::Gaugefield& gf, physics::PRNG& prng, int steps)
{
    assert(steps > 0);
    assert(gf.get_buffers().size() == 1);

    auto gf_dev   = gf.get_buffers()[0];
    auto prng_dev = prng.get_buffers()[0];
    auto code     = gf_dev->get_device()->getHeatbathCode();

    for (int i = 0; i < steps; ++i)
        code->run_overrelax(gf_dev, prng_dev);
}
