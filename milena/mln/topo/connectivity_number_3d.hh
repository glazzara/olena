// Copyright (C) 2011 EPITA Research and Development Laboratory (LRDE)
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

#ifndef MLN_TOPO_CONNECTIVITY_NUMBER_3D_HH
# define MLN_TOPO_CONNECTIVITY_NUMBER_3D_HH

/// \file
/// \brief 3D connectivity numbers computation.

# include <set>

# include <mln/core/concept/image.hh>

# include <mln/value/int_u8.hh>
# include <mln/value/int_u32.hh>

# include <mln/core/alias/neighb3d.hh>
# include <mln/make/box3d.hh>

# include <mln/core/image/image3d.hh>
# include <mln/core/image/vmorph/fun_image.hh>
# include <mln/core/image/dmorph/image_if.hh>
# include <mln/fun/v2b/lnot.hh>
# include <mln/labeling/blobs.hh>

# include <mln/math/abs.hh>


namespace mln
{

  namespace topo
  {

    /** Type of a connectivity number.

	Note: The maximun number of connected components in the
	neighborhood of a point is necessarily lower or equal to half
	the number of neighbors.  This number fits on an
	mln::value::int_u8 when the dimension is lower or equal to
	5.  */
    typedef mln::value::int_u8 conn_number_t;

    /// Type of a 3D (neighborhood) configuration number (index).
    typedef mln::value::int_u32 config_3d_t;


    /** Compute the 3D connectivity number of \a p within \a ima
        according to the 6-connectivity.  If \a ref_val is set to \c
        true, compute the number w.r.t. the foreground, otherwise
        compute it w.r.t. the background.

	Note: The (6+)-connectivity is associated to the
	18-connectivity, while the 6-connectivity is associated to the
	26-connectivity.

        \param ima      The image on which the connectivity number is
                        computed.
        \param p        The site the connectivity number of which is
                        computed.
        \param ref_val  The reference value, telling whether the
                        foreground (true) or the background (false) is
                        considered.  */
    template <typename I>
    conn_number_t
    connectivity_number_3d_c6(const Image<I>& ima, const mln_psite(I)& p,
			      bool ref_val);

    /** Compute the 3D connectivity number of \a p within \a ima
        according to the 26-connectivity.  If \a ref_val is set to \c
        true, compute the number w.r.t. the foreground, otherwise
        compute it w.r.t. the background.

        \param ima      The image on which the connectivity number is
                        computed.
        \param p        The site the connectivity number of which is
                        computed.
        \param ref_val  The reference value, telling whether the
                        foreground (true) or the background (false) is
                        considered.  */
    template <typename I>
    conn_number_t
    connectivity_number_3d_c26(const Image<I>& ima, const mln_psite(I)& p,
			       bool ref_val);

    /** Compute the 3D connectivity number of \a p within \a ima
        according to the (6+)-connectivity.  If \a ref_val is set to \c
        true, compute the number w.r.t. the foreground, otherwise
        compute it w.r.t. the background.

	Note: The 6-connectivity is associated to the 26-connectivity,
	while the (6+)-connectivity is associated to the
	18-connectivity.

        \param ima      The image on which the connectivity number is
                        computed.
        \param p        The site the connectivity number of which is
                        computed.
        \param ref_val  The reference value, telling whether the
                        foreground (true) or the background (false) is
                        considered.  */
    template <typename I>
    conn_number_t
    connectivity_number_3d_c6p(const Image<I>& ima, const mln_psite(I)& p,
			       bool ref_val);

    /** Compute the 3D connectivity number of \a p within \a ima
        according to the 18-connectivity.  If \a ref_val is set to \c
        true, compute the number w.r.t. the foreground, otherwise
        compute it w.r.t. the background.

        \param ima      The image on which the connectivity number is
                        computed.
        \param p        The site the connectivity number of which is
                        computed.
        \param ref_val  The reference value, telling whether the
                        foreground (true) or the background (false) is
                        considered.  */
    template <typename I>
    conn_number_t
    connectivity_number_3d_c18(const Image<I>& ima, const mln_psite(I)& p,
			       bool ref_val);



# ifndef MLN_INCLUDE_ONLY

    /* FIXME: Maybe we could implement these computations in a faster
       way using a mask (using an image_if morpher), instead of
       creating a small 3x3x3 image ?  */

    namespace internal
    {

      /// \brief Create a two 3x3x3 image representing foreground
      /// (resp.  background) values in \a ima around \a p, if \a
      /// ref_val is \c true (resp. \c false).  The value under \a p
      /// is always set to `false'.
      template<typename I>
      inline
      mln_concrete(I)
      local_configuration(const Image<I>& ima_, const mln_psite(I)& p,
			  bool ref_val)
      {
	const I& ima = exact(ima_);

	box3d b = make::box3d(-1,-1,-1, 1,1,1);
	mln_concrete(I) ima_local(b, 0);
	data::fill(ima_local, false);
	point3d center(0, 0, 0);
	mln_niter(neighb3d) n_ima(c26(), p);
	mln_niter(neighb3d) n_local(c26(), center);
	for_all_2(n_ima, n_local)
	  if (ima.has(n_ima) && ima(n_ima) == ref_val)
	    ima_local(n_local) = true;
	return ima_local;
      }


      /// \brief A function stating whether a point of the box
      /// (-1,-1-1)-(+1,+1,+1) is within the 18-c neighborhood of
      /// (0, 0, 0).
      bool
      f_within_c18(const point3d& p)
      {
	return !(math::abs(p.sli()) == 1
		 && math::abs(p.row()) == 1
		 && math::abs(p.col()) == 1);
      }

      /// \brief Functor associated to mln::topo::internal::f_within_c18.
      struct within_c18 : Function_v2b<within_c18>
      {
	typedef bool result;
	bool operator()(const point3d& p) const { return f_within_c18(p); }
      };
    }


    template<typename I>
    inline
    conn_number_t
    connectivity_number_3d_c6(const Image<I>& ima, const mln_psite(I)& p,
			      bool ref_val)
    {
      /* FIXME: Not compatible with proxy/morphers on values.  */
      mlc_equal(mln_value(I), bool)::check();

      mln_concrete(I) ima_local =
	internal::local_configuration(ima, p, ref_val);

      // Create a copy of mln::c6()'s own neighborhood to avoid
      // thread-unsafe accesses to this neighborhood.
      neighb3d nbh = c6();
      conn_number_t unused_nl;
      // Restrict the image to the 18-c neighborhood of P.
      image_if<image3d<conn_number_t>, internal::within_c18> lab =
	labeling::blobs(ima_local | internal::within_c18(),
			nbh, unused_nl);
      std::set<conn_number_t> s;
      point3d center(0, 0, 0);
      mln_niter(neighb3d) n(nbh, center);
      for_all(n)
	if (lab(n) != 0)
	  s.insert(lab(n));
      return s.size();
    }

    template<typename I>
    inline
    conn_number_t
    connectivity_number_3d_c26(const Image<I>& ima, const mln_psite(I)& p,
			       bool ref_val)
    {
      /* FIXME: Not compatible with proxy/morphers on values.  */
      mlc_equal(mln_value(I), bool)::check();

      mln_concrete(I) ima_local =
	internal::local_configuration(ima, p, ref_val);

      // Create a copy of mln::c26()'s own neighborhood to avoid
      // thread-unsafe accesses to this neighborhood.
      neighb3d nbh = c26();
      conn_number_t number;
      labeling::blobs(ima_local, nbh, number);
      return number;
    }

    template<typename I>
    inline
    conn_number_t
    connectivity_number_3d_c6p(const Image<I>& ima, const mln_psite(I)& p,
			       bool ref_val)
    {
      /* FIXME: Not compatible with proxy/morphers on values.  */
      mlc_equal(mln_value(I), bool)::check();

      mln_concrete(I) ima_local =
	internal::local_configuration(ima, p, ref_val);

      // Create a copy of mln::c6()'s own neighborhood to avoid
      // thread-unsafe accesses to this neighborhood.
      neighb3d nbh = c6();
      conn_number_t unused_nl;
      image3d<conn_number_t> lab = labeling::blobs(ima_local, nbh, unused_nl);
      std::set<conn_number_t> s;
      point3d center(0, 0, 0);
      mln_niter_(neighb3d) n(nbh, center);
      for_all(n)
	if (lab(n) != 0)
	  s.insert(lab(n));
      return s.size();
    }

    template<typename I>
    inline
    conn_number_t
    connectivity_number_3d_c18(const Image<I>& ima, const mln_psite(I)& p,
			       bool ref_val)
    {
      /* FIXME: Not compatible with proxy/morphers on values.  */
      mlc_equal(mln_value(I), bool)::check();

      mln_concrete(I) ima_local =
	internal::local_configuration(ima, p, ref_val);

      // Create a copy of mln::c18()'s own neighborhood to avoid
      // thread-unsafe accesses to this neighborhood.
      neighb3d nbh = c18();
      conn_number_t unused_nl;
      image3d<conn_number_t> lab = labeling::blobs(ima_local, nbh, unused_nl);
      std::set<conn_number_t> s;
      point3d center(0, 0, 0);
      mln_niter_(neighb3d) n(nbh, center);
      for_all(n)
	if (lab(n) != 0)
	  s.insert(lab(n));
      return s.size();
    }

# endif // MLN_INCLUDE_ONLY

  } // end of namespace mln::topo

} // end of namespace mln

#endif // ! MLN_TOPO_CONNECTIVITY_NUMBER_3D_HH
