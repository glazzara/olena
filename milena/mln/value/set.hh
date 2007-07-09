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

#ifndef MLN_VALUE_SET_HH
# define MLN_VALUE_SET_HH

/*! \file mln/value/vset.hh
 *
 * \brief Define some basic sets of values from value types.
 */

# include <mln/core/concept/value_set.hh>
# include <mln/value/props.hh>


namespace mln
{

  namespace value
  {

    // Fwd decls.
    template <typename S> struct fwd_viter_;
    template <typename S> struct bkd_viter_;


    /*! Class that defines the set of values of type \c T.
     *
     * This is an exhaustive value set over \c T.
     */
    template <typename T>
    struct set_ : public Value_Set< set_<T> >
    {
      /// Value associated type.
      typedef T value;

      /// Forward Viter associated type.
      typedef fwd_viter_< set_<T> > fwd_viter;

      /// Backward Viter associated type.
      typedef bkd_viter_< set_<T> > bkd_viter;

      /// Viter associated type.
      typedef fwd_viter viter;

      /// Test if \p v belongs to this set: always true!
      bool has(const T& v) const;

      /// Give the \p i-th value.
      T operator[](std::size_t i) const;

      /// Give the index of value \p v in this set.
      std::size_t index_of(const T& v) const;

      /// Give the number of values.
      std::size_t nvalues() const;

      /// Return a singleton.
      static const set_<T>& the();
    };



# ifndef MLN_INCLUDE_ONLY

    template <typename T>
    bool
    set_<T>::has(const T& v) const
    {
      return true;
    }

    template <typename T>
    T
    set_<T>::operator[](std::size_t i) const
    {
      mln_precondition(i < nvalues());
      return mln_min(T) + i;
    }

    template <typename T>
    std::size_t
    set_<T>::index_of(const T& v) const
    {
      return v - mln_min(T);
    }

    template <typename T>
    std::size_t
    set_<T>::nvalues() const
    {
      return mln_card(T);
    }

    template <typename T>
    const set_<T>&
    set_<T>::the()
    {
      static set_<T> the_;
      return the_;
    }

# endif // ! MLN_INCLUDE_ONLY

  } // end of namespace mln::value



  typedef value::set_<bool>     Booleans;
  typedef value::set_<int>      integers();
  typedef value::set_<unsigned> naturals();



} // end of namespace mln


# include <mln/value/viter.hh>


#endif // ! MLN_VALUE_SET_HH
