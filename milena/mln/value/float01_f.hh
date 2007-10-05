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

#ifndef MLN_CORE_VALUE_FLOAT01_F_HH
# define MLN_CORE_VALUE_FLOAT01_F_HH

# include <iostream>
# include <mln/value/internal/value_like.hh>
# include <mln/value/concept/floating.hh>
# include <mln/value/props.hh>

namespace mln
{

  namespace value
  {

    /// Fwd decl.
    class float01;


    /// General float01-level class on n bits.
    class float01_f
      : public Floating< float01_f >,
	public internal::value_like_< float,
				      float01_f >
    {
    public:

      /// Ctor.
      float01_f();

      /// Ctor.
      float01_f(const float val);

      /// Access to std type.
      float value() const;

      /// Op encoding_t.
      operator float() const;

      float01_f& operator=(const float val);
      /// Op==.
      // bool operator==(const float01_f& rhs) const;

    protected:
      float val_;
    };

    template <>
    struct props< float01_f >
    {
      static const std::size_t card_ = 0;
      static const float min() { return 0; }
      static const float max() { return 1; }
      //static const unsigned nbits = n;
      typedef trait::kind::data kind;
      typedef float sum;
      typedef float interop;
    };

# ifndef MLN_INCLUDE_ONLY

    // Float01_F.

    float01_f::float01_f()
      : val_(0)
    {
    }

    float01_f::float01_f(const float val)
      : val_(val)
    {
      mln_precondition(val >= 0);
      mln_precondition(val <= 1);
    }

    float
    float01_f::value() const
    {
      return (val_);
    }

    float01_f&
    float01_f::operator=(const float val)
    {
      mln_precondition(val >= 0);
      mln_precondition(val <= 1);
      this->val_ = val;
      return *this;
    }

    float01_f::operator float() const
    {
      return val_;
    }

# endif


  } // end of namespace mln::value

} // end of namespace mln

#endif // ! MLN_CORE_VALUE_FLOAT01_F_HH
