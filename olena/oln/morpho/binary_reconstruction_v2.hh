// Copyright (C) 2007 EPITA Research and Development Laboratory
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

#ifndef	OLN_MORPHO_BINARY_RECONSTRUCTION_HH
# define OLN_MORPHO_BINARY_RECONSTRUCTION_HH

# include <oln/core/concept/image.hh>
# include <oln/accumulator/max.hh>
# include <oln/level/clone.hh>
# include <oln/level/fill.hh>
# include <oln/level/compare.hh>
# include <oln/level/local_inf.hh>
# include <oln/level/local_sup.hh>
# include <oln/core/internal/f_ch_value.hh> // FIXME : could be auto
					    // included ?

# include <oln/canvas/reconstruction.hh>

namespace oln
{

  namespace morpho
  {

    template <typename I , typename J>
    oln_plain(I)
    binary_reconstruction(const Image_with_Nbh<I>& marker,
			  const Binary_Image<J>& mask);

# ifndef OLN_INCLUDE_ONLY

    namespace impl
    {

      template <typename I, typename J>
      struct binary_reconstruction_
      {
	typedef I Ima;
	const I& marker;
	const J& mask;
	I output, memo;

	accumulator::max_<oln_value(I)> accu;

	binary_reconstruction_(const Binary_Image<I>& marker,
			       const J& mask)
	  : marker(exact(marker)), mask(mask)
	{
	}

	void init()
	{
	}

	bool condition_fwd(const oln_point(I) p)
	{
	  return mask(p);
	}

	bool condition_bkd(const oln_point(I)& p)
	{
	  return mask(p);
	}

	bool is_stable()
	{
	  return output == memo;
	}

	void re_loop()
	{
	}

	void final()
	{
	}

      };

    } // end of namespace oln::morpho::impl

    template <typename I , typename J>
    oln_plain(I)
    binary_reconstruction(const Image_with_Nbh<I>& marker,
			  const Binary_Image<J>& mask)
    {
      impl::binary_reconstruction_<I,J> run(exact(marker), exact(mask));
      canvas::reconstruction(run);
      return run.output;
    }

# endif // ! OLN_INCLUDE_ONLY

  } // end of namespace oln::morpho

} // end of namespace oln


#endif // ! OLN_MORPHO_BINARY_RECONSTRUCTION_HH
