//						       		-*- C++ -*-
// Copyright (C) 2010 EPITA Research and Development Laboratory (LRDE)
//
// This file is part of Olena.
//
// Olena is free software: you can redistribute it and/or modify it under
// the terms of the GNU General Public License as published by the Free
// Software Foundation, version 2 of the License.
//
// Olena is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
// General Public License for more details.
//
// You should have received a copy of the GNU General Public License
// along with Olena.  If not, see <http://www.gnu.org/licenses/>.
//
// As a special exception, you may use this file as part of a free
// software project without restriction.  Specifically, if other files
// instantiate templates or use macros or inline functions from this
// file, or you compile this file and link it with other files to produce
// an executable, this file does not by itself cause the resulting
// executable to be covered by the GNU General Public License.  This
// exception does not however invalidate any other reasons why the
// executable file might be covered by the GNU General Public License.

/// \file complex2.i
/// \brief A wrapper of mln::topo::complex<2>.

%module complex2

%include "complex.ixx"

instantiate_n_faces_set_binary_operators(+, add, 0, 2)
instantiate_n_faces_set_binary_operators(+, add, 1, 2)

instantiate_n_faces_set_binary_operators(-, sub, 0, 2)
instantiate_n_faces_set_binary_operators(-, sub, 1, 2)

instantiate_algebraic_n_face_unary_operators(0, 2)
instantiate_algebraic_n_face_unary_operators(1, 2)


/*-------.
| Face.  |
`-------*/

%template(face_2) mln::topo::face<2>;

instantiate_face_data_conversions(0, 2)
instantiate_face_data_conversions(1, 2)
instantiate_face_data_conversions(2, 2)

instantiate_boolean_binary_operator(==, eq, mln::topo::face<2>)

// Conversion helper for Python.
#if SWIGPYTHON
generate__str__(mln::topo::face< 2 >)
#endif // !SWIGPYTHON

/*---------.
| N-face.  |
`---------*/

instantiate_n_face(0, 2)
instantiate_n_face(1, 2)
instantiate_n_face(2, 2)

/*-----------------.
| Algebraic face.  |
 `----------------*/

%template(algebraic_face_2) mln::topo::algebraic_face<2>;

/*-------------------.
| Algebraic n-face.  |
 `-------------------*/

instantiate_algebraic_n_face(0, 2)
instantiate_algebraic_n_face(1, 2)
instantiate_algebraic_n_face(2, 2)

/*--------------.
| N-faces set.  |
`--------------*/

instantiate_n_faces_set(0, 2)
instantiate_n_faces_set(1, 2)

/*------------.
| Face data.  |
`------------*/

instantiate_face_data(0, 2)
instantiate_face_data(1, 2)
instantiate_face_data(2, 2)

/*----------------.
| Face iterator.  |
`----------------*/

instantiate_face_fwd_iter(face_iter_2, 2)

/*----------.
| Complex.  |
`----------*/

instantiate_complex_add_face(0, 1, 2)
instantiate_complex_add_face(1, 2, 2)

instantiate_complex(complex2, 2)
