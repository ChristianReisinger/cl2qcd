#ifndef _UPDATEH_
#define _UPDATEH_
#include "globaldefs.h"
#include "types.h"
#include "hmcerrs.h"
#include "operations.h"
#include "geometry.h"
#include "random.h"

extern Ran myran;


void SU2Update(hmc_float dst [su2_entries], const hmc_float alpha);

// void reduction (hmc_complex dest[su2_entries], hmc_su3matrix src, const int rand);
// 
// void extend (hmc_su3matrix dest, const int random, hmc_complex src[su2_entries]);

// void calc_staple(hmc_gaugefield * field, hmc_su3matrix * dest, const int pos, const int t, const int mu_in);
void calc_staple(hmc_gaugefield * field, hmc_staplematrix * dest, const int pos, const int t, const int mu_in);

void heatbath_update (hmc_gaugefield * gaugefield, const hmc_float beta);

void heatbath_overrelax (hmc_gaugefield * gaugefield, const hmc_float beta);

void heatbath_update_checkerboard (hmc_gaugefield * gaugefield, const hmc_float beta);

void heatbath_overrelax_checkerboard (hmc_gaugefield * gaugefield, const hmc_float beta);

#endif
