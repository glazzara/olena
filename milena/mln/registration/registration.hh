// Copyright (C) 2008, 2009 EPITA Research and Development Laboratory
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

#ifndef MLN_REGISTRATION_REGISTRATION_HH
# define MLN_REGISTRATION_REGISTRATION_HH

/// \file mln/registration/registration.hh
///
/// Image registration
/// \sa registration::icp

# include <mln/core/image/image3d.hh>
# include <mln/registration/icp.hh>
# include <mln/fun/x2x/all.hh>
# include <mln/fun/x2p/closest_point.hh>
# include <mln/convert/to_p_array.hh>

//FIXME: to be removed.
# include <mln/util/timer.hh>

namespace mln
{

  namespace registration
  {

    using namespace mln::fun::x2x;


    //FIXME: move to registration.hh
    /// Call ICP once and return the resulting transformation.
    template <typename P>
    inline
    composed< translation<P::dim,float>,rotation<P::dim,float> >
    registration1(const p_array<P>& P_,
		  const p_array<P>& X);

    //FIXME: move to registration.hh
    /// Call ICP 10 times.
    /// Do the first call to ICP with all sites then work on a subset of
    /// which size is decreasing.
    /// For each call, a distance criterion is computed on a subset.
    /// Sites part of the subset which are too far or too
    /// close are removed.
    /// Removed sites are *NOT* reused later in the subset.
    template <typename P>
    inline
    composed< translation<P::dim,float>,rotation<P::dim,float> >
    registration2(const p_array<P>& P_,
		  const p_array<P>& X);

    //FIXME: move to registration.hh
    /// Call ICP 10 times.
    /// Do the first call to ICP with all sites then work on a subset.
    /// For each call, a distance criterion is computed on a subset.
    /// A new subset is computed from the whole set of points according
    /// to this distance. It will be used in the next call.
    /// Removed Sites *MAY* be reintegrated.
    template <typename P>
    inline
    composed< translation<P::dim,float>,rotation<P::dim,float> >
    registration3(const p_array<P>& P_,
		  const p_array<P>& X);



# ifndef MLN_INCLUDE_ONLY


    namespace internal
    {

      template <typename P>
      inline
      void
      registration_tests(const p_array<P>& P_, const p_array<P>& X)
      {
	mln_assertion(P_.is_valid());
	mln_assertion(X.is_valid());
	mln_assertion(!X.is_empty());
	mln_assertion(!P_.is_empty());

	// FIXME: Work only in 3D for now...
        mln_precondition(P::dim == 3);
	(void) P_;
	(void) X;
      }

    } // end of namespace mln::registration::internal


    namespace impl
    {

      template <typename P>
      inline
      composed< translation<P::dim,float>,rotation<P::dim,float> >
      registration1(const p_array<P>& P_,
	  const p_array<P>& X)
      {
	trace::entering("mln::registration::registration1");

# ifndef NDEBUG
	util::timer t;
	t.start();
# endif // ! NDEBUG

	registration::closest_point_with_map<P> closest_point(X);

	std::pair<algebra::quat,mln_vec(P)> pair = icp(P_, X, closest_point,
						       algebra::quat(1,0,0,0),
						       literal::zero);
# ifndef NDEBUG
	std::cout << "icp = " << t << std::endl;
# endif // ! NDEBUG

	typedef rotation<3u,float> rot_t;
	rot_t tqR(pair.first);
	typedef translation<3u,float> trans_t;
	trans_t tqT(pair.second);
	composed<trans_t, rot_t> result(tqT, tqR);

	trace::exiting("mln::registration::registration1");

	return result;
      }


      template <typename P>
      inline
      composed< translation<P::dim,float>,rotation<P::dim,float> >
      registration2(const p_array<P>& P_,
	  const p_array<P>& X)
      {
	trace::entering("mln::registration::registration2");

	// Used for debug.
	std::string method = "registration2";

	registration::closest_point_with_map<P> closest_point(X);

# ifndef NDEBUG
	util::timer t;
	t.start();
# endif // ! NDEBUG

	// P_bak is shuffled.
	p_array<P> P_bak = P_;

	unsigned r = 0;
	std::pair<algebra::quat,mln_vec(P)> pair;
	pair.first = algebra::quat(1,0,0,0);
	pair.second = literal::zero;
	box3d box = geom::bbox(X);
	box.enlarge(1, 60);
	box.enlarge(2, 60);

	// Used for debug.
	image3d<value::rgb8> out(box);

	p_array<P> removed_set;

	do
	{

# ifndef NDEBUG
	  std::cout << std::endl << std::endl << "==== New run - " << r << std::endl;
# endif // ! NDEBUG

	  pair = icp(P_bak, X, closest_point,
	      pair.first,
	      pair.second);

# ifndef NDEBUG
	  display_sites_used_in_icp(out, P_bak, P_, X, r, method, pair,
				    "final", literal::blue);
# endif // ! NDEBUG

	  int d_min, d_max;
	  compute_distance_criteria(P_bak, closest_point, pair, r, d_min, d_max);

	  P_bak = remove_too_far_sites(out, P_bak,
	      closest_point, pair, X, removed_set,
	      r, d_min, d_max, method);

# ifndef NDEBUG
	  display_sites_used_in_icp(out, P_bak, P_, X, r, method, pair,
				    "schanges", literal::green);
	  std::cout << "==== End of run" << std::endl;
# endif

	  ++r;

	} while (r < 10);

# ifndef NDEBUG
	std::cout << "icp = " << t << std::endl;
	draw_last_run(box, P_bak, removed_set, X, pair.first, pair.second);
# endif

	typedef rotation<3u,float> rot_t;
	rot_t tqR(pair.first);
	typedef translation<3u,float> trans_t;
	trans_t tqT(pair.second);
	composed<trans_t,rot_t> result(tqT, tqR);

	trace::exiting("mln::registration::registration2");

	return result;
      }


      template <typename P>
      inline
      composed< translation<P::dim,float>,rotation<P::dim,float> >
      registration3(const p_array<P>& P_,
	  const p_array<P>& X)
      {
	trace::entering("mln::registration::registration3");

	registration::closest_point_with_map<P> closest_point(X);
	std::cout << " pmin and pmax: " << std::endl;
	std::cout << closest_point.cp_ima_.domain().pmin() << std::endl;
	std::cout << closest_point.cp_ima_.domain().pmax() << std::endl;

	// Used for debug.
	std::string method = "registration3";

# ifndef NDEBUG
	util::timer t;
	t.start();
# endif // ! NDEBUG

	// P_bak is shuffled.
	p_array<P> P_bak = P_;

	unsigned r = 0;
	std::pair<algebra::quat,mln_vec(P)> pair;
	pair.first = algebra::quat(1,0,0,0);
	pair.second = literal::zero;
	box3d box = geom::bbox(X);
	box.enlarge(1, 60);
	box.enlarge(2, 60);

	// Used for debug.
	image3d<value::rgb8> out(box);

	p_array<P> removed_set;

	do
	{
# ifndef NDEBUG
	  std::cout << std::endl << std::endl << "==== New run - "
		    << r << std::endl;
# endif // ! NDEBUG

	  pair = icp(P_bak, X, closest_point,
	      pair.first,
	      pair.second);

# ifndef NDEBUG
	  display_sites_used_in_icp(out, P_bak, P_, X, r, method, pair,
				    "final", literal::blue);
# endif // ! NDEBUG

	  int d_min, d_max;
	  compute_distance_criteria(P_bak, closest_point, pair, r, d_min, d_max);

	  P_bak = remove_too_far_sites(out, P_,
	      closest_point, pair, X, removed_set,
	      r, d_min, d_max, method);

# ifndef NDEBUG
	  display_sites_used_in_icp(out, P_bak, P_, X, r, method, pair,
				    "schanges", literal::green);
	  std::cout << "==== End of run" << std::endl;
# endif // ! NDEBUG

	  ++r;

	} while (r < 10);

# ifndef NDEBUG
	std::cout << "icp = " << t << std::endl;
	draw_last_run(box, P_bak, removed_set, X, pair.first, pair.second);
# endif // ! NDEBUG

	typedef rotation<3u,float> rot_t;
	rot_t tqR(pair.first);
	typedef translation<3u,float> trans_t;
	trans_t tqT(pair.second);
	composed<trans_t,rot_t> result(tqT, tqR);

	trace::exiting("mln::registration::registration3");

	return result;
      }

    } // end of namespace mln::registration::impl



    // Facade

    template <typename P>
    inline
    composed< translation<P::dim,float>,rotation<P::dim,float> >
    registration1(const p_array<P>& cloud,
                  const p_array<P>& surface)
    {
      trace::entering("registration::registration1");

      internal::registration_tests(cloud, surface);

      composed< translation<P::dim,float>, rotation<P::dim,float> >
	      qk = impl::registration1(cloud, surface);

      trace::exiting("registration::registration1");

      return qk;
    }


    template <typename P>
    inline
    composed< translation<P::dim,float>,rotation<P::dim,float> >
    registration2(const p_array<P>& cloud,
                  const p_array<P>& surface)
    {
      trace::entering("registration::registration2");

      internal::registration_tests(cloud, surface);

      composed< translation<P::dim,float>, rotation<P::dim,float> >
	      qk = impl::registration2(cloud, surface);

      trace::exiting("registration::registration2");

      return qk;
    }


    template <typename P>
    inline
    composed< translation<P::dim,float>,rotation<P::dim,float> >
    registration3(const p_array<P>& cloud,
                  const p_array<P>& surface)
    {
      trace::entering("registration::registration3");

      internal::registration_tests(cloud, surface);

      composed< translation<P::dim,float>, rotation<P::dim,float> >
	      qk = impl::registration3(cloud, surface);

      trace::exiting("registration::registration3");

      return qk;
    }


# endif // ! MLN_INCLUDE_ONLY


  } // end of namespace mln::registration


} // end of namespace mln


#endif // ! MLN_REGISTRATION_REGISTRATION_HH
