# Build specifications for the ContractionCode I/O functionality
#
# Author 2019 Christian Reisinger


find_package(OpenMP REQUIRED)

add_library(contractioncode_io
    contractioncode_io.cpp
)

target_link_libraries(contractioncode_io
    geometry
    OpenMP::OpenMP_CXX
)
