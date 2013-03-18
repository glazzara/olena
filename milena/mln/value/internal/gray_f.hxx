// Copyright (C) 2006, 2007, 2008, 2009, 2011, 2013 EPITA Research and
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

#ifndef MLN_VALUE_INTERNAL_GRAY_F_HXX
# define MLN_VALUE_INTERNAL_GRAY_F_HXX

/// \file
///
/// Definition of the mln::value::internal::gray_f class.

# include <mln/value/internal/gray_f.hh>

# include <mln/math/round.hh>


namespace mln
{

  namespace value
  {

    namespace internal
    {

      // gray_f.

      inline
      gray_f::gray_f()
      {
      }

      inline
      gray_f::gray_f(const gray_f& g)
	: Floating<gray_f>()
      {
	this->v_ = g.v_;
      }

      inline
      gray_f&
      gray_f::operator=(const gray_f& g)
      {
	this->v_ = g.v_;
	return *this;
      }

      template <unsigned n>
      gray_f::gray_f(const gray_<n>& rhs)
      {
	static const float denom =
	  float(metal::math::pow_int<2, n>::value) - 1.f;
	this->v_ = float(rhs.value()) / denom;
      }

      template <unsigned n>
      gray_f&
      gray_f::operator=(const gray_<n>& rhs)
      {
	static const float denom =
	  float(metal::math::pow_int<2, n>::value) - 1.f;
	this->v_ = float(rhs.value()) / denom;
	return *this;
      }

      inline
      gray_f::gray_f(float val)
      {
	this->v_ = val;
      }

      inline
      gray_f&
      gray_f::operator=(float val)
      {
	this->v_ = val;
	return *this;
      }

      inline
      gray_f::gray_f(const graylevel_f& rhs)
      {
	this->v_ = rhs.value();
      }

      inline
      gray_f&
      gray_f::operator=(const graylevel_f& rhs)
      {
	this->v_ = rhs.value();
	return *this;
      }

      template <unsigned m>
      inline
      gray_f::operator graylevel<m>() const
      {
	return graylevel<m>(math::round<int>(this->v_
					     * (mlc_pow_int(2, m) - 1)));
      }


      template <unsigned m>
      inline
      gray_f::operator gray_<m>() const
      {
	return gray_<m>(math::round<int>(this->v_
					 * (mlc_pow_int(2, m) - 1)));
      }

      inline
      gray_f::operator graylevel_f() const
      {
	return graylevel_f(this->v_);
      }

      inline
      float
      gray_f::value() const
      {
	return this->v_;
      }

      // Operators.

      inline
      std::ostream& operator<<(std::ostream& ostr, const gray_f& g)
      {
	return ostr << g.value() << "/gray_f"; // FIXME: Be more explicit!
      }

    } // end of namespace mln::value::internal

  } // end of namespace mln::value

} // end of namespace mln

#endif // ! MLN_VALUE_INTERNAL_GRAY_F_HXX
