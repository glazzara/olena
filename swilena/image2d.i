//						       		-*- C++ -*-
// Copyright (C) 2008 EPITA Research and Development Laboratory (LRDE)
//
// This file is part of the Olena Library.  This library is free
// software; you can redistribute it and/or modify it under the terms
// of the GNU General Public License version 2 as published by the
// Free Software Foundation.
//
// This library is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
// General Public License for more details.
//
// You should have received a copy of the GNU General Public License
// along with this library; see the file COPYING.  If not, write to
// the Free Software Foundation, 51 Franklin Street, Fifth Floor,
// Boston, MA 02111-1307, USA.
//
// As a special exception, you may use this file as part of a free
// software library without restriction.  Specifically, if other files
// instantiate templates or use macros or inline functions from this
// file, or you compile this file and link it with other files to
// produce an executable, this file does not by itself cause the
// resulting executable to be covered by the GNU General Public
// License.
// reasons why the executable file might be covered by the GNU General
// Public License.

/// \file image2d.i
/// \brief A simple wrapping of mln::image2d<int> along with some
/// algorithms.


%module image2d


/*------.
| int.  |
`------*/

%include "cpointer.i"
// Wrap a class interface around an "int *" (see
// // http://www.swig.org/Doc1.3/Library.html#Library_nn4).
// %pointer_class(int, intp);
/* Create some functions for working with "int *" */
%pointer_functions(int, intp);


/*---------------.
| mln::point2d.  |
`---------------*/

%{
#include "mln/core/point.hh"
#include "mln/core/point2d.hh"
%}

%include "mln/core/point.hh";
%include "mln/core/point2d.hh";
/* FIXME: Ignore `mln::point_<M,C>::origin' to circumvent a swig bug.
   Without this ignore clause, the generated code would trigger this
   error :

     image2d-wrap.cc:3115:144: error: macro "SWIG_as_voidptr" passed 2
     arguments, but takes just 1
     image2d-wrap.cc: In function 'PyObject* point2d_origin_get()':
     image2d-wrap.cc:3115: error: 'SWIG_as_voidptr' was not declared in this
     scope

   Check whether this bug has been fixed in a recent release of SWIG
   or if it has been reported.  */
%ignore mln::point_<mln::grid::square,int>::origin;
// Ignoring to_h_vec saves us the wrapping of 
%ignore mln::point_<mln::grid::square,int>::to_h_vec;
// Swig tries to wrap everything by default; prevent it from wrapping
// invalid methods (1D and 3D ctors for a point2d).
%ignore mln::point_<mln::grid::square,int>::point_(const literal::zero_t&);
%ignore mln::point_<mln::grid::square,int>::point_(const literal::one_t&);
%ignore mln::point_<mln::grid::square,int>::point_(int);
%ignore mln::point_<mln::grid::square,int>::point_(int, int, int);
// point2d.
%template(point2d) mln::point_<mln::grid::square,int>;

/*--------------------.
| mln::image2d<int>.  |
`--------------------*/

%{
#include "mln/core/image2d.hh"
%}

%include "mln/core/box2d.hh";
%include "mln/core/dpoint2d.hh";

%include "mln/core/concept/image.hh";

// FIXME: Wrap mln::image2d by hand, for Milena macros disturb swig.
// Annotate the original source code instead?
namespace mln
{
  template <typename T>
  struct image2d
  /* FIXME: This is a simple wrapping; ignore the base class (for the
     moment). */
  // : public internal::image_primary_< box2d, image2d<T> >
  /* FIXME: Cheat, so that generic algorithms can use mln::image2d. */
    :  public Image< image2d<T> >
  {
    // Warning: just to make effective types appear in Doxygen:
    typedef box2d   pset;
    typedef point2d psite;
    typedef point2d point;
    typedef dpoint2d dpoint;
// FIXME: Those typedefs disturb swig.
//     typedef mln_fwd_piter(box2d) fwd_piter;
//     typedef mln_bkd_piter(box2d) bkd_piter;
    typedef line_piter_<point> line_piter;
    // End of warning.


    /// Value associated type.
    typedef T         value;

    /// Return type of read-only access.
    typedef const T& rvalue;

    /// Return type of read-write access.
    typedef T&       lvalue;


    /// Skeleton.
    typedef image2d< tag::value_<T> > skeleton;


    /// Value_Set associated type.
    typedef mln::value::set<T> vset;


    /// Constructor without argument.
    image2d();

    /// Constructor with the numbers of rows and columns and the
    /// border thickness.
    image2d(int nrows, int ncols, unsigned bdr = border::thickness);

    /// Constructor with a box and the border thickness (default is
    /// 3).
    image2d(const box2d& b, unsigned bdr = border::thickness);


    /// Initialize an empty image.
    void init_(const box2d& b, unsigned bdr = border::thickness);


    /// Test if \p p is valid.
    bool owns_(const point2d& p) const;

    /// Give the set of values of the image.
    const vset& values() const;

    /// Give the definition domain.
    const box2d& domain() const;

    /// Give the border thickness.
    unsigned border() const;

    /// Give the number of cells (points including border ones).
    std::size_t ncells() const;

    /// Read-only access to the image value located at point \p p.
    const T& operator()(const point2d& p) const;

    /// Read-write access to the image value located at point \p p.
    T& operator()(const point2d& p);

// FIXME: swig won't wrap this operator.
//     /// Read-only access to the image value located at offset \p o.
//     const T& operator[](unsigned o) const;

// FIXME: swig won't wrap this operator.
//     /// Read-write access to the image value located at offset \p o.
//     T& operator[](unsigned o);

    /// Read-only access to the image value located at (\p row, \p col).
    const T& at(int row, int col) const;

    /// Read-write access to the image value located at (\p row, \p col).
    T& at(int row, int col);


    /// Fast Image method

    /// Give the offset corresponding to the delta-point \p dp.
    int offset(const dpoint2d& dp) const;

    /// Give the point corresponding to the offset \p o.
    point2d point_at_offset(unsigned o) const;

    /// Give a hook to the value buffer.
    const T* buffer() const;

    /// Give a hook to the value buffer.
    T* buffer();


    /// Resize image border with new_border.
    void resize_(unsigned new_border);
  };

} // end of namespace mln

%template(image2d_int) mln::image2d<int>;


/*-------------------.
| mln::level::fill.  |
`-------------------*/

%{
#include "mln/level/fill.hh"
%}

// FIXME: Wrap mln::level::fill by hand, for mln_value(I) disturbs
// swig.  Annotate the original source code instead?
namespace mln
{
  namespace level
  {
    template <typename I>
    void fill(mln::Image<I>& ima, const typename I::value& v);
  } // end of namespace mln::level

} // end of namespace mln

%template(fill_image2d_int) mln::level::fill< mln::image2d<int> >;


/*----------------------.
| mln::debug::println.  |
`----------------------*/

%{
  #include "mln/debug/println.hh"
%}

%include "mln/debug/println.hh"

%template(println_image2d_int) mln::debug::println< mln::image2d<int> >;
