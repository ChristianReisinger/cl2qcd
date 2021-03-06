/*
 * Copyright (c) 2011 Christopher Pinke
 * Copyright (c) 2012,2013 Matthias Bach
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

__kernel void heatbath_odd(__global Matrixsu3StorageType* const restrict gaugefield, const int mu,
                           __global rngStateStorageType* const restrict rngStates,
                           const int fixed_timeslice_num, __constant const int* const fixed_timeslices)
{
    prng_state rnd;
    prng_loadState(&rnd, rngStates);

    PARALLEL_FOR (id, VOL4D_LOCAL / 2) {
        st_index pos = get_odd_st_idx_local(id);
        const coord_temporal t_GLOBAL = GRID_POS_LOCAL * T_EXTENT_LOCAL + pos.time;
        bool is_fixed = false;
        for(int i = 0; i < fixed_timeslice_num; ++i)
        	if(t_GLOBAL == fixed_timeslices[i]) {
        		is_fixed = true;
        		break;
        	}
        if(!is_fixed || mu == 0)
        	perform_heatbath(gaugefield, mu, &rnd, pos.space, pos.time);
    }

    prng_storeState(rngStates, &rnd);
}
