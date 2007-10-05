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

# include <cmath>

# include <mln/metal/vec.hh>
# include <mln/norm/l2.hh>
# include <mln/value/props.hh>
# include <mln/value/internal/vectoriel.hh>

namespace mln
{

  namespace value
  {

    //FIXME doesn't compile
    class quat :// public internal::Vectoriel< quat >,
		 public metal::vec<4, float>
    {
      typedef metal::vec<4, float> super_;
      using super_::data_;

    public:

      /// Encoding associated type.
      typedef float enc;

      /// Equivalent associated type.
      typedef float equiv[4];

      // ctors

      quat();
      quat(float s, float x, float y, float z);

      template <typename T>
      quat(float s, const metal::vec<3,T>& v);

      template <typename T>
      quat(const metal::vec<4,T>& v);

      // accessors/modifiers as a 'scalar+metal::vec<3>'

      float scal() const;
      void set_scal(float s);

      metal::vec<3,float> vect() const;
      void set_vect(float x, float y, float z);
      template <typename T>
      void set_vect(const metal::vec<3,T>& v);

      // multiplication

      quat operator*(const quat& rhs) const;

      // tests

      bool is_unit() const;
      bool is_null() const;

      // conjugate and invert

      quat conj() const;
      quat inv() const;

      // transform into unit quaternion

      quat& set_unit();
      template <typename T>
      void set_unit(float theta, const metal::vec<3,T>& uv);

      // only for unit quaternions described by theta and uv such as:
      // q = ( cos(theta), sin(theta) * uv )

      template <typename T>
      quat(unsigned one, float theta, const metal::vec<3, T>& uv);

      float theta() const;
      void set_theta(float theta);

      metal::vec<3, float> uvect() const;
      template <typename T>
      void set_uvect(const metal::vec<3,T>& uv);

    };

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

    //ctors


    quat::quat()
    {
    }

    quat::quat(float s, float x, float y, float z)
    {
      set_scal(s);
      set_vect(x, y, z);
    }

    template <typename T>
    quat::quat(float s, const metal::vec<3,T>& v)
    {
      set_scal(s);
      set_vect(v);
    }

    template <typename T>
    quat::quat(const metal::vec<4,T>& v)
      : metal::vec<4,float>(v)
    {
    }


    // accessors/modifiers as a 'scalar+vec<3>'


    float quat::scal() const
    {
      return data_[0];
    }


    void quat::set_scal(float s)
    {
      data_[0] = s;
    }


    metal::vec<3, float> quat::vect() const
    {
      return make::vec(data_[1], data_[2], data_[3]);
    }


    void quat::set_vect(float x, float y, float z)
    {
      data_[1] = x;
      data_[2] = y;
      data_[3] = z;
    }

    template <typename T>
    void quat::set_vect(const metal::vec<3,T>& v)
    {
      set_vect(v[0], v[1], v[2]);
    }


    // multiplication


    quat quat::operator*(const quat& rhs) const
    {
      float
	s1 = scal(),
	s2 = rhs.scal();

      metal::vec<3,float>
	v1 = vect(),
	v2 = rhs.vect();

      return quat(s1 * s2 - v1.sprod(v2),
		  metal::vprod(v1, v2)
		  + s1 * v2 + s2 * v1);
    }


    // tests


    bool quat::is_unit() const
    {
      return about_equal(norm::l2(*this), 1.f);
    }


    bool quat::is_null() const
    {
      return about_equal(norm::l2(*this), 0.f);
    }


    // conjugate and invert


    quat quat::conj() const
    {
      return quat(scal(), - vect());
    }


    quat quat::inv() const
    {
      assert(! is_null());
      float f = norm::l2(*this);

      return conj() / (f * f);
    }


    // transform into unit quaternion


    quat& quat::set_unit()
    {
      normalize();
      return *this;
    }


    template <typename T>
    void quat::set_unit(float theta, const metal::vec<3,T>& uv)
    {
      static const float pi = 3.14159265358979323846;

      mln_precondition(theta > - pi - props<float>::epsilon()
		       && theta < pi + props<float>::epsilon());
      mln_precondition(about_equal(norm::l2(uv), 1.f));

      data_[0] = cos(theta);
      float sint = sin(theta);
      data_[1] = uv[0] * sint;
      data_[2] = uv[1] * sint;
      data_[3] = uv[2] * sint;
    }

    // only for unit quaternions described by theta and uv such as:
    // q = ( cos(theta), sin(theta) * uv )


    template <typename T>
    quat::quat(unsigned one, float theta, const metal::vec<3,T>& uv)
    {
      mln_precondition(one == 1);
      set_unit(theta, uv);
    }


    float quat::theta() const
    {
      mln_precondition(is_unit());
      return acos(scal());
    }


    void quat::set_theta(float theta)
    {
      mln_precondition(is_unit());
      set_unit(theta, uvect());
    }

    metal::vec<3, float> quat::uvect() const
    {
      mln_precondition(is_unit());
      metal::vec<3, float> v = vect();
      return v.normalize();
    }

    template <typename T>
    void quat::set_uvect(const metal::vec<3,T>& uv)
    {
      mln_precondition(is_unit());
      set_unit(theta(), uv);
    }


    // overloaded math procs


    quat log(const quat& q)
    {
      mln_precondition(q.is_unit());
      return quat(0.f, q.theta() * q.uvect());
    }


    quat exp(const quat& q)
    {
      mln_precondition(about_equal(q.scal(), 0.f));
      metal::vec<3, float> v = q.vect();
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
      return about_equal<float>(norm::l2(p - q), 0);
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
