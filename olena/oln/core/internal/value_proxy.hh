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

#ifndef OLN_CORE_INTERNAL_VALUE_PROXY_HH
# define OLN_CORE_INTERNAL_VALUE_PROXY_HH

# include <ostream>
# include <oln/core/concept/value.hh>


namespace oln
{

  namespace internal
  {


    template <typename I>
    class value_proxy_
    {
    public:

      // ctor
      value_proxy_(I& ima, const oln_psite(I)& p);

      // assignment => write
      template <typename T>
      value_proxy_<I>& operator=(const T& val);

      // conversion => read
      template <typename T>
      operator T() const;

      // explicit read
      oln_value(I) value() const;

    private:

      I& ima_;
      oln_psite(I) p_;
    };


    template <typename I>
    std::ostream& operator<<(std::ostream& ostr, const value_proxy_<I>& v);


# ifndef OLN_INCLUDE_ONLY

    template <typename I>
    value_proxy_<I>::value_proxy_(I& ima, const oln_psite(I)& p)
      : ima_(ima),
	p_(p)
    {
    }

    template <typename I>
    template <typename T>
    value_proxy_<I>&
    value_proxy_<I>::operator=(const T& val)
    {
      ima_.write_(this->p_, val);
      return *this;
    }

    template <typename I>
    template <typename T>
    value_proxy_<I>::operator T() const
    {
      T tmp = this->ima_.read_(this->p_);
      return tmp;
    }

    template <typename I>
    oln_value(I)
    value_proxy_<I>::value() const
    {
      oln_value(I) tmp = this->ima_.read_(this->p_);
      return tmp;
    }

    template <typename I>
    std::ostream& operator<<(std::ostream& ostr, const value_proxy_<I>& v)
    {
      return ostr << v.value();
    }

# endif // ! OLN_INCLUDE_ONLY

  } // end of namespace oln::internal

} // end of namespace oln


#endif // ! OLN_CORE_INTERNAL_VALUE_PROXY_HH
