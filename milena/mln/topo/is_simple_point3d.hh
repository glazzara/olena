// Copyright (C) 2010, 2011 EPITA Research and Development Laboratory (LRDE)
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

#ifndef MLN_TOPO_IS_SIMPLE_POINT3D_HH
# define MLN_TOPO_IS_SIMPLE_POINT3D_HH

/// \file
/// \brief Testing whether an mln::point3d is a simple point (exact
/// version based on the systematic computation of connectivity
/// numbers).

# include <mln/core/concept/function.hh>
# include <mln/core/concept/image.hh>
# include <mln/core/concept/neighborhood.hh>

# include <mln/core/image/image3d.hh>
# include <mln/core/alias/neighb3d.hh>

# include <mln/value/int_u8.hh>
# include <mln/value/int_u32.hh>

# include <mln/labeling/blobs.hh>
# include <mln/fun/v2b/lnot.hh>
# include <mln/core/image/vmorph/fun_image.hh>
# include <mln/core/image/dmorph/image_if.hh>


// FIXME: Temporary.
/* A function stating whether a point of the box (-1,-1-1)-(+1,+1,+1)
   is within the 18-c neighborhood of (0, 0, 0). */
bool
f_within_c18(const mln::point3d& p)
{
  return !(mln::math::abs(p.sli()) == 1
	   && mln::math::abs(p.row()) == 1
	   && mln::math::abs(p.col()) == 1);
}

// FIXME: Temporary.
// Associated functor.
struct within_c18 : mln::Function_v2b<within_c18>
{
  typedef bool result;
  bool operator()(const mln::point3d& p) const { return f_within_c18(p); }
};


/* FIXME: Factor code with is_simple_point2d.  */

/* FIXME: BTW, we should implement a version of is_simple_point2d
   based on a LUT as well.  */

/* FIXME: Not that this routine produces false results when a point
   has not exactly 26 neighbors (same issue with is_simple_point2d).
   This functor should be placed in an `mln::topo::approx' namespace,
   and an exact should be provided in addition.  */

namespace mln
{

  namespace topo
  {

    // FIXME: Honor the neighborhoods of the fore- and background.

    /// \brief A predicate for the simplicity of an mln::point3d based
    /// on a look-up table.
    template <typename I, typename N>
    class is_simple_point3d : public Function_v2b< is_simple_point3d<I, N> >
    {
    public:
      /// Result type of the functor.
      typedef bool result;

      /// Build a functor.
      ///
      /// \param  nbh_fg  The foreground neighborhood.
      /// \param  nbh_bg  The background neighborhood.
      is_simple_point3d(const Neighborhood<N>& nbh_fg,
			const Neighborhood<N>& nbh_bg);

      /// Build a functor, and assign an image to it.
      ///
      /// \param  nbh_fg  The foreground neighborhood.
      /// \param  nbh_bg  The background neighborhood.
      /// \apram  ima     The image.
      is_simple_point3d(const Neighborhood<N>& nbh_fg,
			const Neighborhood<N>& nbh_bg,
			const Image<I>& ima);

      /// Set the underlying image.
      void set_image(const Image<I>& ima);

      /// \brief Tell whether \a p is a simple point or not.
      ///
      /// This computation is based on connectivity numbers.
      bool operator()(const mln_psite(I)& p) const;

    private:
      /// The foreground neighborhood.
      const N& nbh_fg_;
      /// The background neighborhood.
      const N& nbh_bg_;
      /// The image.
      const I* ima_;
    };



# ifndef MLN_INCLUDE_ONLY

    template <typename I, typename N>
    inline
    is_simple_point3d<I, N>::is_simple_point3d(const Neighborhood<N>& nbh_fg,
					       const Neighborhood<N>& nbh_bg)
      : nbh_fg_(exact(nbh_fg)), nbh_bg_(exact(nbh_bg)),
	ima_(0)
    {
    }

    template <typename I, typename N>
    inline
    is_simple_point3d<I, N>::is_simple_point3d(const Neighborhood<N>& nbh_fg,
					       const Neighborhood<N>& nbh_bg,
					       const Image<I>& ima)
      : nbh_fg_(exact(nbh_fg)), nbh_bg_(exact(nbh_bg)),
	ima_(exact(&ima))
    {
    }

    template <typename I, typename N>
    inline
    void
    is_simple_point3d<I, N>::set_image(const Image<I>& ima)
    {
      ima_ = exact(&ima);
    }

    // FIXME: Honor the neighborhoods of the fore- and background.
    // FIXME: Move the computation of connectivity numbers out of this
    // routine.
    template <typename I, typename N>
    inline
    bool
    is_simple_point3d<I, N>::operator()(const mln_psite(I)& p) const
    {
      mln_precondition(ima_);
      const I& ima = *ima_;

      /** Type of a connectivity number.

	  Note: The maximun number of connected components in the
	  neighborhood of a point is necessarily lower or equal to
	  half the number of neighbors.  This number fits on an
	  mln::value::int_u8 when the dimension is lower or equal
	  to 5.  */
      typedef mln::value::int_u8 conn_number_t;
      /// Type of a 3D (neighborhood) configuration number (index).
      typedef mln::value::int_u32 config_3d_t;

      // Create a two 3x3 image representing foreground and background
      // values in IMA around P.  The value under P is always set to
      // `false'.
      box3d b = make::box3d(-1,-1,-1, 1,1,1);
      I fg_ima_local(b, 0);
      I bg_ima_local(b, 0);
      mln::data::fill(fg_ima_local, false);
      mln::data::fill(bg_ima_local, false);
      point3d center(0, 0, 0);
      mln_niter(N) n_ima(c26(), p);
      mln_niter(N) n_local(c26(), center);
      for_all_2(n_ima, n_local)
	if (ima.has(n_ima))
	    {
	      if (ima(n_ima))
		fg_ima_local(n_local) = true;
	      else
		bg_ima_local(n_local) = true;
	    }

      // Foreground connectivity number.
      conn_number_t fg_conn_number;
      {
	mln::neighb3d nbh = c26();
	labeling::blobs(fg_ima_local, nbh, fg_conn_number);
      }

      // Background connectivity number.
      conn_number_t bg_conn_number;
      {
	mln::neighb3d nbh = c6();
	conn_number_t unused_nl;
	// Restrict the image to the 18-c neighborhood of P.
	image_if<image3d<conn_number_t>, within_c18> lab =
	  labeling::blobs(bg_ima_local | within_c18(), nbh, unused_nl);
	std::set<conn_number_t> s;
	mln_niter(N) n(nbh, center);
	for_all(n)
	  if (lab(n) != 0)
	    s.insert(lab(n));
	bg_conn_number = s.size();
      }

      return fg_conn_number == 1 && bg_conn_number == 1;
    }

# endif // MLN_INCLUDE_ONLY

  } // end of namespace mln::topo

} // end of namespace mln

#endif // ! MLN_TOPO_IS_SIMPLE_POINT2D_HH
