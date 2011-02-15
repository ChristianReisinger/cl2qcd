#ifndef _HMCERRSH_
#define _HMCERRSH_

typedef int hmc_error;

const hmc_error HMC_SUCCESS =  0;
const hmc_error HMC_STDERR  = -1;
const hmc_error HMC_FILEERROR  = -2;
const hmc_error HMC_OCLERROR = -3;
const hmc_error HMC_XMLERROR = -4;
const hmc_error HMC_UNDEFINEDERROR = -999;
const hmc_error HMC_EOERROR = -5;

#endif
