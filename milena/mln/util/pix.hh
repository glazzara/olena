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

#ifndef MLN_UTIL_PIX_HH
# define MLN_UTIL_PIX_HH

/*! \file mln/util/pix.hh
 *
 * \brief Definition of an instant pix.
 */

# include <mln/core/concept/image.hh>


namespace mln
{

  namespace util
  {

    /// Pix structure.
    template <typename I>
    struct pix_
    {
      pix_(const Image<I>& ima, const mln_point(I)& p);
      const I& ima() const;
      const mln_point(I)& p() const;
      operator mln_rvalue(I)() const;
    private:
      const I& ima_;
      const mln_point(I)& p_;
    };
    

    /// Routine to construct a pix on the fly.
    template <typename I>
    pix_<I>
    pix(const Image<I>& ima, const mln_point(I)& p);


# ifndef MLN_INCLUDE_ONLY

    // pix_<I>

    template <typename I>
    pix_<I>::pix_(const Image<I>& ima, const mln_point(I)& p)
      : ima_(exact(ima)),
	p_(p)
    {
    }

    template <typename I>
    const I&
    pix_<I>::ima() const
    {
      return ima_;
    }

    template <typename I>
    const mln_point(I)&
    pix_<I>::p() const
    {
      return p_;
    }

    template <typename I>
    pix_<I>::operator mln_rvalue(I)() const
    {
      return ima_(p_);
    }

    // pix

    template <typename I>
    pix_<I>
    pix(const Image<I>& ima, const mln_point(I)& p)
    {
      pix_<I> tmp(ima, p);
      return tmp;
    }

# endif // ! MLN_INCLUDE_ONLY

  } // end of namespace mln::util

} // end of namespace mln


#endif // ! MLN_UTIL_PIX_HH
