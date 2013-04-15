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

#ifndef MLN_CORE_INTERNAL_IMAGE_VALUE_MORPHER_HH
# define MLN_CORE_INTERNAL_IMAGE_VALUE_MORPHER_HH

/*! \file
 *
 *  \brief Definition of a base class for image morphers w.r.t. value.
 *
 *  A value morpher alters methods dealing with values (even if the
 *  actual values are not changed), but leaves the domain untouched.
 */

# include <mln/core/internal/image_morpher.hh>


namespace mln
{

  namespace internal
  {

    //FIXME: Fix doxygen.

    /*!
      \internal
      \brief A base class for image morphers w.r.t. value.

      Parameter \p S is a point set type.
      Parameter \p P is a value type.
    */
    template <typename I, typename T, typename E>
    class image_value_morpher : public image_morpher<I, T, mln_domain(I), E>
    {
    public:

      const mln_domain(I)& domain() const;
      bool has(const mln_psite(I)& p) const;

    protected:
      image_value_morpher();
    };


# ifndef MLN_INCLUDE_ONLY

    template <typename I, typename T, typename E>
    inline
    image_value_morpher<I,T,E>::image_value_morpher()
    {
    }

    template <typename I, typename T, typename E>
    inline
    const mln_domain(I)&
    image_value_morpher<I,T,E>::domain() const
    {
      mln_precondition(this->delegatee_() != 0);
      return this->delegatee_()->domain();
    }

    template <typename I, typename T, typename E>
    inline
    bool
    image_value_morpher<I,T,E>::has(const mln_psite(I)& p) const
    {
      mln_precondition(this->delegatee_() != 0);
      return this->delegatee_()->has(p);
    }

# endif // ! MLN_INCLUDE_ONLY

  } // end of namespace mln::internal

} // end of namespace mln


#endif // ! MLN_CORE_INTERNAL_IMAGE_VALUE_MORPHER_HH
