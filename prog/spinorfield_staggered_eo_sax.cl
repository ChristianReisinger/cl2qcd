// Description of variables of sax:
//  - x: The input staggered field (an su3vec per each site => vector of VOL4D/2
//       components that are su3vec varibles)
//  - aplha: The complex number by which x has to be multiplied
//  - out: The output staggered field: alpha*x (site by site)

__kernel void sax_staggered_eoprec(__global const staggeredStorageType * const x, __global const hmc_complex * const alpha, __global staggeredStorageType * const out)
{
	int id = get_global_id(0);
	int global_size = get_global_size(0);

	hmc_complex alpha_tmp = complexLoadHack(alpha);
	for(int id_mem = id; id_mem < EOPREC_SPINORFIELDSIZE_MEM; id_mem += global_size) {
		su3vec x_tmp = get_su3vec_from_field_eo(x, id_mem);
		x_tmp = su3vec_times_complex(x_tmp, alpha_tmp);
		put_su3vec_to_field_eo(out, id_mem, x_tmp);
	}
}