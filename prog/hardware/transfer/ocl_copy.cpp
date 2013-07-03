/** @file
 * Implementation of the simple opencl transfer method
 *
 * (c) 2013 Matthias Bach <bach@compeng.uni-frankfurt.de>
 */

#include "ocl_copy.hpp"

#include <stdexcept>
#include "../../exceptions.h"

namespace {
	size_t get_required_buffer_size(const size_t * region);
}

hardware::transfer::OclCopy::OclCopy(hardware::Device * const from, hardware::Device * const to)
	: Transfer(from, to), transfer_buffer(nullptr), load_event(), dump_event()
{
	// nothing to do
}

hardware::transfer::OclCopy::~OclCopy()
{
	if(transfer_buffer) {
		delete transfer_buffer;
	}
}

hardware::SynchronizationEvent hardware::transfer::OclCopy::load(const hardware::buffers::Buffer* orig, const size_t *src_origin, const size_t *region, size_t src_row_pitch, size_t src_slice_pitch, const hardware::SynchronizationEvent& event)
{
	ensure_proper_transfer_buffer_size(region);

	// the transfer may neither overlap with a dump or a load, as both use the transfer buffer
	const size_t transfer_buffer_origin[] = { 0, 0, 0 };
	load_event = copyDataRect(get_src_device(), transfer_buffer, orig, transfer_buffer_origin, src_origin, region, 0, 0, src_row_pitch, src_slice_pitch, {load_event, dump_event, event});

	return load_event;
}

hardware::SynchronizationEvent hardware::transfer::OclCopy::transfer()
{
	// nothing to do
	// if the load is complet "transfer" is also complete (transfer complete means dump can start)
	return load_event;
}

hardware::SynchronizationEvent hardware::transfer::OclCopy::dump(const hardware::buffers::Buffer* dest, const size_t *dest_origin, const size_t *region, size_t dest_row_pitch, size_t dest_slice_pitch, const hardware::SynchronizationEvent& event)
{
	if(!transfer_buffer) {
		throw std::logic_error("Requested dumping data from transfer, but not data has been loaded yet");
	}

	// the transfer may neither overlap with a dump or a load, as both use the transfer buffer
	const size_t transfer_buffer_origin[] = { 0, 0, 0 };
	dump_event = copyDataRect(get_dest_device(), dest, transfer_buffer, dest_origin, transfer_buffer_origin, region, dest_row_pitch, dest_slice_pitch, 0, 0, {load_event, dump_event, event});

	return dump_event;
}

void hardware::transfer::OclCopy::ensure_proper_transfer_buffer_size(const size_t * const region)
{
	auto const required_buffer_size = get_required_buffer_size(region);
	if(!transfer_buffer || required_buffer_size > transfer_buffer->get_bytes()) {
		// TODO too large buffers might also be bad for transfer speed ...
		if(transfer_buffer) {
			delete transfer_buffer;
		}
		// the device for the buffer is only a way to get to the context, it will rebind and we won't use implicit queues
		transfer_buffer = new hardware::buffers::Buffer(required_buffer_size, get_src_device(), false);
	}
}

namespace {
	size_t get_required_buffer_size(const size_t * const region)
	{
		return region[0] * region[1] * region[2];
	}
}
