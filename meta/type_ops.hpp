/** @file
 * Operators und utility functions for the custom types
 *
 * Copyright (c) 2012,2013 Matthias Bach
 * Copyright (c) 2013,2018 Alessandro Sciarra
 * Copyright (c) 2014 Christopher Pinke
 * Copyright (c) 2016 Max Theilig
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
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with CL2QCD. If not, see <http://www.gnu.org/licenses/>.
 */

#ifndef _META_TYPE_OPS_
#define _META_TYPE_OPS_

#include "../common_header_files/operations_complex.hpp"
#include "../common_header_files/types.hpp"
#include "../common_header_files/types_fermions.hpp"

#include <ostream>

inline hmc_complex operator+(const hmc_complex& left, const hmc_complex& right)
{
    return complexadd(left, right);
}
inline hmc_complex operator+=(hmc_complex& left, const hmc_complex& right)
{
    left = complexadd(left, right);
    return left;
}
inline bool operator==(const hmc_complex& left, const hmc_complex& right)
{
    return (left.re == right.re && left.im == right.im);
}
inline bool operator!=(const hmc_complex& left, const hmc_complex& right)
{
    return (left.re != right.re || left.im != right.im);
}
inline std::ostream& operator<<(std::ostream& os, const hmc_complex& data)
{
    return os << '(' << data.re << ',' << data.im << "i)";
}

inline bool operator==(const Matrixsu3& left, const Matrixsu3& right)
{
    return (left.e00 == right.e00 && left.e01 == right.e01 && left.e02 == right.e02 && left.e10 == right.e10 &&
            left.e11 == right.e11 && left.e12 == right.e12 && left.e20 == right.e20 && left.e21 == right.e21 &&
            left.e22 == right.e22);
}
inline bool operator!=(const Matrixsu3& left, const Matrixsu3& right)
{
    return (left.e00 != right.e00 || left.e01 != right.e01 || left.e02 != right.e02 || left.e10 != right.e10 ||
            left.e11 != right.e11 || left.e12 != right.e12 || left.e20 != right.e20 || left.e21 != right.e21 ||
            left.e22 != right.e22);
}
inline std::ostream& operator<<(std::ostream& os, const Matrixsu3& data)
{
    return os << '{' << data.e00 << ',' << data.e01 << ',' << data.e02 << ';' << data.e10 << ',' << data.e11 << ','
              << data.e12 << ';' << data.e20 << ',' << data.e21 << ',' << data.e22 << '}';
}

inline bool operator==(const Matrix6x6& left, const Matrix6x6& right)
{
    return (left.e00 == right.e00 && left.e01 == right.e01 && left.e02 == right.e02 && left.e03 == right.e03 &&
            left.e04 == right.e04 && left.e05 == right.e05 && left.e10 == right.e10 && left.e11 == right.e11 &&
            left.e12 == right.e12 && left.e13 == right.e13 && left.e14 == right.e14 && left.e15 == right.e15 &&
            left.e20 == right.e20 && left.e21 == right.e21 && left.e22 == right.e22 && left.e23 == right.e23 &&
            left.e24 == right.e24 && left.e25 == right.e25 && left.e30 == right.e30 && left.e31 == right.e31 &&
            left.e32 == right.e32 && left.e33 == right.e33 && left.e34 == right.e34 && left.e35 == right.e35 &&
            left.e40 == right.e40 && left.e41 == right.e41 && left.e42 == right.e42 && left.e43 == right.e43 &&
            left.e44 == right.e44 && left.e45 == right.e45 && left.e50 == right.e50 && left.e51 == right.e51 &&
            left.e52 == right.e52 && left.e53 == right.e53 && left.e54 == right.e54 && left.e55 == right.e55);
}
inline bool operator!=(const Matrix6x6& left, const Matrix6x6& right)
{
    return (left.e00 != right.e00 || left.e01 != right.e01 || left.e02 != right.e02 || left.e03 != right.e03 ||
            left.e04 != right.e04 || left.e05 != right.e05 || left.e10 != right.e10 || left.e11 != right.e11 ||
            left.e12 != right.e12 || left.e13 != right.e13 || left.e14 != right.e14 || left.e15 != right.e15 ||
            left.e20 != right.e20 || left.e21 != right.e21 || left.e22 != right.e22 || left.e23 != right.e23 ||
            left.e24 != right.e24 || left.e25 != right.e25 || left.e30 != right.e30 || left.e31 != right.e31 ||
            left.e32 != right.e32 || left.e33 != right.e33 || left.e34 != right.e34 || left.e35 != right.e35 ||
            left.e40 != right.e40 || left.e41 != right.e41 || left.e42 != right.e42 || left.e43 != right.e43 ||
            left.e44 != right.e44 || left.e45 != right.e45 || left.e50 != right.e50 || left.e51 != right.e51 ||
            left.e52 != right.e52 || left.e53 != right.e53 || left.e54 != right.e54 || left.e55 != right.e55);
}
inline std::ostream& operator<<(std::ostream& os, const Matrix6x6& data)
{
    return os << '{' << data.e00 << ',' << data.e01 << ',' << data.e02 << ';' << data.e03 << ',' << data.e04 << ','
              << data.e05 << ';' << data.e10 << ',' << data.e11 << ',' << data.e12 << ';' << data.e13 << ',' << data.e14
              << ',' << data.e15 << ';' << data.e20 << ',' << data.e21 << ',' << data.e22 << ';' << data.e23 << ','
              << data.e24 << ',' << data.e25 << ';' << data.e30 << ',' << data.e31 << ',' << data.e32 << ';' << data.e33
              << ',' << data.e34 << ',' << data.e35 << ';' << data.e40 << ',' << data.e41 << ',' << data.e42 << ';'
              << data.e43 << ',' << data.e44 << ',' << data.e45 << ';' << data.e50 << ',' << data.e51 << ',' << data.e52
              << ';' << data.e53 << ',' << data.e54 << ',' << data.e55 << '}';
}

inline bool operator==(const su3vec& left, const su3vec& right)
{
    return (left.e0 == right.e0 && left.e1 == right.e1 && left.e2 == right.e2);
}
inline bool operator!=(const su3vec& left, const su3vec& right)
{
    return (left.e0 != right.e0 || left.e1 != right.e1 || left.e2 != right.e2);
}
inline std::ostream& operator<<(std::ostream& os, const su3vec& data)
{
    return os << '{' << data.e0 << ',' << data.e1 << ',' << data.e2 << '}';
}

inline bool operator==(const spinor& left, const spinor& right)
{
    return (left.e0 == right.e0 && left.e1 == right.e1 && left.e2 == right.e2 && left.e3 == right.e3);
}
inline bool operator!=(const spinor& left, const spinor& right)
{
    return (left.e0 != right.e0 || left.e1 != right.e1 || left.e2 != right.e2 || left.e3 != right.e3);
}
inline std::ostream& operator<<(std::ostream& os, const spinor& data)
{
    return os << '{' << data.e0 << ',' << data.e1 << ',' << data.e2 << ',' << data.e3 << '}';
}

inline bool operator==(const ae& left, const ae& right)
{
    return (left.e0 == right.e0 && left.e1 == right.e1 && left.e2 == right.e2 && left.e3 == right.e3 &&
            left.e4 == right.e4 && left.e5 == right.e5 && left.e6 == right.e6 && left.e7 == right.e7);
}
inline bool operator!=(const ae& left, const ae& right)
{
    return (left.e0 != right.e0 || left.e1 != right.e1 || left.e2 != right.e2 || left.e3 != right.e3 ||
            left.e4 != right.e4 || left.e5 != right.e5 || left.e6 != right.e6 || left.e7 != right.e7);
}
inline std::ostream& operator<<(std::ostream& os, const ae& data)
{
    return os << '{' << data.e0 << ',' << data.e1 << ',' << data.e2 << ',' << data.e3 << ',' << data.e4 << ','
              << data.e5 << ',' << data.e7 << '}';
}

template<typename T>
inline void fill(T* array, size_t num_elems, int seed = 0)
{
    for (size_t i = 0; i < num_elems; i++) {
        array[i] = i + seed;
    }
}
template<>
void fill(hmc_complex* array, size_t num_elems, int seed);
template<>
void fill(Matrixsu3* array, size_t num_elems, int seed);
template<>
void fill(Matrix6x6* array, size_t num_elems, int seed);
template<>
void fill(spinor* array, size_t num_elems, int seed);
template<>
void fill(su3vec* array, size_t num_elems, int seed);
template<>
void fill(ae* array, size_t num_elems, int seed);

/*
 * OP counts for complex operations
 */
template<typename S, S (*T)(S)>
size_t get_flops();
template<typename S, S (*T)(S, S)>
size_t get_flops();
template<>
size_t get_flops<hmc_complex, complexconj>();
template<>
size_t get_flops<hmc_complex, complexmult>();
template<>
size_t get_flops<hmc_complex, complexadd>();
template<>
size_t get_flops<hmc_complex, complexsubtract>();
template<>
size_t get_flops<hmc_complex, complexdivide>();
template<typename S, S (*T)(S)>
size_t get_read_write_size();
template<typename S, S (*T)(S, S)>
size_t get_read_write_size();
template<>
size_t get_read_write_size<hmc_complex, complexconj>();
template<>
size_t get_read_write_size<hmc_complex, complexmult>();
template<>
size_t get_read_write_size<hmc_complex, complexadd>();
template<>
size_t get_read_write_size<hmc_complex, complexsubtract>();
template<>
size_t get_read_write_size<hmc_complex, complexdivide>();

#endif /* _META_TYPE_OPS */
