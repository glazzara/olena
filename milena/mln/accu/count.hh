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

#ifndef MLN_ACCU_COUNT_HH
# define MLN_ACCU_COUNT_HH

/*! \file mln/accu/count.hh
 *
 * \brief Define an accumulator that counts.
 */

# include <mln/accu/internal/base.hh>
# include <mln/core/concept/meta_accumulator.hh>


namespace mln
{

  namespace accu
  {


    /*! Generic counter accumulator class.
     */
    template <typename T>
    struct count_ : public mln::accu::internal::base_< std::size_t , count_<T> >
    {
      typedef T argument;
      typedef std::size_t result; // FIXME: Up in Accumulator.

      count_();

      void init();
      void take(const argument&);
      void take(const count_<T>& other);

      std::size_t to_result() const;
      void set_value(std::size_t c);

    protected:

      std::size_t count__;
    };


    // FIXME: Doc!
    struct count : public Meta_Accumulator< count >
    {
      template <typename T>
      struct with
      {
	typedef count_<T> ret;
      };
    };


# ifndef MLN_INCLUDE_ONLY

    template <typename T>
    count_<T>::count_()
    {
      init();
    }

    template <typename T>
    void
    count_<T>::init()
    {
      count__ = 0;
    }

    template <typename T>
    void
    count_<T>::take(const argument&)
    {
      ++count__;
    }

    template <typename T>
    void
    count_<T>::take(const count_<T>& other)
    {
      count__ += other.count__;
    }

    template <typename T>
    std::size_t
    count_<T>::to_result() const
    {
      return count__;
    }

    template <typename T>
    void
    count_<T>::set_value(std::size_t c)
    {
      count__ = c;
    }

# endif // ! MLN_INCLUDE_ONLY

  } // end of namespace mln::accu

} // end of namespace mln


#endif // ! MLN_ACCU_COUNT_HH
