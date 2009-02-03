// Copyright (C) 2009 EPITA Research and Development Laboratory (LRDE)
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

#ifndef MLN_REGISTRATION_ICP_HH
# define MLN_REGISTRATION_ICP_HH

/// \file mln/registration/icp.hh
///
/// Register an image over an another using the ICP algorithm.

# include <mln/core/alias/vec3d.hh>
# include <mln/math/jacobi.hh>
# include <mln/fun/x2x/all.hh>
# include <mln/fun/x2v/all.hh>
# include <mln/convert/to.hh>
# include <mln/accu/compute.hh>
# include <mln/accu/center.hh>
# include <mln/accu/rms.hh>
# include <mln/trait/image_from_grid.hh>
# include <mln/set/compute.hh>

//Should be removed when closest_point functors are moved.
# include <mln/core/alias/neighb3d.hh>
# include <mln/transform/internal/closest_point_functor.hh>
# include <mln/canvas/distance_geodesic.hh>
# include <mln/pw/all.hh>

namespace mln
{


  namespace registration
  {

    using namespace fun::x2x;

    /*! Register point in \p c using a function of closest points
     * \p closest_point.
     *
     * \param[in] data_P The cloud of points.
     * \param[in] model_X the reference surface.
     * \param[in] closest_point The function of closest points.
     * \param[out] qk The rigid transformation obtained.
     *
     * \return the rigid transformation which may be use later to create
     * a registered image.
     *
     * WARNING: the function \p closest_point *MUST* take float/double
     * vector as arguments. Otherwise the resulting transformation may be
     * wrong due to the truncation of the vector coordinate values.
     *
     * \pre \p data_p and \p model_X must not be empty.
     *
     * Reference article: "A Method for Registration of 3-D Shapes", Paul J.
     * Besl and Neil D. McKay, IEEE, 2, February 1992.
     *
     */
    template <typename P, typename F>
    composed< rotation<P::dim,float>,translation<P::dim,float> >
    icp(const p_array<P>& data_P,
	const p_array<P>& model_X,
	const F& closest_point,
	const algebra::quat& initial_rot,
	const mln_vec(P)& initial_translation);

    template <typename P, typename F>
    composed< rotation<P::dim,float>,translation<P::dim,float> >
    icp(const p_array<P>& data_P,
	const p_array<P>& model_X,
	const F& closest_point);

# ifndef MLN_INCLUDE_ONLY

    /// Closest point functor based on map distance.
    template <typename P>
    class cp_with_map_t
    {
      typedef mln_image_from_grid(mln_grid(P), P) I;
      typedef mln_ch_value(I, P) cp_ima_t;

    public:

      cp_with_map_t(const p_array<P>& model_X)
      {
	box3d box = geom::bbox(model_X);
	box.enlarge(50);

	typedef mln_ch_value(I, bool) model_t;
	model_t model(box);
	data::fill(model, false);
	data::fill((model | model_X).rw(), true);

	transform::internal::closest_point_functor<I> f;
	typedef mln_ch_value(I,value::int_u16) dmap_t;
	dmap_t dmap_X = canvas::distance_geodesic(model, c6(),
						  mln_max(value::int_u16),
						  f);

	cp_ima_ = f.cp_ima;
      }

      mln_site(I) operator()(const mln_site(I)& p) const
      {
	return cp_ima_(p);
      }

    private:
      cp_ima_t cp_ima_;
    };


    /// Closest point functor based on map distance.
    template <typename P>
    class basic_closest_point
    {
      typedef mln_image_from_grid(mln_grid(P), P) I;
      typedef p_array<P> X_t;

    public:

      basic_closest_point(const p_array<P>& X)
	: X_(X)
      {
      }

      float
      l2_distance(const vec3d_f& vec1, const vec3d_f& vec2) const
      {
	typedef float D;
	D d = 0;
	for (unsigned i = 0; i < 3; ++i)
	  {
	    D sqr_v1_v2 = static_cast<D>(mln::math::sqr(vec1[i] - vec2[i]));
	    d = static_cast<D>(d + sqr_v1_v2);
	  }
	return d;
      }

      mln_site(I) operator()(const vec3d_f& v) const
      {
	vec3d_f best_x = convert::to<vec3d_f>(X_[0].to_vec());

	float best_d = l2_distance(v, best_x);
	mln_piter(X_t) X_i(X_);
	for_all(X_i)
	{
	  float d = l2_distance(v, convert::to<vec3d_f>(X_i));
	  if (d < best_d)
	  {
	    best_d = d;
	    best_x = X_i.to_vec();
	  }
	}
	return best_x;
      }

    private:
	const p_array<P>& X_;
    };


    template <typename P, typename F>
    inline
    float
    compute_d_k(const p_array<P>& data_P,
		const F& closest_point,
		const algebra::quat& qR,
		const algebra::quat& qR_old,
		const vec3d_f& qT,
		const vec3d_f& qT_old)
    {
      accu::rms<vec3d_f, float> accu;
      mln_piter(p_array<P>) p(data_P);
      for_all(p)
      {
	// yk_i - pk+1_i
	vec3d_f Pk_i = qR_old.rotate(p.to_vec()) + qT_old;
	vec3d_f Pk_1_i = qR.rotate(p.to_vec()) + qT;
        accu.take(closest_point(Pk_i).to_vec() - Pk_1_i);
      }
      return accu.to_result();
    }

    template <typename P, typename F>
    inline
    float
    compute_e_k(const p_array<P>& data_P,
		const F& closest_point,
		const algebra::quat& qR,
		const vec3d_f& qT)
    {
      accu::rms<vec3d_f, float> accu;
      mln_piter(p_array<P>) p(data_P);
      for_all(p)
      {
	// yk_i - pk_i
	vec3d_f Pk_i = qR.rotate(p.to_vec()) + qT;
        accu.take(closest_point(Pk_i).to_vec() - Pk_i);
      }
      return accu.to_result();
    }


    /// FIXME: work only for 3d images.
    template <typename P, typename F>
    algebra::quat
    get_rot(const p_array<P>& data_P,
            const vec3d_f& mu_P,
            const vec3d_f& mu_Yk,
	    const F& closest_point,
	    const algebra::quat& qR,
	    const vec3d_f& qT)
    {
      /// Mk: cross-covariance matrix.
      algebra::mat<3u,3u,float> Mk;
      mln_piter(p_array<P>) p(data_P);

      // FIXME: could we use an accu?
      for_all(p)
      {
	vec3d_f P_i  = p.to_vec();
	vec3d_f Pk_i = qR.rotate(p.to_vec()) + qT;
	vec3d_f Yk_i = closest_point(Pk_i).to_vec();
	Mk += make::mat(P_i - mu_P) * trans(make::mat(Yk_i - mu_Yk));
      }
      Mk /= data_P.nsites();

      vec3d_f A;
      A[0] = Mk(1,2) - Mk(2,1);
      A[1] = Mk(2,0) - Mk(0,2);
      A[2] = Mk(0,1) - Mk(1,0);

      algebra::mat<4u,4u,float> Qk;
      float t = tr(Mk);

      Qk(0,0) = t;
      for (int i = 1; i < 4; ++i)
      {
        Qk(i,0) = A[i - 1];
        Qk(0,i) = A[i - 1];
        for (int j = 1; j < 4; ++j)
          if (i == j)
            Qk(i,j) = 2 * Mk(i - 1,i - 1) - t;
      }

      Qk(1,2) = Mk(0,1) + Mk(1,0);
      Qk(2,1) = Mk(0,1) + Mk(1,0);

      Qk(1,3) = Mk(0,2) + Mk(2,0);
      Qk(3,1) = Mk(0,2) + Mk(2,0);

      Qk(2,3) = Mk(1,2) + Mk(2,1);
      Qk(3,2) = Mk(1,2) + Mk(2,1);

      return mln::math::jacobi(Qk);
    }


    // Compute mu_Yk, mass center of Yk.
    template <typename P, typename F>
    inline
    vec3d_f
    get_mu_yk(const p_array<P>& data_P,
	      const F& closest_point,
	      const algebra::quat& qR,
	      const vec3d_f& qT)
    {
      accu::center<P,vec3d_f> accu;

      mln_piter(p_array<P>) p(data_P);
      for_all(p)
      {
	vec3d_f Pk_i = qR.rotate(p.to_vec()) + qT;
	accu.take(closest_point(Pk_i).to_vec());
      }
      return accu.to_result();
    }


    template <typename P, typename F>
    inline
    composed< rotation<P::dim,float>,translation<P::dim,float> >
    icp(const p_array<P>& data_P,
	const p_array<P>& model_X,
	const F& closest_point,
	const algebra::quat& initial_rot,
	const mln_vec(P)& initial_translation)
    {
      trace::entering("registration::icp");

      mln_precondition(P::dim == 3);
      mln_precondition(!data_P.is_empty());
      mln_precondition(!model_X.is_empty());
      mln_precondition(!initial_rot.is_null());

      typedef p_array<P> cloud_t;

      vec3d_f mu_P = set::compute(accu::center<P,vec3d_f>(), data_P);

      vec3d_f qT_old, qT = initial_translation;
      algebra::quat qR_old, qR = initial_rot;
      float ek, ek_old = mln_max(float);
      float dk, dk_old = mln_max(float);
      unsigned k = 0;
      do
      {
	qT_old = qT;
	qR_old = qR;

	// Compute error ek = d(Pk,Yk)
	ek = compute_e_k(data_P, closest_point, qR, qT);

      /// Compute transformation
      ///
	// mu_Yk - Pk's mass center.
	vec3d_f mu_Yk = get_mu_yk(data_P, closest_point, qR_old, qT_old);

	// quaternion qR - rotation
	qR = get_rot(data_P, mu_P, mu_Yk, closest_point, qR_old, qT_old);
	vec3d_f tmp = qR.v();

	// vector qT - translation
	qT = mu_Yk - qR.rotate(mu_P);
      ///
      /// End of "compute transformation"

	// Distance dk = d(Pk+1, Yk)
	dk = compute_d_k(data_P, closest_point, qR, qR_old, qT, qT_old);

	// Check property according the related paper.
	mln_assertion(0 <= dk);
	mln_assertion(dk <= ek);
	mln_assertion(ek <= dk_old);
	mln_assertion(dk_old <= ek_old);

	// Backing up results.
	dk_old = dk;
	ek_old = ek;

	++k;

      } while ((k < 3)
	  || norm::l2((qT - qT_old)) + norm::l2((qR - qR_old).to_vec()) > 1e-5);

      typedef rotation<3u,float> rot_t;
      rot_t tqR(qR);
      typedef translation<3u,float> trans_t;
      trans_t tqT(qT);
      composed<rot_t,trans_t> result(tqR, tqT);

      trace::exiting("registration::icp");
      return result;
    }

    template <typename P, typename F>
    inline
    composed< rotation<P::dim,float>,translation<P::dim,float> >
    icp(const p_array<P>& data_P,
	const p_array<P>& model_X,
	const F& closest_point)
    {
      return icp(data_P, model_X, closest_point,
		 algebra::quat(1,0,0,0), literal::zero);
    }

# endif // ! MLN_INCLUDE_ONLY

  } // end of namespace mln::registration

} // end of namespace mln

#endif // ! MLN_REGISTRATION_ICP_HH
