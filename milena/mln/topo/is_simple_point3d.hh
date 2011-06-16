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

# include <mln/topo/connectivity_number_3d.hh>


namespace mln
{

  namespace topo
  {

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

    template <typename I, typename N>
    inline
    bool
    is_simple_point3d<I, N>::operator()(const mln_psite(I)& p) const
    {
      mln_precondition(ima_);
      const I& ima = *ima_;

      /* FIXME: Maybe we can select the connectivity number functions
	 once and for all when the functor is constructed, instead of
	 selecting them at each call of operator().  */
      bool result;
      if (nbh_fg_ == c6() && nbh_bg_ == c26())
	result =
	  connectivity_number_3d_c6 (ima, p, true)  == 1 &&
	  connectivity_number_3d_c26(ima, p, false) == 1;
      else if (nbh_fg_ == c26() && nbh_bg_ == c6())
	result =
	  connectivity_number_3d_c26(ima, p, true)  == 1 &&
	  connectivity_number_3d_c6 (ima, p, false) == 1;
      else if (nbh_fg_ == c6() && nbh_bg_ == c18())
	result =
	  connectivity_number_3d_c6p(ima, p, true)  == 1 &&
	  connectivity_number_3d_c18(ima, p, false) == 1;
      else if (nbh_fg_ == c18() && nbh_bg_ == c6())
	result =
	  connectivity_number_3d_c18(ima, p, true)  == 1 &&
	  connectivity_number_3d_c6p(ima, p, false) == 1;
      else
	abort();

      return result;
    }

# endif // MLN_INCLUDE_ONLY

  } // end of namespace mln::topo

} // end of namespace mln

#endif // ! MLN_TOPO_IS_SIMPLE_POINT2D_HH
