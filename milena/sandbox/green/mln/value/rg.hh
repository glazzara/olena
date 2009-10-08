// Copyright (C) 2007, 2008, 2009 EPITA Research and Development
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

#ifndef  MLN_VALUE_RG_HH
# define MLN_VALUE_RG_HH


#include <mln/value/concept/vectorial.hh>
# include <mln/value/int_u.hh>

#include <mln/trait/value_.hh>
# include <mln/algebra/vec.hh>

namespace mln
{

  namespace value
  {

    // Forward declaration.
    template <unsigned n> struct rg;

  } // end of namespace mln::value

  namespace trait
  {

    template <unsigned n>
    struct value_< mln::value::rg<n> >
    {
      enum {
	dim = 2,
	nbits = dim * n,
	card  = mln_value_card_from_(nbits)
      };

      typedef trait::value::nature::vectorial nature;
      typedef trait::value::kind::color       kind;
      typedef trait::value::quant::high       quant;

      typedef void comp;
      typedef mln::value::int_u<n> comp_0;
      typedef mln::value::int_u<n> comp_1;

      static comp_0 get_comp_0(const mln::value::rg<n>& v){return v.red();  }
      static comp_1 get_comp_1(const mln::value::rg<n>& v){return v.green();}

      typedef algebra::vec<dim, float> sum;

      static const char* name()
      {
	static std::string s = std::string("rg").append(1, 8 + '0');
	return s.c_str();
      }
    };

  } // end of namespace mln::trait

  namespace value
  {
    template <unsigned n>
    struct rg :public mln::value::Vectorial< rg<n> >,
		public internal::value_like_<
      algebra::vec< 2, int_u<n> >,  // Equivalent.
      algebra::vec< 2, int_u<n> >,  // Encoding.
      algebra::vec< 2, int >,       // Interoperation.
      rg<n> >			    // Exact.
    {
      typedef int_u<n> t_red;
      typedef int_u<n> t_green;

      int_u<n>  red()   const {return this->v_[0];}
      int_u<n>& red()         {return this->v_[0];}
      int_u<n>  green() const {return this->v_[1];}
      int_u<n>& green()       {return this->v_[1];}

      rg(){}
      rg(const rgb8& val){this->v_[0] = val.red(); this->v_[1] = val.green();}

      rg operator=(const rg& rhs)
      {
	if (&rhs != this)
	  this->v_ = rhs.v_;

	return *this;
      }
    };

  } // end of namespace mln::value

} // end of namespace mln

#endif // ! MLN_VALUE_RG_HH
