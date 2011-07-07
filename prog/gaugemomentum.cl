/**
 * @file operations on algebraelements
 */

/** @todo this can propably be defined with a minus...*/
ae acc_factor_times_algebraelement(ae in, hmc_float factor, ae force_in)
{
	ae tmp;
	tmp.e0 = in.e0 + factor * force_in.e0;
	tmp.e1 = in.e1 + factor * force_in.e1;
	tmp.e2 = in.e2 + factor * force_in.e2;
	tmp.e3 = in.e3 + factor * force_in.e3;
	tmp.e4 = in.e4 + factor * force_in.e4;
	tmp.e5 = in.e5 + factor * force_in.e5;
	tmp.e6 = in.e6 + factor * force_in.e6;
	tmp.e7 = in.e7 + factor * force_in.e7;
	return tmp;
}

void update_gaugemomentum(ae in, hmc_float factor, int global_link_pos, __global ae * out)
{
	ae tmp = out[global_link_pos];
	tmp = acc_factor_times_algebraelement(tmp, factor, in);
	out[global_link_pos] = tmp;
}

//calculates the trace of i times generator times 3x3-matrix and stores this in a su3-algebraelement
//now using structs
ae tr_lambda_u(Matrix3x3 in)
{
	ae tmp;
	tmp.e0 = ( -in.e10.im - in.e01.im);
	tmp.e1 = (+in.e10.re - in.e01.re);
	tmp.e2 = (-in.e00.im + in.e11.im);
	tmp.e3 = (-in.e20.im - in.e02.im);
	tmp.e4 = (+in.e20.re - in.e02.re);
	tmp.e5 = (-in.e21.im - in.e12.im);
	tmp.e6 = (+in.e21.re - in.e12.re);
	tmp.e7 = (-in.e00.im - in.e11.im + 2.0 * in.e22.im) * 0.577350269189625;
	return tmp;
}
