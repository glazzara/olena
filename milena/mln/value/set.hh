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

/*! \file mln/value/set.hh
 *
 * \brief Define some basic sets of values from value types.
 */

# include <mln/value/internal/iterable_set.hh>
# include <mln/core/trait/is_lowq.hh>


namespace mln
{

  namespace value
  {


    // Fwd decl.
    template <typename T> struct set;    


    namespace internal
    {

      template <typename T, typename lowq = metal::false_ >
      struct run_set_selector_ // no inheritance
      {};

      template <typename T>
      struct run_set_selector_< T, metal::true_ > // lowq so iterable
	:
	public iterable_set< T, mln::value::set<T> >
      {};

      template <typename T>
      struct set_selector_ : public run_set_selector_< T, mln_is_lowq(T) >
      {};

    } // end of namespace mln::value::internal



    /*! Class that defines the set of values of type \c T.
     *
     * This is the exhaustive set of values obtainable from type \c T.
     */
    template <typename T>
    struct set : public internal::set_selector_<T>
    {
      /// Return a singleton.
      static const set<T>& the();
    };



# ifndef MLN_INCLUDE_ONLY

    template <typename T>
    const set<T>&
    set<T>::the()
    {
      static set<T> the_;
      return the_;
    }

# endif // ! MLN_INCLUDE_ONLY

  } // end of namespace mln::value

} // end of namespace mln


#endif // ! MLN_VALUE_SET_HH
