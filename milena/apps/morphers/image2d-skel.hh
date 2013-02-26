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

#ifndef APPS_MORPHERS_IMAGE2D_SKEL_HH
# define APPS_MORPHERS_IMAGE2D_SKEL_HH

/// \file
/// \brief Definitions for a simplicity criterion and a constraint
/// to be used in the breadth-first thinning of a 2D regular image.

/* FIXME: Remove this file as soon as patches from the mesh-segm-skel
   branch have been merged into the current branch.  */

# include <mln/topo/is_simple_2d.hh>


/** \brief An equivalent (for mln::image2d) of the
    mln::topo::is_simple_cell functor, based on the mask-based
    criterion mln::topo::is_simple_2d.

    This functor acts as an adapter, since mln::topo::is_simple_2d_t
    does not fit (yet) in the canvas of
    mln::topo::skeleton::breadth_first_thinning.  */
template <typename I, typename N>
class is_simple_2d : public mln::Function_v2b< is_simple_2d<I, N> >,
		     private mln::topo::is_simple_2d_t<N>
{
public:
  typedef mln::topo::is_simple_2d_t<N> super;

  /// Result type of the functor.
  typedef bool result;

  /// Build a functor.
  ///
  /// \param  nbh_fg  The foreground neighborhood.
  /// \param  nbh_bg  The background neighborhood.
  is_simple_2d(const mln::Neighborhood<N>& nbh_fg)
    : super(mln::exact(nbh_fg)),
      ima_(0)
  {
  }

  /// Build a functor, and assign an image to it.
  ///
  /// \param  nbh_fg  The foreground neighborhood.
  /// \param  nbh_bg  The background neighborhood.
  /// \apram  ima     The image.
  is_simple_2d(const mln::Neighborhood<N>& nbh_fg,
	       const mln::Image<I>& ima)
    : super(mln::exact(nbh_fg)),
      ima_(mln::exact(&ima))
  {
  }

  /// Set the underlying image.
  void set_image(const mln::Image<I>& ima)
  {
    ima_ = mln::exact(&ima);
  }

  /// Based on connectivity numbers.
  bool operator()(const mln_psite(I)& p) const
  {
    return this->check(*ima_, p);
  }

private:
  /// The image.
  const I* ima_;
};


template <typename I>
void
detach(const mln_psite(I)& p, mln::Image<I>& ima)
{
  mln::exact(ima)(p) = false;
}


template <typename I, typename N>
struct is_not_end_point : public mln::Function_v2b< is_not_end_point<I, N> >
{
  /// Build a functor, and assign an image to it.
  ///
  /// \param  nbh_fg  The foreground neighborhood.
  /// \apram  ima     The image.
  is_not_end_point(const mln::Neighborhood<N>& nbh,
		   const mln::Image<I>& ima)
    : nbh_(mln::exact(nbh)),
      ima_(mln::exact(ima))
  {
  }

  // Is \a p not a end point?
  bool operator()(const mln_psite(I)& p) const
  {
    // Number of foreground neighbors pixels.
    unsigned nneighbs = 0;
    mln_niter(N) n(nbh_, p);
    for_all(n)
      if (ima_.has(n) && ima_(n))
	++nneighbs;
    return nneighbs != 1;
  }

private:
  /// The foreground neighborhood.
  const N& nbh_;
  /// The image.
  const I& ima_;
};

#endif // ! APPS_MORPHERS_IMAGE2D_SKEL_HH
