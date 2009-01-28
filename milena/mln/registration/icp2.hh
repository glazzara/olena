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

# include <mln/fun/x2x/all.hh>
# include <mln/fun/x2v/all.hh>
# include <mln/convert/to.hh>
# include <mln/transform/internal/closest_point_functor.hh>
# include <mln/canvas/distance_geodesic.hh>
# include <mln/accu/compute.hh>
# include <mln/accu/center.hh>

namespace mln
{

  namespace registration
  {

    using namespace fun::x2x;

    /*! Register point in \p c using a map of closest points \p map
     *
     * \param[in] c The cloud of points.
     * \param[in] map The map of closest points.
     * \param[in] epsilon ICP stops if sqr_norm(qk - qk-1) /
     * sqr_norm(qk) > epsilon
     * \param[out] qk The rigid transformation obtained.
     *
     * \pre \p ima has to be initialized.
     */
    template <typename D, typename M, typename T>
    void
    icp(const Image<D>& data, const Image<M>& model, T& qk);

# ifndef MLN_INCLUDE_ONLY

    template <typename P, typename C, typename T>
    inline
    float
    compute_d_k(const p_array<P>& data_P,
		const C& cp_X,
		const T& qk,
		const T& qk_next)
    {
      float f = 0.f;
      mln_piter(p_array<P>) p(data_P);
      for_all(p)
      {
	// yk_i - pk+1_i
        f += norm::l2(cp_X(qk(p.to_site().to_vec())).to_vec() - qk_next(p.to_site().to_vec()));
      }
      return f / data_P.nsites();
    }

    template <typename P, typename C, typename T>
    inline
    float
    compute_e_k(const p_array<P>& data_P,
		const C& cp_X,
		const T& qk)
    {
      float f = 0.f;
      mln_piter(p_array<P>) p(data_P);
      for_all(p)
      {
	// yk_i - pk_i
        f += norm::l2(cp_X(qk(p.to_site().to_vec())).to_vec() - qk(p.to_site().to_vec()));
      }
      return f / data_P.nsites();
    }


    template <typename P, typename T>
    fun::x2x::rotation<3u, float>
    get_rot(const p_array<P>& data_P,
            const algebra::vec<3u,float>& mu_P,
            const algebra::vec<3u,float>& mu_Yk,
	    const T& qk)
    {
      // FIXME: Make use of a cross_covariance accu (maybe not because of map(ck[i]))
      algebra::mat<3u,3u,float> Mk;
      mln_piter(p_array<P>) p(data_P);
      for_all(p)
      {
	algebra::vec<3u,float> P_i  = p.to_site().to_vec();
	algebra::vec<3u,float> Yk_i = qk(p.to_site().to_vec());
	Mk += make::mat(P_i - mu_P) * trans(make::mat(Yk_i - mu_Yk));
      }
      Mk /= data_P.nsites();

      algebra::vec<3u,float> A;
      A[0] = Mk(1,2) - Mk(2,1);
      A[1] = Mk(2,0) - Mk(0,2);
      A[2] = Mk(0,1) - Mk(1,0);

      algebra::mat<4u,4u,float> Qk;
      float t = tr(Mk);

      Qk(0,0) = t;
      for (int i = 1; i < 4; i++)
      {
        Qk(i,0) = A[i - 1];
        Qk(0,i) = A[i - 1];
        for (int j = 1; j < 4; j++)
          if (i == j)
            Qk(i,j) = 2 * Mk(i - 1,i - 1) - t;
      }

      Qk(1,2) = Mk(0,1) + Mk(1,0);
      Qk(2,1) = Mk(0,1) + Mk(1,0);

      Qk(1,3) = Mk(0,2) + Mk(2,0);
      Qk(3,1) = Mk(0,2) + Mk(2,0);

      Qk(2,3) = Mk(1,2) + Mk(2,1);
      Qk(3,2) = Mk(1,2) + Mk(2,1);

      algebra::quat qR = math::jacobi(Qk);

      return fun::x2x::rotation<3u, float>(qR);
    }



    template <typename P, typename C, typename T>
    T
    get_transformation(const p_array<P>& data_P,
		       const algebra::vec<P::dim,float>& mu_P,
		       const C& cp_X,
		       const T& qk)
    {
      algebra::vec<P::dim,float> mu_Yk;

      // Compute mu_Yk, mass center of Yk.
      mln_piter(p_array<P>) p(data_P);
      for_all(p)
        mu_Yk += cp_X(qk(p.to_site().to_vec())).to_vec();
      mu_Yk /= data_P.nsites();

      // qR
      rotation<P::dim, float> tqR = get_rot(data_P, mu_P, mu_Yk, qk);

      // qT
      const algebra::vec<P::dim,float> qT = mu_Yk - tqR(mu_P);
      translation<P::dim, float> tqT(qT);

      return compose(tqR, tqT);
    }


    template <typename D, typename M, typename T>
    inline
    void
    icp(const Image<D>& data, const Image<M>& model_X, T& qk)
    {
      trace::entering("registration::icp");

      typedef mln_psite(M) P;
      typedef p_array<P> cloud_t;

      cloud_t data_P = convert::to<cloud_t>(exact(data));

      // Compute distance map and closest point image.
      transform::internal::closest_point_functor<M> f;
      typedef mln_ch_value(M,value::int_u16) I;
      I dmap_X = canvas::distance_geodesic(model_X, c6(),
					   mln_max(value::int_u16), f);

      const mln_ch_value(M, P)& cp_X = f.cp_ima;

      algebra::vec<P::dim,float> mu_P = set::compute(accu::meta::center(),
						     exact(data).domain()).to_vec();

      float d_k_prev = 0, d_k = mln_max(float);
      float e_k_prev = 0, e_k = mln_max(float);
      do
      {
	d_k_prev = d_k;
	e_k_prev = e_k;

	//compute qk
	T qk_next = get_transformation(data_P, mu_P, cp_X, qk);

	d_k = compute_d_k(data_P, cp_X, qk, qk_next);
	std::cout << d_k << std::endl;
	e_k = compute_e_k(data_P, cp_X, qk);

	qk = qk_next;
      } while ((d_k_prev - d_k) >= 1e-3);

      trace::exiting("registration::icp");
    }

# endif // ! MLN_INCLUDE_ONLY

  } // end of namespace mln::registration

} // end of namespace mln

#endif // ! MLN_REGISTRATION_ICP_HH
