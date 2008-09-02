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

/// \file dynamic_image2d
/// \brief A dynamic version of image2d, extendable in the target language.


%module(directors="1") dynamic_image2d

%import "point2d.i"
%import "box2d.i"

%import "int_u8.i"
// FIXME: The import directive does not include the `%{ ... %}' clauses.
%{
#include "mln/value/int_u8.hh"
%}

%{
# include <mln/core/concept/image.hh>
# include <mln/core/box2d.hh>
# include <mln/core/init.hh>

# include <mln/value/set.hh>
%}

// Shortcuts macros.
%include "mln/core/macros.hh"

// Concept.
%include "mln/core/concept/image.hh"

%feature("director");

%inline %{
namespace mln
{

  // FIXME: Separate interface and implementation (and/or move the C++
  // code into its own file).

  // Fwd decl.
  template <typename T> struct dynamic_image2d;


  namespace trait
  {

    template <typename T>
    struct image_< dynamic_image2d<T> >
      : default_image_< T, dynamic_image2d<T> >
    {
      typedef trait::image::category::primary category;

      typedef trait::image::access::random   access;
      typedef trait::image::space::two_d     space;
      typedef trait::image::size::regular    size;
      typedef trait::image::support::aligned support;

      typedef trait::image::border::none     border;
      typedef trait::image::data::raw        data;
      typedef trait::image::io::read_write   io;
      typedef trait::image::speed::fast      speed;
    };

  } // end of namespace mln::trait


  /* FIXME: Inherit from internal::image_base to factor methods (see
     below)?  */
  // \brief A semi-abstract class that serves as an entry point
  // (called « director » by SWIG) for Python code.
  //
  // This class is not really abstract in the C++ sense, but it lacks
  // methods to be a real image class.
  template <typename T>
  struct dynamic_image2d : Image< dynamic_image2d<T> >
  {
    typedef box2d   pset;
    typedef point2d psite;
    typedef point2d point;
    typedef dpoint2d dpoint;
    typedef mln_fwd_piter(box2d) fwd_piter;
    typedef mln_bkd_piter(box2d) bkd_piter;
    // End of warning.


    /// Value associated type.
    typedef T         value;

    /// Return type of read-only access.
    typedef const T& rvalue;

    /// Return type of read-write access.
    typedef T&       lvalue;


    /// Skeleton.
    typedef dynamic_image2d< tag::value_<T> > skeleton;


    /// Value_Set associated type.
    typedef mln::value::set<T> vset;


    /// Constructor without argument.
    dynamic_image2d();

    /// Constructor with the numbers of rows and columns and the
    /// border thickness.
    dynamic_image2d(int nrows, int ncols);

    /// Constructor with a box and the border thickness (default is
    /// 3).
    dynamic_image2d(const box2d& b);


    /// Initialize an empty image.
    virtual void init_(const box2d& b);


    /// Test if \p p is valid.
    virtual bool owns_(const point2d& p) const;

    /// Give the set of values of the image.
    virtual const vset& values() const;

    /// Give the definition domain.
    virtual const box2d& domain() const;

    /// Read-only access to the image value located at point \p p.
    virtual const T& operator()(const point2d& p) const;

    /// Read-write access to the image value located at point \p p.
    virtual T& operator()(const point2d& p);


    // From internal::image_base.
    
    /// Mesh associated type.
    typedef mln_mesh(pset) mesh;
    /// Coordinate associated type.
    typedef mln_coord(point) coord;

    /// Test if \p p belongs to the image domain.
    virtual bool has(const psite& p) const;
    /// Give a bounding box of the image domain.
    virtual const box2d& bbox() const;
    /// Give the number of points of the image domain.
    virtual std::size_t npoints() const;
    /// Test if this image has been initialized; default impl.
    virtual bool has_data() const;

    // We can set domain_ to protected, otherwise Python subclasses
    // won't see it.
    pset domain_;
  };


  template <typename T, typename J>
  inline
  void init_(tag::image_t, dynamic_image2d<T>& target, const J& model)
  {
    box2d b;
    init_(tag::bbox, b, model);
    target.init_(b);
  }


  template <typename T>
  inline
  dynamic_image2d<T>::dynamic_image2d()
  {
  }

  template <typename T>
  inline
  dynamic_image2d<T>::dynamic_image2d(int nrows, int ncols)
  {
    init_(make::box2d(nrows, ncols));
  }

  template <typename T>
  inline
  dynamic_image2d<T>::dynamic_image2d(const box2d& b)
  {
    init_(b);
  }

  template <typename T>
  inline
  void
  dynamic_image2d<T>::init_(const box2d& b)
  {
    domain_ = b;
  }

  template <typename T>
  inline
  bool
  dynamic_image2d<T>::owns_(const point2d& p) const
  {
    mln_precondition(exact(this)->has_data());
    return exact(this)->has(p);
  }

  template <typename T>
  inline
  const typename dynamic_image2d<T>::vset&
  dynamic_image2d<T>::values() const
  {
    return vset::the();
  }

  template <typename T>
  inline
  const box2d&
  dynamic_image2d<T>::domain() const
  {
    mln_precondition(this->has_data());
    return domain_;
  }

  // Dummy implementation, provided to have swig wrap the class (swig won't
  // wrap an abstract class).
  template <typename T>
  inline
  const T&
  dynamic_image2d<T>::operator()(const point2d& p) const
  {
    // Nothing.
    assert(false);
  }

  // Dummy implementation, provided to have swig wrap the class (swig won't
  // wrap an abstract class).
  template <typename T>
  inline
  T&
  dynamic_image2d<T>::operator()(const point2d& p)
  {
    // Nothing.
    assert(false);
  }

  template <typename T>
  inline
  bool
  dynamic_image2d<T>::has(const psite& p) const
  {
    mln_precondition(exact(this)->has_data());
    return exact(this)->domain().has(p);
  }

  template <typename T>
  inline
  const box2d&
  dynamic_image2d<T>::bbox() const
  {
    mln_precondition(exact(this)->has_data());
    return exact(this)->domain().bbox();
  }

  template <typename T>
  inline
  std::size_t
  dynamic_image2d<T>::npoints() const
  {
    mln_precondition(exact(this)->has_data());
    return exact(this)->domain().npoints();
  }

  template <typename T>
  inline
  bool
  dynamic_image2d<T>::has_data() const
  {
    return true;
  }

} // end of namespace mln
%}


%template(dynamic_image2d_int_u8) mln::dynamic_image2d< mln::value::int_u<8> >;


%include "fill.ixx"
%template(fill) mln::level::fill< mln::dynamic_image2d< mln::value::int_u<8> > >;

%include "println.ixx"
%template(println) mln::debug::println< mln::dynamic_image2d< mln::value::int_u<8> > >;
