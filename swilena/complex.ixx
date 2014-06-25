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

/// \file complex.ixx
/// \brief A wrapper of mln::topo::complex<D>.

%module complex

%include "concat.ixx"
%include "python-utils.ixx";

%{
#include "mln/topo/n_face.hh"
#include "mln/topo/n_faces_set.hh"
#include "mln/topo/algebraic_n_face.hh"
#include "mln/topo/complex.hh"
%}

%include "mln/topo/complex.hh";


/*-------------------------------.
| Operators wrapping shortcuts.  |
`-------------------------------*/

%define instantiate_boolean_binary_operator(Op, Name, T)
%inline
{
  bool
  Name (const T & f1, const T & f2) { return f1 Op f2; }
}

#if SWIGPYTHON
%extend T
{
  bool
  __ ## Name ## __ (const T & rhs) { return *$self Op rhs; }
}
#endif // !SWIGPYTHON
%enddef // !instantiate_boolean_binary_operator


/* It seems we cannot use %template nor %rename to wrap (non member)
   template operators.  Wrap them by hand using %inline.  */

%define instantiate_n_faces_set_binary_operator(Op, Name, N, D, LType, RType)
%inline
{
  mln::topo::n_faces_set< N, D >
  Name (const LType< N, D >& f1, const RType< N, D >& f2) { return f1 Op f2; }
}

#if SWIGPYTHON
%extend LType< N, D >
{
  mln::topo::n_faces_set< N, D >
    __ ## Name ## __ (const RType< N, D >& rhs) { return *$self Op rhs; }
}
#endif // !SWIGPYTHON
%enddef // !instantiate_n_faces_set_binary_operator

%define instantiate_n_faces_set_binary_operators(Op, Name, N, D)
instantiate_n_faces_set_binary_operator(Op, Name, N, D,
					mln::topo::algebraic_n_face,
					mln::topo::algebraic_n_face)
instantiate_n_faces_set_binary_operator(Op, Name, N, D,
					mln::topo::algebraic_n_face,
					mln::topo::n_face)
instantiate_n_faces_set_binary_operator(Op, Name, N, D,
					mln::topo::n_face,
					mln::topo::algebraic_n_face)
instantiate_n_faces_set_binary_operator(Op, Name, N, D,
					mln::topo::n_face,
					mln::topo::n_face)

instantiate_n_faces_set_binary_operator(Op, Name, N, D,
					mln::topo::n_faces_set,
					mln::topo::algebraic_n_face)
instantiate_n_faces_set_binary_operator(Op, Name, N, D,
					mln::topo::n_faces_set,
					mln::topo::n_face)
%enddef // !instantiate_n_faces_set_binary_operators

%define instantiate_algebraic_n_face_unary_operators(N, D)
%inline
{
  mln::topo::algebraic_n_face< N, D >
  neg (const mln::topo::n_face< N, D >& f) { return -f; }

  mln::topo::algebraic_n_face< N, D >
  neg (const mln::topo::algebraic_n_face< N, D >& f) { return -f; }
}

#if SWIGPYTHON
%extend mln::topo::algebraic_n_face < N, D >
{
  mln::topo::algebraic_n_face< N, D > __neg__ () { return -*$self; }
}

%extend mln::topo::n_face < N, D >
{
  mln::topo::algebraic_n_face< N, D > __neg__ () { return -*$self; }
}
#endif // !SWIGPYTHON
%enddef // !instantiate_algebraic_n_face_unary_operators


/*-------.
| Face.  |
`-------*/

%include "mln/topo/face.hh";

// Template members (conversions).
%define instantiate_face_data_conversions(N, D)
%extend mln::topo::face< D >
{
  // Construction from n_face<N, D>.
  face< D > (const mln::topo::n_face<N, D>& f)
  {
    return new mln::topo::face< D >(f);
  }

  // Conversion to face_data<N>.
  %template(data_ ## N) data< N >;
}
%enddef // !instantiate_face_data_conversions


/*---------.
| N-face.  |
`---------*/

%include "mln/topo/n_face.hh";

%define instantiate_n_face(N, D)
  %ignore mln::topo::n_face<N, D>::lower_dim_adj_faces;
  %ignore mln::topo::n_face<N, D>::higher_dim_adj_faces;
  %template(n_face_ ## N ## _ ## D) mln::topo::n_face<N, D>;
%enddef // !instantiate_n_face


/*-----------------.
| Algebraic face.  |
 `----------------*/

%include "mln/topo/algebraic_face.hh";


/*-------------------.
| Algebraic n-face.  |
 `-------------------*/

%include "mln/topo/algebraic_n_face.hh";

%define instantiate_algebraic_n_face(N, D)
  %ignore mln::topo::algebraic_n_face<N, D>::lower_dim_adj_faces;
  %ignore mln::topo::algebraic_n_face<N, D>::higher_dim_adj_faces;
  %template(algebraic_n_face_ ## N ## _ ## D) mln::topo::algebraic_n_face<N, D>;
%enddef // !instantiate_algebraic_n_face


/*--------------.
| N-faces set.  |
`--------------*/

%include "mln/topo/n_faces_set.hh";

%define instantiate_n_faces_set(N, D)
  %template(n_faces_set_ ## N ## _ ## D) mln::topo::n_faces_set<N, D>;
%enddef // !instantiate_n_faces_set


/*------------.
| Face data.  |
`------------*/

%include "mln/topo/face_data.hh";

%define instantiate_face_data(N, D)
  %template(face_data_ ## N ## _ ## D) mln::topo::face_data<N, D>;
%enddef // !instantiate_face_data


/*----------------.
| Face Iterator.  |
`----------------*/

%include "mln/core/concept/iterator.hh";
%include "mln/topo/internal/complex_iterator_base.hh";
%include "mln/topo/internal/complex_set_iterator_base.hh";
%include "mln/topo/face_iter.hh";

%{
#include "mln/topo/face_iter.hh"
%}

// Generate base classes.
%define instantiate_Iterator(ExactName, E)
  %template(Iterator_ ## ExactName) mln::Iterator< E >;
%enddef // !instantiate_Iterator

%define instantiate_complex_iterator_base(ExactName, F, E)
  instantiate_Iterator(ExactName, E)
  %template(complex_iterator_base_ ## ExactName)
    mln::topo::internal::complex_iterator_base< F, E >;
%enddef // !instantiate_complex_iterator_base

%define instantiate_complex_set_iterator_base(ExactName, F, E)
  instantiate_complex_iterator_base(ExactName, F, E)
  %template(complex_set_iterator_base_ ## ExactName)
    mln::topo::internal::complex_set_iterator_base< F, E >;
%enddef // !instantiate_complex_set_iterator_base

// Generate mln::topo::face_fwd_iter< D >.
%define instantiate_face_fwd_iter(Name, D)
instantiate_complex_set_iterator_base(Name,
				      mln::topo::face< D >,
				      mln::topo::face_fwd_iter< D >)

#if SWIGPYTHON
// Handling iterators a la Python.
%extend mln::topo::face_fwd_iter< D >
{
  mln::topo::face_fwd_iter< D >&
  __iter__()
  {
    return *$self;
  }
}

// Raise a Python `StopIteration' exception on `next()' if the
// iterator is invalid.
%exception mln::topo::face_fwd_iter< D >::next
{
  /* FIXME: Is it safe to use `arg1'?  It seems to be pretty
     low-level, and may not be part of Swig's standard interface.  */
  if (!arg1->is_valid())
    {
      PyErr_SetString(PyExc_StopIteration, "Invalid iterator");
      return NULL;
    }
  $action
}

%extend mln::topo::face_fwd_iter< D >
{
  const mln::topo::face< D >
  next()
  {
    // Keep a copy of the current site before incrementation.
    const mln::topo::face< D > current = *$self;
    // Delegate incrementation to `Super_Iterator's `next()' method.
    $self->next();
    return current;
  }
}
#endif // !SWIGPYTHON

%template(Name) mln::topo::face_fwd_iter< D >;
%enddef // !instantiate_face_fwd_iter

// Conversion helper for Python.
#if SWIGPYTHON
generate__str__(mln::topo::face_fwd_iter)
#endif // !SWIGPYTHON


/*----------.
| Complex.  |
`----------*/

// Generate an instantiation of mln::topo::complex<D>.
%define instantiate_complex(Name, D)

#if SWIGPYTHON
// Conversion helper for Python.
generate__str__(mln::topo::complex<D>)

// Python iterator creation.
%extend mln::topo::complex< D >
{
  mln::topo::face_fwd_iter< D >
  __iter__() /* FIXME: No `const' here, to be compatible with
		face_fwd_iter's ctor.  This is bad.  */
  {
    mln::topo::face_fwd_iter< D > f(*$self);
    f.start();
    return f;
  }
}
#endif // !SWIGPYTHON

%template(Name) mln::topo::complex<D>;
%enddef // !instantiate_complex

// Generate mln::topo::complex's template members.
%define instantiate_complex_add_face(N, N_Plus_One, D) 
  %extend mln::topo::complex
  {
    mln::topo::n_face< N_Plus_One, D >
    add_face(const mln::topo::n_faces_set< N, D >& adjacent_faces)
    {
      return $self->add_face(adjacent_faces);
    }
  };
%enddef // !instantiate_complex_add_face
