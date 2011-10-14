// Copyright (C) 2007, 2008, 2009, 2011 EPITA Research and Development
// Laboratory (LRDE)
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

#ifndef MLN_VALUE_BUILTIN_FLOATINGS_HH
# define MLN_VALUE_BUILTIN_FLOATINGS_HH

/// \file
///
/// Some definitions about built-in floating types.

# include <mln/value/internal/limits.hh>

# include <mln/value/concept/built_in.hh>
# include <mln/value/concept/floating.hh>
# include <mln/trait/value_.hh>


namespace mln
{


  template <>
  struct category< float >
  {
    typedef value::Built_In< value::Floating<void> > ret;
  };

  template <>
  struct category< double >
  {
    typedef value::Built_In< value::Floating<void> > ret;
  };


  namespace trait
  {


    // float.

    template <>
    struct value_< float >
    {
      enum {
	dim   = 1,
	nbits = 8 * sizeof(float),
	card  = 0
      };

      typedef value::nature::floating nature;
      typedef value::kind::data       kind;
      typedef value::quant::high      quant;

      static float min()
      {
	static const float min_ = mln::value::internal::limits<float>::min();
	return min_;
      }
      static float max()
      {
	static const float max_ = mln::value::internal::limits<float>::max();
	return max_;
      }
      static float epsilon()
      {
	static const float epsilon_ = mln::value::internal::limits<float>::epsilon();
	return epsilon_;
      }

      typedef float                        sum;

      static const char* name()
      { return "float"; }

    };


    // double.

    template <>
    struct value_< double >
    {
      enum {
	dim   = 1,
	nbits = 8 * sizeof(double),
	card  = 0
      };

      typedef value::nature::floating nature;
      typedef value::kind::data       kind;
      typedef value::quant::high      quant;

      static double min()
      {
	static const double min_ = mln::value::internal::limits<double>::min();
	return min_;
      }
      static double max()
      {
	static const double max_ = mln::value::internal::limits<double>::max();
	return max_;
      }
      static double epsilon()
      {
	static const double epsilon_ = mln::value::internal::limits<double>::epsilon();
	return epsilon_;
      }

      typedef double                        sum;

      static const char* name()
      { return "double"; }

    };

  } // end of namespace mln::trait

} // end of namespace mln


#endif // ! MLN_VALUE_BUILTIN_FLOATINGS_HH
