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

#ifndef MLN_ACCU_NIL_HH
# define MLN_ACCU_NIL_HH

/*! \file mln/accu/nil.hh
 *
 * \brief Define an accumulator that does nothing.
 *
 * \todo A macro to generate the meta version.
 */

# include <mln/core/concept/meta_accumulator.hh>
# include <mln/accu/internal/base.hh>
# include <mln/util/eat.hh>
# include <mln/util/ignore.hh>


namespace mln
{

  namespace accu
  {

    /*! Define an accumulator that does nothing.
     */
    template <typename T>
    struct nil_ : public mln::accu::internal::base_< util::ignore , nil_<T> >
    {
      typedef util::eat    argument;
      typedef util::ignore result;

      nil_();

      void init();
      void take_as_init(const argument&);
      void take(const argument&);
      void take(const nil_<T>&);

      util::ignore to_result() const;
    };



    // FIXME: Doc!
    struct nil : public Meta_Accumulator< nil >
    {
      template <typename V>
      struct with
      {
	typedef nil_<V> ret;
      };
    };


# ifndef MLN_INCLUDE_ONLY

    template <typename T>
    inline
    nil_<T>::nil_()
    {
    }

    template <typename T>
    inline
    void
    nil_<T>::init()
    {
    }

    template <typename T>
    inline
    void
    nil_<T>::take(const argument&)
    {
    }

    template <typename T>
    inline
    void
    nil_<T>::take_as_init(const argument&)
    {
    }

    template <typename T>
    inline
    void
    nil_<T>::take(const nil_<T>&)
    {
    }

    template <typename T>
    inline
    util::ignore
    nil_<T>::to_result() const
    {
      return util::ignore();
    }

# endif // ! MLN_INCLUDE_ONLY

  } // end of namespace mln::accu

} // end of namespace mln


#endif // ! MLN_ACCU_NIL_HH
