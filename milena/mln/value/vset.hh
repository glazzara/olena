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

#ifndef MLN_VALUE_VSET_HH
# define MLN_VALUE_VSET_HH

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


    /*! Class that defines the properties of the value type \c T.
     */
    template <typename T>
    struct vset_ : public Value_Set< vset_<T> >
    {
      typedef T value;

      typedef fwd_viter_< vset_<T> > fwd_viter;
      typedef bkd_viter_< vset_<T> > bkd_viter;
      typedef fwd_viter viter;

      bool has(const T& v) const;

      T operator[](std::size_t i) const;

      std::size_t nvalues() const;
    };


# ifndef MLN_INCLUDE_ONLY

    template <typename T>
    bool
    vset_<T>::has(const T& v) const
    {
      return true;
    }

    template <typename T>
    T
    vset_<T>::operator[](std::size_t i) const
    {
      mln_precondition(i < nvalues());
      return mln_min(T) + i;
    }

    template <typename T>
    std::size_t
    vset_<T>::nvalues() const
    {
      return mln_card(T);
    }

# endif // ! MLN_INCLUDE_ONLY

  } // end of namespace mln::value

} // end of namespace mln


# include <mln/value/viter.hh>


#endif // ! MLN_VALUE_VSET_HH
