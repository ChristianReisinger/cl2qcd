/** @file
 * Declaration of the hardware::buffers::Matrix3x3 class
 *
 * (c) 2012 Matthias Bach <bach@compeng.uni-frankfurt.de>
 */

#ifndef _HARDWARE_BUFFERS_3X3_
#define _HARDWARE_BUFFERS_3X3_

#include "buffer.hpp"
#include "../../types.h"

namespace hardware {
	namespace buffers {

		/**
		 * Check whether Matrix3x3 should be stored SOA style on this device
		 */
		size_t check_Matrix3x3_for_SOA(hardware::Device * device);

		/**
		 * Get the stride for an Matrix3x3 buffer of the given number of elements on the given device
		 */
		size_t get_Matrix3x3_buffer_stride(size_t elems, Device * device);

		/*
		 * A buffer storing Matrix3x3s on the device
		 */
		class Matrix3x3 : public Buffer {

		public:
			/**
			 * Allocate a buffer with the default number of
			 * elemets for this device.
			 *
			 * \param elems The size of the buffer in elements
			 * \param device The device to locate the buffer on
			 */
			Matrix3x3(size_t elems, Device * device);

			/*
			 * Matrix3x3 buffers cannot be copied
			 */
			Matrix3x3& operator=(const Matrix3x3&) = delete;
			Matrix3x3(const Matrix3x3&) = delete;
			Matrix3x3() = delete;

			/**
			 * Get the size of the buffer in elements
			 */
			size_t get_elements() const noexcept;

			/**
			 * Check whether this Buffer uses soa layout
			 */
			bool is_soa() const noexcept;

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

#endif /* _HARDWARE_BUFFERS_3X3_ */
