// Copyright (C) 2006 EPITA Research and Development Laboratory
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

#ifndef OLN_CORE_VALUE_TWO_WAY_HH
# define OLN_CORE_VALUE_TWO_WAY_HH

# include <iostream>
# include <oln/value/proxy.hh>
# include <oln/core/abstract/functions.hh>


namespace oln
{

  namespace value
  {

    template <typename I, typename F>
    class two_way : public abstract::proxy< two_way<I,F> >
    {
    public:
      
      typedef oln_value(I) value_type;

      // Ctor.
      two_way(I& ima,
	      F fun,
	      const oln_psite(I)& p);
      
      // Read.
      template <typename V>
      operator V() const;

      // Explicit read.
      typename F::result_type value() const;

      // Write.
      template <typename V>
      two_way<I,F>& operator=(const V& value);

    protected:
      I ima_;
      F fun_;
      const oln_psite(I)& p_;
    };


    template <typename I, typename F>
    std::ostream& operator<<(std::ostream& ostr, const two_way<I,F>& proxy);


  } // end of namespace oln::value

} // end of namespace oln


#endif // ! OLN_CORE_VALUE_TWO_WAY_HH
