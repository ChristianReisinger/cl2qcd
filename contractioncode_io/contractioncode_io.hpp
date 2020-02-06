/** @file
 * IO utilities for ContractionCode package used at ITP University Frankfurt
 * Author 2019 Christian Reisinger
 */

#include "../common_header_files/types.hpp"
#include "../physics/lattices/latticesInterfaces.hpp"

namespace contractioncode_io {

Matrixsu3* readGaugefieldFromArray(const double* arr,
		const physics::lattices::GaugefieldParametersInterface* parameters);

void writeGaugefieldToArray(double* arr, const Matrixsu3* host_buf,
		const physics::lattices::GaugefieldParametersInterface* parameters);

}
