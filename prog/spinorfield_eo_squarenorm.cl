// hmc_float squarenorm, return in result
// --> use 2 kernels: 1 for the summation in one block and 1 for summation over blockresults
/// NOTE: The reduction used in this kernel is only safe with ls being a power of 2 and bigger than 8!
__kernel void global_squarenorm_eoprec( __global const spinorStorageType * const restrict x, __global hmc_float * const restrict result, __local hmc_float * const restrict result_local )
{
	int local_size = get_local_size(0);
	int global_size = get_global_size(0);
	int id = get_global_id(0);
	int group_id = get_group_id (0);
	int idx = get_local_id(0);

	hmc_float sum;
	sum = 0.;

	for(int id_local = id; id_local < EOPREC_SPINORFIELDSIZE_LOCAL; id_local += global_size) {
		site_idx id_mem = get_eo_site_idx_from_st_idx(get_even_st_idx_local(id_local));
		spinor x_tmp = getSpinor_eo(x, id_mem);
		hmc_float tmp = spinor_squarenorm(x_tmp);
		sum += tmp;
	}

	if(local_size == 1) {
		result[ group_id ] = sum;
	} else {
		result_local[idx] = sum;
		// sync threads
		barrier(CLK_LOCAL_MEM_FENCE);

		//reduction until threads 0-7 hold all partial sums
		int cut1;
		int cut2 = local_size;
		for(cut1 = local_size / 2; cut1 > 4; cut1 /= 2) {
		  for(int i = idx + cut1; i < cut2; i += cut1) {
		    result_local[idx] +=  result_local[i];
		  }
		  barrier(CLK_LOCAL_MEM_FENCE);
		  cut2 = cut1;
		}
		//thread 0 sums up the last 8 results and stores them in the global buffer
		if (idx == 0) {
		  result[ group_id ] =  result_local[0] + result_local[1] + result_local[2] + result_local[3] +
		    result_local[4] + result_local[5] + result_local[6] + result_local[7];
		}
	}
	return;
}
