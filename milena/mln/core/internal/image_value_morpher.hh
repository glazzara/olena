// Copyright (C) 2007, 2008 EPITA Research and Development Laboratory (LRDE)
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

#ifndef MLN_CORE_INTERNAL_IMAGE_VALUE_MORPHER_HH
# define MLN_CORE_INTERNAL_IMAGE_VALUE_MORPHER_HH

/*! \file mln/core/internal/image_value_morpher.hh
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


    /*! \internal A base class for image morphers w.r.t. value.
     * Parameter \p S is a point set type.
     *
     */
    template <typename I, typename E>
    class image_value_morpher_ : public image_morpher_<I, mln_pset(I), E>
    {
    public:

      const mln_pset(I)& domain() const;
      bool owns_(const mln_psite(I)& p) const;

    protected:
      image_value_morpher_();
    };


# ifndef MLN_INCLUDE_ONLY

    template <typename I, typename E>
    inline
    image_value_morpher_<I,E>::image_value_morpher_()
    {
    }

    template <typename I, typename E>
    inline
    const mln_pset(I)&
    image_value_morpher_<I,E>::domain() const
    {
      mln_precondition(this->delegatee_() != 0);
      return this->delegatee_()->domain();
    }

    template <typename I, typename E>
    inline
    bool
    image_value_morpher_<I,E>::owns_(const mln_psite(I)& p) const
    {
      mln_precondition(this->delegatee_() != 0);
      return this->delegatee_()->owns_(p);
    }

# endif // ! MLN_INCLUDE_ONLY

  } // end of namespace mln::internal

} // end of namespace mln


#endif // ! MLN_CORE_INTERNAL_IMAGE_VALUE_MORPHER_HH
