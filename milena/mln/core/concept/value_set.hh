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

#ifndef MLN_CORE_CONCEPT_VALUE_SET_HH
# define MLN_CORE_CONCEPT_VALUE_SET_HH

/*! \file mln/core/concept/value_set.hh
 * \brief Definition of the concept of mln::Value_Set.
 */

# include <mln/core/concept/value_iterator.hh>


namespace mln
{

  /*! \brief Base class for implementation classes of sets of values.
   *
   * \see mln::doc::Value_Set for a complete documentation of this
   * class contents.
   */
  template <typename E>
  struct Value_Set : public Object<E>
  {
    /*
      typedef value;
      typedef fwd_viter;
      typedef bkd_viter;

      bool has(const value& v) const;

      value operator[](std::size_t i) const;
      std::size_t index_of(const value& v) const;

      std::size_t nvalues() const;
     */

  protected:
    Value_Set();
  };


# ifndef MLN_INCLUDE_ONLY

  template <typename E>
  Value_Set<E>::Value_Set()
  {
    typedef mln_value(E) value;
    typedef mln_fwd_viter(E) fwd_viter;
    typedef mln_bkd_viter(E) bkd_viter;
    
    bool (E::*m1)(const value&) const = & E::has;
    m1 = 0;
    value (E::*m2)(std::size_t) const = & E::operator[];
    m2 = 0;
    std::size_t (E::*m3)() const = & E::nvalues;
    m3 = 0;
  }

# endif // ! MLN_INCLUDE_ONLY

} // end of namespace mln


#endif // ! MLN_CORE_CONCEPT_VALUE_SET_HH
