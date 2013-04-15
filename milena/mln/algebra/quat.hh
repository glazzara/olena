// Copyright (C) 2007, 2008, 2009, 2012 EPITA Research and Development
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

#ifndef MLN_ALGEBRA_QUAT_HH
# define MLN_ALGEBRA_QUAT_HH

/// \file
///
/// Define a class for quaternion algebra values.
///
/// \todo Inline computations in rotate routine.

# include <cmath>

# include <mln/value/ops.hh>

# include <mln/value/concept/vectorial.hh>
# include <mln/value/internal/value_like.hh>
# include <mln/trait/value_.hh>

# include <mln/algebra/vec.hh>
# include <mln/math/abs.hh>
# include <mln/norm/l2.hh>

# include <mln/util/couple.hh>
# include <mln/math/pi.hh>
// FIXME: pow, exp etc... are def here and in value::...


namespace mln
{

  // Fwd decls.
  namespace algebra { class quat; }
  namespace literal { struct zero_t; struct one_t; }


  namespace trait
  {

    // quat OP quat

    template <>
    struct set_precise_binary_< op::plus, mln::algebra::quat, mln::algebra::quat >
    {
      typedef mln::algebra::quat ret;
    };

    template <>
    struct set_precise_binary_< op::minus, mln::algebra::quat, mln::algebra::quat >
    {
      typedef mln::algebra::quat ret;
    };

    template <>
    struct set_precise_binary_< op::times, mln::algebra::quat, mln::algebra::quat >
    {
      typedef mln::algebra::quat ret;
    };

    // quat OP scalar

    template < typename S >
    struct set_precise_binary_< op::times, mln::algebra::quat, mln::value::scalar_<S> >
    {
      typedef mln::algebra::quat ret;
    };

    template < typename S >
    struct set_precise_binary_< op::div, mln::algebra::quat, mln::value::scalar_<S> >
    {
      typedef mln::algebra::quat ret;
    };


    // 'quat' as a value.


    template <>
    struct value_< mln::algebra::quat >
    {
      typedef trait::value::nature::vectorial nature;
      typedef trait::value::kind::data        kind;
      typedef trait::value::quant::high       quant;

      enum {
	nbits = 4 * sizeof(float),
	card  = 0
      };

      typedef mln::algebra::quat sum;
    };


  } // end of namespace mln::trait



  namespace algebra
  {

    // FIXME value::Vectorial ??? value ???
    /*! \brief Quaternion values.
      \ingroup mlnalgebratypes
    */
    class quat
      :
      public value::Vectorial< quat >
      ,
      public value::internal::value_like_< algebra::vec<4, float>, // Equivalent.
                                    algebra::vec<4, float>, // Encoding.
                                    algebra::vec<4, float>, // Interoperation.
                                    quat >                // Exact.
    {
    public:

      /// Constructor without argument.
      quat();

      /// Constructor with components.
      quat(float s, float x, float y, float z);

      /// Constructor from a scalar and a 3D vector.
      quat(float s, const algebra::vec<3,float>& v);


      /// Constructor from a 4D vector.
      quat(const algebra::vec<4,float>& v);

      /// Assignment from a 4D vector.
      quat& operator=(const algebra::vec<4,float>& v);


      /// \{ Constructors/assignments with literals zero and one.
      quat(const literal::zero_t&);
      quat& operator=(const literal::zero_t&);
      quat(const literal::one_t&);
      quat& operator=(const literal::one_t&);
      /// \}


      /// Explicit conversion to a 4D algebra::vec.
      const algebra::vec<4,float>& to_vec() const;

      /// Implicit conversion to a 4D algebra::vec.
      operator const algebra::vec<4,float>&() const;

      /// Give the scalar part.
      float  s() const;

      /// Access to the scalar part.
      float& s();

      const algebra::vec<3,float>& v() const;
      algebra::vec<3,float>& v();

      void set_v(float x, float y, float z);

      /// Scalar product.
      float sprod(const quat& rhs) const;

      /// Test if it is a unit quaternion.
      bool is_unit() const;

      /// Test if the quaternion is null.
      bool is_null() const;

      /// Test if it is a pure quaternion.
      bool is_pure() const;

      /// Give the conjugate.
      quat conj() const;

      /// Give the invert.
      quat inv() const; // FIXME: rename invert.

      /// Transform into unit quaternion.
      quat& set_unit();

      /// Rotate using quaternion definition of a rotation
      template <unsigned n, typename T>
      algebra::vec<n,float> rotate(const algebra::vec<n,T>& v) const;

      quat rotate(const quat& q) const;

      /// Transform into unit quaternion.
      template <typename T>
      void set_unit(float theta, const algebra::vec<3,T>& uv);

      // only for unit quaternions described by theta and uv such as:
      // q = ( cos(theta), sin(theta) * uv )

      quat(unsigned one, float theta, const algebra::vec<3,float>& uv);

      float theta() const;
      void set_theta(float theta);

      algebra::vec<3,float> uv() const;
      void set_uv(const algebra::vec<3,float>& uv);
    };


    // Operators.

    std::ostream& operator<<(std::ostream& ostr, const quat& q);

    quat operator+(const quat& lhs, const quat& rhs);
    quat operator-(const quat& lhs, const quat& rhs);
    quat operator*(const quat& lhs, const quat& rhs);
    template <typename S> quat operator*(const quat& lhs, const value::scalar_<S>& rhs);
    template <typename S> quat operator/(const quat& lhs, const value::scalar_<S>& rhs);

    // overloaded math procs

    quat log(const quat& q);
    quat exp(const quat& q);
    quat pow(const quat& q, double t);
    template <typename T>
    bool about_equal(const T& f, const T& q);
    bool about_equal(const quat& p, const quat& q);


    // Misc.

    bool interpol_ok(const quat& p, const quat& q, float h);


    // Linear Quaternion Interpolation.

    quat lerp(const quat& p, const quat& q, float h);


    // Spherical Linear Quaternion Interpolation.

    quat slerp(const quat& p, const quat& q, float h);

    quat slerp_2(const quat& p, const quat& q, float h);

    quat slerp_3(const quat& p, const quat& q, float h);

    quat slerp_4(const quat& p, const quat& q, float h);

    quat slerp_5(const quat& p, const quat& q, float h);


    /// \internal Conversion: quaternion -> (angle_degrees, axis).
    template <typename C>
    void from_to_(const quat& from, mln::util::couple<C, algebra::vec<3,C> >& to);

  } // end of namespace mln::algebra


  namespace make
  {

    template <typename C>
    mln::algebra::quat
    quat(double angle, const mln::algebra::vec<3,C>& axis);

  }

# ifndef MLN_INCLUDE_ONLY


  namespace algebra
  {

    // Constructors.

    inline
    quat::quat()
    {
    }

    inline
    quat::quat(float s, float x, float y, float z)
    {
      v_[0] = s;
      set_v(x, y, z);
    }

    inline
    quat::quat(float s, const algebra::vec<3,float>& v)
    {
      v_[0] = s;
      this->v() = v;
    }

    inline
    quat::quat(const algebra::vec<4,float>& v)
    {
      this->v_ = v;
    }

    inline
    quat&
    quat::operator=(const algebra::vec<4,float>& v)
    {
      this->v_ = v;
      return *this;
    }


    // With literals.

    inline
    quat::quat(const literal::zero_t&)
    {
      v_.set_all(0);
    }

    inline
    quat&
    quat::operator=(const literal::zero_t&)
    {
      v_.set_all(0);
      return *this;
    }

    inline
    quat::quat(const literal::one_t&)
    {
      s() = 1;
      v().set_all(0);
    }

    inline
    quat&
    quat::operator=(const literal::one_t&)
    {
      s() = 1;
      v().set_all(0);
      return *this;
    }


    inline
    const algebra::vec<4,float>&
    quat::to_vec() const
    {
      return this->v_;
    }

    inline
    quat::operator const algebra::vec<4,float>&() const
    {
      return this->v_;
    }

    inline
    float
    quat::s() const
    {
      return this->v_[0];
    }

    inline
    float&
    quat::s()
    {
      return this->v_[0];
    }

    inline
    const algebra::vec<3, float>&
    quat::v() const
    {
      return *(const algebra::vec<3, float>*)(const void*)(& this->v_[1]);
      // return make::vec(this->v_[1], this->v_[2], this->v_[3]);
    }

    inline
    algebra::vec<3, float>&
    quat::v()
    {
      return *(algebra::vec<3, float>*)(void*)(& this->v_[1]);
    }

    inline
    void quat::set_v(float x, float y, float z)
    {
      this->v_[1] = x;
      this->v_[2] = y;
      this->v_[3] = z;
    }

    inline
    float
    quat::sprod(const quat& rhs) const
    {
      return v_ * rhs.to_vec();
    }

    inline
    bool quat::is_unit() const
    {
      return about_equal(norm::l2(v_), 1.f);
    }

    inline
    bool quat::is_null() const
    {
      return about_equal(norm::l2(v_), 0.f);
    }

    inline
    bool quat::is_pure() const
    {
      return about_equal(v_[0], 0.f);
    }

    inline
    quat quat::conj() const
    {
      return quat(s(), - v());
    }

    inline
    quat quat::inv() const
    {
      mln_precondition(! is_null());
      float f = norm::l2(v_);
      return conj().to_vec() / (f * f);
    }

    inline
    quat& quat::set_unit()
    {
      if (about_equal(norm::l2(this->to_vec()), 0.f))
        return *this;

      v_.normalize();
      mln_postcondition(this->is_unit());

      return *this;
    }

    template <typename T>
    inline
    void quat::set_unit(float theta, const algebra::vec<3,T>& uv)
    {
      static const float pi = 3.14159265358979323846f;

      mln_precondition(theta > - pi - mln_epsilon(float)
		       && theta < pi + mln_epsilon(float));
      mln_precondition(about_equal(norm::l2(uv), 1.f));
      (void) pi;

      this->v_[0] = std::cos(theta);
      float sint = std::sin(theta);
      this->v_[1] = uv[0] * sint;
      this->v_[2] = uv[1] * sint;
      this->v_[3] = uv[2] * sint;
    }

    // only for unit quaternions described by theta and uv such as:
    // q = ( cos(theta), sin(theta) * uv )

    inline
    quat::quat(unsigned one, float theta, const algebra::vec<3,float>& uv)
    {
      mln_precondition(one == 1);
      (void) one;
      set_unit(theta, uv);
    }

    inline
    float quat::theta() const
    {
      mln_precondition(is_unit());
      return std::acos(s());
    }

    inline
    void quat::set_theta(float theta)
    {
      mln_precondition(is_unit());
      set_unit(theta, uv());
    }

    inline
    algebra::vec<3, float> quat::uv() const
    {
      mln_precondition(is_unit());
      algebra::vec<3, float> w = v();
      return w.normalize();
    }

    inline
    void quat::set_uv(const algebra::vec<3,float>& uv)
    {
      mln_precondition(is_unit());
      set_unit(theta(), uv);
    }

    template <unsigned n, typename T>
    inline
    algebra::vec<n,float>
    quat::rotate(const algebra::vec<n,T>& v) const
    {
      mln_precondition(is_unit());
      return ((*this) * quat(0. ,v) * (*this).inv()).v();
    }

    inline
    quat quat::rotate(const quat& q) const
    {
      mln_precondition(this->is_unit());
      mln_precondition(q.is_pure());
      return (*this) * q * this->inv();
    }


    // Operators.

    inline
    std::ostream& operator<<(std::ostream& ostr, const quat& q)
    {
      return ostr << q.to_vec();
    }

    inline
    quat operator+(const quat& lhs, const quat& rhs)
    {
      quat tmp(lhs.to_vec() + rhs.to_vec());
      return tmp;
    }

    inline
    quat operator-(const quat& lhs, const quat& rhs)
    {
      quat tmp(lhs.to_vec() - rhs.to_vec());
      return tmp;
    }

    inline
    quat operator*(const quat& lhs, const quat& rhs)
    {
      quat tmp(lhs.s() * rhs.s() - lhs.v() * rhs.v(),
	       algebra::vprod(lhs.v(), rhs.v()) + lhs.s() * rhs.v() + rhs.s() * lhs.v());
      return tmp;
    }

    template <typename S>
    inline
    quat operator*(const quat& lhs, const value::scalar_<S>& rhs)
    {
      mlc_converts_to(S, float)::check();
      quat tmp(lhs.to_vec() * rhs.to_equiv());
      return tmp;
    }

    template <typename S>
    inline
    quat operator/(const quat& lhs, const value::scalar_<S>& rhs_)
    {
      mlc_converts_to(S, float)::check();
      float rhs = rhs_.to_equiv();
      mln_precondition(rhs != 0.f);
      quat tmp(lhs.to_vec() / rhs);
      return tmp;
    }


    // overloaded math procs

    inline
    quat log(const quat& q)
    {
      mln_precondition(q.is_unit());
      return quat(0.f, q.theta() * q.uv());
    }


    inline
    quat exp(const quat& q)
    {
      mln_precondition(about_equal(q.s(), 0.f));
      algebra::vec<3, float> v = q.v();
      float theta = norm::l2(v);
      mln_precondition(!about_equal(theta, 0.f));
      algebra::vec<3, float> uv = v / theta;
      return quat(std::cos(theta), std::sin(theta) * uv);
    }


    inline
    quat pow(const quat& q, double t)
    {
      mln_precondition(q.is_unit());
      return exp(t * log(q));
    }

    template <typename T>
    inline
    bool about_equal(const T& f, const T& q)
    {
      return math::abs(q - f) <= mln_epsilon(T);
    }

    inline
    bool about_equal(const quat& p, const quat& q)
    {
      return about_equal<float>(norm::l2(p.to_vec() - q.to_vec()), 0);
    }

    // Misc.

    inline
    bool interpol_ok(const quat& p, const quat& q, float h)
    {
      return
	p.is_unit() &&
	q.is_unit() &&
	h >= 0 &&
	h <= 1;
    }


    // Linear Quaternion Interpolation.

    inline
    quat lerp(const quat& p, const quat& q, float h)
    {
      assert(interpol_ok(p, q, h));
      return (1 - h) * p + h * q;
    }


    // Spherical Linear Quaternion Interpolation.

    inline
    quat slerp(const quat& p, const quat& q, float h)
    {
      assert(interpol_ok(p, q, h));
      float omega = std::acos(p.sprod(q));
      return
	about_equal(omega, 0.f) ?
	lerp(p, q, h) :
	quat((std::sin((1-h)*omega) * p + std::sin(h*omega) * q) / std::sin(omega));
    }

    inline
    quat slerp_2(const quat& p, const quat& q, float h)
    {
      assert(interpol_ok(p, q, h));
      quat tmp = p * pow(p.conj() * q, h);
      assert(about_equal(tmp, slerp(p, q, h)));
      return tmp;
    }

    inline
    quat slerp_3(const quat& p, const quat& q, float h)
    {
      assert(interpol_ok(p, q, h));
      quat tmp = pow(p * q.conj(), 1 - h) * q;
      assert(about_equal(tmp, slerp(p, q, h)));
      return tmp;
    }

    inline
    quat slerp_4(const quat& p, const quat& q, float h)
    {
      assert(interpol_ok(p, q, h));
      quat tmp = pow(q * p.conj(), h) * p;
      assert(about_equal(tmp, slerp(p, q, h)));
      return tmp;
    }

    inline
    quat slerp_5(const quat& p, const quat& q, float h)
    {
      assert(interpol_ok(p, q, h));
      quat tmp = q * pow(q.conj() * p, 1 - h);
      assert(about_equal(tmp, slerp(p, q, h)));
      return tmp;
    }


    // Conversions.

    template <typename C>
    void from_to_(const quat& from, mln::util::couple<C, algebra::vec<3,C> >& to)
    {
      quat tmp = from;
      tmp.set_unit();

      C	w  = tmp.to_vec()[0],
	angle      = std::acos(w) * 2 * 180/math::pi;

      C sa = std::sin(std::acos(w));
      if (std::fabs( sa ) < 0.0005 )
	sa = 1;

      to.first() = angle;
      to.second()[0] = tmp.to_vec()[1] / sa;
      to.second()[1] = tmp.to_vec()[2] / sa;
      to.second()[2] = tmp.to_vec()[3] / sa;
    }

  } // end of namespace mln::algebra


  namespace make
  {

    template <typename C>
    mln::algebra::quat
    quat(double angle, const mln::algebra::vec<3,C>& axis)
    {
      angle *= mln::math::pi/180;
      C s = std::sin(angle / 2);

      C x = axis[0] * s,
	y = axis[1] * s,
	z = axis[2] * s,
	w = std::cos(angle / 2);

      return mln::algebra::quat(w, x, y, z);
    }

  } // end of namespace mln::make


# endif // ! MLN_INCLUDE_ONLY

} // end of namespace mln

#endif // ! MLN_ALGEBRA_QUAT_HH
