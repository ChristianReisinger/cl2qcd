/*
 * Copyright 2014 Christopher Pinke
 *
 * This file is part of CL2QCD.
 *
 * CL2QCD is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * CL2QCD is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with CL2QCD.  If not, see <http://www.gnu.org/licenses/>.
 */

#include "SpinorTester.hpp"
#include "../../host_functionality/host_geometry.h"

SpinorTester::SpinorTester(std::string kernelName, std::string inputfileIn, int numberOfValues):
	KernelTester(kernelName, getSpecificInputfile(inputfileIn), numberOfValues)
	{
	code = device->get_spinor_code();
	prng = new physics::PRNG(*system);
	doubleBuffer = new hardware::buffers::Plain<double> (1, device);
	
	//todo: some of these could also be put into the specific child-classes where they are actually used.
	spinorfieldElements = hardware::code::get_spinorfieldsize(*parameters);
	spinorfieldEvenOddElements = hardware::code::get_eoprec_spinorfieldsize(*parameters);
	(parameters->get_solver() == meta::Inputparameters::cg) ? useRandom = false : useRandom =true;
	(parameters->get_read_multiple_configs() ) ? evenOrOdd = true : evenOrOdd = false;
	alpha_host = {parameters->get_beta(), parameters->get_rho()};
	beta_host = {parameters->get_kappa(), parameters->get_mu()};
	iterations = parameters->get_integrationsteps(0);
	parameters->get_read_multiple_configs() ? calcVariance=false : calcVariance = true;
}

SpinorTester::~SpinorTester()
{
	delete doubleBuffer;
	delete prng;
}

void SpinorTester::fill_with_one(spinor * in, int size)
{
  for(int i = 0; i < size; ++i) {
    in[i].e0.e0 = hmc_complex_one;
    in[i].e0.e1 = hmc_complex_one;
    in[i].e0.e2 = hmc_complex_one;
    in[i].e1.e0 = hmc_complex_one;
    in[i].e1.e1 = hmc_complex_one;
    in[i].e1.e2 = hmc_complex_one;
    in[i].e2.e0 = hmc_complex_one;
    in[i].e2.e1 = hmc_complex_one;
    in[i].e2.e2 = hmc_complex_one;
    in[i].e3.e0 = hmc_complex_one;
    in[i].e3.e1 = hmc_complex_one;
    in[i].e3.e2 = hmc_complex_one;
  }
  return;
}

void SpinorTester::fill_with_random(spinor * in, int size, int seed)
{
  prng_init(seed);
  for(int i = 0; i < size; ++i) {
    in[i].e0.e0.re = prng_double();
    in[i].e0.e1.re = prng_double();
    in[i].e0.e2.re = prng_double();
    in[i].e1.e0.re = prng_double();
    in[i].e1.e1.re = prng_double();
    in[i].e1.e2.re = prng_double();
    in[i].e2.e0.re = prng_double();
    in[i].e2.e1.re = prng_double();
    in[i].e2.e2.re = prng_double();
    in[i].e3.e0.re = prng_double();
    in[i].e3.e1.re = prng_double();
    in[i].e3.e2.re = prng_double();
    
    in[i].e0.e0.im = prng_double();
    in[i].e0.e1.im = prng_double();
    in[i].e0.e2.im = prng_double();
    in[i].e1.e0.im = prng_double();
    in[i].e1.e1.im = prng_double();
    in[i].e1.e2.im = prng_double();
    in[i].e2.e0.im = prng_double();
    in[i].e2.e1.im = prng_double();
    in[i].e2.e2.im = prng_double();
    in[i].e3.e0.im = prng_double();
    in[i].e3.e1.im = prng_double();
    in[i].e3.e2.im = prng_double();
  }
  return;
}

spinor * SpinorTester::createSpinorfield(size_t numberOfElements, int seed)
{
  spinor * in;
  in = new spinor[numberOfElements];
  useRandom ? fill_with_random(in, numberOfElements, seed) : fill_with_one(in, numberOfElements);
  BOOST_REQUIRE(in);
  return in;
}

spinor * SpinorTester::createSpinorfieldWithOnesAndZerosDependingOnSiteParity()
{
  spinor * in;
  in = new spinor[spinorfieldElements];
  fill_with_one_eo(in, spinorfieldElements, evenOrOdd);
  return in;
}

std::string SpinorTester::getSpecificInputfile(std::string inputfileIn)
{
  return "spinors/" + inputfileIn;
}

void SpinorTester::fill_with_one_eo(spinor * in, int size, bool eo)
	{
		int x, y, z, t;
		hmc_complex content;
		int coord[4];
		bool parityOfSite;
		int nspace;
		int global_pos;
		int ns, nt;
		
		ns = parameters->get_nspace();
		nt = parameters->get_ntime();

		for (x = 0; x < ns; x++) {
			for (y = 0; y < ns; y++) {
				for (z = 0; z < ns; z++) {
					for (t = 0; t < nt; t++) {
						coord[0] = t;
						coord[1] = x;
						coord[2] = y;
						coord[3] = z;
						nspace =  get_nspace(coord, *parameters);
						global_pos = get_global_pos(nspace, t, *parameters);
						if (global_pos > size)
							break;

						parityOfSite = (x + y + z + t) % 2 == 0;
						content = (parityOfSite) ?
							(eo ? hmc_complex_one : hmc_complex_zero) :
							(eo ? hmc_complex_zero : hmc_complex_one);

						in[global_pos].e0.e0 = content;
						in[global_pos].e0.e1 = content;
						in[global_pos].e0.e2 = content;
						in[global_pos].e1.e0 = content;
						in[global_pos].e1.e1 = content;
						in[global_pos].e1.e2 = content;
						in[global_pos].e2.e0 = content;
						in[global_pos].e2.e1 = content;
						in[global_pos].e2.e2 = content;
						in[global_pos].e3.e0 = content;
						in[global_pos].e3.e1 = content;
						in[global_pos].e3.e2 = content;
					}
				}
			}
		}
		return;
	}

	hmc_float SpinorTester::count_sf(spinor * in, int size)
	{
		hmc_float sum = 0.;
		for (int i = 0; i < size; i++) {
			sum +=
				in[i].e0.e0.re + in[i].e0.e0.im
				+ in[i].e0.e1.re + in[i].e0.e1.im
				+ in[i].e0.e2.re + in[i].e0.e2.im
				+ in[i].e1.e0.re + in[i].e1.e0.im
				+ in[i].e1.e1.re + in[i].e1.e1.im
				+ in[i].e1.e2.re + in[i].e1.e2.im
				+ in[i].e2.e0.re + in[i].e2.e0.im
				+ in[i].e2.e1.re + in[i].e2.e1.im
				+ in[i].e2.e2.re + in[i].e2.e2.im
				+ in[i].e3.e0.re + in[i].e3.e0.im
				+ in[i].e3.e1.re + in[i].e3.e1.im
				+ in[i].e3.e2.re + in[i].e3.e2.im;
		}
		return sum;
	}

	hmc_float SpinorTester::calc_var(hmc_float in, hmc_float mean)
	{
		return (in - mean) * (in - mean);
	}

	hmc_float SpinorTester::calc_var_sf(spinor * in, int size, hmc_float sum)
	{
		hmc_float var = 0.;
		for(int k = 0; k < size; k++) {
			var +=
				calc_var( in[k].e0.e0.re , sum)
				+ calc_var( in[k].e0.e0.im , sum)
				+ calc_var( in[k].e0.e1.re , sum)
				+ calc_var( in[k].e0.e1.im , sum)
				+ calc_var( in[k].e0.e2.re , sum)
				+ calc_var( in[k].e0.e2.im , sum)
				+ calc_var( in[k].e1.e0.re , sum)
				+ calc_var( in[k].e1.e0.im , sum)
				+ calc_var( in[k].e1.e1.re , sum)
				+ calc_var( in[k].e1.e1.im , sum)
				+ calc_var( in[k].e1.e2.re , sum)
				+ calc_var( in[k].e1.e2.im , sum)
				+ calc_var( in[k].e2.e0.re , sum)
				+ calc_var( in[k].e2.e0.im , sum)
				+ calc_var( in[k].e2.e1.re , sum)
				+ calc_var( in[k].e2.e1.im , sum)
				+ calc_var( in[k].e2.e2.re , sum)
				+ calc_var( in[k].e2.e2.im , sum)
				+ calc_var( in[k].e3.e0.re , sum)
				+ calc_var( in[k].e3.e0.im , sum)
				+ calc_var( in[k].e3.e1.re , sum)
				+ calc_var( in[k].e3.e1.im , sum)
				+ calc_var( in[k].e3.e2.re , sum)
				+ calc_var( in[k].e3.e2.im , sum);
		}
		return var;
	}

void SpinorTester::calcSquarenormAndStoreAsKernelResult(const hardware::buffers::Plain<spinor> * in)
{
  code->set_float_to_global_squarenorm_device(in, doubleBuffer);
  doubleBuffer->dump(&kernelResult[0]);
}

void SpinorTester::calcSquarenormEvenOddAndStoreAsKernelResult(const hardware::buffers::Spinor * in)
{
  code->set_float_to_global_squarenorm_eoprec_device(in, doubleBuffer);
  doubleBuffer->dump(&kernelResult[0]);
}