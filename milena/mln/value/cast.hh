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

#ifndef MLN_VALUE_CAST_HH
# define MLN_VALUE_CAST_HH

/*! \file mln/value/cast.hh
 *
 * \brief Definition of an image class FIXME
 */

# include <mln/core/concept/image.hh>
# include <mln/value/set.hh>


namespace mln
{

  namespace value
  {

    /*! \brief FIXME
     *
     */
    template <typename T, typename I>
    struct cast_image : public internal::image_base_< mln_pset(I), cast_image<T,I> >
    {
      /// Point_Site associated type.
      typedef mln_psite(I) psite;

      /// Point_Set associated type.
      typedef mln_pset(I) pset;

      /// Value associated type.
      typedef T value;

      /// Return type of read-only access.
      typedef T rvalue;

      /// Return type of read-write access.
      typedef void lvalue; // FIXME

      /// Value set associated type.
      typedef mln::value::set_<T> vset;


      /// Constructor.
      cast_image(const Image<I>& ima);


      /// Test if this image has been initialized.
      bool has_data() const;

      /// Test if a pixel value is accessible at \p p.
      bool owns_(const psite& p) const;

      /// Give the definition domain.
      const mln_pset(I)& domain() const;

      /// Read-only access of pixel value at point site \p p.
      T operator()(const psite& p) const;

      /// Read-write access of pixel value at point site \p p.
      void operator()(const psite& p);

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



    template <typename T, typename I>
    cast_image<T,I>
    cast(const Image<I>& ima)
    {
      mln_precondition(exact(ima).has_data());
      cast_image<T,I> tmp(ima);
      return tmp;
    }
  


# ifndef MLN_INCLUDE_ONLY

    template <typename T, typename I>
    cast_image<T,I>::cast_image(const Image<I>& ima)
      : ima_(exact(ima))
    {
      mln_precondition(exact(ima).has_data());
    }

    template <typename T, typename I>
    bool cast_image<T,I>::has_data() const
    {
      mln_invariant(ima_.has_data());
      return true;
    }

    template <typename T, typename I>
    bool cast_image<T,I>::owns_(const psite& p) const
    {
      return ima_.owns_(p);
    }

    template <typename T, typename I>
    const mln_pset(I)&
    cast_image<T,I>::domain() const
    {
      return ima_.domain();
    }

    template <typename T, typename I>
    T
    cast_image<T,I>::operator()(const psite& p) const
    {
      mln_precondition(ima_.owns_(p));
      return ima_(p);
    }

    template <typename T, typename I>
    void
    cast_image<T,I>::operator()(const psite& p)
    {
      mln_invariant(0); // FIXME: Turn into a compile-time error...
    }

    template <typename T, typename I>
    const mln::value::set_<T>&
    cast_image<T,I>::values() const
    {
      return vset::the();
    }
  
# endif // ! MLN_INCLUDE_ONLY

  } // end of namespace mln::value

} // end of namespace mln


#endif // ! MLN_VALUE_CAST_HH
