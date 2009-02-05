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

# include <algorithm>

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

# include <mln/io/ppm/save.hh>
# include <mln/io/pbm/save.hh>
# include <mln/debug/colorize.hh>

# include <mln/literal/black.hh>
# include <mln/literal/white.hh>
# include <mln/literal/colors.hh>

# include <mln/core/image/slice_image.hh>
# include <mln/util/timer.hh>

#include <mln/core/image/tr_image.hh>
#include <mln/core/image/extension_fun.hh>

#include <mln/accu/histo.hh>
#include <mln/debug/histo.hh>


namespace mln
{


  namespace registration
  {

    using namespace fun::x2x;

    /*! Register point in \p c using a function of closest points
     * \p closest_point.
     *
     * \param[in] P_ The cloud of points.
     * \param[in] X the reference surface.
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
     * \pre \p P_ and \p X must not be empty.
     *
     * Reference article: "A Method for Registration of 3-D Shapes", Paul J.
     * Besl and Neil D. McKay, IEEE, 2, February 1992.
     *
     */
    template <typename P, typename F>
//    composed< translation<P::dim,float>,rotation<P::dim,float> >
    std::pair<algebra::quat,mln_vec(P)>
    icp(const p_array<P>& P_,
	const p_array<P>& X,
	const F& closest_point,
	const algebra::quat& initial_rot,
	const mln_vec(P)& initial_translation);

    template <typename P, typename F>
    composed< translation<P::dim,float>,rotation<P::dim,float> >
    icp(const p_array<P>& P_,
	const p_array<P>& X,
	const F& closest_point);

# ifndef MLN_INCLUDE_ONLY

    /// Closest point functor based on map distance.
    template <typename P>
    class closest_point_with_map
    {
      typedef mln_image_from_grid(mln_grid(P), P) I;
      typedef mln_ch_value(I, P) cp_ima_t;
      typedef mln_ch_value(I,value::int_u8) dmap_t;

    public:

      closest_point_with_map(const p_array<P>& X)
      {
	box3d box = geom::bbox(X);
	box.enlarge(1, box.nrows() / 2);
	box.enlarge(2, box.ncols() / 2);
	std::cout << "Map image defined on " << box << std::endl;

	typedef mln_ch_value(I, bool) model_t;
	model_t model(box);
	data::fill(model, false);
	data::fill((model | X).rw(), true);

	transform::internal::closest_point_functor<model_t> f;
	util::timer t;
	t.start();
	dmap_X_ = canvas::distance_geodesic(model, c6(),
					    mln_max(value::int_u8),
					    f);
	std::cout << "canvas::distance_geodesic - " << t << "s" << std::endl;

	cp_ima_ = f.cp_ima;

#ifndef NDEBUG
	mln_ch_value(I, bool) debug2(box);
	data::fill(debug2, false);
	mln_ch_value(I, value::rgb8) debug(box);
	mln_piter(p_array<P>) p(X);
	for_all(p)
	{
	  debug(p) = debug::internal::random_color(value::rgb8());
	  debug2(p) = true;
	}
	io::pbm::save(slice(debug2,0), "debug2-a.ppm");

	mln_piter(I) pi(cp_ima_.domain());
	for_all(pi)
	{
	  debug(pi) = debug(cp_ima_(pi));
	  debug2(pi) = debug2(cp_ima_(pi));
	}

	io::pbm::save(slice(debug2,0), "debug2-b.ppm");
	io::ppm::save(slice(debug,0), "debug.ppm");
	std::cout << "map saved" << std::endl;
#endif
      }

      mln_site(I) operator()(const mln_site(I)& p) const
      {
	return cp_ima_(p);
      }

      dmap_t dmap_X_;
    private:
      cp_ima_t cp_ima_;
    };


    /// Closest point functor based on map distance.
    template <typename P>
    class closest_point_basic
    {
      typedef mln_image_from_grid(mln_grid(P), P) I;
      typedef p_array<P> X_t;

    public:

      closest_point_basic(const p_array<P>& X)
	: X_(X)
      {
      }

      mln_site(I) operator()(const vec3d_f& v) const
      {
	vec3d_f best_x = convert::to<vec3d_f>(X_[0].to_vec());

	float best_d = norm::l2_distance(v, best_x);
	mln_piter(X_t) X_i(X_);
	for_all(X_i)
	{
	  float d = norm::l2_distance(v, convert::to<vec3d_f>(X_i));
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


    template <typename P>
    void
    draw_last_run(const box3d& box, const p_array<P>& kept,
		  const p_array<P>& removed, const p_array<P>& X,
		  const algebra::quat& qR, const vec3d_f qT)
    {
      typedef image3d<value::rgb8> result_t;
      result_t result(box);
      typedef extension_fun<result_t,pw::cst_<mln_value(result_t)> > ext_result_t;
      ext_result_t ext_result(result, pw::cst(value::rgb8(0,0,0)));

      data::fill(ext_result, literal::black);
      data::fill((ext_result | X).rw(), literal::white);

      mln_piter(p_array<P>) p(kept);
      for_all(p)
	ext_result(qR.rotate(p.to_vec()) + qT) = literal::green;

      mln_piter(p_array<P>) p2(removed);
      for_all(p2)
	ext_result(qR.rotate(p2.to_vec()) + qT) = literal::red;

      io::ppm::save(slice(ext_result,0), "registered-2.ppm");
    }



    template <typename P, typename F>
    float compute_standard_deviation(const p_array<P>& P_,
				     const std::pair<algebra::quat,mln_vec(P)>& pair,
				     const F& closest_point)
    {
      accu::rms<vec3d_f,float> e_k_accu;

      // Standard Deviation
      float sd;
      mln_piter(p_array<P>) p(P_);
      for_all(p)
      {
	vec3d_f Pk_i = pair.first.rotate(p.to_vec()) + pair.second;
	vec3d_f Yk_i = closest_point(Pk_i).to_vec();
	// yk_i - pk_i
	e_k_accu.take(Yk_i - Pk_i);
      }

      float d = e_k_accu.to_result();
      sd = math::sqrt((e_k_accu.hook_value_() - 2.5 * math::sqr(d)) / P_.nsites());
      return sd;
    }


    template <typename P, typename F>
    p_array<P>
    remove_too_far_sites(image3d<value::rgb8>& out, const p_array<P>& P_bak,
			 const F& closest_point,
			 const std::pair<algebra::quat,mln_vec(P)>& pair,
			 const p_array<P>& X, p_array<P>& removed_set,
			 unsigned r)
    {
      float sd = compute_standard_deviation(P_bak, pair, closest_point);
      std::cout << "Standard deviation = " << sd << std::endl;

      p_array<P> tmp;
      unsigned removed = 0;

      data::fill(out, literal::white);
      data::fill((out | X).rw(), literal::black);

      accu::histo<value::int_u8> h;
      mln_piter(p_array<P>) p(P_bak);
      for_all(p)
      {
	vec3d_f Pk_i = pair.first.rotate(p.to_vec()) + pair.second;
	vec3d_f Yk_i = closest_point(Pk_i).to_vec();

	float norm = norm::l2_distance(Yk_i, Pk_i);
	h.take(closest_point.dmap_X_(Pk_i));
	if (norm < sd && norm > sd / 2)
	{
	  tmp.append(p);
	  out(Pk_i) = literal::green;
	}
	else
	{
	  ++removed;
	  removed_set.append(p);
	  out(Pk_i) = literal::red;
	}
      }

      std::ostringstream ss1;
      ss1 << "histo_" << r << ".dat";
      debug::histo_plot(h, ss1.str());
      std::cout << h << std::endl;

      std::ostringstream ss2;
      ss2 << "out_0" << r << ".ppm";
      io::ppm::save(mln::slice(out,0), ss2.str());

      std::cout << "Points removed: " << removed << std::endl;

      return tmp;
    }


    template <typename P, typename F>
    inline
    float
    compute_d_k(const p_array<P>& P_,
		const F& closest_point,
		const algebra::quat& qR,
		const algebra::quat& qR_old,
		const vec3d_f& qT,
		const vec3d_f& qT_old)
    {
      accu::rms<vec3d_f, float> accu;
      mln_piter(p_array<P>) p(P_);
      for_all(p)
      {
	// yk_i - pk+1_i
	vec3d_f Pk_i = qR_old.rotate(p.to_vec()) + qT_old;
	vec3d_f Pk_1_i = qR.rotate(p.to_vec()) + qT;
        accu.take(closest_point(Pk_i).to_vec() - Pk_1_i);
      }
      return accu.to_result();
    }

    /// FIXME: work only for 3d images.
    template <typename P, typename F>
    algebra::quat
    get_rot(const p_array<P>& P_,
            const vec3d_f& mu_P,
            const vec3d_f& mu_Yk,
	    const F& closest_point,
	    const algebra::quat& qR,
	    const vec3d_f& qT)
    {
      /// Spx: cross-covariance matrix.
      algebra::mat<3u,3u,float> Spx;
      mln_piter(p_array<P>) p(P_);

      // FIXME: could we use an accu?
      for_all(p)
      {
	vec3d_f P_i  = p.to_vec();
	vec3d_f Pk_i = qR.rotate(p.to_vec()) + qT;
	vec3d_f Yk_i = closest_point(Pk_i).to_vec();
	Spx += make::mat(P_i - mu_P) * trans(make::mat(Yk_i - mu_Yk));
      }
      Spx /= P_.nsites();

      vec3d_f A;
      A[0] = Spx(1,2) - Spx(2,1);
      A[1] = Spx(2,0) - Spx(0,2);
      A[2] = Spx(0,1) - Spx(1,0);

      algebra::mat<4u,4u,float> Qk;
      float t = tr(Spx);

      Qk(0,0) = t;
      for (int i = 1; i < 4; ++i)
      {
        Qk(i,0) = A[i - 1];
        Qk(0,i) = A[i - 1];
        for (int j = 1; j < 4; ++j)
          if (i == j)
            Qk(i,j) = 2 * Spx(i - 1,i - 1) - t;
      }

      Qk(1,2) = Spx(0,1) + Spx(1,0);
      Qk(2,1) = Spx(0,1) + Spx(1,0);

      Qk(1,3) = Spx(0,2) + Spx(2,0);
      Qk(3,1) = Spx(0,2) + Spx(2,0);

      Qk(2,3) = Spx(1,2) + Spx(2,1);
      Qk(3,2) = Spx(1,2) + Spx(2,1);

      return mln::math::jacobi(Qk);
    }


    // Compute mu_Yk, mass center of Yk.
    template <typename P, typename F>
    inline
    vec3d_f
    get_mu_yk(const p_array<P>& P_,
	      const F& closest_point,
	      const algebra::quat& qR,
	      const vec3d_f& qT,
	      float& e_k)
    {
      accu::rms<vec3d_f,float> e_k_accu;
      accu::center<P,vec3d_f> mu_yk;

      mln_piter(p_array<P>) p(P_);
      for_all(p)
      {
	// yk_i - pk_i
	vec3d_f Pk_i = qR.rotate(p.to_vec()) + qT;
	vec3d_f Yk_i = closest_point(Pk_i).to_vec();
	mu_yk.take(Yk_i);
        e_k_accu.take(Yk_i - Pk_i);
      }

      e_k = e_k_accu.to_result();
      return mu_yk.to_result();
    }



    /// Base version of the ICP algorithm. It is called in other variants.
    template <typename P, typename F>
    inline
//    composed< translation<P::dim,float>,rotation<P::dim,float> >
    std::pair<algebra::quat,mln_vec(P)>
    icp(const p_array<P>& P_,
	const p_array<P>& X,
	const F& closest_point,
	const algebra::quat& initial_rot,
	const mln_vec(P)& initial_translation)
    {
      trace::entering("registration::icp");

      (void) X;
      mln_precondition(P::dim == 3);
      mln_precondition(!P_.is_empty());
      mln_precondition(!X.is_empty());
      mln_precondition(!initial_rot.is_null());

      typedef p_array<P> cloud_t;

      vec3d_f mu_P = set::compute(accu::center<P,vec3d_f>(), P_);

      vec3d_f qT_old, qT = initial_translation;
      algebra::quat qR_old, qR = initial_rot;
      float e_k, e_k_old = mln_max(float);
      float d_k, d_k_old = mln_max(float);
      unsigned k = 0;

# ifndef NDEBUG
      box3d box = geom::bbox(X);
      //FIXME: too large?
      box.enlarge(1, box.nrows() / 2);
      box.enlarge(2, box.ncols() / 2);
      image3d<value::rgb8> debug(box);
      data::fill(debug, literal::black);
      data::fill((debug | X).rw(), literal::white);
# endif

      do
      {
	qT_old = qT;
	qR_old = qR;

      /// Compute transformation
      ///
	// mu_Yk - Pk's mass center.
	// + Compute error ek = d(Pk,Yk)
	vec3d_f mu_Yk = get_mu_yk(P_, closest_point, qR_old, qT_old, e_k);

	// quaternion qR - rotation
	qR = get_rot(P_, mu_P, mu_Yk, closest_point, qR_old, qT_old);
	vec3d_f tmp = qR.v();

	// vector qT - translation
	qT = mu_Yk - qR.rotate(mu_P);
      ///
      /// End of "compute transformation"

	// Distance dk = d(Pk+1, Yk)
	d_k = compute_d_k(P_, closest_point, qR, qR_old, qT, qT_old);


#ifndef NDEBUG
	image3d<value::rgb8> tmp_ = duplicate(debug);
	mln_piter(p_array<P>) p_dbg(P_);
	for_all(p_dbg)
	  tmp_(qR_old.rotate(p_dbg.to_vec()) + qT_old) = literal::green;
	std::ostringstream ss;
	ss << "reg/tmp_0";
	if (k < 10)
	  ss << "0";
	ss << k << ".ppm";
	io::ppm::save(mln::slice(tmp_,0), ss.str());
#endif

	std::cout << "e_" << k << "=" << e_k << std::endl;
	std::cout << "d_" << k << "=" << d_k << std::endl;

	// Check distance and error according to the related paper.
	mln_assertion(0 <= d_k);
	mln_assertion(d_k <= e_k);

	// Disabled because of the following 'if'
//	mln_assertion(e_k <= d_k_old);
//	mln_assertion(d_k_old <= e_k_old);

	if (d_k > d_k_old)
	{
	  qR = qR_old;
	  qT = qT_old;
	  break;
	}

	// Backing up results.
	d_k_old = d_k;
	e_k_old = e_k;

	++k;

      } while ((k < 3)
	  || norm::l2((qT - qT_old)) + norm::l2((qR - qR_old).to_vec()) > 1e-3);

      trace::exiting("registration::icp");
      return std::make_pair(qR, qT);
    }



    /// Single call to ICP with all sites.
    template <typename P, typename F>
    inline
    composed< translation<P::dim,float>,rotation<P::dim,float> >
    icp(const p_array<P>& P_,
	const p_array<P>& X,
	const F& closest_point)
    {
      util::timer t;
      t.start();

      std::pair<algebra::quat,mln_vec(P)> pair = icp(P_, X, closest_point,
						     algebra::quat(1,0,0,0),
						     literal::zero);
      std::cout << "icp = " << t << std::endl;

      typedef rotation<3u,float> rot_t;
      rot_t tqR(pair.first);
      typedef translation<3u,float> trans_t;
      trans_t tqT(pair.second);
      composed<trans_t, rot_t> result(tqT, tqR);

      return result;
    }


    /// Shuffle the sites in P_.
    /// Use one third of P_'s sites for each run.
    /// For each run, it removes sites which are too close or too far.
    template <typename P, typename F>
    inline
    composed< translation<P::dim,float>,rotation<P::dim,float> >
    icp_clean(const p_array<P>& P_,
	      const p_array<P>& X,
	      const F& closest_point)
    {
      util::timer t;
      t.start();

      // P_bak is shuffled.
      p_array<P> P_bak = P_;
      std::vector<mln_element(p_array<P>)>& v = P_bak.hook_std_vector_();
      std::random_shuffle(v.begin(), v.end());

      // P_sub = 1/3 * P_bak;
      p_array<P> P_sub = P_bak;
      P_sub.hook_std_vector_().resize(P_bak.nsites() / 3);

      unsigned r = 0;
      std::pair<algebra::quat,mln_vec(P)> pair;
      pair.first = algebra::quat(1,0,0,0);
      pair.second = literal::zero;
      box3d box = geom::bbox(X);
      box.enlarge(40);
      image3d<value::rgb8> out(box);
      p_array<P> removed_set;
      do
      {
	/// Compute transformation.
	pair = icp(P_sub, X, closest_point,
		   pair.first,
		   pair.second);

	pair = icp(P_sub, X, closest_point,
		   pair.first,
		   pair.second);

	P_sub = remove_too_far_sites(out, P_sub,
				     closest_point, pair, X, removed_set, r);


	++r;

	//Add more data
	if (r < 3)
	  for (unsigned i = (P_bak.nsites() / 3) * r;
	      i < (P_bak.nsites() / 3) * (r + 1); ++i)
	  {
	    P_sub.append(P_bak[i]);
	  }

      } while (r < 4);
      std::cout << "icp = " << t << std::endl;

      typedef rotation<3u,float> rot_t;
      rot_t tqR(pair.first);
      typedef translation<3u,float> trans_t;
      trans_t tqT(pair.second);
      composed<trans_t,rot_t> result(tqT, tqR);

      return result;
    }


    /// Shuffle sites in P_.
    /// Do the first run with all sites.
    /// For each run, remove sites which are too far or too close.
    template <typename P, typename F>
    inline
    composed< translation<P::dim,float>,rotation<P::dim,float> >
    icp_clean2(const p_array<P>& P_,
	      const p_array<P>& X,
	      const F& closest_point)
    {
      util::timer t;
      t.start();

      // P_bak is shuffled.
      p_array<P> P_bak = P_;

      unsigned r = 0;
      std::pair<algebra::quat,mln_vec(P)> pair;
      pair.first = algebra::quat(1,0,0,0);
      pair.second = literal::zero;
      box3d box = geom::bbox(X);
      box.enlarge(40);
      image3d<value::rgb8> out(box);
      p_array<P> removed_set;
      do
      {
	pair = icp(P_bak, X, closest_point,
		   pair.first,
		   pair.second);

	P_bak = remove_too_far_sites(out, P_bak,
				     closest_point, pair, X, removed_set, r);

	++r;

      } while (r < 4);
      std::cout << "icp = " << t << std::endl;

      draw_last_run(box, P_bak, removed_set, X, pair.first, pair.second);

      typedef rotation<3u,float> rot_t;
      rot_t tqR(pair.first);
      typedef translation<3u,float> trans_t;
      trans_t tqT(pair.second);
      composed<trans_t,rot_t> result(tqT, tqR);

      return result;
    }



    /// Run icp once with 1/10 of the sites and run it once again with the
    /// resulting tranformation and all the sites.
    template <typename P, typename F>
    inline
    composed< translation<P::dim,float>,rotation<P::dim,float> >
    icp_fast(const p_array<P>& P_,
	     const p_array<P>& X,
	     const F& closest_point)
    {
      typedef std::pair<algebra::quat,mln_vec(P)> pair_t;

      p_array<P> P_sub = P_;
      std::vector<mln_element(p_array<P>)>& v = P_sub.hook_std_vector_();
      std::random_shuffle(v.begin(), v.end());
      v.resize(P_.nsites() / 10);

      util::timer t;
      t.start();
      pair_t tmp = icp(P_sub, X, closest_point,
		     algebra::quat(1,0,0,0), literal::zero);

      std::cout << "icp_1 - " << t << "s" << std::endl;
      t.restart();

      pair_t tmp2 = icp(P_, X, closest_point,
			tmp.first, tmp.second);

      std::cout << "icp_2 - " << t << "s" << std::endl;

      typedef rotation<3u,float> rot_t;
      rot_t tqR(tmp2.first);
      typedef translation<3u,float> trans_t;
      trans_t tqT(tmp2.second);
      composed<rot_t,trans_t> result(tqR, tqT);

      return result;
    }

# endif // ! MLN_INCLUDE_ONLY

  } // end of namespace mln::registration

} // end of namespace mln

#endif // ! MLN_REGISTRATION_ICP_HH
