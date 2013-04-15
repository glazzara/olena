// Copyright (C) 2009, 2012 EPITA Research and Development Laboratory
// (LRDE)
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

#ifndef MLN_CORE_INTERNAL_PITER_IDENTITY_HH
# define MLN_CORE_INTERNAL_PITER_IDENTITY_HH

/// \file
///
/// \brief Definition of site iterator identity adaptors.

# include <mln/core/internal/piter_adaptor.hh>


namespace mln
{

  namespace internal
  {

    /*!
      \internal
      \brief A base class for site iterator identity adaptors.

      Parameter \c Pi is the type of the site iterator adaptee;
      parameter E is the exact type.
    */
    template <typename Pi, typename E>
    class piter_identity_ : public piter_adaptor_< Pi,            // Adaptee.
						   mln_pset(Pi),  // Site set.
						   E >            // Exact type.
    {
      typedef piter_adaptor_< Pi, mln_pset(Pi), E > super_;

    protected:

      /// Constructor without argument.
      piter_identity_();

      /// Constructor from a point iterator \p piter.
      piter_identity_(const Pi& piter);
    };


# ifndef MLN_INCLUDE_ONLY

    template <typename Pi, typename E>
    inline
    piter_identity_<Pi,E>::piter_identity_()
    {
    }

    template <typename Pi, typename E>
    inline
    piter_identity_<Pi,E>::piter_identity_(const Pi& pi)
      : super_(pi)
    {
    }

# endif // ! MLN_INCLUDE_ONLY

  } // end of namespace mln::internal

} // end of namespace mln


#endif // ! MLN_CORE_INTERNAL_PITER_IDENTITY_HH
