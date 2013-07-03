/** @file
 * Implementation of the transfer method factory
 *
 * (c) 2013 Matthias Bach <bach@compeng.uni-frankfurt.de>
 */

#include "transfer.hpp"
#include "ocl_copy.hpp"

std::unique_ptr<hardware::Transfer> hardware::create_transfer(hardware::Device * const src, hardware::Device * const dest)
{
	return std::unique_ptr<hardware::Transfer>(new transfer::OclCopy(src, dest));
}
