// Copyright (C) 2007, 2008, 2009, 2010, 2011, 2012 EPITA Research and
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

#ifndef MLN_FUN_X2X_ROTATION_HH
# define MLN_FUN_X2X_ROTATION_HH

/// \file
///
/// \brief Define a rotation function.
///
/// \todo store the quaternion instead of (axis, alpha)
///	  => better precision while composing two rotation matrices.
///
/// FIXME: Find a better reference...
/// Conversion from Quaternion to (angle,axis), Source:
///   http://jeux.developpez.com/faq/matquat/?page=quaternions#Q56

# include <cstdlib>
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
	// (Axis, angle)-based rotation: general case (not implemented).
	template < unsigned n, typename C >
	algebra::h_mat<n, C>
	get_rot_h_mat(const C alpha_, const algebra::vec<n,C>& axis_)
	{
	  (void) alpha_;
	  (void) axis_;

	  std::cerr
	    << __FILE__ << ":" << __LINE__ << ": error:"
	    << " generic mln::fun::x2x::internal::get_rot_h_mat<n, C>"
	    << " not implemented."
	    << std::endl;
	  std::abort();
	}

	// (Angle)-based rotation: 2D case.
	template <typename C >
	algebra::h_mat<2, C>
	get_rot_h_mat(const C alpha, const algebra::vec<2,C>& /* unused */)
	{
	  const C cos_a = cos(alpha);
	  const C sin_a = sin(alpha);

	  algebra::h_mat<2, C> m;

	  m(0,0) = cos_a; m(0,1) = -sin_a; m(0,2) = 0;
	  m(1,0) = sin_a; m(1,1) = cos_a;  m(1,2) = 0;
	  m(2,0) = 0;     m(2,1) = 0;      m(2,2) = 1;

	  return m;
	}

	// (Axis, angle)-based rotation: 3D case.
	template <typename C >
	algebra::h_mat<3, C>
	get_rot_h_mat(const C alpha, const algebra::vec<3,C>& axis)
	{
	  // Ensure axis is valid.
	  typedef algebra::vec<3,C> vec_t;
	  // FIXME: This check is not precise enough when the vector
	  // holds floating point values.
	  mln_precondition(axis != vec_t(literal::zero));

	  algebra::vec<3,C> normed_axis = axis;
	  normed_axis.normalize();

	  const C cos_a = cos(alpha);
	  const C sin_a = sin(alpha);
	  const C u = normed_axis[0];
	  const C v = normed_axis[1];
	  const C w = normed_axis[2];
	  const C u2 = u * u;
	  const C v2 = v * v;
	  const C w2 = w * w;

	  algebra::h_mat<3, C> m;

	  m(0,0) = u2 + (1 - u2) * cos_a;
	  m(0,1) = u * v * (1 - cos_a) - w * sin_a;
	  m(0,2) = u * w * (1 - cos_a) + v * sin_a;
	  m(0,3) = 0;

	  m(1,0) = u * v * (1 - cos_a) + w * sin_a;
	  m(1,1) = v2 + (1 - v2) * cos_a;
	  m(1,2) = v * w * (1 - cos_a) - u * sin_a;
	  m(1,3) = 0;

	  m(2,0) = u * w * (1 - cos_a) - v * sin_a;
	  m(2,1) = v * w * (1 - cos_a) + u * sin_a;
	  m(2,2) = w2 + (1 - w2) * cos_a;
	  m(2,3) = 0;

	  m(3,0) = 0;
	  m(3,1) = 0;
	  m(3,2) = 0;
	  m(3,3) = 1;

	  return m;
	}

      } // end of namespace internal


      /// Represent a rotation function.
      template <unsigned n, typename C>
      struct rotation
	: fun::internal::x2x_linear_impl_< algebra::vec<n,C>, C, rotation<n,C> >,
	  public Function_v2v< rotation<n,C> >
      {
	/// Type of the underlying data stored in vectors and matrices.
	typedef C data_t;

	/// Type of the inverse function.
	typedef rotation<n,C> invert;
	/// Return the inverse function.
	invert inv() const;

	/// Constructor without argument.
	rotation();
	/// Constructor with radian alpha and an optional direction
	/// (rotation axis).
	rotation(const C& alpha, const algebra::vec<n,C>& axis = literal::zero);
	/// Constructor with quaternion
	rotation(const algebra::quat& q);
	/// Constructor with h_mat.
	rotation(const algebra::h_mat<n,C>& m);

	/// Perform the rotation of the given vector.
	algebra::vec<n,C> operator()(const algebra::vec<n,C>& v) const;

      protected:
	void update(const C& alpha, const algebra::vec<n,C>& axis);
	bool check_rotation(const algebra::quat& q);

      };


# ifndef MLN_INCLUDE_ONLY


      template <unsigned n, typename C>
      inline
      rotation<n,C>::rotation()
      {
      }

      template <unsigned n, typename C>
      inline
      rotation<n,C>::rotation(const C& alpha, const algebra::vec<n,C>& axis)
      {
	this->m_ = algebra::h_mat<n,C>::Id;
	update(alpha, axis);
      }

      template <unsigned n, typename C>
      inline
      rotation<n,C>::rotation(const algebra::quat& q)
      {
	// FIXME: Should also work for 2D.
	mlc_bool(n == 3)::check();
	mln_precondition(q.is_unit());

	this->m_ = mln::make::h_mat(C(), q);
	mln_assertion(check_rotation(q));
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
	typename rotation::invert res(this->m_._1());
	return res;
      }

      // Homogenous matrix for a rotation of a point (x,y,z)
      // about the vector (u,v,w) by the angle alpha.
      template <unsigned n, typename C>
      inline
      void
      rotation<n,C>::update(const C& alpha, const algebra::vec<n,C>& axis)
      {
	this->m_ = internal::get_rot_h_mat(alpha, axis);
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
	return norm::l2(p_rot_1 - p_rot_2) < mln_epsilon(C);
      }

# endif // ! MLN_INCLUDE_ONLY


    } // end of namespace mln::fun::x2x

  } // end of namespace mln::fun

} // end of namespace mln


#endif // ! MLN_FUN_X2X_ROTATION_HH
