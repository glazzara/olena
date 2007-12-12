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

#ifndef MLN_ACCU_INTERNAL_BASE_HH
# define MLN_ACCU_INTERNAL_BASE_HH

/*! \file mln/accu/internal/base.hh
 *
 * \brief Define a base class for implementation of accumulator
 * classes.
 */

# include <mln/core/concept/accumulator.hh>
# include <mln/metal/unconst.hh>
# include <mln/metal/unref.hh>


namespace mln
{

  namespace accu
  {

    namespace internal
    {

      /*! \internal Generic min accumulator class.
       * Base class for implementation of accumulator classes.
       */
      template <typename R, typename E>
      struct base_ : public Accumulator<E>
      {
	typedef mlc_unconst(R)  tmp_;
	typedef mlc_unref(tmp_) result_;
      public:

	typedef R result;

	operator result_() const;

      protected:
	base_();
      };


# ifndef MLN_INCLUDE_ONLY

      template <typename R, typename E>
      inline
      base_<R,E>::base_()
      {
      }

      template <typename R, typename E>
      inline
      base_<R,E>::operator typename base_<R,E>::result_ () const
      {
	return exact(this)->to_result();
      }

# endif // ! MLN_INCLUDE_ONLY

    } // end of namespace mln::accu::internal

  } // end of namespace mln::accu

} // end of namespace mln


#endif // ! MLN_ACCU_INTERNAL_BASE_HH
