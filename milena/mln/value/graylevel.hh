// Copyright (C) 2006, 2007 EPITA Research and Development Laboratory
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

#ifndef MLN_CORE_VALUE_GRAYLEVEL_HH
# define MLN_CORE_VALUE_GRAYLEVEL_HH

# include <iostream>

# include <mln/core/contract.hh>
# include <mln/metal/math.hh>
# include <mln/metal/bexpr.hh>

# include <mln/value/int_u.hh>
# include <mln/value/gray.hh>
# include <mln/value/props.hh>


namespace mln
{

  namespace value
  {

    /// Fwd decl.
    class gray;


    /// General gray-level class on n bits.
    template <unsigned n>
    struct graylevel
      :
      public Integer< graylevel<n> >,
      
      public internal::value_like_< int_u<n>,          // Equivalent.
				    mln_enc(int_u<n>), // Encoding.
				    gray,              // Interoperation.
				    graylevel<n> >     // Exact.
    {
      /// Ctor.
      graylevel();

      /// Ctor.
      explicit graylevel(int val);

      /// Access to std type.
      mln_enc(int_u<n>) value() const;

      graylevel<n>& operator=(int val);
    };


    template <unsigned n>
    struct props< graylevel<n> >
    {
      static const std::size_t card_ = metal::pow<2, n>::value;
      static const graylevel<n> min() { return 0; }
      static const graylevel<n> max() { return card_ - 1; }
      static const unsigned nbits = n;
      typedef trait::value::kind::data kind;
      typedef float sum;
      typedef int interop;
    };


    /// Op<<.
    template <unsigned n>
    std::ostream& operator<<(std::ostream& ostr, const graylevel<n>& g);


    /// Aliases.
    typedef graylevel<8>  gl8;
    typedef graylevel<16> gl16;
    typedef graylevel<32> gl32;


    template <unsigned n, unsigned m>
    bool operator==(const graylevel<n>& lhs, const graylevel<m>& rhs);

    template <unsigned n, unsigned m>
    gray operator+(const graylevel<n>& lhs, const graylevel<m>& rhs);

    template <unsigned n, unsigned m>
    gray operator-(const graylevel<n>& lhs, const graylevel<m>& rhs);




# ifndef MLN_INCLUDE_ONLY

    // Graylevel<n>.

    template <unsigned n>
    graylevel<n>::graylevel()
    {
    }

    template <unsigned n>
    graylevel<n>::graylevel(int val)
    {
      mln_precondition(val >= 0);
      mln_precondition(unsigned(val) <= mln_max(mln_enc(int_u<n>)));
      this->v_ = val;
    }

    template <unsigned n>
    mln_enc(int_u<n>)
    graylevel<n>::value() const
    {
      return this->v_;
    }

    template <unsigned n>
    graylevel<n>&
    graylevel<n>::operator=(int val)
    {
      mln_precondition(val >= 0);
      mln_precondition(unsigned(val) <= mln_max(mln_enc(int_u<n>)));
      this->v_ = val;
      return *this;
    }

    template <unsigned n>
    std::ostream& operator<<(std::ostream& ostr, const graylevel<n>& g)
    {
      return ostr << g.value();
    }

    template <unsigned n, unsigned m>
    bool operator==(const graylevel<n>& lhs, const graylevel<m>& rhs)
    {
      return gray(lhs) == gray(rhs);
    }

# endif // ! MLN_INCLUDE_ONLY


  } // end of namespace mln::value

} // end of namespace mln

#endif // ! MLN_CORE_VALUE_GRAYLEVEL_HH
