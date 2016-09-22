/*
 * Copyright 2012, 2013 Lars Zeidlewicz, Christopher Pinke,
 * Matthias Bach, Christian Schäfer, Stefano Lottini, Alessandro Sciarra
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

/*
 clover-term fermion matrix 1+T for eoprec spinorfields (c.f. paper of Jansen, Liu: Implementation of Symanzik's improvement program for simulations of dynamical wilson fermions in lattice QCD, URL:http://arxiv.org/abs/hep-lat/9603008)
 T=i/2*c_sw*kappa*sigma_{\mu \nu}*F_{\mu \nu}*delta_xy, with F=field strength tensor on the lattice
 sigma_{\mu \nu} = i/2 * [gamma_\mu, gamma_\nu]
*/

su3vec field_strength_tensor_times_su3vec(su3vec in, __global Matrixsu3StorageType  const * const restrict field, const st_idx idx_arg, const dir_idx dir1, const dir_idx dir2)
{
    //calculation of the lattice-field-strength-tensor according to the paper of Jansen and Liu, equation (6)
    //it consits of a sum of 8 terms which are products of link variables
    //psi1 = 1.term * in
    //dir1 = mu, dir2 = nu
    su3vec out;
    st_idx idx_neigh, idx_neigh1;
    Matrixsu3 U;
    //////////////////////
    // 1.term = U_mu(x) * U_nu(x+mu) * U_mu(x+nu)^dagger * U_nu(x)^dagger
    su3vec psi1 = in;
    //////////////////////
    // U_nu(x)^dagger
    U = getSU3(field, get_link_idx(dir2, idx_arg));
    psi1 = su3matrix_dagger_times_su3vec(U, psi1);
    //////////////////////
    // U_mu(x+nu)^dagger
    idx_neigh = get_neighbor_from_st_idx(idx_arg, dir2);
    U = getSU3(field, get_link_idx(dir1, idx_neigh));
    psi1 = su3matrix_dagger_times_su3vec(U, psi1);
    //////////////////////
    // U_nu(x+mu)
    idx_neigh = get_neighbor_from_st_idx(idx_arg, dir1);
    U = getSU3(field, get_link_idx(dir2, idx_neigh));
    psi1 = su3matrix_times_su3vec(U, psi1);
    //////////////////////
    // U_mu(x)
    U = getSU3(field, get_link_idx(dir1, idx_arg));
    psi1 = su3matrix_times_su3vec(U, psi1);
    
    //////////////////////
    // 2.term = U_nu(x) * U_mu(x+nu-mu)^dagger * U_nu(x-nu)^dagger * U_mu(x-mu)
    su3vec psi2 = in;
    //////////////////////
    // U_mu(x-mu)
    idx_neigh = get_lower_neighbor_from_st_idx(idx_arg, dir1);
    U = getSU3(field, get_link_idx(dir1, idx_neigh));
    psi2 = su3matrix_times_su3vec(U, psi2);
    //////////////////////
    // U_nu(x_nu)^dagger
    idx_neigh = get_lower_neighbor_from_st_idx(idx_arg, dir2);
    U = getSU3(field, get_link_idx(dir2, idx_neigh));
    psi2 = su3matrix_dagger_times_su3vec(U, psi2);
    //////////////////////
    // U_mu(x+nu-mu)^dagger
    idx_neigh1 = get_neighbor_from_st_idx(idx_arg, dir2); // x+nu
    idx_neigh = get_lower_neighbor_from_st_idx(idx_neigh1, dir1); //(x+nu)-mu
    U = getSU3(field, get_link_idx(dir1, idx_neigh));
    psi2 = su3matrix_dagger_times_su3vec(U, psi2);
    //////////////////////
    // U_nu(x)
    U = getSU3(field, get_link_idx(dir2, idx_arg));
    psi2 = su3matrix_times_su3vec(U, psi2);
    
    //////////////////////
    // 3.term = U_mu(x-mu)^dagger * U_nu(x-mu-nu)^dagger * U_mu(x-mu-nu) * U_nu(x-nu)
    su3vec psi3 = in;
    //////////////////////
    // U_nu(x-nu)
    idx_neigh = get_lower_neighbor_from_st_idx(idx_arg, dir2);
    U = getSU3(field, get_link_idx(dir2, idx_neigh));
    psi3 = su3matrix_times_su3vec(U, psi3);
    //////////////////////
    // U_mu(x-mu-nu)
    idx_neigh1 = get_lower_neighbor_from_st_idx(idx_arg, dir1); // x-mu
    idx_neigh = get_neighbor_from_st_idx(idx_neigh1, dir2); // (x-mu)-nu
    U = getSU3(field, get_link_idx(dir1, idx_neigh));
    psi3 = su3matrix_times_su3vec(U, psi3);
    //////////////////////
    // U_nu(x-mu-nu)^dagger
    idx_neigh1 = get_lower_neighbor_from_st_idx(idx_arg, dir1); // x-mu
    idx_neigh = get_neighbor_from_st_idx(idx_neigh1, dir2); // (x-mu)-nu
    U = getSU3(field, get_link_idx(dir2, idx_neigh));
    psi3 = su3matrix_dagger_times_su3vec(U, psi3);
    //////////////////////
    // U_mu(x-mu)^dagger
    idx_neigh = get_lower_neighbor_from_st_idx(idx_arg, dir1);
    U = getSU3(field, get_link_idx(dir1, idx_neigh));
    psi3 = su3matrix_dagger_times_su3vec(U, psi3);
    
    ///////////////////////
    // 4.term = U_nu(x-nu)^dagger * U_mu(x-nu) * U_nu(x-nu+mu) * U_mu(x)^dagger
    su3vec psi4 = in;
    // U_mu(x)^dagger
    U = getSU3(field, get_link_idx(dir1, idx_arg));
    psi4 = su3matrix_dagger_times_su3vec(U, psi4);
    ////////////////////////
    // U_nu(x-nu+mu)
    idx_neigh1 = get_lower_neighbor_from_st_idx(idx_arg, dir2); // x-nu
    idx_neigh = get_neighbor_from_st_idx(idx_neigh1, dir1); // (x-nu)+mu
    U = getSU3(field, get_link_idx(dir2, idx_neigh));
    psi4 = su3matrix_times_su3vec(U, psi4);
    ////////////////////////
    // U_mu(x-nu)
    idx_neigh = get_lower_neighbor_from_st_idx(idx_arg, dir2);
    U = getSU3(field, get_link_idx(dir1, idx_neigh));
    psi4 = su3matrix_times_su3vec(U, psi4);
    ////////////////////////
    // U_nu(x-nu)^dagger
    idx_neigh = get_lower_neighbor_from_st_idx(idx_arg, dir2);
    U = getSU3(field, get_link_idx(dir2, idx_neigh));
    psi4 = su3matrix_dagger_times_su3vec(U, psi4);
    
    ////////////////////////
    // 5.term = U_nu(x) * U_mu(x+nu) * U_nu(x+mu)^dagger * U_mu(x)^dagger
    su3vec psi5 = in;
    // U_mu(x)^dagger
    U = getSU3(field, get_link_idx(dir1, idx_arg));
    psi5 = su3matrix_dagger_times_su3vec(U, psi5);
    ////////////////////////
    // U_nu(x+mu)^dagger
    idx_neigh = get_neighbor_from_st_idx(idx_arg, dir1);
    U = getSU3(field, get_link_idx(dir2, idx_neigh));
    psi5 = su3matrix_dagger_times_su3vec(U, psi5);
    ////////////////////////
    // U_mu(x+nu)
    idx_neigh = get_neighbor_from_st_idx(idx_arg, dir2);
    U = getSU3(field, get_link_idx(dir1, idx_neigh));
    psi5 = su3matrix_times_su3vec(U, psi5);
    /////////////////////////
    // U_nu(x)
    U = getSU3(field, get_link_idx(dir2, idx_arg));
    psi5 = su3matrix_times_su3vec(U, psi5);
    
    //////////////////////////
    // 6.term = U_mu(x-mu)^dagger * U_nu(x-mu) * U_mu(x+nu-mu) * U_nu(x)^dagger
    su3vec psi6 = in;
    // U_nu(x)^dagger
    U = getSU3(field, get_link_idx(dir2, idx_arg));
    psi6 = su3matrix_dagger_times_su3vec(U, psi6);
    //////////////////////////
    // U_mu(x+nu-mu)
    idx_neigh1 = get_neighbor_from_st_idx(idx_arg, dir2); // x+nu
    idx_neigh = get_lower_neighbor_from_st_idx(idx_neigh1, dir1); //(x+nu)-mu
    U = getSU3(field, get_link_idx(dir1, idx_neigh));
    psi6 = su3matrix_times_su3vec(U, psi6);
    /////////////////////////
    // U_nu(x-mu)
    idx_neigh = get_lower_neighbor_from_st_idx(idx_arg, dir1);
    U = getSU3(field, get_link_idx(dir2, idx_neigh));
    psi6 = su3matrix_times_su3vec(U, psi6);
    // U_mu(x-mu)^dagger
    idx_neigh = get_lower_neighbor_from_st_idx(idx_arg, dir1);
    U = getSU3(field, get_link_idx(dir1, idx_neigh));
    psi6 = su3matrix_dagger_times_su3vec(U, psi6);
    
    ////////////////////////////
    // 7.term = U_nu(x-nu)^dagger * U_mu(x-nu-mu)^dagger * U_nu(x-nu-mu) * U_mu(x-mu)
    su3vec psi7 = in;
    // U_mu(x-mu)
    idx_neigh = get_lower_neighbor_from_st_idx(idx_arg, dir1);
    U = getSU3(field, get_link_idx(dir1, idx_neigh));
    psi7 = su3matrix_times_su3vec(U, psi7);
    // U_nu(x-nu-mu)
    idx_neigh1 = get_lower_neighbor_from_st_idx(idx_arg, dir1); // x-mu
    idx_neigh = get_neighbor_from_st_idx(idx_neigh1, dir2); // (x-mu)-nu
    U = getSU3(field, get_link_idx(dir2, idx_neigh));
    psi7 = su3matrix_times_su3vec(U, psi7);
    // U_mu(x-nu-mu)^dagger
    idx_neigh1 = get_lower_neighbor_from_st_idx(idx_arg, dir1); // x-mu
    idx_neigh = get_neighbor_from_st_idx(idx_neigh1, dir2); // (x-mu)-nu
    U = getSU3(field, get_link_idx(dir1, idx_neigh));
    psi7 = su3matrix_dagger_times_su3vec(U, psi7);
    // U_nu(x-nu)^dagger
    idx_neigh = get_lower_neighbor_from_st_idx(idx_arg,dir2);
    U = getSU3(field, get_link_idx(dir2, idx_neigh));
    psi7 = su3matrix_dagger_times_su3vec(U, psi7);
    
    ///////////////////////////
    // 8.term = U_mu(x) * U_nu(x-nu+mu)^dagger * U_mu(x-nu)^dagger * U_nu(x-nu)
    su3vec psi8 = in;
    // U_nu(x-nu)
    idx_neigh = get_lower_neighbor_from_st_idx(idx_arg, dir2);
    U = getSU3(field, get_link_idx(dir2, idx_neigh));
    psi8 = su3matrix_times_su3vec(U, psi8);
    // U_mu(x-nu)^dagger
    idx_neigh = get_lower_neighbor_from_st_idx(idx_arg, dir2);
    U = getSU3(field, get_link_idx(dir1, idx_neigh));
    psi8 = su3matrix_dagger_times_su3vec(U, psi8);
    // U_nu(x-nu+mu)^dagger
    idx_neigh1 = get_lower_neighbor_from_st_idx(idx_arg, dir2); // x-nu
    idx_neigh = get_neighbor_from_st_idx(idx_neigh1, dir1); // (x-nu)+mu
    U = getSU3(field, get_link_idx(dir2, idx_neigh));
    psi8 = su3matrix_dagger_times_su3vec(U, psi8);
    // U_mu(x)
    U = getSU3(field, get_link_idx(dir1, idx_arg));
    psi8 = su3matrix_times_su3vec(U, psi8);
    
    
    //add psi1,...,psi8 and multiply by factor 1/8
    hmc_float factor = 1/8;
    out = psi1;
    out = su3vec_acc(out, psi2);
    out = su3vec_acc(out, psi3);
    out = su3vec_acc(out, psi4);
    out = su3vec_acc(out, psi5);
    out = su3vec_acc(out, psi6);
    out = su3vec_acc(out, psi7);
    out = su3vec_acc(out, psi8);
    out = su3vec_times_real(out, factor);
    
    return out;
}

spinor field_strength_tensor_times_spinor(spinor in, __global Matrixsu3StorageType  const * const restrict field, const st_idx idx_arg, const dir_idx dir1, const dir_idx dir2)
{
    spinor out = set_spinor_zero();

    out.e0 = field_strength_tensor_times_su3vec(in.e0, field, idx_arg, dir1, dir2);
    out.e1 = field_strength_tensor_times_su3vec(in.e1, field, idx_arg, dir1, dir2);
    out.e2 = field_strength_tensor_times_su3vec(in.e2, field, idx_arg, dir1, dir2);
    out.e3 = field_strength_tensor_times_su3vec(in.e3, field, idx_arg, dir1, dir2);

    return out;
}

spinor clover_eoprec_unified_local(__global const spinorStorageType * const restrict in, __global Matrixsu3StorageType  const * const restrict field, const st_idx idx_arg, const dir_idx dir, hmc_float kappa_in, hmc_float csw)
{
    dir_idx dir2;
    
    spinor out_tmp, phi, tmp, psi;
    site_idx pos_eo;
    
    out_tmp = set_spinor_zero();
    pos_eo = get_eo_site_idx_from_st_idx(idx_arg);
    phi = getSpinor_eo(in, pos_eo);
    
    //this is used to save the BC-conditions...
    hmc_complex bc_tmp = (dir == TDIR) ? (hmc_complex) {
        1./4. * csw * kappa_in * TEMPORAL_RE, 1./4. * csw * kappa_in * TEMPORAL_IM
    } :
    (hmc_complex) {
        1./4. * csw * kappa_in * SPATIAL_RE, 1./4. * csw * kappa_in * SPATIAL_IM
    };

    
    if(dir == TDIR) {
        /////////////////////////////////
        // nu = 1
        dir2 = XDIR;
        /////////////////////////////////
        //Calculate sigma_01 * phi
        psi = sigma_mu_nu_times_spinor(phi, dir, dir2);
        //calculate field-strength-tensor * psi
	tmp = field_strength_tensor_times_spinor(psi, field, idx_arg, dir, dir2);
        out_tmp = spinor_acc(out_tmp, tmp);
        
        /////////////////////////////////
        // nu = 2
        dir2 = YDIR;
        /////////////////////////////////
        //Calculate sigma_02 * phi
        psi = sigma_mu_nu_times_spinor(phi, dir, dir2);
        //calculate field-strength-tensor * psi
	tmp = field_strength_tensor_times_spinor(psi, field, idx_arg, dir, dir2);
        out_tmp = spinor_acc(out_tmp, tmp);
        
        
        /////////////////////////////////
        // nu = 3
        dir2 = ZDIR;
        /////////////////////////////////
        //Calculate sigma_03 * phi
        psi = sigma_mu_nu_times_spinor(phi, dir, dir2);
        //calculate field-strength-tensor * psi
	tmp = field_strength_tensor_times_spinor(psi, field, idx_arg, dir, dir2);
        out_tmp = spinor_acc(out_tmp, tmp);
        
    }
    if(dir == XDIR) {
        /////////////////////////////////
        // nu = 0
        dir2 = TDIR;
        /////////////////////////////////
        //Calculate sigma_10 * phi
        psi = sigma_mu_nu_times_spinor(phi, dir, dir2);
        //calculate field-strength-tensor * psi
	tmp = field_strength_tensor_times_spinor(psi, field, idx_arg, dir, dir2);
        out_tmp = spinor_acc(out_tmp, tmp);
        
        /////////////////////////////////
        // nu = 2
        dir2 = YDIR;
        /////////////////////////////////
        //Calculate sigma_12 * phi
        psi = sigma_mu_nu_times_spinor(phi, dir, dir2);
        //calculate field-strength-tensor * psi
	tmp = field_strength_tensor_times_spinor(psi, field, idx_arg, dir, dir2);
        out_tmp = spinor_acc(out_tmp, tmp);
        
        /////////////////////////////////
        // nu = 3
        dir2 = ZDIR;
        /////////////////////////////////
        //Calculate sigma_13 * phi
        psi = sigma_mu_nu_times_spinor(phi, dir, dir2);
        //calculate field-strength-tensor * psi
	tmp = field_strength_tensor_times_spinor(psi, field, idx_arg, dir, dir2);
        out_tmp = spinor_acc(out_tmp, tmp);
    }
    if(dir == YDIR) {
        /////////////////////////////////
        // nu = 0
        dir2 = TDIR;
        /////////////////////////////////
        //Calculate sigma_20 * phi
        psi = sigma_mu_nu_times_spinor(phi, dir, dir2);
        //calculate field-strength-tensor * psi
	tmp = field_strength_tensor_times_spinor(psi, field, idx_arg, dir, dir2);
        out_tmp = spinor_acc(out_tmp, tmp);
        
        
        /////////////////////////////////
        // nu = 1
        dir2 = XDIR;
        /////////////////////////////////
        //Calculate sigma_21 * phi
        psi = sigma_mu_nu_times_spinor(phi, dir, dir2);
        //calculate field-strength-tensor * psi
	tmp = field_strength_tensor_times_spinor(psi, field, idx_arg, dir, dir2);
        out_tmp = spinor_acc(out_tmp, tmp);

        /////////////////////////////////
        // nu = 3
        dir2 = ZDIR;
        /////////////////////////////////
        //Calculate sigma_23 * phi
        psi = sigma_mu_nu_times_spinor(phi, dir, dir2);
        //calculate field-strength-tensor * psi
	tmp = field_strength_tensor_times_spinor(psi, field, idx_arg, dir, dir2);
        out_tmp = spinor_acc(out_tmp, tmp);
    }
    if(dir == ZDIR) {
        /////////////////////////////////
        // nu = 0
        dir2 = TDIR;
        /////////////////////////////////
        //Calculate sigma_30 * phi
        psi = sigma_mu_nu_times_spinor(phi, dir, dir2);
        //calculate field-strength-tensor * psi
	tmp = field_strength_tensor_times_spinor(psi, field, idx_arg, dir, dir2);
        out_tmp = spinor_acc(out_tmp, tmp);
        
        /////////////////////////////////
        // nu = 1
        dir2 = XDIR;
        /////////////////////////////////
        //Calculate sigma_31 * phi
        psi = sigma_mu_nu_times_spinor(phi, dir, dir2);
        //calculate field-strength-tensor * psi
	tmp = field_strength_tensor_times_spinor(psi, field, idx_arg, dir, dir2);
        out_tmp = spinor_acc(out_tmp, tmp);
        
        
        /////////////////////////////////
        // nu = 2
        dir2 = YDIR;
        /////////////////////////////////
        //Calculate sigma_32 * phi
        psi = sigma_mu_nu_times_spinor(phi, dir, dir2);
        //calculate field-strength-tensor * psi
	tmp = field_strength_tensor_times_spinor(psi, field, idx_arg, dir, dir2);
        out_tmp = spinor_acc(out_tmp, tmp);  
    }

    out_tmp = spinor_times_complex(out_tmp, bc_tmp);
    out_tmp = spinor_times_complex(out_tmp, hmc_complex_i);
    return out_tmp;
}


void clover_eo_for_site(__global const spinorStorageType * const restrict in, __global spinorStorageType * const restrict out, __global const Matrixsu3StorageType * const restrict field, hmc_float kappa_in, hmc_float csw, st_idx const pos)
{
    spinor out_tmp = set_spinor_cold(); //note: clover = 1 + T
    spinor out_tmp2;
    
    out_tmp2 = clover_eoprec_unified_local(in, field, pos, TDIR, kappa_in, csw);
    out_tmp = spinor_acc(out_tmp, out_tmp2);
    out_tmp2 = clover_eoprec_unified_local(in, field, pos, XDIR, kappa_in, csw);
    out_tmp = spinor_acc(out_tmp, out_tmp2);
    out_tmp2 = clover_eoprec_unified_local(in, field, pos, YDIR, kappa_in, csw);
    out_tmp = spinor_acc(out_tmp, out_tmp2);
    out_tmp2 = clover_eoprec_unified_local(in, field, pos, ZDIR, kappa_in, csw);
    out_tmp = spinor_acc(out_tmp, out_tmp2);
    
    putSpinor_eo(out, get_eo_site_idx_from_st_idx(pos), out_tmp);
}

__kernel void clover_eo(__global const spinorStorageType * const restrict in, __global spinorStorageType * const restrict out, __global const Matrixsu3StorageType * const restrict field, const int evenodd, hmc_float kappa_in, hmc_float csw)
{
    PARALLEL_FOR(id_local, EOPREC_SPINORFIELDSIZE_LOCAL) {
        st_idx pos = (evenodd == EVEN) ? get_even_st_idx_local(id_local) : get_odd_st_idx_local(id_local);
        clover_eo_for_site(in, out, field, kappa_in, csw, pos);
    }
}
