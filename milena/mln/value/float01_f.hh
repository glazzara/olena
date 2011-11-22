// Copyright (C) 2006, 2007, 2008, 2009, 2011 EPITA Research and
// Development Laboratory (LRDE)
//
// This file is part of Olena.
//
// Olena is free software: you can redistribute it and/or modify it under
// the terms of the GNU General Public License as published by the Free
// Software Foundation, version 2 of the License.
//
// Olena is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
// General Public License for more details.
//
// You should have received a copy of the GNU General Public License
// along with Olena.  If not, see <http://www.gnu.org/licenses/>.
//
// As a special exception, you may use this file as part of a free
// software project without restriction.  Specifically, if other files
// instantiate templates or use macros or inline functions from this
// file, or you compile this file and link it with other files to produce
// an executable, this file does not by itself cause the resulting
// executable to be covered by the GNU General Public License.  This
// exception does not however invalidate any other reasons why the
// executable file might be covered by the GNU General Public License.

#ifndef MLN_VALUE_FLOAT01_F_HH
# define MLN_VALUE_FLOAT01_F_HH

/*! \file
 *
 * \brief FIXME.
 */

# include <iostream>
# include <mln/value/internal/value_like.hh>
# include <mln/value/concept/floating.hh>
# include <mln/value/builtin/floatings.hh>
# include <mln/trait/value_.hh>


namespace mln
{

  // Fwd decls.
  namespace value {
    class float01;
    struct float01_f;
  }


  namespace trait
  {

    template <>
    struct value_< mln::value::float01_f >
    {
      typedef trait::value::nature::floating nature;
      typedef trait::value::kind::data       kind;
      typedef trait::value::quant::high      quant;

      enum {
	dim = 1,
	nbits = 8 * sizeof(float),
	card  = 0
      };

      static float min() { return 0; }
      static float max() { return 1; }
      static float epsilon() { return mln_epsilon(float); }

      typedef float comp;

      typedef float sum;
    };

  } // end of namespace trait



  namespace value
  {

    /// Class for floating values restricted to the interval [0..1].
    struct float01_f
      :
      public Floating< float01_f >,

      public internal::value_like_< float,      // Equivalent.
				    float,      // Encoding.
				    float,      // Interoperation.
				    float01_f > // Exact.
    {
      /// Constructor without argument.
      float01_f();

      /// Constructor from a float.
      float01_f(float val);

      /// Assignment from a float.
      float01_f& operator=(const float val);

      /// Access to float value.
      float value() const;

      /// Conversion to a float.
      operator float() const;
    };


# ifndef MLN_INCLUDE_ONLY

    // Float01_F.

    inline
    float01_f::float01_f()
    {
    }

    inline
    float01_f::float01_f(float val)
    {
      mln_precondition(val >= 0);
      mln_precondition(val <= 1);
      this->v_ = val;
    }

    inline
    float
    float01_f::value() const
    {
      return this->v_;
    }

    inline
    float01_f&
    float01_f::operator=(float val)
    {
      mln_precondition(val >= 0);
      mln_precondition(val <= 1);
      this->v_ = val;
      return *this;
    }

    inline
    float01_f::operator float() const
    {
      return this->v_;
    }

# endif // ! MLN_INCLUDE_ONLY

  } // end of namespace mln::value

} // end of namespace mln


#endif // ! MLN_VALUE_FLOAT01_F_HH
