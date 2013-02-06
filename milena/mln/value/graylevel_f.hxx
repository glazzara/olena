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

#ifndef MLN_VALUE_GRAYLEVEL_F_HXX
# define MLN_VALUE_GRAYLEVEL_F_HXX

/// \file
///
/// Definition of the mln::value::graylevel_f class.

# include <mln/value/graylevel_f.hh>


namespace mln
{

  namespace value
  {

    // graylevel_f.

    inline
    graylevel_f::graylevel_f()
    {
    }


    inline
    graylevel_f::graylevel_f(float val)
    {
      mln_precondition(val >= 0);
      mln_precondition(val <= 1);
      this->v_ = val;
    }

    inline
    graylevel_f&
    graylevel_f::operator=(float val)
    {
      mln_precondition(val >= 0);
      mln_precondition(val <= 1);
      this->v_ = val;
      return *this;
    }

    template <unsigned n>
    graylevel_f::graylevel_f(const graylevel<n>& rhs)
    {
      mln_precondition(rhs.to_float() >= 0);
      mln_precondition(rhs.to_float() <= 1);
      this->v_ = rhs.to_float();
    }

    template <unsigned n>
    graylevel_f&
    graylevel_f::operator=(const graylevel<n>& rhs)
    {
      mln_precondition(rhs.to_float() >= 0);
      mln_precondition(rhs.to_float() <= 1);
      this->v_ = rhs.to_float();
      return *this;
    }

    inline
    graylevel_f::graylevel_f(const graylevel_f& rhs)
      : Floating<graylevel_f>()
    {
      this->v_ = rhs.v_;
    }

    inline
    graylevel_f&
    graylevel_f::operator=(const graylevel_f& rhs)
    {
      mln_precondition(rhs.v_ >= 0);
      mln_precondition(rhs.v_ <= 1);
      this->v_ = rhs.v_;
      return *this;
    }

    inline
    graylevel_f::graylevel_f(const mln::literal::black_t&)
    {
      this->v_ = 0.0f;
    }

    inline
    graylevel_f&
    graylevel_f::operator=(const mln::literal::black_t&)
    {
      this->v_ = 0.0f;
      return *this;
    }

    inline
    graylevel_f::graylevel_f(const mln::literal::medium_gray_t&)
    {
      this->v_ = 0.5f;
    }

    inline
    graylevel_f&
    graylevel_f::operator=(const mln::literal::medium_gray_t&)
    {
      this->v_ = 0.5f;
      return *this;
    }

    inline
    graylevel_f::graylevel_f(const mln::literal::white_t&)
    {
      this->v_ = 1.0f;
    }

    inline
    graylevel_f&
    graylevel_f::operator=(const mln::literal::white_t&)
    {
      this->v_ = 1.0f;
      return *this;
    }

    template <unsigned n>
    inline
    graylevel_f::operator graylevel<n>() const
    {
      return graylevel<n>(internal::gray_f(*this));
    }

    inline
    float
    graylevel_f::value() const
    {
      return this->v_;
    }


    // Operators.

    inline
    std::ostream& operator<<(std::ostream& ostr, const graylevel_f& g)
    {
      return ostr << g.value() << "/gl_f"; // FIXME: Be more explicit!.
    }


    template <typename I>
    inline
    bool
    operator==(const Integer<I>& lhs, const graylevel_f& rhs)
    {
      return rhs.value() == exact(lhs);
    }

    inline
    bool
    operator==(const graylevel_f& lhs, const graylevel_f& rhs)
    {
      return rhs.value() == lhs.value();
    }


    inline
    mln_trait_op_plus_(graylevel_f, graylevel_f)
    operator+(const graylevel_f& lhs, const graylevel_f& rhs)
    {
      return lhs.value() + rhs.value();
    }


    template <unsigned n>
    inline
    mln_trait_op_plus(graylevel_f, graylevel<n>)
    operator+(const graylevel_f& lhs, const graylevel<n>& rhs)
    {
      return lhs.value() + graylevel_f(rhs).value();
    }

    template <unsigned n>
    inline
    mln_trait_op_plus(graylevel_f, graylevel<n>)
    operator+(const graylevel<n>& lhs, const graylevel_f& rhs)
    {
      return rhs.value() + graylevel_f(lhs).value();
    }

    template <typename I>
    inline
    void
    operator+(const graylevel_f& lhs, const I& i)
    {
      (void) lhs;
      (void) i;
      typename
	Object<I>::wrong_use_of_graylevel_f___Please_use_the__to_enc__method a;
    }

    template <typename I>
    inline
    void
    operator+(const I& i, const graylevel_f& rhs)
    {
      (void) i;
      (void) rhs;
      typename
	Object<I>::wrong_use_of_graylevel_f___Please_use_the__to_enc__method a;
    }


    inline
    mln_trait_op_minus_(graylevel_f, graylevel_f)
    operator-(const graylevel_f& lhs, const graylevel_f& rhs)
    {
      return lhs.value() - rhs.value();
    }

    template <typename I>
    inline
    void
    operator-(const graylevel_f& lhs, const I& i)
    {
      (void) lhs;
      (void) i;
      typename
	Object<I>::wrong_use_of_graylevel_f___Please_use_the__to_enc__method a;
    }

    template <typename I>
    inline
    void
    operator-(const I& i, const graylevel_f& rhs)
    {
      (void) i;
      (void) rhs;
      typename
	Object<I>::wrong_use_of_graylevel_f___Please_use_the__to_enc__method a;
    }


    inline
    mln_trait_op_times_(graylevel_f, graylevel_f)
    operator*(const graylevel_f& lhs, const graylevel_f& rhs)
    {
      return lhs.value() * rhs.value();
    }



    template <typename I>
    inline
    mln_trait_op_times(graylevel_f, I)
    operator*(const graylevel_f& lhs, const Integer<I>& rhs)
    {
      return lhs.value() * int(exact(rhs));
    }

    template <typename F>
    inline
    mln_trait_op_times(graylevel_f, F)
    operator*(const graylevel_f& lhs, const Floating<F>& rhs)
    {
      return lhs.value() * exact(rhs);
    }

    template <typename F>
    inline
    mln_trait_op_times(F, graylevel_f)
    operator*(const Floating<F>& lhs, const graylevel_f& rhs)
    {
      return rhs.value() * exact(lhs);
    }


    template <typename S>
    inline
    mln_trait_op_times(graylevel_f, S)
    operator*(const graylevel_f& lhs, const scalar_<S>& rhs)
    {
      return lhs.value() * rhs.to_equiv();
    }

    template <typename S>
    inline
    mln_trait_op_div(graylevel_f, S)
    operator/(const graylevel_f& lhs, const scalar_<S>& rhs)
    {
      mln_precondition(rhs.to_equiv() != S(0));
      return lhs.value() / rhs.to_equiv();
    }

  } // end of namespace mln::value

} // end of namespace mln


#endif // ! MLN_VALUE_GRAYLEVEL_F_HXX
