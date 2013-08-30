// Copyright (C) 2011, 2013 EPITA Research and Development Laboratory (LRDE)
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

#ifndef MLN_TOPO_IS_NOT_1D_ISTHMUS_HH
# define MLN_TOPO_IS_NOT_1D_ISTHMUS_HH

/// \file
/// \brief Non 1D isthmus predicate for (2D and 3D) binary images.

# include <mln/core/concept/function.hh>
# include <mln/core/concept/image.hh>
# include <mln/core/concept/neighborhood.hh>

# include <mln/topo/connectivity_number_2d.hh>
# include <mln/topo/connectivity_number_3d.hh>


namespace mln
{

  namespace topo
  {

    template <typename I, typename N>
    struct is_not_1d_isthmus
      : public Function_v2b< is_not_1d_isthmus<I, N> >
    {
      /// Build a functor.
      ///
      /// \param  nbh_fg  The foreground neighborhood.
      /// \param  nbh_bg  The background neighborhood.
      is_not_1d_isthmus(const Neighborhood<N>& nbh_fg,
			const Neighborhood<N>& nbh_bg);

      /// Build a functor, and assign an image to it.
      ///
      /// \param  nbh_fg  The foreground neighborhood.
      /// \param  nbh_bg  The background neighborhood.
      /// \apram  ima     The image.
      is_not_1d_isthmus(const Neighborhood<N>& nbh_fg,
			const Neighborhood<N>& nbh_bg,
			const Image<I>& ima);

      /// Set the underlying image.
      void set_image(Image<I>& ima);

      // Is \a p not a 1D isthmus point?
      bool operator()(const mln_psite(I)& p) const;

    private:
      /// The foreground neighborhood.
      const N& nbh_fg_; 
      /// The background neighborhood.
      const N& nbh_bg_;
     /// The image.
      const I* ima_;

      /// The type of pointer on function computing a 3D connectivity
      /// number.
      typedef conn_number_t (*connectivity_number_fun_t)(const Image<I>&,
							 const mln_psite(I)&,
							 bool);
      /// The function (pointer) computing the (foreground) connectivity
      /// number.
      connectivity_number_fun_t connectivity_number_fg_;
    };



# ifndef MLN_INCLUDE_ONLY

    /* FIXME: This is way too complicated.  Maybe the simplest thing
       is to create two functors is_not_1d_isthmus_point2d and
       is_not_1d_isthmus_point3d and factor common parts.  */
    namespace internal
    {

      template <typename I>
      struct conn_number_fun
      {
	typedef conn_number_t (*connectivity_number_fun_t)(const Image<I>&,
							   const mln_psite(I)&,
							   bool);

	// Default case.
	template <typename N>
	static connectivity_number_fun_t get(const N& /* nbh_fg */,
					     const N& /* nbh_bg */)
	{
	  // We only handle the cases where N is either neighb2d or neighb3d.
	  abort();
	  return 0;
	}

	// 2D cases.
	static connectivity_number_fun_t get(const neighb2d& nbh_fg,
					     const neighb2d& nbh_bg)
	{
	  // 2D neighborhoods.
	  if (nbh_fg == c4() && nbh_bg == c8())
	    return connectivity_number_2d_c4<I>;
	  else if (nbh_fg == c8() && nbh_bg == c4())
	    return connectivity_number_2d_c8<I>;
	  else
	    {
	      // Bad or unknown combination.
	      abort();
	      return 0;
	    }
	}

	// 3D cases.
	static connectivity_number_fun_t get(const neighb3d& nbh_fg,
					     const neighb3d& nbh_bg)
	{
	  // 3D neighborhoods.
	  if (nbh_fg == c6() && nbh_bg == c26())
	    return connectivity_number_3d_c6<I>;
	  else if (nbh_fg == c26() && nbh_bg == c6())
	    return connectivity_number_3d_c26<I>;
	  else if (nbh_fg == c6() && nbh_bg == c18())
	    return connectivity_number_3d_c6p<I>;
	  else if (nbh_fg == c18() && nbh_bg == c6())
	    return connectivity_number_3d_c18<I>;
	  else
	    {
	      // Bad or unknown combination.
	      abort();
	      return 0;
	    }
	}

      };


    } // end of namespace mln::io::topo::internal


    template <typename I, typename N>
    inline
    is_not_1d_isthmus<I, N>::is_not_1d_isthmus(const Neighborhood<N>& nbh_fg,
					       const Neighborhood<N>& nbh_bg)
      : nbh_fg_(exact(nbh_fg)),
	nbh_bg_(exact(nbh_bg)),
	connectivity_number_fg_(internal::conn_number_fun<I>::get(nbh_fg_,
								  nbh_bg_))
    {
    }

    template <typename I, typename N>
    inline
    is_not_1d_isthmus<I, N>::is_not_1d_isthmus(const Neighborhood<N>& nbh_fg,
					       const Neighborhood<N>& nbh_bg,
					       const Image<I>& ima)
      : nbh_fg_(exact(nbh_fg)),
	nbh_bg_(exact(nbh_bg)),
	ima_(exact(&ima)),
	connectivity_number_fg_(internal::conn_number_fun<I>::get(nbh_fg_,
								  nbh_bg_))
    {
    }

    template <typename I, typename N>
    inline
    void
    is_not_1d_isthmus<I, N>::set_image(Image<I>& ima)
    {
      ima_ = exact(&ima);
    }

    template <typename I, typename N>
    inline
    bool
    is_not_1d_isthmus<I, N>::operator()(const mln_psite(I)& p) const
    {
      mln_precondition(ima_);
      return connectivity_number_fg_(*ima_, p, true) < 2u;
    }

# endif // MLN_INCLUDE_ONLY

  } // end of namespace topo

} // end of namespace mln

#endif // ! MLN_TOPO_IS_NOT_1D_ISTHMUS_HH
