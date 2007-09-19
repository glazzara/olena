// Copyright (C) 2007 EPITA Research and Development Laboratory
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
// License.  This exception does not however invalidate any other
// reasons why the executable file might be covered by the GNU General
// Public License.

#ifndef MLN_CORE_INTERPOLATED_HH
# define MLN_CORE_INTERPOLATED_HH

/*! \file mln/core/interpolated.hh
 *
 * \brief Definition of an image class FIXME
 */

# include <cmath>

# include <mln/core/concept/image.hh>
# include <mln/metal/vec.hh>


namespace mln
{

  /*! \brief FIXME
   *
   */
  template <typename I>
  struct interpolated : public mln::internal::image_base_< mln_pset(I), interpolated<I> >
  {
    /// Point_Site associated type.
    typedef mln_psite(I) psite;

    /// Value associated type.
    typedef mln_value(I) value;

    /// Return type of read-write access.
    typedef mln_lvalue(I) lvalue;

    /// Return type of read-only access.
    typedef mln_rvalue(I) rvalue;

    /// Value set associated type.
    typedef mln::value::set<value> vset;



    /// Constructor.
    interpolated(const Image<I>& ima);


    /// Test if this image has been initialized.
    bool has_data() const;

    /// Test if a pixel value is accessible at \p p.
    bool owns_(const psite& p) const;

    /// Give the definition domain.
    const mln_pset(I)& domain() const;

    /// Read-only access of pixel value at point site \p p.
    mln_value(I) operator()(const psite& p) const;

    /// Mutable access is only OK for reading (not writing).
    mln_value(I) operator()(const psite& p);

    mln_value(I) operator()(const mln::metal::vec<I::point::dim, float>& v) const;
    
    mln_value(I) operator()(const mln::metal::vec<I::point::dim, float>& v);
    
      
    /// Give the set of values of the image.
    const vset& values() const;

    /// Change value type.
    template <typename U>     
      struct change_value
      {
	typedef mln_ch_value(I, U) ret;
      };

  protected:
    const I& ima_;
  };



# ifndef MLN_INCLUDE_ONLY

  template <typename I>
  interpolated<I>::interpolated(const Image<I>& ima)
    : ima_(exact(ima))
  {
    mln_precondition(exact(ima).has_data());
  }

  template <typename I>
  bool interpolated<I>::has_data() const
  {
    mln_invariant(ima_.has_data());
    return true;
  }

  template <typename I>
  bool interpolated<I>::owns_(const psite& p) const
  {
    return ima_.owns_(p);
  }

  template <typename I>
  const mln_pset(I)&
  interpolated<I>::domain() const
  {
    return ima_.domain();
  }

  template <typename I>
  mln_value(I)
  interpolated<I>::operator()(const psite& p) const
  {
    mln_precondition(ima_.owns_(p));
    return ima_(p);
  }

  template <typename I>
  mln_value(I)
  interpolated<I>::operator()(const psite& p)
  {
    return ima_(p);
  }

  template <typename I>
  mln_value(I)
  interpolated<I>::operator()(const mln::metal::vec<I::point::dim, float>& v) const
  {
    mln_point(I) p;
    for (unsigned i = 0; i < I::point::dim; ++i)
      p[i] = static_cast<int>(round(v[i]));
    mln_assertion(ima_.owns_(p));
    return (ima_(p));
  }

  template <typename I>
  mln_value(I)
  interpolated<I>::operator()(const mln::metal::vec<I::point::dim, float>& v)
  {
    mln_point(I) p;
    for (unsigned i = 0; i < I::point::dim; ++i)
      p[i] = static_cast<int>(round(v[i]));
    mln_assertion(ima_.owns_(p));
    return (ima_(p));
  }

  template <typename I>
  const mln::value::set<mln_value(I) >&
  interpolated<I>::values() const
  {
    return vset::the();
  }
  
# endif // ! MLN_INCLUDE_ONLY

} // end of namespace mln


#endif // ! MLN_CORE_INTERPOLATED_HH
