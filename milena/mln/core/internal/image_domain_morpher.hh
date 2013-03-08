// Copyright (C) 2007, 2008, 2009, 2012 EPITA Research and Development
// Laboratory (LRDE)
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

#ifndef MLN_CORE_INTERNAL_IMAGE_DOMAIN_MORPHER_HH
# define MLN_CORE_INTERNAL_IMAGE_DOMAIN_MORPHER_HH

/// \file
///
/// Definition of a base class for image morphers w.r.t. domain.

# include <mln/core/internal/image_morpher.hh>


namespace mln
{

  namespace internal
  {


    /*!
      \internal
      \brief A base class for image morphers w.r.t. domain.

      Parameter \p I is the morphed image type.
      Parameter \p S is the morpher site set type.
      Parameter \p E is the exact (morpher image) type.

    */
    template <typename I, typename S, typename E>
    class image_domain_morpher : public image_morpher<I, mln_value(I), S, E>
    {
    public:

      /// Return type of read-only access.
      typedef mln_rvalue(I) rvalue;

      /// Return type of read-write access.
      typedef mln_morpher_lvalue(I) lvalue;


      /// Read-only access of pixel value at point site \p p.
      rvalue operator()(const mln_psite(S)& p) const;

      /// Read and "write if possible" access of pixel value at point site \p p.
      lvalue operator()(const mln_psite(S)& p);

    protected:

      /// Constructor (protected) without argument.
      image_domain_morpher();
    };


# ifndef MLN_INCLUDE_ONLY

    template <typename I, typename S, typename E>
    inline
    image_domain_morpher<I,S,E>::image_domain_morpher()
    {
    }

    template <typename I, typename S, typename E>
    inline
    mln_rvalue(I)
    image_domain_morpher<I,S,E>::operator()(const mln_psite(S)& p) const
    {
      mln_precondition(this->delegatee_() != 0);
      mln_precondition(exact(this)->has(p));
      mln_precondition(this->delegatee_()->has(p));
      return this->delegatee_()->operator()(p);
    }

    template <typename I, typename S, typename E>
    inline
    typename image_domain_morpher<I,S,E>::lvalue
    image_domain_morpher<I,S,E>::operator()(const mln_psite(S)& p)
    {
      mln_precondition(this->delegatee_() != 0);
      mln_precondition(exact(this)->has(p));
      mln_precondition(this->delegatee_()->has(p));
      return this->delegatee_()->operator()(p);
    }

# endif // ! MLN_INCLUDE_ONLY

  } // end of namespace mln::internal

} // end of namespace mln


#endif // ! MLN_CORE_INTERNAL_IMAGE_DOMAIN_MORPHER_HH
