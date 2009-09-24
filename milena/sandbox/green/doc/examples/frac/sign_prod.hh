// Copyright (C) 2006, 2007, 2008, 2009 EPITA Research and Development Laboratory (LRDE)
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

#ifndef MLN_MATH_SIGN_PROD_HH
# define MLN_MATH_SIGN_PROD_HH

#include <mln/math/sign.hh>

namespace mln
{

  namespace math
  {
    template <typename T>
    sign_t sign_prod(const T& a, const T& b);

    template <typename T>
    T      sign_to_coeff(const sign_t& s);

    sign_t sign_prod_(const sign_t& a, const sign_t& b);

    bool   is_positive(const sign_t& s);
    bool   is_negative(const sign_t& s);
    bool   is_positive_or_null(const sign_t& s);
    bool   is_negative_or_null(const sign_t& s);
    bool   is_null(const sign_t& s);
    bool   is_not_null(const sign_t& s);

# ifndef MLN_INCLUDE_ONLY

    template <typename T>
    sign_t sign_prod(const T& a, const T& b)           {return sign_prod_(math::sign(a), math::sign(b));}

    template <typename T>
    T      sign_to_coeff(const sign_t& s)              {return static_cast<T>(s);}


    sign_t sign_prod_(const sign_t& a, const sign_t& b){return static_cast<sign_t>(a*b);   }

    bool   is_positive(const sign_t& s)                {return 0 <  s;}
    bool   is_negative(const sign_t& s)                {return 0 >  s;}
    bool   is_positive_or_null(const sign_t& s)        {return 0 <= s;}
    bool   is_negative_or_null(const sign_t& s)        {return 0 >= s;}
    bool   is_null(const sign_t& s)                    {return 0 == s;}
    bool   is_not_null(const sign_t& s)                {return 0 != s;}

# endif // ! MLN_INCLUDE_ONLY

  }
}

#endif // ! MLN_MATH_SIGN_PROD_HH
