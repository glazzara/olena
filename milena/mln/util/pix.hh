// Copyright (C) 2007, 2008 EPITA Research and Development Laboratory
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

#ifndef MLN_UTIL_PIX_HH
# define MLN_UTIL_PIX_HH

/// \file mln/util/pix.hh
///
/// Definition of an instant pix.
///
/// \todo Rename as util::pixel.
///
/// \todo Think about separating this class between
/// util::pixel<"mutable" I> and util::pixel<const I>.
///
/// \todo Look at the todo entry of mln/core/pixel.hh.

# include <mln/core/concept/image.hh>


namespace mln
{

  /// Select namespace (FIXME doc).
  namespace select
  {

    /// Structure p_of.
    template <typename P>
    struct p_of : P
    {
      operator typename P::psite() const
      {
	return this->p();
      }
    };

  }

  namespace util
  {

    /// Structure pix.
    ///
    ///
    template <typename I>
    struct pix
    {

      /// Point_Site associated type.
      typedef mln_psite(I) psite;

      /// Value associated type.
      typedef mln_value(I) value;

      /// Constructor.
      ///
      /// \param[in] ima The image.
      /// \param[in] p The p_site.
      pix(const Image<I>& ima, const mln_psite(I)& p);

      /// The getter of the image associate to pix structure.
      ///
      /// \return The image ima_.
      const I& ima() const;

      /// The getter of psite associate to pix structure.
      ///
      /// \return The psite p_.
      const mln_psite(I)& p() const;

      /// The getter of value associate to pix structure.
      ///
      /// \return The value of pix.
      mln_rvalue(I) v() const;

    private:

      /// The image associate to pix.
      const I& ima_;

      /// The psite associate to pix.
      const mln_psite(I)& p_;
    };


# ifndef MLN_INCLUDE_ONLY

    template <typename I>
    inline
    pix<I>::pix(const Image<I>& ima, const mln_psite(I)& p)
      : ima_(exact(ima)),
	p_(p)
    {
    }

    template <typename I>
    inline
    const I&
    pix<I>::ima() const
    {
      return ima_;
    }

    template <typename I>
    inline
    const mln_psite(I)&
    pix<I>::p() const
    {
      return p_;
    }

    template <typename I>
    inline
    mln_rvalue(I)
    pix<I>::v() const
    {
      return ima_(p_);
    }

# endif // ! MLN_INCLUDE_ONLY

  } // end of namespace mln::util

} // end of namespace mln


# include <mln/make/pix.hh>


#endif // ! MLN_UTIL_PIX_HH
