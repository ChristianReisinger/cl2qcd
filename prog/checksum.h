/** @file
 * Dekleration of the checksum class
 */

#ifndef _CHECKSUM_H_
#define _CHECKSUM_H_

class Checksum {

public:
	/**
	 * Create class given precalculated checksum values
	 */
	Checksum(uint32_t suma, uint32_t sumb) noexcept : suma(suma), sumb(sumb) { };

	/**
	 * Create an empty checksum (checksum of 0 bytes)
	 */
	Checksum() noexcept : suma(0), sumb(0) { };

	inline uint32_t get_suma() const noexcept {
		return suma;
	}

	inline uint32_t get_sumb() const noexcept {
		return sumb;
	}

private:
	uint32_t suma;
	uint32_t sumb;
};

inline std::ostream& operator<<(std::ostream& out, const Checksum& sum) noexcept {
	out << "SumA: " << std::hex << sum.get_suma() << ", SumB: " << std::hex << sum.get_sumb();
}

#endif /* _CHECKSUM_H_ */
