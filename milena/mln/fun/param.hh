// Copyright (C) 2007, 2008 EPITA Research and Development Laboratory (LRDE)
//
// This file is part of the Olena Library.  This library is free
// software; you can redistribute it and/or modify it under the terms
// of the GNU General Public License version 2 as published by the
// Free Software Foundation.
//
// This library is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR F PARTICULAR PURPOSE.  See the GNU
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

#ifndef MLN_FUN_PARAM_HH
# define MLN_FUN_PARAM_HH

namespace mln
{

  namespace fun
  {

    template <typename T>
    struct stored
    {
      typedef T value;

      const T& to_value() const
      {
	return t_;
      }

      T& to_value()
      {
	return t_;
      }

      operator const T& () const
      {
	return to_value();
      }

      operator T& ()
      {
	return to_value();
      }

      stored& operator = (const T& t)
      {
	t_ = t;
	return *this;
      }

    protected:
      T t_;
    };

    template <>
    struct stored<void>
    {
    };

    template <typename F>
    struct parameter
    {
      typedef void param;
      typedef void storage;
    };

  } // end of namespace mln::fun

} // end of namespace mln

#endif /* ! MLN_FUN_PARAM_HH */
