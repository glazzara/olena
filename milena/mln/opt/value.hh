// Copyright (C) 2008 EPITA Research and Development Laboratory
// (LRDE)
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

#ifndef MLN_OPT_VALUE_HH
# define MLN_OPT_VALUE_HH

/// \file mln/opt/value.hh
///
/// Define the opt::value routine.

# include <mln/core/concept/image.hh>
# include <mln/trait/images.hh>

namespace mln
{

  namespace opt
  {

    template <typename I>
    inline
    mln_rvalue(I) value(const Image<I>& ima);

    template <typename I>
    inline
    mln_lvalue(I) value(Image<I>& ima);


# ifndef MLN_INCLUDE_ONLY

    namespace impl
    {

      template <typename I>
      inline
      mln_rvalue(I) value_impl(trait::image::category::domain_morpher,
                               const Image<I>& ima)
      {
        return value(*exact(ima).delegatee_());
      }

      template <typename I>
      inline
      mln_rvalue(I) value_impl(trait::image::category::any,
                               const Image<I>& ima)
      {
        mlc_is(mln_trait_image_value_storage(I),
               trait::image::value_storage::singleton)::check();
        return exact(ima).value_();
      }




      template <typename I>
      inline
      mln_lvalue(I) value_impl(trait::image::category::domain_morpher,
                               Image<I>& ima)
      {
        return value(*exact(ima).delegatee_());
      }

      template <typename I>
      inline
      mln_lvalue(I) value_impl(trait::image::category::any,
                               Image<I>& ima)
      {
        mlc_is(mln_trait_image_value_storage(I),
               trait::image::value_storage::singleton)::check();
        return exact(ima).value_();
      }
    }


    template <typename I>
    inline
    mln_rvalue(I) value(const Image<I>& ima)
    {
      return impl::value_impl(mln_trait_image_category(I)(), ima);
    }

    template <typename I>
    inline
    mln_lvalue(I) value(Image<I>& ima)
    {
      return impl::value_impl(mln_trait_image_category(I)(), ima);
    }




# endif // ! MLN_INCLUDE_ONLY

  } // end of namespace mln::opt

} // end of namespace mln


#endif // ! MLN_OPT_VALUE_HH
