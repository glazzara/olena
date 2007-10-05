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
# include <mln/value/internal/value_like.hh>
# include <mln/value/internal/integer.hh>
# include <mln/value/internal/encoding.hh>
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
    class graylevel
      : public internal::Integer< graylevel<n> >,
	public internal::value_like_< typename internal::encoding_unsigned_<n>::ret,
				      graylevel<n> >
    {
    protected:
      typedef internal::value_like_< typename internal::encoding_unsigned_<n>::ret,
				     graylevel<n> > like;

    public:

      /// Encoding associated type.
      typedef typename like::enc enc;

      /// Ctor.
      graylevel();

      /// Ctor.
      explicit graylevel(const int val);

      /// Access to std type.
      enc value() const;

      /// Op<.
      bool operator<(const graylevel<n>& rhs) const;

      graylevel<n>& operator=(const int val);

    protected:
      enc val_;
    };


    template <unsigned n>
    struct props< graylevel<n> >
    {
      static const std::size_t card_ = metal::pow<2, n>::value;
      static const graylevel<n> min() { return 0; }
      static const graylevel<n> max() { return card_ - 1; }
      static const unsigned nbits = n;
      typedef trait::kind::data kind;
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
    graylevel<n>::graylevel(const int val)
      : val_(val)
    {
      mln_precondition(val >= 0);
      mln_precondition(unsigned(val) <= mln_max(enc));
    }

    template <unsigned n>
    typename graylevel<n>::enc
    graylevel<n>::value() const
    {
      return val_;
    }

    template <unsigned n>
    graylevel<n>&
    graylevel<n>::operator=(const int val)
    {
      mln_precondition(val >= 0);
      mln_precondition(unsigned(val) <= mln_max(enc));
      this->val_ = val;
      return *this;
    }

    template <unsigned n>
    bool graylevel<n>::operator<(const graylevel<n>& rhs) const
    {
      return val_ < rhs.val_;
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

# endif


  } // end of namespace mln::value

} // end of namespace mln

#endif // ! MLN_CORE_VALUE_GRAYLEVEL_HH
