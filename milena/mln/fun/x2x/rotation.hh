// Copyright (C) 2007, 2008, 2009 EPITA Research and Development
// Laboratory (LRDE)
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

#ifndef MLN_FUN_X2X_ROTATION_HH
# define MLN_FUN_X2X_ROTATION_HH

/// \file mln/fun/x2x/rotation.hh
///
/// Define a rotation function.
///
/// \todo store the quaternion instead of (axis, alpha)
///	  => better precision while composing two rotation matrices.
///
/// Conversion from Quaternion to (angle,axis), Source:
///   http://jeux.developpez.com/faq/matquat/?page=quaternions#Q56

# include <cmath>
# include <mln/core/concept/function.hh>
# include <mln/fun/internal/x2x_linear_impl.hh>
# include <mln/algebra/vec.hh>
# include <mln/algebra/mat.hh>
# include <mln/algebra/quat.hh>
# include <mln/make/h_mat.hh>

# include <mln/norm/l2.hh>

namespace mln
{

  namespace fun
  {

    namespace x2x
    {

      namespace internal
      {
        template < unsigned n, typename C >
        algebra::h_mat<n, C>
        get_rot_h_mat(const float alpha_, const algebra::vec<n,C>& axis_)
        {
          mln_assertion(!"get_h_mat : n not implemented");
        }

        template <typename C >
        algebra::h_mat<3, C>
        get_rot_h_mat(const float alpha_, const algebra::vec<3,C>& axis_)
        {
	  //test axis is valid
	  typedef algebra::vec<3,C> vec_t;
	  //FIXME: cannot use '!=' operator.
          mln_precondition(!(axis_ == vec_t(literal::zero)));

	  /// Be sure that axis is normalized.
	  algebra::vec<3,C> axis = axis_;
	  axis.normalize();

          const float cos_a = cos(alpha_);
          const float sin_a = sin(alpha_);
          const float u = axis_[0];
          const float v = axis_[1];
          const float w = axis_[2];
          const float u2 = u * u;
          const float v2 = v * v;
          const float w2 = w * w;

          algebra::h_mat<3, C> m_;

	  m_(0,0) = u2 + (1 - u2) * cos_a;
          m_(0,1) = u*v * (1 - cos_a) - w * sin_a;
          m_(0,2) = u*w * (1 - cos_a) + v * sin_a;
          m_(0,3) = 0;

          m_(1,0) = u*v * (1 - cos_a) + w * sin_a;
          m_(1,1) = v2 + (1 - v2) * cos_a;
          m_(1,2) = v * w * (1 - cos_a) - u * sin_a;
          m_(1,3) = 0;

          m_(2,0) = u * w * (1 - cos_a) - v * sin_a;
          m_(2,1) = u * w * (1 - cos_a) + u * sin_a;
          m_(2,2) = w2 + (1 - w2) * cos_a;
          m_(2,3) = 0;

          m_(3,0) = 0;
          m_(3,1) = 0;
          m_(3,2) = 0;
          m_(3,3) = 1;

          return m_;
        }

        template <typename C >
        algebra::h_mat<2, C>
        get_rot_h_mat(const float alpha_, const algebra::vec<2,C>&)
        {
          const float cos_a = cos(alpha_);
          const float sin_a = sin(alpha_);

          algebra::h_mat<2, C> m_;

          m_(0,0) = cos_a; m_(0,1) = -sin_a; m_(0,2) = 0;
          m_(1,0) = sin_a; m_(1,1) = cos_a;  m_(1,2) = 0;
          m_(2,0) = 0;     m_(2,1) = 0;      m_(2,2) = 1;

          return m_;
        }

      } // end of namespace internal


      /// Represent a rotation function.
      template <unsigned n, typename C>
      struct rotation
	: fun::internal::x2x_linear_impl_< algebra::vec<n,C>, rotation<n,C> >
	, public Bijection_x2x< rotation<n,C> >
      {
	typedef fun::internal::x2x_linear_impl_< algebra::vec<n,C>, rotation<n,C> > super_;

	/// Type of the inverse function.
	typedef rotation<n,C> invert;
	/// Return the invere function.
	invert inv() const;

	/// Constructor without argument.
        rotation();
	/// Constructor with grade alpha and a facultative direction (rotation axis).
        rotation(float alpha, const algebra::vec<n,float>& axis);
        /// Constructor with quaternion
        rotation(const algebra::quat& q);
	/// Constructor with h_mat.
        rotation(const algebra::h_mat<n,C>& m);

        using super_::operator();
	/// Perform the rotation of the given vector.
        algebra::vec<n,C> operator()(const algebra::vec<n,C>& v) const;

	/// Set a new grade alpha.
        void set_alpha(float alpha);
	/// Set a new rotation axis.
        void set_axis(const algebra::vec<n,float>& axis);

      protected:
        void update();
	bool check_rotation(const algebra::quat& q);


	/// FIXME: Is it useful?
        float alpha_;
        algebra::vec <n,float> axis_;
      };


# ifndef MLN_INCLUDE_ONLY

      template <unsigned n, typename C>
      inline
      rotation<n,C>::rotation()
      {
      }

      template <unsigned n, typename C>
      inline
      rotation<n,C>::rotation(float alpha, const algebra::vec<n,float>& axis)
	:alpha_(alpha),
	 axis_(axis)
      {
	this->m_ = algebra::h_mat<n,C>::Id;
	update();
      }

      template <unsigned n, typename C>
      inline
      rotation<n,C>::rotation(const algebra::quat& q)
      {
        // FIXME: Should also work for 2d.
        mlc_bool(n == 3)::check();
        mln_precondition(q.is_unit());

        float
          w = q.to_vec()[0],
          x = q.to_vec()[1],  x2 = 2*x*x,  xw = 2*x*w,
          y = q.to_vec()[2],  y2 = 2*y*y,  xy = 2*x*y,  yw = 2*y*w,
          z = q.to_vec()[3],  z2 = 2*z*z,  xz = 2*x*z,  yz = 2*y*z,  zw = 2*z*w;

        float t[9] = {1.f - y2 - z2,  xy - zw,  xz + yw,
                      xy + zw,  1.f - x2 - z2,  yz - xw,
                      xz - yw,  yz + xw,  1.f - x2 - y2};

        this->m_ = mln::make::h_mat(t);
	mln_assertion(check_rotation(q));

	/// Update attributes

	alpha_ = acos(w) * 2;
	axis_[0] = x;
	axis_[1] = y;
	axis_[2] = z;
	axis_.normalize();
      }


      template <unsigned n, typename C>
      inline
      rotation<n,C>::rotation(const algebra::h_mat<n,C>& m)
      {
	this->m_ = m;
      }


      template <unsigned n, typename C>
      inline
      algebra::vec<n,C>
      rotation<n,C>::operator()(const algebra::vec<n,C>& v) const
      {
	algebra::mat<n+1,1,C> hmg;
	algebra::mat<n+1,1,C> tmp;
	algebra::vec<n,C> res;
	for (unsigned i = 0; i < n; ++i)
	  hmg(i,0) = v[i];
	hmg(n,0) = 1;
	tmp = this->m_ * hmg;
	mln_assertion(tmp(n,0) == 1);
	for (unsigned i = 0; i < n; ++i)
	  res[i] = tmp(i,0);
	return res;
      }

      template <unsigned n, typename C>
      inline
      rotation<n,C>
      rotation<n,C>::inv() const
      {
	typename rotation::invert res(-alpha_, axis_);
	return res;
      }

      template <unsigned n, typename C>
      inline
      void
      rotation<n,C>::set_alpha(float alpha)
      {
	alpha_ = alpha;
	update();
      }

      template <unsigned n, typename C>
      inline
      void
      rotation<n,C>::set_axis(const algebra::vec<n,float>& axis)
      {
        axis_ = axis;
	update();
      }

      // Homogenous matrix for a rotation of a point (x,y,z)
      // about the vector (u,v,w) by the angle alpha
      template <unsigned n, typename C>
      inline
      void
      rotation<n,C>::update()
      {
        this->m_ = internal::get_rot_h_mat(alpha_, axis_);
      }

      template <unsigned n, typename C>
      inline
      bool
      rotation<n,C>::check_rotation(const algebra::quat& q)
      {
	srand(time(0));
	assert(q.is_unit());

	algebra::vec<n,C>
	  tmp = make::vec(rand(), rand(), rand()),
	      p = tmp / norm::l2(tmp),
	      p_rot_1 = q.rotate(p),
	      p_rot_2 = (*this)(p);
	return norm::l2(p_rot_1 - p_rot_2) < mln_epsilon(float);
      }

# endif // ! MLN_INCLUDE_ONLY


    } // end of namespace mln::fun::x2x

  } // end of namespace mln::fun

} // end of namespace mln


#endif // ! MLN_FUN_X2X_ROTATION_HH
