//site = pos + VOLSPACE*t =  x + y*NSPACE + z*NSPACE*NSPACE + VOLSPACE*t
//idx = mu + NDIM*site
int inline ocl_gaugefield_element(int c, int a, int b, int mu, int spacepos, int t){
#ifdef _RECONSTRUCT_TWELVE_
  return c + 2*a + 2*(NC-1)*b+2*NC*(NC-1)*mu+2*NC*(NC-1)*NDIM*spacepos+2*NC*(NC-1)*NDIM*VOLSPACE*t;
#else
  return c + 2*a + 2*NC*b+2*NC*NC*mu+2*NC*NC*NDIM*spacepos+2*NC*NC*NDIM*VOLSPACE*t;
#endif
}

int inline ocl_su3matrix_element(int a, int b){
#ifdef _RECONSTRUCT_TWELVE_
  return a + (NC-1)*b;
#else
  return a + NC*b;
#endif
}

//it is assumed that idx iterates only over half the number of sites
void inline get_even_site(int idx, int * out_space, int * out_t){
  int x,y,z,t;
  x = idx;
  t = convert_int(idx/(VOLSPACE/2));
  x -= t*VOLSPACE/2;
  z = convert_int(x/(NSPACE*NSPACE/2));
  x -= z*NSPACE*NSPACE/2;
  y = convert_int(x/NSPACE);
  x -= y*NSPACE;
  (*out_space) =  convert_int((z+t)%2)*(1 + 2*x - convert_int (2*x/NSPACE)) + convert_int((t+z+1)%2)*(2*x + convert_int (2*x/NSPACE)) + 2*NSPACE*y + NSPACE*NSPACE*z;
  (*out_t) = t;
}

//it is assumed that idx iterates only over half the number of sites
void inline get_odd_site(int idx, int * out_space, int * out_t){
  int x,y,z,t;
  x = idx;
  t = convert_int(idx/(VOLSPACE/2));
  x -= t*VOLSPACE/2;
  z = convert_int(x/(NSPACE*NSPACE/2));
  x -= z*NSPACE*NSPACE/2;
  y = convert_int(x/NSPACE);
  x -= y*NSPACE;
  (*out_space) =  convert_int((z+t+1)%2)*(1 + 2*x - convert_int (2*x/NSPACE)) + convert_int((t+z)%2)*(2*x + convert_int (2*x/NSPACE)) + 2*NSPACE*y + NSPACE*NSPACE*z;
  (*out_t) = t;
}

int inline get_global_pos(int spacepos, int t){
  return spacepos + VOLSPACE * t;
}

hmc_complex complexconj(__private hmc_complex *in){
  hmc_complex tmp;
  tmp.re = (*in).re;
  tmp.im = -(*in).im;
  return tmp;
}

hmc_complex complexmult(__private hmc_complex *a,__private hmc_complex *b){
  hmc_complex res;
  res.re = (*a).re*(*b).re - (*a).im*(*b).im;
  res.im = (*a).im*(*b).re + (*a).re*(*b).im;
  return res;
}

hmc_complex complexadd(__private hmc_complex *a,__private hmc_complex *b){
  hmc_complex res;
  res.re = (*a).re + (*b).re;
  res.im = (*a).im + (*b).im;
  return res;
}

hmc_complex complexsubtract(__private hmc_complex *a,__private hmc_complex *b){
  hmc_complex res;
  res.re = (*a).re - (*b).re;
  res.im = (*a).im - (*b).im;
  return res;
}

void complexaccumulate(__private hmc_complex *inout,__private hmc_complex *incr){
  (*inout).re += (*incr).re;
  (*inout).im += (*incr).im;
  return;
}

hmc_complex complexdivide(hmc_complex* numerator, hmc_complex* denominator){
  hmc_float norm = (*denominator).re*(*denominator).re + (*denominator).im*(*denominator).im;
  hmc_complex res;
  res.re = ((*numerator).re*(*denominator).re+(*numerator).im*(*denominator).im)/norm;
  res.im = ((*numerator).im*(*denominator).re-(*numerator).re*(*denominator).im)/norm;
  return res;
}

void get_su3matrix(__private hmc_ocl_su3matrix* out, __global hmc_ocl_gaugefield * in, int spacepos, int timepos, int mu){
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

void put_su3matrix(__global hmc_ocl_gaugefield * field, __private hmc_ocl_su3matrix * in, int spacepos, int timepos, int mu){
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

void copy_su3matrix(__private hmc_ocl_su3matrix * out, __private hmc_ocl_su3matrix * in){
  for(int n = 0; n<SU3SIZE; n++) {
    (out[n]).re = (in[n]).re;
    (out[n]).im = (in[n]).im;
  }
  return;
}

void unit_su3matrix(__private hmc_ocl_su3matrix *out){
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

void zero_su3matrix(__private hmc_ocl_su3matrix *out){
  for(int n=0; n<SU3SIZE; n++) {
    out[n].re = 0;
    out[n].im = 0;
  }
  return;
}

#ifdef _RECONSTRUCT_TWELVE_
hmc_complex reconstruct_su3(__private hmc_ocl_su3matrix *in, int ncomp){
  int jplusone = (ncomp+1)%NC;
  int jplustwo = (ncomp+2)%NC;
  hmc_complex first = complexmult(&(in[(NC-1)*jplusone]),&(in[1+(NC-1)*jplustwo]));
  hmc_complex second = complexmult(&(in[(NC-1)*jplustwo]),&(in[1+(NC-1)*jplusone]));
  hmc_complex result = complexsubtract(&first,&second);
  return complexconj(&result);
}
#endif

void multiply_su3matrices(__private hmc_ocl_su3matrix *out,__private hmc_ocl_su3matrix *p,__private hmc_ocl_su3matrix *q){
#ifdef _RECONSTRUCT_TWELVE_
  for(int n=0; n<NC*(NC-1); n++) {
      out[n].re=0;
      out[n].im=0;
      for(int j=0;j<NC;j++) {
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
	hmc_complex tmp = complexmult(&p[np],&qcomponent);
	complexaccumulate(&out[n],&tmp);
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

void accumulate_su3matrix_prod(__private hmc_ocl_su3matrix *acc,__private hmc_ocl_su3matrix *mult){
  hmc_ocl_su3matrix tmp[SU3SIZE];
  multiply_su3matrices(tmp,acc,mult);
  copy_su3matrix(acc,tmp);
  return;
}


void adjoin_su3matrix(__private hmc_ocl_su3matrix * mat){
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
    mat[n] = complexconj(&element);
  }
#else
  hmc_ocl_su3matrix tmp[SU3SIZE];
  copy_su3matrix(tmp, mat);
  for(int a=0; a<NC; a++) {
    for(int b=0; b<NC; b++) {
      mat[ocl_su3matrix_element(a,b)] =
      complexconj(&(tmp[ocl_su3matrix_element(b,a)]));
    }
  }
#endif
  return;
}

hmc_complex trace_su3matrix(__private hmc_ocl_su3matrix * mat){
  hmc_complex trace;
#ifdef _RECONSTRUCT_TWELVE_
  trace = reconstruct_su3(mat,NC-1);
  for(int n=0; n<(NC-1); n++) complexaccumulate(&trace,&(mat[NC*n]));
#else
  trace.re=0;
  trace.im=0;
  for(int a=0; a<NC; a++) complexaccumulate(&trace,&(mat[ocl_su3matrix_element(a,a)]));;
#endif
  return trace;
}

hmc_complex det_su3matrix(__private hmc_ocl_su3matrix * U){
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
  tmp1 = complexmult( &U[0], &U[3] );
  tmp2 = reconstruct_su3(U,2);
  tmp3 = complexmult( &tmp1, &tmp2 );
  complexaccumulate(&det,&tmp3);

  tmp1 = complexmult( &U[2], &U[5] );
  tmp2 = reconstruct_su3(U,0);
  tmp3 = complexmult( &tmp1, &tmp2 );
  complexaccumulate(&det,&tmp3);

  tmp1 = complexmult( &U[4], &U[1] );
  tmp2 = reconstruct_su3(U,1);
  tmp3 = complexmult( &tmp1, &tmp2 );
  complexaccumulate(&det,&tmp3);

  tmp1 = complexmult( &U[3], &U[4] );
  tmp2 = reconstruct_su3(U,0);
  tmp3 = complexmult( &tmp1, &tmp2 );
  complexaccumulate(&subdet,&tmp3);

  tmp1 = complexmult( &U[5], &U[0] );
  tmp2 = reconstruct_su3(U,1);
  tmp3 = complexmult( &tmp1, &tmp2 );
  complexaccumulate(&subdet,&tmp3);

  tmp1 = complexmult( &U[1], &U[2] );
  tmp2 = reconstruct_su3(U,2);
  tmp3 = complexmult( &tmp1, &tmp2 );
  complexaccumulate(&subdet,&tmp3);

  det.re -= subdet.re;
  det.im -= subdet.im;

#else
  hmc_complex det, det1, det2, det3, det4, det5, det6, tmp1, tmp2, tmp3, tmp4, tmp5, tmp6;
  det.re=0;
  det.im=0;
  tmp1 = complexmult( &U[ocl_su3matrix_element(1,1)], &U[ocl_su3matrix_element(2,2)] );
  det1 = complexmult( &U[ocl_su3matrix_element(0,0)] , &tmp1);
  tmp2 = complexmult( &U[ocl_su3matrix_element(1,2)], &U[ocl_su3matrix_element(2,0)] );
  det2 = complexmult( &U[ocl_su3matrix_element(0,1)] , &tmp2);
  tmp3 = complexmult( &U[ocl_su3matrix_element(1,0)], &U[ocl_su3matrix_element(2,1)] );
  det3 = complexmult( &U[ocl_su3matrix_element(0,2)] , &tmp3);
  tmp4 = complexmult( &U[ocl_su3matrix_element(1,1)], &U[ocl_su3matrix_element(2,0)] );
  det4 = complexmult( &U[ocl_su3matrix_element(0,2)] , &tmp4);
  tmp5 = complexmult( &U[ocl_su3matrix_element(1,0)], &U[ocl_su3matrix_element(2,2)] );
  det5 = complexmult( &U[ocl_su3matrix_element(0,1)] , &tmp5);
  tmp6 = complexmult( &U[ocl_su3matrix_element(1,2)], &U[ocl_su3matrix_element(2,1)] );
  det6 = complexmult( &U[ocl_su3matrix_element(0,0)] , &tmp6);

  det.re = det1.re + det2.re + det3.re - det4.re - det5.re - det6.re;
  det.im = det1.im + det2.im + det3.im - det4.im - det5.im - det6.im;

#endif
  return det;
}

void project_su3(__private hmc_ocl_su3matrix *U){
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
  norm.re = pow(detsqunorm,hmc_one_f/6.)*cos(phi/3.);
  norm.im = pow(detsqunorm,hmc_one_f/6.)*sin(phi/3.);
  
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

void adjoin_su3(__global hmc_ocl_gaugefield * in,__global hmc_ocl_gaugefield * out){
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

hmc_complex global_trace_su3(__global hmc_ocl_gaugefield * field, int mu) {
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

//LZ: everything above has been tested...

void copy_staplematrix(__private hmc_ocl_staplematrix *out,__private hmc_ocl_staplematrix *in){
  for(int n=0; n<NC*NC; n++) {
    out[n] = in[n];
  }
  return;
}

void zero_staplematrix(__private hmc_ocl_staplematrix * u){
  for(int n=0; n<STAPLEMATRIXSIZE; n++) {
    u[n].re = 0;
    u[n].im = 0;
  }
  return;
}

void unit_staplematrix(__private hmc_ocl_staplematrix * u){
  u[0].re = 1.;
  u[0].im = 0;
  u[4].re = 1.;
  u[4].im = 0;
  u[8].re = 1.;
  u[8].im = 0;
  
  return;
}

void multiply_staplematrix(__private hmc_ocl_staplematrix *out, __private hmc_ocl_su3matrix *p,__private  hmc_ocl_staplematrix *q){
#ifdef _RECONSTRUCT_TWELVE_
  for(int n=0; n<NC*(NC-1); n++) {
      out[n].re=0;
      out[n].im=0;
      for(int j=0;j<NC;j++) {
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
	hmc_complex tmp = complexmult(&p[np],&qcomponent);
	complexaccumulate(&out[n],&tmp);
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
    
    tmp = complexmult(&X,&q[0]);
    complexaccumulate(&out[6],&tmp);
    tmp = complexmult(&Y,&q[1]);
    complexaccumulate(&out[6],&tmp);
    tmp = complexmult(&Z,&q[6]);
    complexaccumulate(&out[6],&tmp);

    tmp = complexmult(&X,&q[2]);
    complexaccumulate(&out[7],&tmp);
    tmp = complexmult(&Y,&q[3]);
    complexaccumulate(&out[7],&tmp);
    tmp = complexmult(&Z,&q[7]);
    complexaccumulate(&out[7],&tmp);

    tmp = complexmult(&X,&q[4]);
    complexaccumulate(&out[8],&tmp);
    tmp = complexmult(&Y,&q[5]);
    complexaccumulate(&out[8],&tmp);
    tmp = complexmult(&Z,&q[8]);
    complexaccumulate(&out[8],&tmp);
    
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

void accumulate_su3matrices_add(__private hmc_ocl_staplematrix *p,__private hmc_ocl_su3matrix *q){
#ifdef _RECONSTRUCT_TWELVE_
  for(int n=0; n<NC*(NC-1); n++) {
    complexaccumulate(&(p[n]), &(q[n]));
  }
  for(int n=NC*(NC-1);  n<NC*NC; n++) {
    hmc_complex tmp = reconstruct_su3(q, n-NC*(NC-1)); 
    complexaccumulate(&(p[n]), &(tmp));
  }  
#else
  for(int k=0; k<NC*NC; k++) {
    complexaccumulate(&(p[k]),&(q[k]));
  }
#endif
  return;
}

void reduction (hmc_complex dest[su2_entries],__private hmc_ocl_staplematrix* src, const int rand){
#ifdef _RECONSTRUCT_TWELVE_
  if(rand == 1)
  { 
    dest[0] = src[0]; 
    dest[1] = src[2]; 
    dest[2] = src[1]; 
    dest[3] = src[3]; 
  } 
  else if (rand==2) 
  { 
    dest[0] = src[3]; 
    dest[1] = src[5]; 
    dest[2] = src[7];
    dest[3] = src[8];
  } 
  else if (rand==3)
  { 
    dest[0] = src[0]; 
    dest[1] = src[4]; 
    dest[2] = src[6];
    dest[3] = src[8];
  } 
#else
  if(rand == 1)
  {
    dest[0] = src[ocl_su3matrix_element(0,0)];
    dest[1] = src[ocl_su3matrix_element(0,1)];
    dest[2] = src[ocl_su3matrix_element(1,0)];
    dest[3] = src[ocl_su3matrix_element(1,1)];
  }
  else if (rand==2)
  {
    dest[0] = src[ocl_su3matrix_element(1,1)];
    dest[1] = src[ocl_su3matrix_element(1,2)];
    dest[2] = src[ocl_su3matrix_element(2,1)];
    dest[3] = src[ocl_su3matrix_element(2,2)];
  }
  else if (rand==3)
  {
    dest[0] = src[ocl_su3matrix_element(0,0)];
    dest[1] = src[ocl_su3matrix_element(0,2)];
    dest[2] = src[ocl_su3matrix_element(2,0)];
    dest[3] = src[ocl_su3matrix_element(2,2)];
  }
#endif
}

// return an SU2 matrix (std basis) extended to SU3 (std basis)
void extend (__private hmc_ocl_su3matrix * dest, const int random, hmc_complex src[su2_entries]){
#ifdef _RECONSTRUCT_TWELVE_
  if (random == 1){
    dest[0] = src[0];
    dest[2] = src[1];
    dest[4] = hmc_complex_zero;
    dest[1] = src[2];
    dest[3] = src[3];
    dest[5] = hmc_complex_zero;
  }
  else if (random == 2){
    dest[0] = hmc_complex_one;
    dest[2] = hmc_complex_zero;
    dest[4] = hmc_complex_zero;
    dest[1] = hmc_complex_zero;
    dest[3] = src[0];
    dest[5] = src[1];
  }
  else if (random == 3){
    dest[0] = src[0];
    dest[2] = hmc_complex_zero;
    dest[4] = src[1];
    dest[1] = hmc_complex_zero;
    dest[3] = hmc_complex_one;
    dest[5] = hmc_complex_zero;
  }

#else
  if (random == 1){
    dest[ocl_su3matrix_element(0,0)] = src[0];
    dest[ocl_su3matrix_element(0,1)] = src[1];
    dest[ocl_su3matrix_element(0,2)] = hmc_complex_zero;
    dest[ocl_su3matrix_element(1,0)] = src[2];
    dest[ocl_su3matrix_element(1,1)] = src[3];
    dest[ocl_su3matrix_element(1,2)] = hmc_complex_zero;
    dest[ocl_su3matrix_element(2,0)] = hmc_complex_zero;
    dest[ocl_su3matrix_element(2,1)] = hmc_complex_zero;
    dest[ocl_su3matrix_element(2,2)] = hmc_complex_one;
  }
  else if (random == 2){
    dest[ocl_su3matrix_element(0,0)] = hmc_complex_one;
    dest[ocl_su3matrix_element(0,1)] = hmc_complex_zero;
    dest[ocl_su3matrix_element(0,2)] = hmc_complex_zero;
    dest[ocl_su3matrix_element(1,0)] = hmc_complex_zero;
    dest[ocl_su3matrix_element(1,1)] = src[0];
    dest[ocl_su3matrix_element(1,2)] = src[1];
    dest[ocl_su3matrix_element(2,0)] = hmc_complex_zero;
    dest[ocl_su3matrix_element(2,1)] = src[2];
    dest[ocl_su3matrix_element(2,2)] = src[3];
  }
  else if (random == 3){
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



//spinor operations
void su3matrix_times_colorvector(hmc_ocl_su3matrix* u, hmc_color_vector* in, hmc_color_vector* out){
#ifdef _RECONSTRUCT_TWELVE_
  for(int a=0; a<NC-1; a++) {
    out[a] = hmc_complex_zero;
    for(int b=0; b<NC; b++) {
      hmc_complex tmp = complexmult(&((*u)[a+(NC-1)*b]),&(in[b]));
      complexaccumulate(&out[a],&tmp);
    }
  }
  out[2] = hmc_complex_zero;
  for(int b=0; b<NC; b++) {
    hmc_complex rec = reconstruct_su3(u,b);
    hmc_complex tmp = complexmult(&rec,&(in[b]));
    complexaccumulate(&out[2],&tmp);
  }
#else
  for(int a=0; a<NC; a++) {
    out[a] = hmc_complex_zero;
    for(int b=0; b<NC; b++) {
      hmc_complex tmp = complexmult(&((u)[a + NC*b]),&(in[b]));
      complexaccumulate(&(out[a]),&tmp);
    }
  }
#endif
  return;
}

int spinor_color(int spinor_element){
  return (int)(spinor_element/NSPIN);
}

int spinor_spin(int spinor_element,int color){
  return spinor_element - NSPIN*color;
}

int inline spinor_element(int alpha, int color) {
  return alpha + NSPIN*color;
}

int inline spinor_field_element(int alpha, int color, int nspace, int t) {
  return alpha + NSPIN*color + NSPIN*NC*t+NSPIN*NC*NTIME*nspace;
}

void set_zero_spinor(hmc_spinor_field *field) {
  for (int n=0; n<SPINORFIELDSIZE; n++) {
	field[n].re=0;
	field[n].im=0;
  }
  return;
}

void set_zero_eoprec_spinor(hmc_eoprec_spinor_field *field) {
  for (int n=0; n<EOPREC_SPINORFIELDSIZE; n++) {
	field[n].re=0;
	field[n].im=0;
  }
  return;
}

hmc_float local_squarenorm(hmc_spinor_field *field, int spacepos, int timepos) {
  hmc_float sum=0;
  for (int a=0; a<NSPIN; a++) {
    for(int c=0; c<NC; c++) {
      hmc_float dummy_re = field[spinor_field_element(a,c,spacepos,timepos)].re;
      hmc_float dummy_im = field[spinor_field_element(a,c,spacepos,timepos)].im;
      sum += dummy_re*dummy_re + dummy_im*dummy_im;
    }
  }
  return sum;
}

hmc_float global_squarenorm(hmc_spinor_field *field) {
  hmc_float sum=0;
  for (int t=0; t<NTIME; t++) {
    for (int n=0; n<VOLSPACE; n++) {
      sum += local_squarenorm(field,n,t);
    }
  }
  return sum;
}

void fill_with_one(hmc_spinor_field *field, int spacepos, int timepos, int alpha, int color){
  field[spinor_field_element(alpha,color,spacepos,timepos)].re = hmc_one_f;
  field[spinor_field_element(alpha,color,spacepos,timepos)].im = 0;
  return;
}







//eoprec operations
// int inline eoprec_spinor_field_element(int alpha, int color, int nspace, int t) {
//   return alpha + NSPIN*color + NSPIN*NC*get_n_eoprec(t,nspace);
// }

int inline eoprec_spinor_field_element(int alpha, int color, int n_eoprec) {
  return alpha + NSPIN*color + NSPIN*NC*n_eoprec;
}

void convert_to_eoprec(hmc_eoprec_spinor_field* even, hmc_eoprec_spinor_field* odd, hmc_spinor_field* in){
  for(int n=0; n<VOL4D/2; n++) {
    for(int alpha=0; alpha<NSPIN; alpha++) {
      for(int color=0; color<NC; color++) {
	int spacepos = nspace_from_even_index[n];
	int timepos = ntime_from_even_index[n];
	even[eoprec_spinor_field_element(alpha,color,n)] = in[spinor_field_element(alpha,color,spacepos,timepos)];
	spacepos = nspace_from_odd_index[n];
	timepos = ntime_from_odd_index[n];
	odd[eoprec_spinor_field_element(alpha,color,n)] = in[spinor_field_element(alpha,color,spacepos,timepos)];
      }
    }
  }
  return;
}

void convert_from_eoprec(hmc_eoprec_spinor_field* even, hmc_eoprec_spinor_field* odd, hmc_spinor_field* out){
  for(int n=0; n<VOL4D/2; n++) {
    for(int alpha=0; alpha<NSPIN; alpha++) {
      for(int color=0; color<NC; color++) {
	int spacepos = nspace_from_even_index[n];
	int timepos = ntime_from_even_index[n];
        out[spinor_field_element(alpha,color,spacepos,timepos)] = even[eoprec_spinor_field_element(alpha,color,n)];
	spacepos = nspace_from_odd_index[n];
	timepos = ntime_from_odd_index[n];
	out[spinor_field_element(alpha,color,spacepos,timepos)] = odd[eoprec_spinor_field_element(alpha,color,n)];
      }
    }
  }
  return;
}

void convert_to_kappa_format(__global hmc_spinor_field* inout,hmc_float kappa){
  for(int n=0; n<SPINORFIELDSIZE; n++) {
    inout[n].re *= sqrt(2.*kappa);
    inout[n].im *= sqrt(2.*kappa);
  }
  return;
}

void convert_to_kappa_format_eoprec(__global hmc_eoprec_spinor_field* inout,hmc_float kappa){
  for(int n=0; n<EOPREC_SPINORFIELDSIZE; n++) {
    inout[n].re *= sqrt(2.*kappa);
    inout[n].im *= sqrt(2.*kappa);
  }
  return;
}

void convert_from_kappa_format(__global hmc_spinor_field* in, __global hmc_spinor_field * out,hmc_float kappa){
  for(int n=0; n<SPINORFIELDSIZE; n++) {
    out[n].re = (in[n].re)/sqrt(2.*kappa);
    out[n].im = (in[n].im)/sqrt(2.*kappa);
  }
  return;
}

void convert_from_kappa_format_eoprec(__global hmc_eoprec_spinor_field* in, __global hmc_eoprec_spinor_field * out, hmc_float kappa){
  for(int n=0; n<EOPREC_SPINORFIELDSIZE; n++) {
    out[n].re = (in[n].re)/sqrt(2.*kappa);
    out[n].im = (in[n].im)/sqrt(2.*kappa);
  }
  return;
}


hmc_float global_squarenorm_eoprec(hmc_eoprec_spinor_field *in) {
  hmc_float sum=0;
  for (int n=0; n<EOPREC_SPINORFIELDSIZE; n++) {
    sum += in[n].re*in[n].re + in[n].im*in[n].im;
  }
  return sum;
}

hmc_complex scalar_product_eoprec(hmc_eoprec_spinor_field* a, hmc_eoprec_spinor_field* b){
  // (a,b) = sum_k conj(a_k)*b_k
  hmc_complex res;
  res.re=0;
  res.im=0;
  for(int n=0; n<EOPREC_SPINORFIELDSIZE; n++) {
    res.re += a[n].re*b[n].re + a[n].im*b[n].im;
    res.im += a[n].re*b[n].im - a[n].im*b[n].re;
  }
  return res;
}

hmc_complex scalar_product(hmc_spinor_field* a, hmc_spinor_field* b){
  // (a,b) = sum_k conj(a_k)*b_k
  hmc_complex res;
  res.re=0;
  res.im=0;
  for(int n=0; n<SPINORFIELDSIZE; n++) {
    res.re += a[n].re*b[n].re + a[n].im*b[n].im;
    res.im += a[n].re*b[n].im - a[n].im*b[n].re;
  }
//   if((res.re != res.re) && (res.im != res.im)) 
//     printf("%f, %f\n", res.re, res.im);
  return res;
}

void get_spinor_from_eoprec_field(hmc_eoprec_spinor_field* in, hmc_spinor* out, int n_eoprec){
  for(int alpha=0; alpha<NSPIN; alpha++) {
    for(int color=0; color<NC; color++) {
      out[spinor_element(alpha,color)] = in[eoprec_spinor_field_element(alpha,color,n_eoprec)];
    }
  }
  return;
}

void put_spinor_to_eoprec_field(hmc_spinor* in, hmc_eoprec_spinor_field* out, int n_eoprec){
  for(int alpha=0; alpha<NSPIN; alpha++) {
    for(int color=0; color<NC; color++) {
      out[eoprec_spinor_field_element(alpha,color,n_eoprec)]=in[spinor_element(alpha,color)];
    }
  }
  return;
}

void get_spinor_from_field(hmc_spinor_field* in, hmc_spinor* out, int n, int t){
  for(int alpha=0; alpha<NSPIN; alpha++) {
    for(int color=0; color<NC; color++) {
      out[spinor_element(alpha,color)] = in[spinor_field_element(alpha,color,n,t)];
    }
  }
  return;
}

void put_spinor_to_field(hmc_spinor* in, hmc_spinor_field* out, int n, int t){
  for(int alpha=0; alpha<NSPIN; alpha++) {
    for(int color=0; color<NC; color++) {
      out[spinor_field_element(alpha,color,n,t)]=in[spinor_element(alpha,color)];
    }
  }
  return;
}

void set_local_zero_spinor(hmc_spinor* inout){
  for(int j=0; j<SPINORSIZE; j++) {
    inout[j].re = 0;
    inout[j].im = 0;
  }
  return;
}

hmc_float spinor_squarenorm(hmc_spinor* in){
  hmc_float res=0;
  for(int j=0; j<SPINORSIZE; j++) {
    hmc_complex tmp = complexconj(&in[j]);
    hmc_complex incr= complexmult(&tmp,&in[j]);
    res+=incr.re;
  }
  return res;
}

void real_multiply_spinor(hmc_spinor* inout, hmc_float factor){
  for(int j=0; j<SPINORSIZE; j++) inout[j].re *=factor;
  return;
}

void spinprojectproduct_gamma0(hmc_ocl_su3matrix* u, hmc_spinor* spin,hmc_float sign){
  //out = u*(1+sign*gamma0)*in
  hmc_color_vector vec1[NC];
  hmc_color_vector vec2[NC];
  hmc_color_vector uvec1[NC];
  hmc_color_vector uvec2[NC];
  for(int c=0; c<NC; c++) {
    vec1[c].re = spin[spinor_element(0,c)].re - sign*spin[spinor_element(3,c)].im;
    vec1[c].im = spin[spinor_element(0,c)].im + sign*spin[spinor_element(3,c)].re;
    vec2[c].re = spin[spinor_element(1,c)].re - sign*spin[spinor_element(2,c)].im;
    vec2[c].im = spin[spinor_element(1,c)].im + sign*spin[spinor_element(2,c)].re;
  }
  su3matrix_times_colorvector(u,vec1,uvec1);
  su3matrix_times_colorvector(u,vec2,uvec2);
  for(int c=0; c<NC; c++) {
    spin[spinor_element(0,c)].re = uvec1[c].re;
    spin[spinor_element(0,c)].im = uvec1[c].im;
    spin[spinor_element(1,c)].re = uvec2[c].re;
    spin[spinor_element(1,c)].im = uvec2[c].im;
    spin[spinor_element(2,c)].re = sign*uvec2[c].im;
    spin[spinor_element(2,c)].im = -sign*uvec2[c].re;
    spin[spinor_element(3,c)].re = sign*uvec1[c].im;
    spin[spinor_element(3,c)].im = -sign*uvec1[c].re;
  }
  return;
}

void spinprojectproduct_gamma1(hmc_ocl_su3matrix* u, hmc_spinor* spin, hmc_float sign){
  //out = u*(1+sign*gamma1)*in
  hmc_color_vector vec1[NC];
  hmc_color_vector vec2[NC];
  hmc_color_vector uvec1[NC];
  hmc_color_vector uvec2[NC];
  for(int c=0; c<NC; c++) {
    vec1[c].re = spin[spinor_element(0,c)].re - sign*spin[spinor_element(3,c)].re;
    vec1[c].im = spin[spinor_element(0,c)].im - sign*spin[spinor_element(3,c)].im;
    vec2[c].re = spin[spinor_element(1,c)].re + sign*spin[spinor_element(2,c)].re;
    vec2[c].im = spin[spinor_element(1,c)].im + sign*spin[spinor_element(2,c)].im;
  }
  su3matrix_times_colorvector(u,vec1,uvec1);
  su3matrix_times_colorvector(u,vec2,uvec2);
  for(int c=0; c<NC; c++) {
    spin[spinor_element(0,c)].re = uvec1[c].re;
    spin[spinor_element(0,c)].im = uvec1[c].im;
    spin[spinor_element(1,c)].re = uvec2[c].re;
    spin[spinor_element(1,c)].im = uvec2[c].im;
    spin[spinor_element(2,c)].re = sign*uvec2[c].re;
    spin[spinor_element(2,c)].im = sign*uvec2[c].im;
    spin[spinor_element(3,c)].re = -sign*uvec1[c].re;
    spin[spinor_element(3,c)].im = -sign*uvec1[c].im;
  }
  return;
}

void spinprojectproduct_gamma2(hmc_ocl_su3matrix* u, hmc_spinor* spin, hmc_float sign){
  //out = u*(1+sign*gamma2)*in
  hmc_color_vector vec1[NC];
  hmc_color_vector vec2[NC];
  hmc_color_vector uvec1[NC];
  hmc_color_vector uvec2[NC];
  for(int c=0; c<NC; c++) {
    vec1[c].re = spin[spinor_element(0,c)].re - sign*spin[spinor_element(2,c)].im;
    vec1[c].im = spin[spinor_element(0,c)].im + sign*spin[spinor_element(2,c)].re;
    vec2[c].re = spin[spinor_element(1,c)].re + sign*spin[spinor_element(3,c)].im;
    vec2[c].im = spin[spinor_element(1,c)].im - sign*spin[spinor_element(3,c)].re;
  }
  su3matrix_times_colorvector(u,vec1,uvec1);
  su3matrix_times_colorvector(u,vec2,uvec2);
  for(int c=0; c<NC; c++) {
    spin[spinor_element(0,c)].re = uvec1[c].re;
    spin[spinor_element(0,c)].im = uvec1[c].im;
    spin[spinor_element(1,c)].re = uvec2[c].re;
    spin[spinor_element(1,c)].im = uvec2[c].im;
    spin[spinor_element(2,c)].re = sign*uvec1[c].im;
    spin[spinor_element(2,c)].im = -sign*uvec1[c].re;
    spin[spinor_element(3,c)].re = -sign*uvec2[c].im;
    spin[spinor_element(3,c)].im = sign*uvec2[c].re;
  }
  return;
}

void spinprojectproduct_gamma3(hmc_ocl_su3matrix* u, hmc_spinor* spin, hmc_float sign){
  //out = u*(1+sign*gamma3)*in
  hmc_color_vector vec1[NC];
  hmc_color_vector vec2[NC];
  hmc_color_vector uvec1[NC];
  hmc_color_vector uvec2[NC];
  for(int c=0; c<NC; c++) {
    vec1[c].re = spin[spinor_element(0,c)].re + sign*spin[spinor_element(2,c)].re;
    vec1[c].im = spin[spinor_element(0,c)].im + sign*spin[spinor_element(2,c)].im;
    vec2[c].re = spin[spinor_element(1,c)].re + sign*spin[spinor_element(3,c)].re;
    vec2[c].im = spin[spinor_element(1,c)].im + sign*spin[spinor_element(3,c)].im;
  }
  su3matrix_times_colorvector(u,vec1,uvec1);
  su3matrix_times_colorvector(u,vec2,uvec2);
  for(int c=0; c<NC; c++) {
    spin[spinor_element(0,c)].re = uvec1[c].re;
    spin[spinor_element(0,c)].im = uvec1[c].im;
    spin[spinor_element(1,c)].re = uvec2[c].re;
    spin[spinor_element(1,c)].im = uvec2[c].im;
    spin[spinor_element(2,c)].re = sign*uvec1[c].re;
    spin[spinor_element(2,c)].im = sign*uvec1[c].im;
    spin[spinor_element(3,c)].re = sign*uvec2[c].re;
    spin[spinor_element(3,c)].im = sign*uvec2[c].im;
  }
  return;
}

void spinors_accumulate(hmc_spinor* inout, hmc_spinor* incr){
  for(int j=0; j<SPINORSIZE; j++) {
    inout[j].re += incr[j].re;
    inout[j].im += incr[j].im;
  }
  return;
}

void multiply_spinor_factor_gamma5(hmc_spinor* in, hmc_spinor* out, hmc_float factor){
  for(int c=0; c<NC; c++) {
    out[spinor_element(0,c)].re = -factor*in[spinor_element(0,c)].im;
    out[spinor_element(1,c)].re = -factor*in[spinor_element(1,c)].im;
    out[spinor_element(2,c)].re = factor*in[spinor_element(2,c)].im;
    out[spinor_element(3,c)].re = factor*in[spinor_element(3,c)].im;
    out[spinor_element(0,c)].im = factor*in[spinor_element(0,c)].re;
    out[spinor_element(1,c)].im = factor*in[spinor_element(1,c)].re;
    out[spinor_element(2,c)].im = -factor*in[spinor_element(2,c)].re;
    out[spinor_element(3,c)].im = -factor*in[spinor_element(3,c)].re;
  }
  return;
}

void multiply_spinor_gamma0(hmc_spinor* in,hmc_spinor* out){
  for(int c=0; c<NC; c++) {
    out[spinor_element(0,c)].re = -in[spinor_element(3,c)].im;
    out[spinor_element(1,c)].re = -in[spinor_element(2,c)].im;
    out[spinor_element(2,c)].re = in[spinor_element(1,c)].im;
    out[spinor_element(3,c)].re = in[spinor_element(0,c)].im;
    out[spinor_element(0,c)].im = in[spinor_element(3,c)].re;
    out[spinor_element(1,c)].im = in[spinor_element(2,c)].re;
    out[spinor_element(2,c)].im = -in[spinor_element(1,c)].re;
    out[spinor_element(3,c)].im = -in[spinor_element(0,c)].re;
  }
  return;
}
void multiply_spinor_gamma1(hmc_spinor* in,hmc_spinor* out){
  for(int c=0; c<NC; c++) {
    out[spinor_element(0,c)].re = -in[spinor_element(3,c)].re;
    out[spinor_element(1,c)].re = in[spinor_element(2,c)].re;
    out[spinor_element(2,c)].re = in[spinor_element(1,c)].re;
    out[spinor_element(3,c)].re = -in[spinor_element(0,c)].re;
    out[spinor_element(0,c)].im = -in[spinor_element(3,c)].im;
    out[spinor_element(1,c)].im = in[spinor_element(2,c)].im;
    out[spinor_element(2,c)].im = in[spinor_element(1,c)].im;
    out[spinor_element(3,c)].im = -in[spinor_element(0,c)].im;
  }
  return;
}
void multiply_spinor_gamma2(hmc_spinor* in,hmc_spinor* out){
  for(int c=0; c<NC; c++) {
    out[spinor_element(0,c)].re = -in[spinor_element(2,c)].im;
    out[spinor_element(1,c)].re = in[spinor_element(3,c)].im;
    out[spinor_element(2,c)].re = in[spinor_element(0,c)].im;
    out[spinor_element(3,c)].re = -in[spinor_element(1,c)].im;
    out[spinor_element(0,c)].im = in[spinor_element(2,c)].re;
    out[spinor_element(1,c)].im = -in[spinor_element(3,c)].re;
    out[spinor_element(2,c)].im = -in[spinor_element(0,c)].re;
    out[spinor_element(3,c)].im = in[spinor_element(1,c)].re;
  }
  return;
}
void multiply_spinor_gamma3(hmc_spinor* in,hmc_spinor* out){
  for(int c=0; c<NC; c++) {
    out[spinor_element(0,c)].re = in[spinor_element(2,c)].re;
    out[spinor_element(1,c)].re = in[spinor_element(3,c)].re;
    out[spinor_element(2,c)].re = in[spinor_element(0,c)].re;
    out[spinor_element(3,c)].re = in[spinor_element(1,c)].re;
    out[spinor_element(0,c)].im = in[spinor_element(2,c)].im;
    out[spinor_element(1,c)].im = in[spinor_element(3,c)].im;
    out[spinor_element(2,c)].im = in[spinor_element(0,c)].im;
    out[spinor_element(3,c)].im = in[spinor_element(1,c)].im;
  }
  return;
}

void su3matrix_times_spinor(hmc_ocl_su3matrix* u, hmc_spinor* in, hmc_spinor* out){
  for (int alpha=0; alpha<NSPIN; alpha++) {
    hmc_color_vector vec_in[NC];
    hmc_color_vector vec_out[NC];
    for(int c=0; c<NC; c++) {
      vec_in[c].re = in[spinor_element(alpha,c)].re;
      vec_in[c].im = in[spinor_element(alpha,c)].im;
    }
    su3matrix_times_colorvector(u, vec_in, vec_out);
    for(int c=0; c<NC; c++) {
      in[spinor_element(alpha,c)].re = vec_out[c].re;
      in[spinor_element(alpha,c)].im = vec_out[c].im;
    }
  }
  return;
}


void spinor_apply_bc(hmc_spinor * in, hmc_float theta){
  for(int n = 0; n<SPINORSIZE; n++){
    hmc_float tmp1 = in[n].re;
    hmc_float tmp2 = in[n].im;
    in[n].re = cos(theta)*tmp1 - sin(theta)*tmp2;
    in[n].im = sin(theta)*tmp1 + cos(theta)*tmp2;
  }
  return; 
}

//new stuff

void copy_spinor(hmc_complex * in, hmc_complex * out){
  for (int n=0; n<SPINORFIELDSIZE; n++) {
    out[n].re = in[n].re;
    out[n].im = in[n].im;
  }
  return;
}

void get_spinor(__global hmc_complex * in, hmc_complex * out){
  for (int n=0; n<SPINORFIELDSIZE; n++) {
    out[n].re = in[n].re;
    out[n].im = in[n].im;
  }
  return;
}

void put_spinor(hmc_complex * in, __global hmc_complex * out){
  for (int n=0; n<SPINORFIELDSIZE; n++) {
    out[n].re = in[n].re;
    out[n].im = in[n].im;
  }
  return;
}

// -alpha*x + y
//CP: defined with a minus!!!
void saxpy(hmc_spinor_field * x, hmc_spinor_field * y, hmc_complex * alpha, hmc_spinor_field * out){
  for (int n=0; n<SPINORFIELDSIZE; n++) {
    hmc_complex tmp1 = complexmult(alpha,&x[n]);
    ((out)[n]).re = -(tmp1).re + y[n].re;
    ((out)[n]).im = -(tmp1).im + y[n].im;
  }
  return;
}

//alpha*x + beta*y + z
void saxsbypz(hmc_spinor_field * x, hmc_spinor_field * y,  hmc_spinor_field * z, hmc_complex * alpha, hmc_complex * beta, hmc_spinor_field * out){
  for (int n=0; n<SPINORFIELDSIZE; n++) {
    hmc_complex tmp1 = complexmult(alpha,&x[n]);
    hmc_complex tmp2 = complexmult(beta,&y[n]);
    ((out)[n]).re = (tmp1).re + (tmp2).re + z[n].re;
    ((out)[n]).im = (tmp1).im + (tmp2).im + z[n].im;
  }
  return;
}

void create_point_source(hmc_spinor_field* b, int i, int spacepos, int timepos, hmc_float kappa, hmc_float mu, __global hmc_ocl_gaugefield * gaugefield){
  set_zero_spinor(b);

  int color = spinor_color(i);
  int spin = spinor_spin(i,color);

  b[spinor_field_element(spin,color,spacepos,timepos)].re = sqrt(2.*kappa);

  return;
}

