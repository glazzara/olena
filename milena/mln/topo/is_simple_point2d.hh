// Copyright (C) 2010, 2011, 2013 EPITA Research and Development
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

#ifndef MLN_TOPO_IS_SIMPLE_POINT2D_HH
# define MLN_TOPO_IS_SIMPLE_POINT2D_HH

/// \file
/// \brief Testing whether an mln::point2d is a simple point.

# include <mln/core/concept/function.hh>
# include <mln/core/concept/image.hh>
# include <mln/core/concept/neighborhood.hh>

# include <mln/topo/is_simple_2d.hh>


namespace mln
{

  namespace topo
  {

    /** \brief A predicate for the simplicity of an mln::point2d based
	on the mask-based criterion mln::topo::is_simple_2d.

	This functor acts as an adapter, since mln::topo::is_simple_2d
	does not fit (yet) in the canvas of
	mln::topo::skeleton::breadth_first_thinning.  Moreover, this
	code is a bit easier to read since it does not make use of a
	dual neighborhood (having a long and complex type).  */
    template <typename I, typename N>
    class is_simple_point2d : public Function_v2b< is_simple_point2d<I, N> >
    {
    public:
      /// Result type of the functor.
      typedef bool result;

      /// Build a functor.
      ///
      /// \param  nbh_fg  The foreground neighborhood.
      /// \param  nbh_bg  The background neighborhood.
      is_simple_point2d(const Neighborhood<N>& nbh_fg,
			const Neighborhood<N>& nbh_bg);

      /// Build a functor, and assign an image to it.
      ///
      /// \param  nbh_fg  The foreground neighborhood.
      /// \param  nbh_bg  The background neighborhood.
      /// \apram  ima     The image.
      is_simple_point2d(const Neighborhood<N>& nbh_fg,
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
    is_simple_point2d<I, N>::is_simple_point2d(const Neighborhood<N>& nbh_fg,
					       const Neighborhood<N>& nbh_bg)
      : nbh_fg_(exact(nbh_fg)), nbh_bg_(exact(nbh_bg)),
	ima_(0)
    {
    }

    template <typename I, typename N>
    inline
    is_simple_point2d<I, N>::is_simple_point2d(const Neighborhood<N>& nbh_fg,
					       const Neighborhood<N>& nbh_bg,
					       const Image<I>& ima)
      : nbh_fg_(exact(nbh_fg)), nbh_bg_(exact(nbh_bg)),
	ima_(exact(&ima))
    {
    }

    template <typename I, typename N>
    inline
    void
    is_simple_point2d<I, N>::set_image(const Image<I>& ima)
    {
      ima_ = exact(&ima);
    }

    template <typename I, typename N>
    inline
    bool
    is_simple_point2d<I, N>::operator()(const mln_psite(I)& p) const
    {
      mln_precondition(ima_);
      return
	connectivity_number_2d(*ima_, nbh_fg_, p, true ) == 1 &&
	connectivity_number_2d(*ima_, nbh_bg_, p, false) == 1;
    }

# endif // MLN_INCLUDE_ONLY

  } // end of namespace mln::topo

} // end of namespace mln

#endif // ! MLN_TOPO_IS_SIMPLE_POINT2D_HH
