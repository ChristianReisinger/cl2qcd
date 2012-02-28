// alpha*x
__kernel void sax_eoprec(__global const spinorStorageType * const restrict x, __global const hmc_complex * const restrict alpha, __global spinorStorageType * const restrict out)
{
	int id = get_global_id(0);
	int global_size = get_global_size(0);

	hmc_complex alpha_tmp = complexLoadHack(alpha);
	for(int id_tmp = id; id_tmp < EOPREC_SPINORFIELDSIZE; id_tmp += global_size) {
		spinor x_tmp = getSpinorSOA_eo(x, id_tmp);
		x_tmp = spinor_times_complex(x_tmp, alpha_tmp);
		putSpinorSOA_eo(out, id_tmp, x_tmp);
	}
}
