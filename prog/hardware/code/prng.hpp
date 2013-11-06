/** @file
 * Heatbath for OpenCL
 */
#ifndef _HARDWARE_CODE_PRNG_
#define _HARDWARE_CODE_PRNG_

#include "opencl_module.hpp"

#include "../buffers/prng_buffer.hpp"

namespace hardware {

namespace code {

/**
 * An OpenCL device
 *
 * Adds random numbers to basic Opencl_Module class
 *
 * @todo Everything is public to faciliate inheritance. Actually, more parts should be private.
 */
class PRNG : public Opencl_Module {
public:
	friend hardware::Device;

	virtual ~PRNG();

	ClSourcePackage get_sources() const noexcept;

#ifdef USE_PRNG_RANLUX
	/**
	 * Initialize the state of the PRNG with the given seed.
	 */
	void initialize(const hardware::buffers::PRNGBuffer * buffer, cl_uint seed) const;
#endif /* USE_PRNG_RANLUX */

protected:
	/**
	 * Return amount of Floating point operations performed by a specific kernel per call.
	 * NOTE: this is meant to be the "netto" amount in order to be comparable.
	 *
	 * @param in Name of the kernel under consideration.
	 */
	virtual uint64_t get_flop_size(const std::string&) const {
		return 0;
	};

	/**
	 * Return amount of bytes read and written by a specific kernel per call.
	 *
	 * @param in Name of the kernel under consideration.
	 */
	virtual size_t get_read_write_size(const std::string&) const {
		return 0;
	};

private:

	/**
	 * @param[in] params points to an instance of inputparameters
	 */
	PRNG(const meta::Inputparameters& params, hardware::Device * device);

	/**
	 * A set of sources required to use the PRNG.
	 */
	ClSourcePackage prng_code;

#ifdef USE_PRNG_RANLUX
	cl_kernel init_kernel;
#endif // USE_PRNG_???
};

}

}

#endif // _HARDWARE_CODE_PRNG_
