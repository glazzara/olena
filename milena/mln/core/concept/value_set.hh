// Copyright (C) 2007, 2009, 2010, 2011, 2013 EPITA Research and
// Development Laboratory (LRDE)
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

#ifndef MLN_CORE_CONCEPT_VALUE_SET_HH
# define MLN_CORE_CONCEPT_VALUE_SET_HH

/*! \file
 * \brief Definition of the concept of mln::Value_Set.
 */

# include <mln/core/concept/value_iterator.hh> // FIXME: Why not object.hh?


namespace mln
{

  // Fwd decl.
  template <typename E> struct Value_Set;

  /// \cond INTERNAL_API
  /// \brief Value_Set category flag type.
  template <>
  struct Value_Set<void>
  {
    typedef Object<void> super;
  };
  /// \endcond


  /*!
    \brief Base class for implementation classes of sets of values.

    \ingroup modconcepts
   */
  template <typename E>
  struct Value_Set : public Object<E>
  {
    typedef Value_Set<void> category;

    /*
      typedef value;
      typedef fwd_viter;
      typedef bkd_viter;

      bool has(const value& v) const;

      value operator[](unsigned i) const;
      unsigned index_of(const value& v) const;

      unsigned nvalues() const;
     */

  protected:
    Value_Set();
  };


  /// \relates mln::Value_Set
  template <typename E>
  std::ostream& operator<<(std::ostream& ostr, const Value_Set<E>& vs);


# ifndef MLN_INCLUDE_ONLY

  template <typename E>
  inline
  Value_Set<E>::Value_Set()
  {
    typedef mln_value(E) value;
    typedef mln_fwd_viter(E) fwd_viter;
    typedef mln_bkd_viter(E) bkd_viter;

    bool (E::*m1)(const value&) const = & E::has;
    (void) m1;
    value (E::*m2)(unsigned) const = & E::operator[];
    (void) m2;
    unsigned (E::*m3)() const = & E::nvalues;
    (void) m3;
  }

  template <typename E>
  inline
  std::ostream& operator<<(std::ostream& ostr, const Value_Set<E>& vs_)
  {
    const E& vs = exact(vs_);
    ostr << "{ ";
    for (unsigned i = 0; i < vs.nvalues(); ++i)
      ostr << vs[i] << ' ';
    return ostr << '}';
  }

# endif // ! MLN_INCLUDE_ONLY

} // end of namespace mln


#endif // ! MLN_CORE_CONCEPT_VALUE_SET_HH
