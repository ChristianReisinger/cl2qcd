/** @file
 * Device code for operations on the fermion matrix
 */

//opencl_operations_gaugefield.cl

void get_su3matrix(__private hmc_ocl_su3matrix* out, __global hmc_ocl_gaugefield * in, int spacepos, int timepos, int mu)
{
#ifdef _RECONSTRUCT_TWELVE_
	for(int a=0; a<NC-1; a++) {
#else
	for(int a=0; a<NC; a++) {
#endif
		for(int b=0; b<NC; b++) {
			out[ocl_su3matrix_element(a,b)].re = in[ocl_gaugefield_element(0,a,b,mu,spacepos,timepos)];
			out[ocl_su3matrix_element(a,b)].im = in[ocl_gaugefield_element(1,a,b,mu,spacepos,timepos)];
		}
#ifdef _RECONSTRUCT_TWELVE_
	}
#else
	}
#endif
	return;
}

void put_su3matrix(__global hmc_ocl_gaugefield * field, __private hmc_ocl_su3matrix * in, int spacepos, int timepos, int mu)
{
#ifdef _RECONSTRUCT_TWELVE_
	for(int a=0; a<NC-1; a++) {
#else
	for(int a=0; a<NC; a++) {
#endif
		for(int b=0; b<NC; b++) {
			field[ocl_gaugefield_element(0,a,b,mu,spacepos,timepos)] = in[ocl_su3matrix_element(a,b)].re;
			field[ocl_gaugefield_element(1,a,b,mu,spacepos,timepos)] = in[ocl_su3matrix_element(a,b)].im;
		}
#ifdef _RECONSTRUCT_TWELVE_
	}
#else
	}
#endif
	return;
}

void copy_su3matrix(__private hmc_ocl_su3matrix * out, __private hmc_ocl_su3matrix * in)
{
	for(int n = 0; n<SU3SIZE; n++) {
		(out[n]).re = (in[n]).re;
		(out[n]).im = (in[n]).im;
	}
	return;
}

void unit_su3matrix(__private hmc_ocl_su3matrix *out)
{
#ifdef _RECONSTRUCT_TWELVE_
	for(int a = 0; a < NC-1; a++) {
#else
	for(int a = 0; a < NC; a++) {
#endif
		for(int b = 0; b < NC; b++) {
			if(a==b) {
				out[ocl_su3matrix_element(a,b)].re = hmc_one_f;
			} else {
				out[ocl_su3matrix_element(a,b)].re = 0;
			}
			out[ocl_su3matrix_element(a,b)].im = 0;
		}
#ifdef _RECONSTRUCT_TWELVE_
	}
#else
	}
#endif
	return;
}

void zero_su3matrix(__private hmc_ocl_su3matrix *out)
{
	for(int n=0; n<SU3SIZE; n++) {
		out[n].re = 0;
		out[n].im = 0;
	}
	return;
}

void multiply_su3matrices(__private hmc_ocl_su3matrix *out,__private hmc_ocl_su3matrix *p,__private hmc_ocl_su3matrix *q)
{
#ifdef _RECONSTRUCT_TWELVE_
	for(int n=0; n<NC*(NC-1); n++) {
		out[n].re=0;
		out[n].im=0;
		for(int j=0; j<NC; j++) {
			int k = (int)(n/(NC-1));
			int i = n - (NC-1)*k;
			int np = i + (NC-1)*j;
			hmc_complex qcomponent;
			if(j==2) {
				qcomponent = reconstruct_su3(q,k);
			} else {
				int nq = j + (NC-1)*k;
				qcomponent = q[nq];
			}
			hmc_complex tmp = complexmult(p[np], qcomponent);
			out[n] = complexadd(out[n], tmp);
		}
	}
#else
	out[0].re = p[0].re*q[0].re + p[3].re*q[1].re + p[6].re*q[2].re -
	            (p[0].im*q[0].im + p[3].im*q[1].im + p[6].im*q[2].im );
	out[3].re = p[0].re*q[3].re + p[3].re*q[4].re + p[6].re*q[5].re-
	            (p[0].im*q[3].im + p[3].im*q[4].im + p[6].im*q[5].im);
	out[6].re = p[0].re*q[6].re + p[3].re*q[7].re + p[6].re*q[8].re -
	            (p[0].im*q[6].im + p[3].im*q[7].im + p[6].im*q[8].im);
	out[1].re = p[1].re*q[0].re + p[4].re*q[1].re + p[7].re*q[2].re -
	            (p[1].im*q[0].im + p[4].im*q[1].im + p[7].im*q[2].im);
	out[4].re = p[1].re*q[3].re + p[4].re*q[4].re + p[7].re*q[5].re -
	            (p[1].im*q[3].im + p[4].im*q[4].im + p[7].im*q[5].im);
	out[7].re = p[1].re*q[6].re + p[4].re*q[7].re + p[7].re*q[8].re -
	            (p[1].im*q[6].im + p[4].im*q[7].im + p[7].im*q[8].im);
	out[2].re = p[2].re*q[0].re + p[5].re*q[1].re + p[8].re*q[2].re -
	            (p[2].im*q[0].im + p[5].im*q[1].im + p[8].im*q[2].im);
	out[5].re = p[2].re*q[3].re + p[5].re*q[4].re + p[8].re*q[5].re -
	            (p[2].im*q[3].im + p[5].im*q[4].im + p[8].im*q[5].im);
	out[8].re = p[2].re*q[6].re + p[5].re*q[7].re + p[8].re*q[8].re -
	            (p[2].im*q[6].im + p[5].im*q[7].im + p[8].im*q[8].im);

	out[0].im = p[0].re*q[0].im + p[3].re*q[1].im + p[6].re*q[2].im +
	            p[0].im*q[0].re + p[3].im*q[1].re + p[6].im*q[2].re;
	out[3].im = p[0].re*q[3].im + p[3].re*q[4].im + p[6].re*q[5].im +
	            p[0].im*q[3].re + p[3].im*q[4].re + p[6].im*q[5].re;
	out[6].im = p[0].re*q[6].im + p[3].re*q[7].im + p[6].re*q[8].im +
	            p[0].im*q[6].re + p[3].im*q[7].re + p[6].im*q[8].re;
	out[1].im = p[1].re*q[0].im + p[4].re*q[1].im + p[7].re*q[2].im +
	            p[1].im*q[0].re + p[4].im*q[1].re + p[7].im*q[2].re;
	out[4].im = p[1].re*q[3].im + p[4].re*q[4].im + p[7].re*q[5].im +
	            p[1].im*q[3].re + p[4].im*q[4].re + p[7].im*q[5].re ;
	out[7].im = p[1].re*q[6].im + p[4].re*q[7].im + p[7].re*q[8].im +
	            p[1].im*q[6].re + p[4].im*q[7].re + p[7].im*q[8].re;
	out[2].im = p[2].re*q[0].im + p[5].re*q[1].im + p[8].re*q[2].im +
	            p[2].im*q[0].re + p[5].im*q[1].re + p[8].im*q[2].re;
	out[5].im = p[2].re*q[3].im + p[5].re*q[4].im + p[8].re*q[5].im +
	            p[2].im*q[3].re + p[5].im*q[4].re + p[8].im*q[5].re;
	out[8].im = p[2].re*q[6].im + p[5].re*q[7].im + p[8].re*q[8].im +
	            p[2].im*q[6].re + p[5].im*q[7].re + p[8].im*q[8].re;


//     for(int i=0; i<NC; i++) {
//       for(int k=0; k<NC; k++) {
// 	out[ocl_su3matrix_element(i,k)].re=0;
// 	out[ocl_su3matrix_element(i,k)].im=0;
// 	for(int j=0;j<NC;j++) {
// 	  hmc_complex tmp = complexmult(&p[ocl_su3matrix_element(i,j)],&q[ocl_su3matrix_element(j,k)]);
// 	  if(i==k) complexaccumulate(&out[ocl_su3matrix_element(i,k)],&tmp);
// 	}
//       }
//     }
#endif
	return;
}

void accumulate_su3matrix_prod(__private hmc_ocl_su3matrix *acc,__private hmc_ocl_su3matrix *mult)
{
	hmc_ocl_su3matrix tmp[SU3SIZE];
	multiply_su3matrices(tmp,acc,mult);
	copy_su3matrix(acc,tmp);
	return;
}


void adjoin_su3matrix(__private hmc_ocl_su3matrix * mat)
{
#ifdef _RECONSTRUCT_TWELVE_
	hmc_ocl_su3matrix tmp[SU3SIZE];
	copy_su3matrix(tmp, mat);
	for(int n=0; n<NC*(NC-1); n++) {
		int j = (int)(n/(NC-1));
		int i = n - (NC-1)*j;
		hmc_complex element;
		if ( j==2 ) {
			element = reconstruct_su3(tmp,i);
		} else {
			int nnew = j + (NC-1)*i;
			element = tmp[nnew];
		}
		mat[n] = complexconj(element);
	}
#else
	hmc_ocl_su3matrix tmp[SU3SIZE];
	copy_su3matrix(tmp, mat);
	for(int a=0; a<NC; a++) {
		for(int b=0; b<NC; b++) {
			mat[ocl_su3matrix_element(a,b)] =
			  complexconj(tmp[ocl_su3matrix_element(b,a)]);
		}
	}
#endif
	return;
}

hmc_complex trace_su3matrix(__private hmc_ocl_su3matrix * mat)
{
	hmc_complex trace;
#ifdef _RECONSTRUCT_TWELVE_
	trace = reconstruct_su3(mat,NC-1);
	for(int n=0; n<(NC-1); n++)
		trace = complexadd( trace, mat[NC*n] );
#else
	trace.re=0;
	trace.im=0;
	for(int a=0; a<NC; a++)
		trace = complexadd( trace, mat[ocl_su3matrix_element(a,a)] );
#endif
	return trace;
}

hmc_complex det_su3matrix(__private hmc_ocl_su3matrix * U)
{
#ifdef _RECONSTRUCT_TWELVE_
	hmc_complex det;
	det.re=0;
	det.im=0;
	hmc_complex subdet;
	subdet.re=0;
	subdet.im=0;
	hmc_complex tmp1;
	hmc_complex tmp2;
	hmc_complex tmp3;
	tmp1 = complexmult( U[0], U[3] );
	tmp2 = reconstruct_su3(U,2);
	tmp3 = complexmult( tmp1, tmp2 );
	det = complexadd( det, tmp3 );

	tmp1 = complexmult( U[2], U[5] );
	tmp2 = reconstruct_su3(U,0);
	tmp3 = complexmult( tmp1, tmp2 );
	det = complexadd( det, tmp3 );

	tmp1 = complexmult( U[4], U[1] );
	tmp2 = reconstruct_su3(U,1);
	tmp3 = complexmult( tmp1, tmp2 );
	det = complexadd( det, tmp3 );

	tmp1 = complexmult( U[3], U[4] );
	tmp2 = reconstruct_su3(U,0);
	tmp3 = complexmult( tmp1, tmp2 );
	subdet = complexadd( subdet, tmp3 );

	tmp1 = complexmult( U[5], U[0] );
	tmp2 = reconstruct_su3(U,1);
	tmp3 = complexmult( tmp1, tmp2 );
	subdet = complexadd( subdet, tmp3 );

	tmp1 = complexmult( U[1], U[2] );
	tmp2 = reconstruct_su3(U,2);
	tmp3 = complexmult( tmp1, tmp2 );
	subdet = complexadd( subdet, tmp3 );

	det.re -= subdet.re;
	det.im -= subdet.im;

#else
	hmc_complex det, det1, det2, det3, det4, det5, det6, tmp1, tmp2, tmp3, tmp4, tmp5, tmp6;
	det.re=0;
	det.im=0;
	tmp1 = complexmult( U[ocl_su3matrix_element(1,1)], U[ocl_su3matrix_element(2,2)] );
	det1 = complexmult( U[ocl_su3matrix_element(0,0)], tmp1);
	tmp2 = complexmult( U[ocl_su3matrix_element(1,2)], U[ocl_su3matrix_element(2,0)] );
	det2 = complexmult( U[ocl_su3matrix_element(0,1)], tmp2);
	tmp3 = complexmult( U[ocl_su3matrix_element(1,0)], U[ocl_su3matrix_element(2,1)] );
	det3 = complexmult( U[ocl_su3matrix_element(0,2)], tmp3);
	tmp4 = complexmult( U[ocl_su3matrix_element(1,1)], U[ocl_su3matrix_element(2,0)] );
	det4 = complexmult( U[ocl_su3matrix_element(0,2)], tmp4);
	tmp5 = complexmult( U[ocl_su3matrix_element(1,0)], U[ocl_su3matrix_element(2,2)] );
	det5 = complexmult( U[ocl_su3matrix_element(0,1)], tmp5);
	tmp6 = complexmult( U[ocl_su3matrix_element(1,2)], U[ocl_su3matrix_element(2,1)] );
	det6 = complexmult( U[ocl_su3matrix_element(0,0)], tmp6);

	det.re = det1.re + det2.re + det3.re - det4.re - det5.re - det6.re;
	det.im = det1.im + det2.im + det3.im - det4.im - det5.im - det6.im;

#endif
	return det;
}

void project_su3(__private hmc_ocl_su3matrix *U){

  //Extract initial vectors
  hmc_complex a[NC];
  hmc_complex b[NC];
  hmc_complex c[NC];
#ifdef _RECONSTRUCT_TWELVE_
  a[0] = U[0];
  a[1] = U[2];
  a[2] = U[4];
  b[0] = U[1];
  b[1] = U[3];
  b[2] = U[5];
  c[0] = reconstruct_su3(U,0);
  c[1] = reconstruct_su3(U,1);
  c[2] = reconstruct_su3(U,2);
#else
    for (int i = 0; i<NC; i++){
     a[i] = U[ocl_su3matrix_element(0,i)];
     b[i] = U[ocl_su3matrix_element(1,i)];
     c[i] = U[ocl_su3matrix_element(2,i)];
    }
#endif
  
  //New SU3-Matrix
  //first vector
  //norm
  hmc_float norm = 0.;
  for (int i=0; i<NC; i++){
    hmc_complex tmp = complexconj(a[i]);
    tmp = complexmult (a[i], tmp);
    norm += tmp.re;
  }
  norm = 1./sqrt(norm);
  //rescale
  for (int i=0; i<NC; i++){
    a[i].re *= norm;
    a[i].im *= norm;
  }
  
  //second vector
  //orthogonal vector
  hmc_complex factor;
  factor.re = 0.0;
  factor.im = 0.0;
  for (int i=0; i<NC; i++){
    hmc_complex tmp;
    tmp = complexconj (b[i]);
    tmp = complexmult (a[i], tmp);
    factor = complexadd (factor, tmp);
  }
  for (int i=0; i<NC; i++){
    hmc_complex tmp;
    tmp = complexmult(factor, a[i]);
    b[i] = complexsubtract(b[i], tmp); 
  }
  
//norm
  norm = 0.;
  for (int i=0; i<NC; i++)
  {
    hmc_complex tmp;
    tmp = complexconj(b[i]);
    tmp = complexmult (b[i], tmp);
    norm +=  tmp.re;
  }
  norm = 1./sqrt(norm);
  //rescale
  for  (int i=0; i<NC; i++){
    b[i].re *= norm;
    b[i].im *= norm;
  }

#ifdef _RECONSTRUCT_TWELVE_
  //third vector 
  //orthogonal vector
  hmc_complex tmp;
  hmc_complex tmp2;
  tmp = complexmult(a[1], b[2]);
  tmp = complexconj(tmp);
  tmp2 = complexmult(a[2], b[1]);
  tmp2 = complexconj(tmp2);
  c[0] = complexsubtract(tmp, tmp2);
  tmp = complexmult(a[2], b[0]);
  tmp = complexconj(tmp);
  tmp2 = complexmult(a[0], b[2]);
  tmp2 = complexconj(tmp2);
  c[1] = complexsubtract(tmp, tmp2);
  
  //Set new values to matrix
  U[0] = a[0];
  U[1] = b[0];
  U[2] = a[1];
  U[3] = b[1];
  U[4] = a[2];
  U[5] = b[2];
#else
  //third vector 
  //orthogonal vector
  hmc_complex tmp;
  hmc_complex tmp2;
  tmp = complexmult(a[1], b[2]);
  tmp = complexconj(tmp);
  tmp2 = complexmult(a[2], b[1]);
  tmp2 = complexconj(tmp2);
  c[0] = complexsubtract(tmp, tmp2);
  tmp = complexmult(a[2], b[0]);
  tmp = complexconj(tmp);
  tmp2 = complexmult(a[0], b[2]);
  tmp2 = complexconj(tmp2);
  c[1] = complexsubtract(tmp, tmp2);
  tmp = complexmult(a[0], b[1]);
  tmp = complexconj(tmp);
  tmp2 = complexmult(a[1], b[0]);
  tmp2 = complexconj(tmp2);
  c[2] = complexsubtract(tmp, tmp2);
  
  //Set new values to matrix
  for(int i=0; i<NC; i++) {
     U[ocl_su3matrix_element(0,i)] = a[i];
     U[ocl_su3matrix_element(1,i)] = b[i];
     U[ocl_su3matrix_element(2,i)] = c[i];
  }
#endif
}

void project_su3_old(__private hmc_ocl_su3matrix *U)
{
	hmc_complex det = det_su3matrix(U);
	hmc_float detsqunorm = det.re*det.re + det.im*det.im;

	hmc_float phi;
	if(det.re*det.re<projectioneps) {
		phi = PI/2.;
	} else {
		phi = atan(det.im/det.re);
		if(det.re<0) phi += PI;
	}

	hmc_complex norm;
	norm.re = pow(detsqunorm,hmc_one_f/6)*cos(phi/3);
	norm.im = pow(detsqunorm,hmc_one_f/6)*sin(phi/3);

	hmc_float normsqunorm = norm.re*norm.re+norm.im*norm.im;

#ifdef _RECONSTRUCT_TWELVE_
	for(int n=0; n<NC*(NC-1); n++) {
		hmc_complex tmp = (U)[n];
		(U)[n].re = (tmp.re*norm.re+tmp.im*norm.im)/normsqunorm;
		(U)[n].im = (tmp.im*norm.re-tmp.re*norm.im)/normsqunorm;
	}
#else
	for(int a=0; a<NC; a++) {
		for(int b=0; b<NC; b++) {
			hmc_complex tmp = (U)[ocl_su3matrix_element(a,b)];
			(U)[ocl_su3matrix_element(a,b)].re = (tmp.re*norm.re+tmp.im*norm.im)/normsqunorm;
			(U)[ocl_su3matrix_element(a,b)].im = (tmp.im*norm.re-tmp.re*norm.im)/normsqunorm;
		}
	}
#endif
	return;
}

void adjoin_su3(__global hmc_ocl_gaugefield * in,__global hmc_ocl_gaugefield * out)
{
	for(int t=0; t<NTIME; t++) {
		for(int n=0; n<VOLSPACE; n++) {
			for(int mu=0; mu<NDIM; mu++) {
				hmc_ocl_su3matrix tmp[SU3SIZE];
				get_su3matrix(tmp, in, n, t, mu);
				adjoin_su3matrix(tmp);
				put_su3matrix(out, tmp, n, t, mu);
			}
		}
	}
	return;
}

hmc_complex global_trace_su3(__global hmc_ocl_gaugefield * field, int mu)
{
	hmc_complex sum;
	sum.re = 0;
	sum.im = 0;
	for(int t=0; t<NTIME; t++) {
		for(int n=0; n<VOLSPACE; n++) {
			hmc_ocl_su3matrix tmp[SU3SIZE];
			get_su3matrix(tmp, field, n, t, mu);
			sum.re += trace_su3matrix(tmp).re;
			sum.im += trace_su3matrix(tmp).im;
		}
	}
	return sum;
}

void copy_staplematrix(__private hmc_ocl_staplematrix *out,__private hmc_ocl_staplematrix *in)
{
	for(int n=0; n<NC*NC; n++) {
		out[n] = in[n];
	}
	return;
}

void zero_staplematrix(__private hmc_ocl_staplematrix * u)
{
	for(int n=0; n<STAPLEMATRIXSIZE; n++) {
		u[n].re = 0;
		u[n].im = 0;
	}
	return;
}

void unit_staplematrix(__private hmc_ocl_staplematrix * u)
{
	u[0].re = 1.;
	u[0].im = 0;
	u[4].re = 1.;
	u[4].im = 0;
	u[8].re = 1.;
	u[8].im = 0;

	return;
}

void multiply_staplematrix(__private hmc_ocl_staplematrix *out, __private hmc_ocl_su3matrix *p,__private  hmc_ocl_staplematrix *q)
{
#ifdef _RECONSTRUCT_TWELVE_
	for(int n=0; n<NC*(NC-1); n++) {
		out[n].re=0;
		out[n].im=0;
		for(int j=0; j<NC; j++) {
			int k = (int)(n/(NC-1));
			int i = n - (NC-1)*k;
			int np = i + (NC-1)*j;
			hmc_complex qcomponent;
			if(j==2) {
// 	  qcomponent = reconstruct_su3(q,k);
				qcomponent = q[NC*(NC-1)+k];
			} else {
				int nq = j + (NC-1)*k;
				qcomponent = q[nq];
			}
			hmc_complex tmp = complexmult(p[np], qcomponent);
			out[n] = complexadd(out[n], tmp);
		         }
	         }
	         //the left components:
	         hmc_complex X = reconstruct_su3(p,0);
	hmc_complex Y = reconstruct_su3(p,1);
	hmc_complex Z = reconstruct_su3(p,2);
	hmc_complex tmp;
	out[6].re=0;
	out[6].im=0;
	out[7].re=0;
	out[7].im=0;
	out[8].re=0;
	out[8].im=0;

	tmp = complexmult(X, q[0]);
	out[6] = complexadd(out[6], tmp);
	tmp = complexmult(Y, q[1]);
	out[6] = complexadd(out[6], tmp);
	tmp = complexmult(Z, q[6]);
	out[6] = complexadd(out[6], tmp);

	tmp = complexmult(X, q[2]);
	out[7] = complexadd(out[7], tmp);
	tmp = complexmult(Y, q[3]);
	out[7] = complexadd(out[7], tmp);
	tmp = complexmult(Z, q[7]);
	out[7] = complexadd(out[7], tmp);

	tmp = complexmult(X, q[4]);
	out[8] = complexadd(out[8], tmp);
	tmp = complexmult(Y, q[5]);
	out[8] = complexadd(out[8], tmp);
	tmp = complexmult(Z, q[8]);
	out[8] = complexadd(out[8], tmp);

#else
	multiply_su3matrices(out, p, q);
	/*
	for(int i=0; i<NC; i++) {
	  for(int k=0; k<NC; k++) {
		out[ocl_su3matrix_element(i,k)].re=0;
		out[ocl_su3matrix_element(i,k)].im=0;
		for(int j=0;j<NC;j++) {
	hmc_complex tmp = complexmult(&p[ocl_su3matrix_element(i,j)],&q[ocl_su3matrix_element(j,k)]);
	complexaccumulate(&out[ocl_su3matrix_element(i,k)],&tmp);
		}
	  }
	}
	*/
#endif
	return;
}

void accumulate_su3matrices_add(__private hmc_ocl_staplematrix *p,__private hmc_ocl_su3matrix *q)
{
#ifdef _RECONSTRUCT_TWELVE_
	for(int n=0; n<NC*(NC-1); n++) {
		p[n] = complexadd(p[n] , q[n]);
	}
	for(int n=NC*(NC-1);  n<NC*NC; n++) {
		hmc_complex tmp = reconstruct_su3(q, n-NC*(NC-1));
		p[n] = complexadd(p[n] , tmp);
	}
#else
	for(int k=0; k<NC*NC; k++) {
		p[k] = complexadd(p[k] , q[k]);
	}
#endif
	return;
}

void reduction (hmc_complex dest[su2_entries],__private hmc_ocl_staplematrix* src, const int rand)
{
#ifdef _RECONSTRUCT_TWELVE_
	if(rand == 1) {
		dest[0] = src[0];
		dest[1] = src[2];
		dest[2] = src[1];
		dest[3] = src[3];
	} else if (rand==2) {
		dest[0] = src[3];
		dest[1] = src[5];
		dest[2] = src[7];
		dest[3] = src[8];
	} else if (rand==3) {
		dest[0] = src[0];
		dest[1] = src[4];
		dest[2] = src[6];
		dest[3] = src[8];
	}
#else
	if(rand == 1) {
		dest[0] = src[ocl_su3matrix_element(0,0)];
		dest[1] = src[ocl_su3matrix_element(0,1)];
		dest[2] = src[ocl_su3matrix_element(1,0)];
		dest[3] = src[ocl_su3matrix_element(1,1)];
	} else if (rand==2) {
		dest[0] = src[ocl_su3matrix_element(1,1)];
		dest[1] = src[ocl_su3matrix_element(1,2)];
		dest[2] = src[ocl_su3matrix_element(2,1)];
		dest[3] = src[ocl_su3matrix_element(2,2)];
	} else if (rand==3) {
		dest[0] = src[ocl_su3matrix_element(0,0)];
		dest[1] = src[ocl_su3matrix_element(0,2)];
		dest[2] = src[ocl_su3matrix_element(2,0)];
		dest[3] = src[ocl_su3matrix_element(2,2)];
	}
#endif
}

// return an SU2 matrix (std basis) extended to SU3 (std basis)
void extend (__private hmc_ocl_su3matrix * dest, const int random, hmc_complex src[su2_entries])
{
#ifdef _RECONSTRUCT_TWELVE_
	if (random == 1) {
		dest[0] = src[0];
		dest[2] = src[1];
		dest[4] = hmc_complex_zero;
		dest[1] = src[2];
		dest[3] = src[3];
		dest[5] = hmc_complex_zero;
	} else if (random == 2) {
		dest[0] = hmc_complex_one;
		dest[2] = hmc_complex_zero;
		dest[4] = hmc_complex_zero;
		dest[1] = hmc_complex_zero;
		dest[3] = src[0];
		dest[5] = src[1];
	} else if (random == 3) {
		dest[0] = src[0];
		dest[2] = hmc_complex_zero;
		dest[4] = src[1];
		dest[1] = hmc_complex_zero;
		dest[3] = hmc_complex_one;
		dest[5] = hmc_complex_zero;
	}

#else
	if (random == 1) {
		dest[ocl_su3matrix_element(0,0)] = src[0];
		dest[ocl_su3matrix_element(0,1)] = src[1];
		dest[ocl_su3matrix_element(0,2)] = hmc_complex_zero;
		dest[ocl_su3matrix_element(1,0)] = src[2];
		dest[ocl_su3matrix_element(1,1)] = src[3];
		dest[ocl_su3matrix_element(1,2)] = hmc_complex_zero;
		dest[ocl_su3matrix_element(2,0)] = hmc_complex_zero;
		dest[ocl_su3matrix_element(2,1)] = hmc_complex_zero;
		dest[ocl_su3matrix_element(2,2)] = hmc_complex_one;
	} else if (random == 2) {
		dest[ocl_su3matrix_element(0,0)] = hmc_complex_one;
		dest[ocl_su3matrix_element(0,1)] = hmc_complex_zero;
		dest[ocl_su3matrix_element(0,2)] = hmc_complex_zero;
		dest[ocl_su3matrix_element(1,0)] = hmc_complex_zero;
		dest[ocl_su3matrix_element(1,1)] = src[0];
		dest[ocl_su3matrix_element(1,2)] = src[1];
		dest[ocl_su3matrix_element(2,0)] = hmc_complex_zero;
		dest[ocl_su3matrix_element(2,1)] = src[2];
		dest[ocl_su3matrix_element(2,2)] = src[3];
	} else if (random == 3) {
		dest[ocl_su3matrix_element(0,0)] = src[0];
		dest[ocl_su3matrix_element(0,1)] = hmc_complex_zero;
		dest[ocl_su3matrix_element(0,2)] = src[1];
		dest[ocl_su3matrix_element(1,0)] = hmc_complex_zero;
		dest[ocl_su3matrix_element(1,1)] = hmc_complex_one;
		dest[ocl_su3matrix_element(1,2)] = hmc_complex_zero;
		dest[ocl_su3matrix_element(2,0)] = src[2];
		dest[ocl_su3matrix_element(2,1)] = hmc_complex_zero;
		dest[ocl_su3matrix_element(2,2)] = src[3];
	}
#endif
}

void gaugefield_apply_bc(__private hmc_ocl_su3matrix * in, hmc_float theta)
{
	hmc_float tmp1,tmp2;
#ifdef _RECONSTRUCT_TWELVE_
	for(int a=0; a<NC; a++) {
		for(int b=0; b<NC-1; b++) {
// 			for(int n=0; n<NC*(NC-1); n++) {
			tmp1 = ((in)[ocl_su3matrix_element(a,b)]).re;
			tmp2 = ((in)[ocl_su3matrix_element(a,b)]).im;
			((in)[ocl_su3matrix_element(a,b)]).re = cos(theta)*tmp1 - sin(theta)*tmp2;
			((in)[ocl_su3matrix_element(a,b)]).im = sin(theta)*tmp1 + cos(theta)*tmp2;
		}
	}
#else
	for(int a=0; a<NC; a++) {
		for(int b=0; b<NC; b++) {
			tmp1 = ((in)[ocl_su3matrix_element(a,b)]).re;
			tmp2 = ((in)[ocl_su3matrix_element(a,b)]).im;
			((in)[ocl_su3matrix_element(a,b)]).re = cos(theta)*tmp1 - sin(theta)*tmp2;
			((in)[ocl_su3matrix_element(a,b)]).im = sin(theta)*tmp1 + cos(theta)*tmp2;
		}
	}
#endif
	return;
}

//CP: the following two chem-pot functions are one in the host-code
void gaugefield_apply_chem_pot_real(__private hmc_ocl_su3matrix * u, __private hmc_ocl_su3matrix * udagger, hmc_float chem_pot_re)
{
	hmc_float tmp1,tmp2;
#ifdef _RECONSTRUCT_TWELVE_
	for(int a=0; a<NC; a++) {
		for(int b=0; b<NC-1; b++) {
//   for(int n=0; n<NC*(NC-1); n++) {
			tmp1 = ((u)[ocl_su3matrix_element(a,b)]).re;
			tmp2 = ((u)[ocl_su3matrix_element(a,b)]).im;
			((u)[ocl_su3matrix_element(a,b)]).re = exp(chem_pot_re)*tmp1;
			((u)[ocl_su3matrix_element(a,b)]).im = exp(chem_pot_re)*tmp2;
			tmp1 = ((udagger)[ocl_su3matrix_element(a,b)]).re;
			tmp2 = ((udagger)[ocl_su3matrix_element(a,b)]).im;
			((udagger)[ocl_su3matrix_element(a,b)]).re = exp(-chem_pot_re)*tmp1;
			((udagger)[ocl_su3matrix_element(a,b)]).im = exp(-chem_pot_re)*tmp2;
		}
	}
#else
	for(int a=0; a<NC; a++) {
		for(int b=0; b<NC; b++) {
			tmp1 = ((u)[ocl_su3matrix_element(a,b)]).re;
			tmp2 = ((u)[ocl_su3matrix_element(a,b)]).im;
			((u)[ocl_su3matrix_element(a,b)]).re = exp(chem_pot_re)*tmp1;
			((u)[ocl_su3matrix_element(a,b)]).im = exp(chem_pot_re)*tmp2;
			tmp1 = ((udagger)[ocl_su3matrix_element(a,b)]).re;
			tmp2 = ((udagger)[ocl_su3matrix_element(a,b)]).im;
			((udagger)[ocl_su3matrix_element(a,b)]).re = exp(-chem_pot_re)*tmp1;
			((udagger)[ocl_su3matrix_element(a,b)]).im = exp(-chem_pot_re)*tmp2;
		}
	}
#endif
	return;
}

void gaugefield_apply_chem_pot_imag(__private hmc_ocl_su3matrix * u, __private hmc_ocl_su3matrix * udagger, hmc_float chem_pot_im)
{
	hmc_float tmp1,tmp2;
#ifdef _RECONSTRUCT_TWELVE_
	for(int a=0; a<NC; a++) {
		for(int b=0; b<NC-1; b++) {
//   for(int n=0; n<NC*(NC-1); n++) {
			tmp1 = ((u)[ocl_su3matrix_element(a,b)]).re;
			tmp2 = ((u)[ocl_su3matrix_element(a,b)]).im;
			((u)[ocl_su3matrix_element(a,b)]).re = ( cos(chem_pot_im)*tmp1 - sin(chem_pot_im)*tmp2 );
			((u)[ocl_su3matrix_element(a,b)]).im = ( sin(chem_pot_im)*tmp1 + cos(chem_pot_im)*tmp2 );
			tmp1 = ((udagger)[ocl_su3matrix_element(a,b)]).re;
			tmp2 = ((udagger)[ocl_su3matrix_element(a,b)]).im;
			((udagger)[ocl_su3matrix_element(a,b)]).re = ( cos(chem_pot_im)*tmp1 + sin(chem_pot_im)*tmp2 );
			((udagger)[ocl_su3matrix_element(a,b)]).im = ( -sin(chem_pot_im)*tmp1 + cos(chem_pot_im)*tmp2 );
		}
	}
#else
	for(int a=0; a<NC; a++) {
		for(int b=0; b<NC; b++) {
			tmp1 = ((u)[ocl_su3matrix_element(a,b)]).re;
			tmp2 = ((u)[ocl_su3matrix_element(a,b)]).im;
			((u)[ocl_su3matrix_element(a,b)]).re = ( cos(chem_pot_im)*tmp1 - sin(chem_pot_im)*tmp2 );
			((u)[ocl_su3matrix_element(a,b)]).im = ( sin(chem_pot_im)*tmp1 + cos(chem_pot_im)*tmp2 );
			tmp1 = ((udagger)[ocl_su3matrix_element(a,b)]).re;
			tmp2 = ((udagger)[ocl_su3matrix_element(a,b)]).im;
			((udagger)[ocl_su3matrix_element(a,b)]).re = ( cos(chem_pot_im)*tmp1 + sin(chem_pot_im)*tmp2 );
			((udagger)[ocl_su3matrix_element(a,b)]).im = ( -sin(chem_pot_im)*tmp1 + cos(chem_pot_im)*tmp2 );
		}
	}
#endif
	return;
}

//calculate polyakov-loop matrix at spatial site n in time-direction
void local_polyakov(__global hmc_ocl_gaugefield * field, hmc_ocl_su3matrix * prod, int n)
{
	unit_su3matrix(prod);
	for(int t=0; t<NTIME; t++) {
		hmc_ocl_su3matrix tmp[SU3SIZE];
		get_su3matrix(tmp,field,n,t,0);
		accumulate_su3matrix_prod(prod,tmp);
	}
	return;
}

//calculate plaquette-matrix at site n,t in direction mu and nu
void local_plaquette(__global hmc_ocl_gaugefield * field, hmc_ocl_su3matrix * prod, int n, int t, int mu, int nu )
{
	hmc_ocl_su3matrix tmp[SU3SIZE];
	//u_mu(x)
	get_su3matrix(prod,field,n,t,mu);
	//u_nu(x+mu)
	if(mu==0) {
		int newt = (t+1)%NTIME; //(haha)
		get_su3matrix(tmp,field,n,newt,nu);
	} else {
		get_su3matrix(tmp,field,get_neighbor(n,mu),t,nu);
	}
	accumulate_su3matrix_prod(prod,tmp);
	//adjoint(u_mu(x+nu))
	if(nu==0) {
		int newt = (t+1)%NTIME;
		get_su3matrix(tmp,field,n,newt,mu);
	} else {
		get_su3matrix(tmp,field,get_neighbor(n,nu),t,mu);
	}
	adjoin_su3matrix(tmp);
	accumulate_su3matrix_prod(prod,tmp);
	//adjoint(u_nu(x))
	get_su3matrix(tmp,field,n,t,nu);
	adjoin_su3matrix(tmp);
	accumulate_su3matrix_prod(prod,tmp);

	return;
}

void local_Q_plaquette(__private hmc_ocl_3x3matrix * out, __global const hmc_ocl_gaugefield const * field,
		       const int n, const int t, const int mu, const int nu ){
  hmc_ocl_su3matrix tmp [SU3SIZE];
  int newpos;
  
  //1st plaq
  hmc_ocl_su3matrix plaq1[SU3SIZE];
  //u_mu(x)
  get_su3matrix(plaq1,field,n,t,mu);
  //u_nu(x+mu)
  if(mu==0) {
    int newt = (t+1)%NTIME;
    get_su3matrix(tmp,field,n,newt,nu);
  }
  else
    get_su3matrix(tmp,field,get_neighbor(n,mu),t,nu);
  accumulate_su3matrix_prod(plaq1,tmp);
  //adjoint(u_mu(x+nu))
  if(nu==0) {
    int newt = (t+1)%NTIME;
    get_su3matrix(tmp,field,n,newt,mu);
  }
  else
    get_su3matrix(tmp,field,get_neighbor(n,nu),t,mu);
  adjoin_su3matrix(tmp);
  accumulate_su3matrix_prod(plaq1,tmp);
  //adjoint(u_nu(x))
  get_su3matrix(tmp,field,n,t,nu);
  adjoin_su3matrix(tmp);
  accumulate_su3matrix_prod(plaq1,tmp);

  //2nd plaq
  hmc_ocl_su3matrix plaq2 [SU3SIZE];
  //U_nu(x)
  get_su3matrix(plaq2,field,n,t,nu);
  //adj (u_mu(x-mu+nu))
  newpos = get_lower_neighbor(n, mu);
  if (nu==0){
    int newt =  (t+1)%NTIME;
    get_su3matrix(tmp,field,newpos,newt,mu);
  }
  else if (mu==0){
    int newt = (t-1+NTIME)%NTIME;
    get_su3matrix(tmp,field,get_neighbor(n,nu),newt,mu);
  }
  else
    get_su3matrix(tmp,field,get_neighbor(newpos, nu),t,mu);
  adjoin_su3matrix(tmp);
  accumulate_su3matrix_prod(plaq2,tmp);
  //adj (u_nu(x-mu))
  if (mu==0){
    int newt = (t-1+NTIME)%NTIME;
    get_su3matrix(tmp,field,n,newt, nu);
  }
  else
    get_su3matrix(tmp,field, get_lower_neighbor(n, mu),t, nu);
  adjoin_su3matrix(tmp);
  accumulate_su3matrix_prod(plaq2,tmp);
  //u_mu(x-mu)
  if (mu==0){
    int newt = (t-1+NTIME)%NTIME;
    get_su3matrix(tmp,field,n,newt, mu);
  }
  else
    get_su3matrix(tmp,field, get_lower_neighbor(n, mu),t, mu);
  accumulate_su3matrix_prod(plaq2,tmp);

  //3rd plaq
  hmc_ocl_su3matrix plaq3 [SU3SIZE];
  //adj (u_mu(x-mu))
  if (mu==0){
    int newt = (t-1+NTIME)%NTIME;
    get_su3matrix(tmp,field,n,newt, mu);
  }
  else
    get_su3matrix(tmp,field, get_lower_neighbor(n, mu),t, mu);
  adjoin_su3matrix(tmp);
  copy_su3matrix(plaq3, tmp); 
  //adj (u_nu(x-mu-nu))
  newpos = get_lower_neighbor(n, mu);
  if (nu==0){
    int newt = (t-1+NTIME)%NTIME;
    get_su3matrix(tmp,field, newpos,newt,nu);
  }
  else if (mu==0){
    int newt = (t-1+NTIME)%NTIME;
  get_su3matrix(tmp,field,get_lower_neighbor(n,nu),newt,nu);
  }
  else
    get_su3matrix(tmp,field,get_lower_neighbor(newpos, nu),t,nu);
  adjoin_su3matrix(tmp);
  accumulate_su3matrix_prod(plaq3,tmp);
  //u_mu(x-mu-nu)
  newpos = get_lower_neighbor(n, mu);
  if (nu==0){
    int newt = (t-1+NTIME)%NTIME;
    get_su3matrix(tmp,field,newpos,newt,mu);
  }
  else if (mu==0){
    int newt = (t-1+NTIME)%NTIME;
    get_su3matrix(tmp,field,get_lower_neighbor(n,nu),newt,mu);
  }
  else
    get_su3matrix(tmp,field,get_lower_neighbor(newpos, nu),t,mu);
  accumulate_su3matrix_prod(plaq3,tmp);
  //u_nu(x-nu)
  if (nu==0){
    int newt = (t-1+NTIME)%NTIME;
    get_su3matrix(tmp,field, n,newt, nu);
  }
  else
    //this causes for nu=1 a speicherzugriffsfehler
    get_su3matrix(tmp,field,get_lower_neighbor(n, nu),t,nu);
  accumulate_su3matrix_prod(plaq3,tmp);

  //4th plaq
  hmc_ocl_su3matrix plaq4[SU3SIZE];
  //adj(u_nu(x-nu))
  adjoin_su3matrix(tmp);
  copy_su3matrix(plaq4, tmp); 
  //u_mu(x-nu)
   if (nu==0){
    int newt = (t-1+NTIME)%NTIME;
    get_su3matrix(tmp,field, n,newt, mu);
  }
  else
    get_su3matrix(tmp,field, get_lower_neighbor(n, nu),t,mu);
  accumulate_su3matrix_prod(plaq4,tmp);
  //u_nu(x+mu-nu)
  newpos = get_lower_neighbor(n, nu);
  if (mu==0){
    int newt =  (t+1)%NTIME;
    get_su3matrix(tmp,field,newpos,newt,nu);
  }
  else if (nu==0){
    int newt = (t-1+NTIME)%NTIME;
    get_su3matrix(tmp,field,get_neighbor(n,mu),newt,nu);
  }
  else
    get_su3matrix(tmp,field,get_neighbor(newpos, mu),t,nu);
  accumulate_su3matrix_prod(plaq4,tmp);
  //adj (u_mu(x))
  get_su3matrix(tmp,field,n,t,mu);
  adjoin_su3matrix(tmp);
  accumulate_su3matrix_prod(plaq4,tmp);

  //Sum up
  su3matrix_to_3x3matrix (out, plaq1);
  accumulate_su3matrix_3x3_add(out, plaq2);
  accumulate_su3matrix_3x3_add(out, plaq3);
  accumulate_su3matrix_3x3_add(out, plaq4);
}
