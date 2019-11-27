/** @file
 * IO utilities for ContractionCode package used at ITP University Frankfurt
 * Author 2019 Christian Reisinger
 */

#include "../contractioncode_io/contractioncode_io.hpp"

#include "../geometry/latticeExtents.hpp"
#include "../geometry/index.hpp"

//direction l: 0,1,2,3 = x,y,z,t

Matrixsu3* contractioncode_io::readGaugefieldFromArray(const double* arr,
		const physics::lattices::GaugefieldParametersInterface* parameters)
		{
	const unsigned T = parameters->getNt();
	const unsigned L = parameters->getNs();

	Matrixsu3* gf_host = new Matrixsu3[parameters->getNumberOfElements()];
	for (size_t t = 0; t < T; t++)
		for (size_t x = 0; x < L; x++)
			for (size_t y = 0; y < L; y++)
				for (size_t z = 0; z < L; z++) {
					const Index site_index(z, y, x, t, LatticeExtents(L, T));

					for (int l = 0; l < NDIM; l++) {
						LinkIndex link_index(site_index, static_cast<Direction>((l + 1) % NDIM));

						hmc_complex tmp[NC][NC];
						for (int i = 0; i < NC; i++)
							for (int j = 0; j < NC; j++) {
								const uint elem_index = link_index.get_su3_idx_ildg_format(j, i);
								tmp[i][j].re = *(arr + elem_index);
								tmp[i][j].im = *(arr + elem_index + 1);
							}

						Matrixsu3 destElem;
						destElem.e00 = tmp[0][0];
						destElem.e01 = tmp[0][1];
						destElem.e02 = tmp[0][2];
						destElem.e10 = tmp[1][0];
						destElem.e11 = tmp[1][1];
						destElem.e12 = tmp[1][2];
						destElem.e20 = tmp[2][0];
						destElem.e21 = tmp[2][1];
						destElem.e22 = tmp[2][2];

						gf_host[uint(LinkIndex(site_index, static_cast<Direction>((l + 1) % NDIM)))] = destElem;
					}
				}
	return gf_host;
}

void contractioncode_io::writeGaugefieldToArray(double* arr, const Matrixsu3* host_buf,
		const physics::lattices::GaugefieldParametersInterface* parameters)
		{
	const unsigned T = parameters->getNt();
	const unsigned L = parameters->getNs();

	for (size_t t = 0; t < T; t++)
		for (size_t x = 0; x < L; x++)
			for (size_t y = 0; y < L; y++)
				for (size_t z = 0; z < L; z++) {
					const Index site_index_CL2QCD(x, y, z, t, LatticeExtents(L, T));

					for (int l = 0; l < NDIM; l++) {
						Matrixsu3 srcElem = host_buf[uint(LinkIndex(site_index_CL2QCD, static_cast<Direction>(l)))];

						hmc_complex destElem[NC][NC];
						destElem[0][0] = srcElem.e00;
						destElem[0][1] = srcElem.e01;
						destElem[0][2] = srcElem.e02;
						destElem[1][0] = srcElem.e10;
						destElem[1][1] = srcElem.e11;
						destElem[1][2] = srcElem.e12;
						destElem[2][0] = srcElem.e20;
						destElem[2][1] = srcElem.e21;
						destElem[2][2] = srcElem.e22;

						const Index site_index_ContractionCode(z, y, x, t, LatticeExtents(L, T));
						LinkIndex link_index(site_index_ContractionCode, static_cast<Direction>(l));
						for (int i = 0; i < NC; i++)
							for (int j = 0; j < NC; j++) {
								const uint elem_index = link_index.get_su3_idx_ildg_format(j, i);
								arr[elem_index] = destElem[i][j].re;
								arr[elem_index + 1] = destElem[i][j].im;
							}
					}
				}
}
