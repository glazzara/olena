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

#ifndef MLN_VALUE_BUILTIN_FLOATINGS_HH
# define MLN_VALUE_BUILTIN_FLOATINGS_HH

/*! \file mln/value/builtin/floatings.hh
 *
 * \brief Some definitions about built-in floating types.
 */

# include <limits>

# include <mln/value/concept/built_in.hh>
# include <mln/value/concept/floating.hh>
# include <mln/trait/value_.hh>


namespace mln
{


  template <>
  struct category< float >
  {
    typedef value::Built_In<void*> ret;
    typedef value::Floating<void> super;
  };

  template <>
  struct category< double >
  {
    typedef value::Built_In<void*> ret;
    typedef value::Floating<void> super;
  };


  namespace trait
  {

    template <>
    struct value_< float >
    {
      enum {
	nbits = 8 * sizeof(float),
	card  = 0
      };

      typedef value::nature::floating nature;
      typedef value::kind::data       kind;
      typedef value::quant::high      quant;

      static const float min()
      {
	static const float min_ = std::numeric_limits<float>::min();
	return min_;
      }
      static const float max()
      {
	static const float max_ = std::numeric_limits<float>::max();
	return max_;
      }
      static const float epsilon()
      {
	static const float epsilon_ = std::numeric_limits<float>::epsilon();
	return epsilon_;
      }

      typedef float                        sum;
    };

    template <>
    struct value_< double >
    {
      enum {
	nbits = 8 * sizeof(double),
	card  = 0
      };

      typedef value::nature::floating nature;
      typedef value::kind::data       kind;
      typedef value::quant::high      quant;

      static const double min()
      {
	static const double min_ = std::numeric_limits<double>::min();
	return min_;
      }
      static const double max()
      {
	static const double max_ = std::numeric_limits<double>::max();
	return max_;
      }
      static const double epsilon()
      {
	static const double epsilon_ = std::numeric_limits<double>::epsilon();
	return epsilon_;
      }

      typedef double                        sum;
    };

  } // end of namespace mln::trait

} // end of namespace mln


#endif // ! MLN_VALUE_BUILTIN_FLOATINGS_HH
