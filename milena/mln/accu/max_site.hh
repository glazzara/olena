// Copyright (C) 2009 EPITA Research and Development Laboratory (LRDE)
//
// This file is part of the Milena Library.  This library is free
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

#ifndef MLN_ACCU_MAX_SITE_HH
# define MLN_ACCU_MAX_SITE_HH

/// \file mln/accu/max_site.hh
///
/// Define an accumulator that computes the first site with the maximum value
/// in an image.
///
/// \todo Use accu::pair just like in accu::stat::min_max.

# include <mln/core/concept/meta_accumulator.hh>
# include <mln/accu/internal/base.hh>
# include <mln/util/pix.hh>

namespace mln
{

  namespace accu
  {


    /// \brief Define an accumulator that computes the first site with the maximum
    /// value in an image.
    ///
    /// \ingroup modaccuimages
    //
    template <typename I>
    struct max_site : public mln::accu::internal::base< mln_psite(I), max_site<I> >
    {
      typedef mln::util::pix<I> argument;
      typedef mln_psite(I) result;

      max_site();

      /// Manipulators.
      /// \{
      void init();
      void take(const argument& t);
      void take(const max_site<I>& other);
      /// \}

      /// Get the value of the accumulator.
      mln_psite(I) to_result() const;
      operator mln_psite(I) () const;

      /// Underlying value.
      mln_value(I) value_() const;

      /// Check whether this accu is able to return a result.
      /// Always true here.
      bool is_valid() const;

    protected:
      bool is_valid_;
      mln_psite(I) max_p_;
      mln_value(I) max_v_;
    };



    namespace meta
    {

      /// Meta accumulator for max_site.
      struct max_site : public Meta_Accumulator< max_site >
      {
	template <typename I>
	struct with
	{
	  typedef accu::max_site<I> ret;
	};
      };

    } // end of namespace mln::accu::meta


# ifndef MLN_INCLUDE_ONLY

    template <typename I>
    inline
    max_site<I>::max_site()
    {
      init();
    }

    template <typename I>
    inline
    void
    max_site<I>::init()
    {
      is_valid_ = false;
      max_v_ = mln_min(mln_value(I));
    }

    template <typename I>
    inline
    void max_site<I>::take(const argument& t)
    {
      if (t.v() > max_v_)
      {
	max_v_ = t.v();
	max_p_ = t.p();
	is_valid_ = true;
      }
    }

    template <typename I>
    inline
    void
    max_site<I>::take(const max_site<I>& other)
    {
      mln_precondition(other.is_valid());

      if (other.value_() > max_v_)
      {
	max_v_ = other.value_();
	max_p_ = other.to_result();
	is_valid_ = true;
      }
    }

    template <typename I>
    inline
    mln_psite(I)
    max_site<I>::to_result() const
    {
      mln_precondition(is_valid());
      return max_p_;
    }

    template <typename I>
    inline
    max_site<I>::operator mln_psite(I)() const
    {
      return to_result();
    }

    template <typename I>
    inline
    mln_value(I)
    max_site<I>::value_() const
    {
      return max_v_;
    }

    template <typename I>
    inline
    bool
    max_site<I>::is_valid() const
    {
      return is_valid_;
    }

# endif // ! MLN_INCLUDE_ONLY

  } // end of namespace mln::accu

} // end of namespace mln


#endif // ! MLN_ACCU_MAX_SITE_HH
