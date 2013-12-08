/** @file
 * PRNG PRNG unit declaration
 */

#ifndef _PHYSICS_PRNG_
#define _PHYSICS_PRNG_

#include "../hardware/system.hpp"
#include "../hardware/buffers/prng_buffer.hpp"
#include "../meta/util.hpp"

/**
 * This package contains the actual "business" logic of the library.
 */
namespace physics {

	/**
	 * The PRNG PRNG for host and device.
	 *
	 * WARNING: Must only be used as a singleton!
	 */
	class PRNG {
		public:
			/**
			 * Initialize the ranlux instance
			 */
			PRNG(const hardware::System& system);

			~PRNG();

			/*
			 * non-copyable
			 */
			PRNG& operator=(const PRNG&) = delete;
			PRNG(const PRNG&) = delete;
			PRNG() = delete;

			/**
			 * Get random numbers
			 */
			double get_double() noexcept;

			/**
			 * Get the buffers containing the random state on the device.
			 */
			const std::vector<const hardware::buffers::PRNGBuffer*> get_buffers() const noexcept;

			/**
			 * Store the current PRNG state to a file of the given name.
			 *
			 * \param filename The name of the file to store the random state in
			 */
			void store(const std::string filename) const;

			/**
			 * Save current PRNG state to a file with name prng_prefix + number + prng_postfix
			 * @param[in] number The trajectory number to be stored in the file
			 */
			void save(int number);
			/**
			 * Save current PRNG state to a file with specific name based on
			 * current trajectory number.
			 * @param[in] number The trajectory number to be stored in the file
			 */
			void saveToSpecificFile(int number);

		private:
			/**
			 * Reference to the PRNG Buffers used on each device
			 */
			std::vector<const hardware::buffers::PRNGBuffer*> buffers;

			/**
			 * Reference to the system this PRNG is for.
			 */
			const hardware::System& system;
	};

	void gaussianComplexVector(hmc_complex * vector, int length, hmc_float sigma, physics::PRNG& prng);
	void gaussianNormalPair(hmc_float * z1, hmc_float * z2, physics::PRNG& prng);
	Matrixsu3 random_matrixsu3(physics::PRNG& prng);
}

#endif /* _PHYSICS_PRNG_ */
