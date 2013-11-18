/** @file
 * Declaration of the hardware::buffers::Spinor class
 *
 * (c) 2012 Matthias Bach <bach@compeng.uni-frankfurt.de>
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

#ifndef _HARDWARE_BUFFERS_SPINOR_
#define _HARDWARE_BUFFERS_SPINOR_

#include "buffer.hpp"
#include "../../types_fermions.h"

namespace hardware {
namespace buffers {

/**
 * Check whether Spinor should be stored SOA style on this device
 */
size_t check_Spinor_for_SOA(hardware::Device * device);

/**
 * Get the stride for an Spinor buffer of the given number of elements on the given device
 */
size_t get_Spinor_buffer_stride(size_t elems, Device * device);

/*
 * A buffer storing Spinors on the device
 */
class Spinor : public Buffer {

public:
	/**
	 * Allocate a buffer with the default number of
	 * elemets for this device.
	 *
	 * \param elems The size of the buffer in elements
	 * \param device The device to locate the buffer on
	 */
	Spinor(size_t elems, Device * device);

	/*
	 * Spinor buffers cannot be copied
	 */
	Spinor& operator=(const Spinor&) = delete;
	Spinor(const Spinor&) = delete;
	Spinor() = delete;

	/**
	 * Load data from the given pointer into the buffer.
	 *
	 * Note that this requires creation of a temporary buffer in case the buffer uses a SOA layout!
	 *
	 * \param elems Allows to limit the number of elements loaded from the given pointer
	 * \param offset Allows to store the elements at the given offset into the buffer
	 */
	void load(const spinor *, size_t elems = 0, size_t offset = 0) const;

	/**
	 * Store data from the buffer into the given pointer.
	 *
	 * Note that this requires creation of a temporary buffer in case the buffer uses a SOA layout!
	 *
	 * \param elems Allows to limit the number of elements dumped to the given pointer
	 * \param offset Allows to read the elements at the given offset into the buffer
	 */
	void dump(spinor *, size_t elems = 0, size_t offset = 0) const;

	/**
	 * Load raw data from the given pointer into the buffer.
	 *
	 * This also works for SoA buffer, but use with care, as this method does not ensure correct data format.
	 *
	 * \param elems Allows to limit the number of bytes loaded from the given memory
	 * \param offset Allows to store the bytes at the given offset into the buffer
	 */
	void load_raw(const void *, size_t bytes = 0, size_t offset = 0) const;

	/**
	 * Store the raw data from the buffer into the given pointer.
	 *
	 * This also works for SoA buffer, but use with care, as this method does not ensure correct data format.
	 *
	 * \param elems Allows to limit the number of bytes dumped to the given memory
	 * \param offset Allows to read the bytes at the given offset into the buffer
	 */
	void dump_raw(void *, size_t bytes = 0, size_t offset = 0) const;

	void loadRect_raw(const void* dest, const size_t *buffer_origin, const size_t *host_origin, const size_t *region, size_t buffer_row_pitch, size_t buffer_slice_pitch, size_t host_row_pitch, size_t host_slice_pitch) const;
	void dumpRect_raw(void* dest, const size_t *buffer_origin, const size_t *host_origin, const size_t *region, size_t buffer_row_pitch, size_t buffer_slice_pitch, size_t host_row_pitch, size_t host_slice_pitch) const;
	hardware::SynchronizationEvent loadRect_rawAsync(const void* src, const size_t *buffer_origin, const size_t *host_origin, const size_t *region, size_t buffer_row_pitch, size_t buffer_slice_pitch, size_t host_row_pitch, size_t host_slice_pitch, const hardware::SynchronizationEvent& event) const;
	hardware::SynchronizationEvent dumpRect_rawAsync(void* dest, const size_t *buffer_origin, const size_t *host_origin, const size_t *region, size_t buffer_row_pitch, size_t buffer_slice_pitch, size_t host_row_pitch, size_t host_slice_pitch, const hardware::SynchronizationEvent& event) const;

	/**
	 * Get the size of the buffer in elements
	 */
	size_t get_elements() const noexcept;

	/**
	 * Check whether this Buffer uses soa layout
	 */
	bool is_soa() const noexcept;

	/**
	 * Get the size of the type used for storage.
	 */
	size_t get_storage_type_size() const noexcept;

	/**
	 * Get the stride between two lanes (in elements).
	 *
	 * 0 if not a SOA buffer
	 */
	size_t get_lane_stride() const noexcept;

	/**
	 * Get the number of lanes.
	 *
	 * 1 if not a SOA buffer
	 */
	size_t get_lane_count() const noexcept;

private:

	/**
	 * The size of the buffer in bytes.
	 */
	const size_t elems;

	/**
	 * Whether the data is stored in a soa fashion
	 */
	const bool soa;
};
}
}

#endif /* _HARDWARE_BUFFERS_SPINOR_ */
