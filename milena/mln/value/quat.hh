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

#ifndef MLN_VALUE_QUAT_HH
# define MLN_VALUE_QUAT_HH

/*! \file mln/value/quat.hh
 *
 * \brief Define a class for quaternion values.
 */

# include <cmath>

# include <mln/value/ops.hh>

# include <mln/value/concept/vectorial.hh>
# include <mln/value/internal/value_like.hh>
# include <mln/value/props.hh>

# include <mln/metal/vec.hh>
# include <mln/norm/l2.hh>


namespace mln
{

  // Fwd decls.
  namespace value { class quat; }
  namespace literal { struct zero_t; struct one_t; }


  namespace trait
  {

    // quat OP quat

    template <>
    struct set_precise_binary_< op::plus, mln::value::quat, mln::value::quat >
    {
      typedef mln::value::quat ret;
    };

    template <>
    struct set_precise_binary_< op::minus, mln::value::quat, mln::value::quat >
    {
      typedef mln::value::quat ret;
    };

    template <>
    struct set_precise_binary_< op::times, mln::value::quat, mln::value::quat >
    {
      typedef mln::value::quat ret;
    };

    // quat OP scalar

    template < typename S >
    struct set_precise_binary_< op::times, mln::value::quat, mln::value::scalar_<S> >
    {
      typedef mln::value::quat ret;
    };

    template < typename S >
    struct set_precise_binary_< op::div, mln::value::quat, mln::value::scalar_<S> >
    {
      typedef mln::value::quat ret;
    };

  } // end of namespace mln::trait



  namespace value
  {

    // FIXME doesn't compile

    class quat
      :
      public Vectorial< quat >
      ,
      public internal::value_like_< metal::vec<4, float>, // Equivalent.
				    metal::vec<4, float>, // Encoding.
				    metal::vec<4, float>, // Interoperation.
				    quat >                // Exact.
    {
    public:

      /// Constructor without argument.
      quat();

      /// Constructor with components.
      quat(float s, float x, float y, float z);

      /// Constructor from a scalar and a 3D vector.
      quat(float s, const metal::vec<3,float>& v);


      /// Constructor from a 4D vector.
      quat(const metal::vec<4,float>& v);

      /// Assignment from a 4D vector.
      quat& operator=(const metal::vec<4,float>& v);


      /// \{ Constructors/assignments with literals zero and one.
      quat(const literal::zero_t&);
      quat& operator=(const literal::zero_t&);
      quat(const literal::one_t&);
      quat& operator=(const literal::one_t&);
      /// \}


      /// Explicit conversion to a 4D metal::vec.
      const metal::vec<4,float>& to_vec() const;


      /// Give the scalar part.
      float  s() const;

      /// Access to the scalar part.
      float& s();

      const metal::vec<3,float>& v() const;
      metal::vec<3,float>& v();

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
      quat inv() const; // FIXME: Rename inv as invert. 

      /// Transform into unit quaternion.
      quat& set_unit();

      /// Transform into unit quaternion.
      template <typename T>
      void set_unit(float theta, const metal::vec<3,T>& uv);

      // only for unit quaternions described by theta and uv such as:
      // q = ( cos(theta), sin(theta) * uv )

      quat(unsigned one, float theta, const metal::vec<3,float>& uv);

      float theta() const;
      void set_theta(float theta);

      metal::vec<3,float> uv() const;
      void set_uv(const metal::vec<3,float>& uv);
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



# ifndef MLN_INCLUDE_ONLY

    // Constructors.

    quat::quat()
    {
    }

    quat::quat(float s, float x, float y, float z)
    {
      v_[0] = s;
      set_v(x, y, z);
    }

    quat::quat(float s, const metal::vec<3,float>& v)
    {
      v_[0] = s;
      this->v() = v;
    }

    quat::quat(const metal::vec<4,float>& v)
    {
      this->v_ = v;
    }

    quat&
    quat::operator=(const metal::vec<4,float>& v)
    {
      this->v_ = v;
      return *this;
    }


    // With literals.

    quat::quat(const literal::zero_t&)
    {
      v_.set_all(0);
    }

    quat&
    quat::operator=(const literal::zero_t&)
    {
      v_.set_all(0);
      return *this;
    }

    quat::quat(const literal::one_t&)
    {
      s() = 1;
      v().set_all(0);
    }

    quat&
    quat::operator=(const literal::one_t&)
    {
      s() = 1;
      v().set_all(0);
      return *this;
    }


    const metal::vec<4,float>&
    quat::to_vec() const
    {
      return this->v_;
    }

    float
    quat::s() const
    {
      return this->v_[0];
    }

    float&
    quat::s()
    {
      return this->v_[0];
    }

    const metal::vec<3, float>&
    quat::v() const
    {
      return *(const metal::vec<3, float>*)(const void*)(& this->v_[1]);
      // return make::vec(this->v_[1], this->v_[2], this->v_[3]);
    }

    metal::vec<3, float>&
    quat::v()
    {
      return *(metal::vec<3, float>*)(void*)(& this->v_[1]);
    }

    void quat::set_v(float x, float y, float z)
    {
      this->v_[1] = x;
      this->v_[2] = y;
      this->v_[3] = z;
    }

    float
    quat::sprod(const quat& rhs) const
    {
      return v_ * rhs.to_vec();
    }

    bool quat::is_unit() const
    {
      return about_equal(norm::l2(v_), 1.f);
    }

    bool quat::is_null() const
    {
      return about_equal(norm::l2(v_), 0.f);
    }

    bool quat::is_pure() const
    {
      return about_equal(v_[0], 0.f);
    }

    quat quat::conj() const
    {
      return quat(s(), - v());
    }

    quat quat::inv() const
    {
      assert(! is_null());
      float f = norm::l2(v_);
      return conj().to_vec() / (f * f);
    }

    quat& quat::set_unit()
    {
      v_.normalize();
      return *this;
    }

    template <typename T>
    void quat::set_unit(float theta, const metal::vec<3,T>& uv)
    {
      static const float pi = 3.14159265358979323846;

      mln_precondition(theta > - pi - props<float>::epsilon()
		       && theta < pi + props<float>::epsilon());
      mln_precondition(about_equal(norm::l2(uv), 1.f));

      this->v_[0] = cos(theta);
      float sint = sin(theta);
      this->v_[1] = uv[0] * sint;
      this->v_[2] = uv[1] * sint;
      this->v_[3] = uv[2] * sint;
    }

    // only for unit quaternions described by theta and uv such as:
    // q = ( cos(theta), sin(theta) * uv )

    quat::quat(unsigned one, float theta, const metal::vec<3,float>& uv)
    {
      mln_precondition(one == 1);
      set_unit(theta, uv);
    }

    float quat::theta() const
    {
      mln_precondition(is_unit());
      return acos(s());
    }

    void quat::set_theta(float theta)
    {
      mln_precondition(is_unit());
      set_unit(theta, uv());
    }

    metal::vec<3, float> quat::uv() const
    {
      mln_precondition(is_unit());
      metal::vec<3, float> w = v();
      return w.normalize();
    }

    void quat::set_uv(const metal::vec<3,float>& uv)
    {
      mln_precondition(is_unit());
      set_unit(theta(), uv);
    }


    // Operators.

    std::ostream& operator<<(std::ostream& ostr, const quat& q)
    {
      return ostr << q.to_vec();
    }

    quat operator+(const quat& lhs, const quat& rhs)
    {
      quat tmp(lhs.to_vec() + rhs.to_vec());
      return tmp;
    }

    quat operator-(const quat& lhs, const quat& rhs)
    {
      quat tmp(lhs.to_vec() - rhs.to_vec());
      return tmp;
    }

    quat operator*(const quat& lhs, const quat& rhs)
    {
      quat tmp(lhs.s() * rhs.s() - lhs.v() * rhs.v(),
	       metal::vprod(lhs.v(), rhs.v()) + lhs.s() * rhs.v() + rhs.s() * lhs.v());
      return tmp;
    }

    template <typename S>
    quat operator*(const quat& lhs, const value::scalar_<S>& rhs)
    {
      mlc_converts_to(S, float)::check();
      quat tmp(lhs.to_vec() * float(rhs));
      return tmp;
    }

    template <typename S>
    quat operator/(const quat& lhs, const value::scalar_<S>& rhs_)
    {
      mlc_converts_to(S, float)::check();
      float rhs = float(rhs_);
      mln_precondition(rhs != 0.f);
      quat tmp(lhs.to_vec() / rhs);
      return tmp;
    }


    // overloaded math procs

    quat log(const quat& q)
    {
      mln_precondition(q.is_unit());
      return quat(0.f, q.theta() * q.uv());
    }


    quat exp(const quat& q)
    {
      mln_precondition(about_equal(q.s(), 0.f));
      metal::vec<3, float> v = q.v();
      float theta = norm::l2(v);
      mln_precondition(!about_equal(theta, 0.f));
      metal::vec<3, float> uv = v / theta;
      return quat(cos(theta), sin(theta) * uv);
    }


    quat pow(const quat& q, double t)
    {
      mln_precondition(q.is_unit());
      return exp(t * log(q));
    }

    template <typename T>
    bool about_equal(const T& f, const T& q)
    {
      if (f > q)
	return (f - q ) < props<T>::epsilon();
      return (q - f) < props<T>::epsilon();
    }

    bool about_equal(const quat& p, const quat& q)
    {
      return about_equal<float>(norm::l2(p.to_vec() - q.to_vec()), 0);
    }

    // Misc.

    bool interpol_ok(const quat& p, const quat& q, float h)
    {
      return
	p.is_unit() &&
	q.is_unit() &&
	h >= 0 &&
	h <= 1;
    }


    // Linear Quaternion Interpolation.

    quat lerp(const quat& p, const quat& q, float h)
    {
      assert(interpol_ok(p, q, h));
      return (1 - h) * p + h * q;
    }


    // Spherical Linear Quaternion Interpolation.

    quat slerp(const quat& p, const quat& q, float h)
    {
      assert(interpol_ok(p, q, h));
      float omega = acos(p.sprod(q));
      return
	about_equal(omega, 0.f) ?
	lerp(p, q, h) :
	quat((sin((1-h)*omega) * p + sin(h*omega) * q) / sin(omega));
    }

    quat slerp_2(const quat& p, const quat& q, float h)
    {
      assert(interpol_ok(p, q, h));
      quat tmp = p * value::pow(p.conj() * q, h);
      assert(about_equal(tmp, slerp(p, q, h)));
      return tmp;
    }

    quat slerp_3(const quat& p, const quat& q, float h)
    {
      assert(interpol_ok(p, q, h));
      quat tmp = value::pow(p * q.conj(), 1 - h) * q;
      assert(about_equal(tmp, slerp(p, q, h)));
      return tmp;
    }

    quat slerp_4(const quat& p, const quat& q, float h)
    {
      assert(interpol_ok(p, q, h));
      quat tmp = value::pow(q * p.conj(), h) * p;
      assert(about_equal(tmp, slerp(p, q, h)));
      return tmp;
    }

    quat slerp_5(const quat& p, const quat& q, float h)
    {
      assert(interpol_ok(p, q, h));
      quat tmp = q * value::pow(q.conj() * p, 1 - h);
      assert(about_equal(tmp, slerp(p, q, h)));
      return tmp;
    }


# endif // ! MLN_INCLUDE_ONLY

  } // end of namespace mln::value

} // end of namespace mln

#endif // ! MLN_VALUE_QUAT_HH
