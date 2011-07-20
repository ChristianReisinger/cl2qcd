/** @file
 * Input file handling
 */

#ifndef _INPUTPARAMETERSH_
#define _INPUTPARAMETERSH_

#include "hmcerrs.h"
#include "types.h"
#include "globaldefs.h"
#include "logger.hpp"

#include <cstdlib>
#include <cstdio>
#include <fstream>
#include <iostream>
#include <cstring>
using namespace std;

/**
 * Parser for the input file.
 */
class inputparameters {
public:
	/**
	 * Default constructor loads default values.
	 */
	inputparameters() {
		set_defaults();
	};
	/**
	 * Parse the given file, overwriting current
	 * parameter values.
	 *
	 * \param ifn Name of the input file to parse
	 * \return Error as defined in hmcerrs.h:
	 *         \li HMC_FILEERROR if file cannot be opened
	 *         \li HMC_SUCCESS otherwise
	 */
	hmc_error readfile(char* ifn);
	/**
	 * Reset all parameters to default values.
	 *
	 * \return Should always return HMC_SUCCESS.
	 */
	hmc_error set_defaults();
	hmc_float get_kappa();
	void set_mubar_negative();
	void calc_mubar();
	hmc_float get_mubar();
	hmc_float get_beta();
	hmc_float get_tau();
	hmc_float get_theta_fermion_spatial();
	hmc_float get_theta_fermion_temporal();
	hmc_float get_theta_gaugefield();
	hmc_float get_mu();
	hmc_float get_csw();
	hmc_float get_chem_pot_re();
	hmc_float get_chem_pot_im();
	int get_use_eo();
	int get_cgmax();
	/**
	 * The precision to be used for gaugefield storage in bits.
	 * @li 32 for single precision
	 * @li 64 for double precision
	 */
	int get_prec();
	int get_startcondition();
	int get_thermalizationsteps();
	int get_heatbathsteps();
	int get_overrelaxsteps();
	int get_hmcsteps();
	//this is suppposed to be used for gauge-sector
	int get_integrationsteps1();
	//this is supposed to be used for fermion-sector
	int get_integrationsteps2();
	int get_saveconfigs();
	int get_savefrequency();
	int get_writefrequency();
	int get_fermact();
	void display_sourcefile();
	void display_sourcefilenumber();
	int get_num_dev();

	/**
	 * Print info for heatbath executable using logger.
	 * @param progname Name of the executable.
	 */
	void print_info_heatbath(char* progname);

	/**
	 * Print info for heatbath executable using output stream.
	 * @param progname Name of the executable.
	 * @param os Pointer to wanted output stream.
	 */
	void print_info_heatbath(char* progname, ostream* os);

	/**
	 * Print info for inverter executable using logger.
	 * @param progname Name of the executable.
	 */
	void print_info_inverter(char* progname);

	/**
	 * Print info for inverter executable using output stream.
	 * @param progname Name of the executable.
	 * @param os Pointer to wanted output stream.
	 */
	void print_info_inverter(char* progname, ostream* os);

	/**
	 * Print info for tk_kappa executable using logger.
	 * @param progname Name of the executable.
	 */
	void print_info_tkkappa(char* progname);

	/**
	 * Print info for tk_kappa executable using output stream.
	 * @param progname Name of the executable.
	 * @param os Pointer to wanted output stream.
	 */
	void print_info_tkkappa(char* progname, ostream* os);

	/**
	 * Print info for hmc executable using logger.
	 * @param progname Name of the executable.
	 */
	void print_info_hmc(char* progname);

	/**
	 * Print info for hmc executable using output stream.
	 * @param progname Name of the executable.
	 * @param os Pointer to wanted output stream.
	 */
	void print_info_hmc(char* progname, ostream* os);

	//CP
	//this is out of laziness
	std::string sourcefile;
	std::string sourcefilenumber;
private:
	hmc_float kappa;
	hmc_float beta;
	hmc_float mu;
	hmc_float mubar;
	hmc_float csw;
	hmc_float theta_fermion_spatial;
	hmc_float theta_fermion_temporal;
	hmc_float theta_gaugefield;
	hmc_float chem_pot_re;
	hmc_float chem_pot_im;
	hmc_float tau;
	int num_dev;
	int cgmax;
	int use_eo;
	int fermact;
	int prec;
	int startcondition;
	int thermalizationsteps;
	int heatbathsteps;
	int overrelaxsteps;
	int hmcsteps;
	int savefrequency;
	int saveconfigs;
	int writefrequency;
	int integrationsteps1;
	int integrationsteps2;
	void val_assign(hmc_float* out, std::string line);
	void val_assign(int * out, std::string line);
	void sourcefilenumber_assign(std::string * out);
	void cond_assign(int * out, std::string line);
	void fermact_assign(int * out, std::string line);
	void savecond_assign(int * out, std::string line);
	void val_assign(std::string * out, std::string line);
	void eocond_assign(int * out, std::string line);
};

#endif /* _INPUTPARAMETERSH_ */
