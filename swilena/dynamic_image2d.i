//						       		-*- C++ -*-
// Copyright (C) 2008, 2009, 2014 EPITA Research and Development
// Laboratory (LRDE).
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
# include <mln/core/alias/box2d.hh>
# include <mln/core/routine/init.hh>

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

    // These traits actually depend on the exact (here dynamic) type
    // of the image, therefore we should provide several entry points
    // (dynamic_image_ implementations.
    template <typename T>
    struct image_< dynamic_image2d<T> >
      : default_image_< T, dynamic_image2d<T> >
    {
      // misc
      typedef trait::image::category::primary category;
      typedef trait::image::speed::slow       speed;
      typedef trait::image::size::regular     size;

      // value
      typedef trait::image::vw_io::none                    vw_io;
      typedef trait::image::vw_set::none                   vw_set;
      typedef trait::image::value_access::indirect         value_access;
      typedef trait::image::value_storage::disrupted       value_storage;
      typedef trait::image::value_browsing::site_wise_only value_browsing;
      typedef trait::image::value_alignment::with_grid     value_alignment;
      typedef trait::image::value_io::read_write           value_io;

      // site / domain
      typedef trait::image::pw_io::read_write        pw_io;
      typedef trait::image::localization::basic_grid localization;
      typedef trait::image::dimension::two_d         dimension;

      // extended domain
      typedef trait::image::ext_domain::none      ext_domain;
      typedef trait::image::ext_value::irrelevant ext_value;
      typedef trait::image::ext_io::irrelevant    ext_io;
    };

  } // end of namespace mln::trait


  /* FIXME: Inherit from internal::image_base to factor methods (see
     below)?  Probably not, as internal::image_base contains data
     (through a tracked_ptr).  */
  // \brief A semi-abstract class that serves as an entry point
  // (called « director » by SWIG) for Python code.
  //
  // This class is not really abstract in the C++ sense, but it lacks
  // methods to be a real image class.
  template <typename T>
  struct dynamic_image2d : Image< dynamic_image2d<T> >
  {
    typedef box2d domain_t;
    typedef point2d site;
    typedef point2d psite;
    typedef dpoint2d dpoint;
    typedef mln_fwd_piter(box2d) fwd_piter;
    typedef mln_bkd_piter(box2d) bkd_piter;
    typedef fwd_piter piter;

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

    virtual ~dynamic_image2d();

    /// Initialize an empty image.
    virtual void init_(const box2d& b);


    /// Give the set of values of the image.
    virtual const vset& values() const;

    /// Give the definition domain.
    virtual const box2d& domain() const;

    /// Read-only access to the image value located at point \p p.
    virtual const T& operator()(const point2d& p) const;

    /// Read-write access to the image value located at point \p p.
    virtual T& operator()(const point2d& p);


    /// Test if \p p belongs to the image domain.
    virtual bool has(const psite& p) const;
    /// Give a bounding box of the image domain.
    virtual const box2d& bbox() const;

    /// Is this image valid?  (dummy method).
    virtual bool is_valid() const;

    /// Eligible-value-set associated type.
    typedef mln::value::set<T> t_eligible_values_set;

    // Return the set of the image eligigle values
    const t_eligible_values_set& values_eligible() const;

    /// Value space associated type.
    typedef mln::value::set<
      typename mln::value::super_value<T>::ret > t_values_space;

    /// Return the value space of the image.
    const t_values_space& values_space() const;

    // We can set domain_ to protected, otherwise Python subclasses
    // won't see it.
    domain_t domain_;
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
  dynamic_image2d<T>::~dynamic_image2d()
  {
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
    mln_precondition(this->is_valid());
    return domain_;
  }

  // Dummy implementation, provided to have swig wrap the class (swig won't
  // wrap an abstract class).
  template <typename T>
  inline
  const T&
  dynamic_image2d<T>::operator()(const point2d&) const
  {
    abort();
    static T v;
    return v;
  }

  // Dummy implementation, provided to have swig wrap the class (swig won't
  // wrap an abstract class).
  template <typename T>
  inline
  T&
  dynamic_image2d<T>::operator()(const point2d&)
  {
    abort();
    static T v;
    return v;
  }

  template <typename T>
  inline
  bool
  dynamic_image2d<T>::has(const psite& p) const
  {
    mln_precondition(this->is_valid());
    return exact(this)->domain().has(p);
  }

  template <typename T>
  inline
  const box2d&
  dynamic_image2d<T>::bbox() const
  {
    mln_precondition(this->is_valid());
    return exact(this)->domain().bbox();
  }

  template <typename T>
  inline
  bool
  dynamic_image2d<T>::is_valid() const
  {
    // Dummy.
    return false;
  }

  template <typename T>
  inline
  const typename dynamic_image2d<T>::t_eligible_values_set&
  dynamic_image2d<T>::values_eligible() const
  {
    return t_eligible_values_set::the();
  }

  template <typename T>
  inline
  const typename dynamic_image2d<T>::t_values_space&
  dynamic_image2d<T>::values_space() const
  {
    return t_values_space::the();
  }

} // end of namespace mln
%}


%template(dynamic_image2d_int_u8) mln::dynamic_image2d< mln::value::int_u<8> >;


%include "fill.ixx"
%template(fill) mln::data::fill< mln::dynamic_image2d< mln::value::int_u<8> > >;

%include "println.ixx"
%template(println) mln::debug::println< mln::dynamic_image2d< mln::value::int_u<8> > >;
