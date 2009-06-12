// Copyright (C) 2007, 2008 EPITA Research and Development Laboratory
// (LRDE)
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

#ifndef MLN_ACCU_BLEN_PIX_HH
# define MLN_ACCU_BLEN_PIX_HH

/// \file mln/accu/blen_pix.hh
///
/// Define an accumulator that computes a sum.

# include <mln/core/concept/meta_accumulator.hh>
# include <mln/accu/shape/bbox.hh>
# include <mln/accu/internal/base.hh>

# include <mln/util/pix.hh> // To prevent accu::blen_pix to work on pixels (ambiguous).

# include <mln/trait/value_.hh>      // For mln_blen_pix.
# include <mln/value/builtin/all.hh> // In the case of summing builtin values.
# include <mln/literal/zero.hh>      // For initialization.

# include <mln/core/alias/point3d.hh>


namespace mln
{


  namespace accu
  {


    /// Generic blen_pix accumulator class.
    template <typename I>
    struct blen_pix : public mln::accu::internal::base< unsigned, blen_pix<I> >
    {
      typedef util::pix<I> argument;

      blen_pix();

      /// Manipulators.
      /// \{
      void init();
      void take(const argument& t);
      void take(const blen_pix<I>& other);
      /// \}

      void set_value(unsigned v)
      {
	len_ = v;
      }

      /// Get the value of the accumulator.
      unsigned to_result() const;

      /// Check whether this accu is able to return a result.
      /// Always true here.
      bool is_valid() const;

      const accu::shape::bbox<point3d>& b() const
      {
	return b_;
      }

      accu::shape::bbox<point3d>& b()
      {
	return b_;
      }

    protected:

      accu::shape::bbox<point3d> b_;
      unsigned len_;
    };


    template <typename B>
    unsigned max_len(const Box<B>& b_)
    {
      const B& b = exact(b_);
      typedef mln_site(B) P;
      enum { n = P::dim };
      unsigned len = b.len(0);
      for (unsigned i = 1; i < n; ++i)
	if (b.len(i) > len)
	  len = b.len(i);
      return len;
    }


    template <typename B>
    unsigned sum_len(const Box<B>& b_)
    {
      const B& b = exact(b_);
      typedef mln_site(B) P;
      enum { n = P::dim };
      unsigned len = 0;
      for (unsigned i = 0; i < n; ++i)
	len += b.len(i);
      return len;
    }


# ifndef MLN_INCLUDE_ONLY

    template <typename I>
    inline
    blen_pix<I>::blen_pix()
    {
      init();
    }

    template <typename I>
    inline
    void
    blen_pix<I>::init()
    {
      b_.init();
      len_ = 0;
    }

    template <typename I>
    inline
    void blen_pix<I>::take(const argument& pxl)
    {
      const mln_site(I)& p = pxl.p();
      accu_blen_take(b_, p);
      len_ = sum_len(b_.to_result());
    }

    template <typename I>
    inline
    void
    blen_pix<I>::take(const blen_pix<I>& other)
    {
      b_.take(other.b());
      len_ = sum_len(b_.to_result());
    }

    template <typename I>
    inline
    unsigned
    blen_pix<I>::to_result() const
    {
      return len_;
    }

    template <typename I>
    inline
    bool
    blen_pix<I>::is_valid() const
    {
      return true;
    }

# endif // ! MLN_INCLUDE_ONLY

  } // end of namespace mln::accu

} // end of namespace mln


#endif // ! MLN_ACCU_BLEN_PIX_HH
