//this is the kernel merging dslash_eo and gamma5

//Here, two cases are possible:
//evenodd = ODD or EVEN
//	ODD corresponds to the D_oe case: Dslash acts on even indices (the "x+mu" in the formulae) and the function
//	saves the outcoming spinor with an odd index.
//	EVEN is then D_eo.

__kernel void dslash_AND_M_tm_inverse_sitediagonal_eo(__global const spinorStorageType * const restrict in, __global spinorStorageType * const restrict out, __global const Matrixsu3StorageType * const restrict field, const int evenodd, hmc_float kappa_in)
{
	PARALLEL_FOR(id_tmp, EOPREC_SPINORFIELDSIZE) {
		st_idx pos = (evenodd == ODD) ? get_even_st_idx(id_tmp) : get_odd_st_idx(id_tmp);

		spinor out_tmp = set_spinor_zero();
		spinor out_tmp2;

		//calc dslash (this includes mutliplication with kappa)

		out_tmp2 = dslash_eoprec_unified_local(in, field, pos, TDIR, kappa_in);
		out_tmp = spinor_dim(out_tmp, out_tmp2);
		out_tmp2 = dslash_eoprec_unified_local(in, field, pos, XDIR, kappa_in);
		out_tmp = spinor_dim(out_tmp, out_tmp2);
		out_tmp2 = dslash_eoprec_unified_local(in, field, pos, YDIR, kappa_in);
		out_tmp = spinor_dim(out_tmp, out_tmp2);
		out_tmp2 = dslash_eoprec_unified_local(in, field, pos, ZDIR, kappa_in);
		out_tmp = spinor_dim(out_tmp, out_tmp2);
		
		//gamma5 part
		out_tmp = gamma5_local(out_tmp);

		putSpinor_eo(out, id_tmp, out_tmp);
	}
}
