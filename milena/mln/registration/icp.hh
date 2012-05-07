// Copyright (C) 2008, 2009, 2010, 2011 EPITA Research and Development
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

#ifndef MLN_REGISTRATION_ICP_HH
# define MLN_REGISTRATION_ICP_HH

/// \file
///
/// Register an image over an another using the ICP algorithm.
///
/// \todo encode distances on 12 bits.

# include <cmath>
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
# include <mln/core/image/dmorph/slice_image.hh>
# include <mln/core/image/imorph/tr_image.hh>
# include <mln/core/image/dmorph/extension_fun.hh>

# include <mln/core/alias/neighb3d.hh>

# include <mln/transform/distance_and_closest_point_geodesic.hh>
# include <mln/canvas/distance_geodesic.hh>
# include <mln/pw/all.hh>

# include <mln/io/ppm/save.hh>
# include <mln/io/pbm/save.hh>

# include <mln/labeling/colorize.hh>
# include <mln/debug/histo.hh>

# include <mln/accu/histo.hh>
# include <mln/accu/math/sum.hh>

# include <mln/value/int_u16.hh>

# include <mln/literal/black.hh>
# include <mln/literal/white.hh>
# include <mln/literal/colors.hh>

# include <mln/util/timer.hh>

# include <mln/io/cloud/save.hh>


namespace mln
{

  namespace registration
  {

    //FIXME: used for debug purpose. Should be removed later.

    using namespace fun::x2x;

    /*! Register point in \p c using a function of closest points
     * \p closest_point.
     * This overload allows to specify initial transformations.
     *
     * \param[in] P_ The cloud of points.
     * \param[in] X the reference surface.
     * \param[in] closest_point The function of closest points.
     * \param[in] initial_rot An initial rotation.
     * \param[in] initial_translation An initial translation.
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
    std::pair<algebra::quat,mln_vec(P)>
    icp(const p_array<P>& P_,
	const p_array<P>& X,
	const F& closest_point,
	const algebra::quat& initial_rot,
	const mln_vec(P)& initial_translation);


    /*! Register point in \p c using a function of closest points
     * \p closest_point.
     *
     * \param[in] P_ The cloud of points.
     * \param[in] X the reference surface.
     * \param[in] closest_point The function of closest points.
     *
     * \return the rigid transformation which may be use later to create
     * a registered image.
     */
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
      typedef mln_ch_value(I, unsigned) cp_ima_t;
      typedef mln_ch_value(I,value::int_u16) dmap_t;

    public:

      closest_point_with_map(const p_array<P>& X)
      {
	box3d box = geom::bbox(X);
	box.enlarge(0, box.nslis());
	box.enlarge(1, box.nrows());
	box.enlarge(2, box.ncols());

	mln_postcondition(box.is_valid());

	std::cout << "Map image defined on " << box << std::endl;

	X_ = X;
	init(X, box);
      }

      closest_point_with_map(const p_array<P>& X, const box<P>& box)
      {
	X_ = X;
	init(X, box);
      }

      void init(const p_array<P>& X, const box<P>& box)
      {
	typedef mln_ch_value(I, bool) model_t;
	model_t model(box);
	data::fill(model, false);
	data::fill((model | X).rw(), true);


	typedef util::couple<mln_ch_value(model_t, value::int_u16),
			     mln_ch_value(model_t, unsigned)> couple_t;
	couple_t cpl = transform::distance_and_closest_point_geodesic(X, box,
								      c6(),
								      mln_max(value::int_u16));

	dmap_X_ = cpl.first();
	cp_ima_ = cpl.second();

	mln_postcondition(cp_ima_.is_valid());
	mln_postcondition(cp_ima_.domain().is_valid());
        std::cout << "pmin = " << cp_ima_.domain().pmin() << std::endl;;
        std::cout << "pmax = " << cp_ima_.domain().pmax() << std::endl;;

#ifndef NDEBUG
	mln_ch_value(I, bool) debug2(box);
	data::fill(debug2, false);
	mln_ch_value(I, value::rgb8) debug(box);
	mln_piter(p_array<P>) p(X);
	for_all(p)
	{
	  debug(p) = labeling::internal::random_color(value::rgb8());
	  debug2(p) = true;
	}
	io::pbm::save(slice(debug2,0), "debug2-a.ppm");

	mln_piter(I) pi(cp_ima_.domain());
	for_all(pi)
	{
	  debug(pi) = debug(X[cp_ima_(pi)]);
	  debug2(pi) = debug2(X[cp_ima_(pi)]);
	}

	io::pbm::save(slice(debug2,0), "debug2-b.ppm");
	io::ppm::save(slice(debug,0), "debug.ppm");
	std::cout << "map saved" << std::endl;
#endif
      }

      mln_site(I) operator()(const mln_site(I)& p) const
      {
	return X_[cp_ima_(p)];
      }


      // Distance map
      dmap_t dmap_X_;

    private:
      p_array<P> X_;
      // Closest point image.
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
	vec3d_f best_x = X_[0];

	float best_d = norm::l2_distance(v, best_x);
	mln_piter(X_t) X_i(X_);
	for_all(X_i)
	{
	  vec3d_f X_i_vec = X_i;
	  float d = norm::l2_distance(v, X_i_vec);
	  if (d < best_d)
	  {
	    best_d = d;
	    best_x = X_i_vec;
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
	vec3d_f Pk_i = pair.first.rotate(p) + pair.second;
	vec3d_f Yk_i = closest_point(Pk_i).to_vec();
	// yk_i - pk_i
	e_k_accu.take(Yk_i - Pk_i);
      }

      float d = e_k_accu.to_result();
      sd = std::sqrt(e_k_accu.hook_value_() / P_.nsites() - d * d);
      return sd;
    }


    template <typename P, typename F>
    void
    remove_too_far_sites_debug(image3d<value::rgb8>& out, const p_array<P>& P_,
			 const F& closest_point,
			 const std::pair<algebra::quat,mln_vec(P)>& pair,
			 const p_array<P>& X,
			 unsigned r, int d_min, int d_max, unsigned prefix)
    {
      unsigned removed = 0;
      accu::histo<value::int_u8> h;
      mln_piter(p_array<P>) p(P_);
      data::fill(out, literal::black);
      data::fill((out | X).rw(), literal::white);

      for_all(p)
      {
	vec3d_f Pk_i = pair.first.rotate(p) + pair.second;
	vec3d_f Yk_i = closest_point(Pk_i);

	int d_i = closest_point.dmap_X_(Pk_i);
	if (d_i >= d_min && d_i <= d_max)
	  out(Pk_i) = literal::green;
	else
	{
	  ++removed;
	  out(Pk_i) = literal::red;
	}
      }

#ifndef NDEBUG
      std::ostringstream ss1;
      ss1 << "histo_" << prefix << r << ".dat";
      std::cout << h << std::endl;

      std::ostringstream ss2;
      ss2 << "out_" << prefix << r << ".ppm";
      io::ppm::save(mln::slice(out,0), ss2.str());
#endif
      std::cout << "Points removed with the whole set and current d_min/d_max: " << removed << std::endl;

    }


    template <typename P, typename F>
    void
    compute_distance_criteria(const p_array<P>& P_,
			 const F& closest_point,
			 const std::pair<algebra::quat,mln_vec(P)>& pair,
			 unsigned r, int& d_min, int& d_max)
    {
      mln_piter(p_array<P>) p(P_);
      accu::histo<value::int_u8> h;

      float sd;
      {
	accu::math::sum<float> s, s2;
	for_all(p)
	{
	  vec3d_f Pk_i = pair.first.rotate(p.to_vec()) + pair.second;
	  unsigned d_i = closest_point.dmap_X_(Pk_i);
	  h.take(d_i);
	  s.take(d_i);
	  s2.take(d_i * d_i);
	}
	float mean = s / P_.nsites();
	sd = std::sqrt(s2 / P_.nsites() - mean * mean);
	d_min = int(mean - sd);
	d_max = int(mean + sd);
      }

      std::cout << "Standard deviation = " << sd << std::endl;
      std::ostringstream ss1;
      ss1 << "histo_" << r << ".dat";
      std::cout << h << std::endl;
      std::cout << "d thresholds = " << d_min << ' ' << d_max << std::endl;
    }

    template <typename P, typename F>
    p_array<P>
    remove_too_far_sites(image3d<value::rgb8>& out, const p_array<P>& P_,
			 const F& closest_point,
			 const std::pair<algebra::quat,mln_vec(P)>& pair,
			 const p_array<P>& X, p_array<P>& removed_set,
			 unsigned r, int d_min, int d_max,
			 const std::string& method)
    {
      p_array<P> tmp;
      unsigned removed = 0;

# ifndef NDEBUG
      data::fill(out, literal::black);
      data::fill((out | X).rw(), literal::white);
# endif // ! NDEBUG

      mln_piter(p_array<P>) p(P_);
      for_all(p)
      {
	vec3d_f Pk_i = pair.first.rotate(p.to_vec()) + pair.second;
	vec3d_f Yk_i = closest_point(Pk_i);

	int d_i = closest_point.dmap_X_(Pk_i);
	if (d_i >= d_min && d_i <= d_max)
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

      {
	std::ostringstream ss2;
	ss2 << method << "_" << r << "_removed_sites" << ".cloud";
	io::cloud::save(removed_set, ss2.str());
      }
      {
	std::ostringstream ss2;
	ss2 << method << "_" << r << "_kept_sites" << ".cloud";
	io::cloud::save(tmp, ss2.str());
      }

# ifndef NDEBUG
      std::ostringstream ss2;
      ss2 << method << "_" << r << "_removed_sites" << ".ppm";
      io::ppm::save(mln::slice(out,0), ss2.str());

      std::cout << "Points removed: " << removed << std::endl;
# endif // ! NDEBUG
      // They are used for debug purpose only.
      // When NDEBUG is set, they are unused.
      (void) X;
      (void) r;
      (void) method;

      return tmp;
    }

    template <typename P>
    void
    display_sites_used_in_icp(image3d<value::rgb8>& out, const p_array<P>& P_sub,
			      const p_array<P>& P_, const p_array<P>& X,
			      unsigned r, const std::string& prefix,
			      const std::pair<algebra::quat,mln_vec(P)>& pair,
			      const std::string& period, const value::rgb8& c)
    {
      data::fill(out, literal::black);
      data::fill((out | X).rw(), literal::white);

      mln_piter(p_array<P>) p1(P_);
      for_all(p1)
      {
	vec3d_f Pk_i = pair.first.rotate(p1.to_vec()) + pair.second;
	out(Pk_i) = literal::red;
      }

      mln_piter(p_array<P>) p2(P_sub);
      for_all(p2)
      {
	vec3d_f Pk_i = pair.first.rotate(p2.to_vec()) + pair.second;
	out(Pk_i) = c;
      }

      std::ostringstream ss;
      ss << prefix << "_" << r << "_" << period << ".ppm";

      io::ppm::save(slice(out,0), ss.str());
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
	vec3d_f P_i = p;
	vec3d_f Pk_i = qR_old.rotate(P_i) + qT_old;
	vec3d_f Pk_1_i = qR.rotate(P_i) + qT;
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
      algebra::mat<3u,3u,float> Spx(literal::zero);
      mln_piter(p_array<P>) p(P_);

      // FIXME: could we use an accu?
      for_all(p)
      {
	vec3d_f P_i  = p;
	vec3d_f Pk_i = qR.rotate(P_i) + qT;
	vec3d_f Yk_i = closest_point(Pk_i);
	Spx += (P_i - mu_P) * (Yk_i - mu_Yk).t();
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

      return math::jacobi(Qk);
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
	vec3d_f Yk_i = closest_point(Pk_i);
	mu_yk.take(Yk_i);
        e_k_accu.take(Yk_i - Pk_i);
      }

      e_k = e_k_accu.to_result();
      return mu_yk.to_result();
    }



    /// Base version of the ICP algorithm. It is called in other variants.
    template <typename P, typename F>
    inline
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
	ss << "tmp_0";
	if (k < 10)
	  ss << "0";
	ss << k << ".ppm";
	io::ppm::save(mln::slice(tmp_,0), ss.str());
#endif

	std::cout << "e_" << k << "=" << e_k << std::endl;
	std::cout << "d_" << k << "=" << d_k << std::endl;

	// Check distance and error according to the related paper.
	// Disabled because of the following 'if'
//	mln_assertion(0 <= d_k);
//	mln_assertion(d_k <= e_k);

//	mln_assertion(e_k <= d_k_old);
//	mln_assertion(d_k_old <= e_k_old);

	// During the first runs, d_k may be higher than e_k.
	// Hence, there we test k > 3.
	if (k > 3 && (d_k > e_k || d_k > d_k_old || e_k > e_k_old))
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


# endif // ! MLN_INCLUDE_ONLY

  } // end of namespace mln::registration

} // end of namespace mln

#endif // ! MLN_REGISTRATION_ICP_HH
