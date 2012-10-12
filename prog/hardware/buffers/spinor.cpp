/** @file
 * Implementation of the hardware::buffers::Spinor class
 *
 * (c) 2012 Matthias Bach <bach@compeng.uni-frankfurt.de>
 */

#include "spinor.hpp"

#include <stdexcept>

typedef hmc_complex soa_storage_t;
const size_t soa_storage_lanes = 12;

static size_t calculate_spinor_buffer_size(size_t elems, hardware::Device * device);

hardware::buffers::Spinor::Spinor(size_t elems, hardware::Device * device)
	: Buffer(calculate_spinor_buffer_size(elems, device), device),
	  elems(elems),
	  soa(check_Spinor_for_SOA(device))
{
	// nothing to do
}

size_t hardware::buffers::check_Spinor_for_SOA(hardware::Device * device)
{
	return device->get_device_type() == CL_DEVICE_TYPE_GPU;
}

static size_t calculate_spinor_buffer_size(size_t elems, hardware::Device * device)
{
	using namespace hardware::buffers;
	if(check_Spinor_for_SOA(device)) {
		size_t stride = get_Spinor_buffer_stride(elems, device);
		return stride * soa_storage_lanes * sizeof(soa_storage_t);
	} else {
		return elems * sizeof(spinor);
	}
}

size_t hardware::buffers::get_Spinor_buffer_stride(size_t elems, Device * device)
{
	return device->recommend_stride(elems, sizeof(soa_storage_t), soa_storage_lanes);
}

size_t hardware::buffers::Spinor::get_elements() const noexcept
{
	return elems;
}

bool hardware::buffers::Spinor::is_soa() const noexcept
{
	return soa;
}

void hardware::buffers::Spinor::load(const spinor * ptr) const
{
	if(is_soa()) {
		throw std::logic_error("Data cannot be loaded into SOA buffers.");
	} else {
		Buffer::load(ptr);
	}
}

void hardware::buffers::Spinor::dump(spinor * ptr) const
{
	if(is_soa()) {
		throw std::logic_error("Data cannot be dumped from SOA buffers.");
	} else {
		Buffer::dump(ptr);
	}
}
