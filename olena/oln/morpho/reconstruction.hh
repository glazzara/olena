// Copyright (C) 2007 EPITA Research and Development Laboratory
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

#ifndef	OLN_MORPHO_RECONSTRUCTION_HH
# define OLN_MORPHO_RECONSTRUCTION_HH

# include <oln/core/concept/image.hh>
# include <oln/accumulator/max.hh>
# include <oln/level/clone.hh>
# include <oln/level/fill.hh>

namespace oln
{

  namespace morpho
  {

    template <typename I , typename J>
    void
    reconstruction(const Mutable_Image<I>& marker,
		   const Binary_Image<J>& mask);

# ifndef OLN_INCLUDE_ONLY

    namespace impl
    {

      template <typename I>
      bool
      stability(const Image_with_Nbh<I>& marker,
		const Image_with_Nbh<I>& tmp)
      {
	oln_piter(I) p(marker.points());
	for_all(p) // FIXME : Concept doesn't own .image()? add != is better
	  if (exact(marker).image()(p) != exact(tmp).image()(p))
	    return false;
	return true;
      }

      template <typename I>
      oln_plain(I)
      reconstruction_loop(const Image_with_Nbh<I>& marker,
			  const Binary_Image<I>&   mask)
      {
	oln_plain(I) output;
	prepare(output, with, marker);

	accumulator::max_<oln_value(I)> max;

	// first pass
	oln_fwd_piter(I) p(marker.points());
	for_all(p)
	  output(p) = local_sup(max, marker, p) and mask(p);

	// second pass
	oln_bkd_piter(I) p2(marker.points());
	for_all(p2)
	  output(p2) = local_inf(max, marker, p2) and mask(p2);

	return output;
      }

      template <typename I , typename J>
      void // FIXME : Slow impl.
      reconstruction_(const Image_with_Nbh<I>& marker,
		      const Binary_Image<J>& mask)
      {
	oln_plain(I) tmp = level::clone(marker);

	while ( not stability(marker, tmp) )
	{
	  level::fill(exact(marker).image(), exact(tmp).image()); //add fill version better.
	  tmp = reconstruction_loop(marker, mask);
	}

	level::fill(exact(marker).image(), exact(tmp).image());
      }

    } // end of namespace oln::morpho::impl

    template <typename I , typename J>
    void
    reconstruction(const Mutable_Image<I>& marker,
		   const Binary_Image<J>& mask)
    {
      impl::reconstruction_(exact(marker), exact(mask));
    }

# endif // ! OLN_INCLUDE_ONLY

  } // end of namespace oln::morpho

} // end of namespace oln


#endif // ! OLN_MORPHO_DILATION_HH
