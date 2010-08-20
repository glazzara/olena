// Copyright (C) 2010 EPITA Research and Development Laboratory (LRDE)
//
// This file is part of the Milena Library.  This library is free
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

#ifndef APPS_GENERIC_SKEL_IMAGE2D_SKEL_HH
# define APPS_GENERIC_SKEL_IMAGE2D_SKEL_HH

/// \file
/// \brief Definitions for a simplicity criterion and a constraint
/// to be used in the breadth-first thinning of a 2D regular image.

# include <mln/topo/is_simple_2d.hh>


// FIXME: Split this file?


/** \brief An equivalent (for mln::image2d) of the
    mln::topo::is_simple_cell functor, based on the mask-based
    criterion mln::topo::is_simple_2d.

    This functor acts as an adapter, since mln::topo::is_simple_2d
    does not fit (yet) in the canvas of
    mln::topo::skeleton::breadth_first_thinning.  Moreover, this code
    is a bit easier to read since it does not make use of a dual
    neighborhood (having a long and complex type).  */
template <typename I, typename N>
class is_simple_2d : public mln::Function_v2b< is_simple_2d<I, N> >
{
public:
  /// Result type of the functor.
  typedef bool result;

  /// Build a functor.
  ///
  /// \param  nbh_fg  The foreground neighborhood.
  /// \param  nbh_bg  The background neighborhood.
  is_simple_2d(const mln::Neighborhood<N>& nbh_fg,
	       const mln::Neighborhood<N>& nbh_bg)
    : nbh_fg_(mln::exact(nbh_fg)), nbh_bg_(mln::exact(nbh_bg)),
      ima_(0)
  {
  }

  /// Build a functor, and assign an image to it.
  ///
  /// \param  nbh_fg  The foreground neighborhood.
  /// \param  nbh_bg  The background neighborhood.
  /// \apram  ima     The image.
  is_simple_2d(const mln::Neighborhood<N>& nbh_fg,
	       const mln::Neighborhood<N>& nbh_bg,
	       const mln::Image<I>& ima)
    : nbh_fg_(mln::exact(nbh_fg)), nbh_bg_(mln::exact(nbh_bg)),
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
    return
      mln::connectivity_number_2d(*ima_, nbh_fg_, p, true ) == 1 &&
      mln::connectivity_number_2d(*ima_, nbh_bg_, p, false) == 1;
  }

private:
  /// The foreground neighborhood.
  const N& nbh_fg_;
  /// The background neighborhood.
  const N& nbh_bg_;
  /// The image.
  const I* ima_;
};


template <typename I>
class detach
{
public:
  /// Build a functor.
  detach()
    : ima_(0)
  {
  }

  /// Build a functor, and assign an image to it.
  ///
  /// \apram  ima     The image.
  detach(mln::Image<I>& ima)
    : ima_(mln::exact(&ima))
  {
  }

  /// Set the underlying image.
  void set_image(mln::Image<I>& ima)
  {
    ima_ = mln::exact(&ima);
  }

  void operator()(const mln_psite(I)& p) const
  {
    (*ima_)(p) = false;
  }

private:
  /// The image.
  I* ima_;
};


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


#endif // ! APPS_GENERIC_SKEL_IMAGE2D_SKEL_HH
