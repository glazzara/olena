// Copyright (C) 2001, 2002, 2003, 2004, 2008, 2009, 2010, 2011 EPITA
// Research and Development Laboratory (LRDE)
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

#ifndef MLN_LINEAR_GAUSSIAN_IMPL_HH
# define MLN_LINEAR_GAUSSIAN_IMPL_HH

/// \file
///
/// \brief Gaussian filter implementation.
///
/// \todo Add a clean reference Rachid Deriche
///	 Recursively implementing the gaussian and its derivatives (1993)

# include <vector>
# include <cmath>

# include <mln/core/concept/image.hh>
# include <mln/core/alias/point2d.hh>
# include <mln/core/alias/dpoint1d.hh>
# include <mln/core/alias/dpoint3d.hh>
# include <mln/extension/adjust_fill.hh>
# include <mln/geom/ncols.hh>
# include <mln/geom/nrows.hh>
# include <mln/geom/ninds.hh>
# include <mln/geom/nslis.hh>
# include <mln/data/paste.hh>
# include <mln/data/stretch.hh>
# include <mln/algebra/vec.hh>

# include <mln/linear/gaussian/internal/coefficients.hh>


namespace mln
{

  namespace linear
  {

    namespace gaussian
    {

      namespace impl
      {


# ifndef MLN_INCLUDE_ONLY

	template <class WorkType, class I>
	inline
	void
	recursivefilter_(I& ima,
			 const internal::coefficients& c,
			 const mln_psite(I)& start,
			 const mln_psite(I)& finish,
			 int len,
			 const mln_deduce(I, psite, delta)& d)
	{
	  std::vector<WorkType>	tmp1(len);
	  std::vector<WorkType>	tmp2(len);

	  // The fourth degree approximation implies to have a special
	  // look on the four first points we consider that there is
	  // no signal before 0 (to be discussed)

	  // --
	  // Causal part

	  tmp1[0] =
	    c.n[0] * ima(start);

	  tmp1[1] =
	    c.n[0] * ima(start + d)
	    + c.n[1] * ima(start)
	    - c.d[1] * tmp1[0];

	  tmp1[2] =
	    c.n[0] * ima(start + d + d)
	    + c.n[1] * ima(start + d)
	    + c.n[2] * ima(start)
	    - c.d[1] * tmp1[1]
	    - c.d[2] * tmp1[0];

	  tmp1[3] =
	    c.n[0] * ima(start + d + d + d)
	    + c.n[1] * ima(start + d + d)
	    + c.n[2] * ima(start + d)
	    + c.n[3] * ima(start)
	    - c.d[1] * tmp1[2] - c.d[2] * tmp1[1]
	    - c.d[3] * tmp1[0];

	  mln_psite(I) current(start + d + d + d + d);
	  for (mln_deduce(I, site, coord) i = 4; i < len; ++i)
	    {
	      tmp1[i] =
		c.n[0] * ima(current)
		+ c.n[1] * ima(current - d)
		+ c.n[2] * ima(current - d - d)
		+ c.n[3] * ima(current - d - d - d)
		- c.d[1] * tmp1[i - 1] - c.d[2] * tmp1[i - 2]
		- c.d[3] * tmp1[i - 3] - c.d[4] * tmp1[i - 4];
	      current = current + d;
	    }

	  // Non causal part

	  tmp2[len - 1] = WorkType(); // FIXME : = 0, literal::zero ...?

	  tmp2[len - 2] =
	    c.nm[1] * ima(finish);

	  tmp2[len - 3] =
	    c.nm[1] * ima(finish - d)
	    + c.nm[2] * ima(finish)
	    - c.dm[1] * tmp2[len - 2];

	  tmp2[len - 4] =
	    c.nm[1] * ima(finish - d - d)
	    + c.nm[2] * ima(finish - d)
	    + c.nm[3] * ima(finish)
	    - c.dm[1] * tmp2[len - 3]
	    - c.dm[2] * tmp2[len - 2];

	  current = finish - d - d - d ;

	  for (int i = len - 5; i >= 0; --i)
	    {
	      tmp2[i] =
		c.nm[1] * ima(current)
		+ c.nm[2] * ima(current + d)
		+ c.nm[3] * ima(current + d + d)
		+ c.nm[4] * ima(current + d + d + d)
		- c.dm[1] * tmp2[i + 1] - c.dm[2] * tmp2[i + 2]
		- c.dm[3] * tmp2[i + 3] - c.dm[4] * tmp2[i + 4];
	      current = current - d;
	    }

	  // Combine results from causal and non-causal parts.
	  current = start;
	  for (int i = 0; i < len; ++i)
	    {
	      ima(current) = tmp1[i] + tmp2[i];
	      current = current + d;
	    }
	}


	template <class I, class F>
	inline
	void
	generic_filter_(trait::image::dimension::one_d,
			Image<I>& img_, const F& coef, int dir)
	{
	  I& img = exact(img_);
	  typedef mln_site(I) S; // Help g++-2.95.

	  mln_precondition(dir < S::dim);

	  recursivefilter_<mln_value(I)>(img, coef,
					 point1d(static_cast<def::coord>(-img.border())),
					 point1d(static_cast<def::coord>(geom::ninds(img) - 1 +
									 img.border())),
					 geom::ninds(img) + 2 * img.border(),
					 dpoint1d(1));
	}

	template <class I, class F>
	inline
	void
	generic_filter_(trait::image::dimension::two_d,
			Image<I>& img_, const F& coef, int dir)
	{
	  I& img = exact(img_);
	  typedef mln_site(I) S; // Help g++-2.95.

	  mln_precondition(dir < S::dim);

	  if (dir == 0)
	    {
	      // Apply on rows.
	      for (unsigned j = 0; j < geom::ncols(img); ++j)
		recursivefilter_<mln_value(I)>(img, coef,
					       point2d(static_cast<def::coord>(-img.border()),
						       static_cast<def::coord>(j)),
					       point2d(static_cast<def::coord>(geom::nrows(img) - 1 +
									       img.border()),
						       static_cast<def::coord>(j)),
					       geom::nrows(img) + 2 * img.border(),
					       dpoint2d(1, 0));
	    }

	  if (dir == 1)
	    {
	      // Apply on columns.
	      for (unsigned i = 0; i < geom::nrows(img); ++i)
		recursivefilter_<mln_value(I)>(img, coef,
					       point2d(static_cast<def::coord>(i),
						       static_cast<def::coord>(-img.border())),
					       point2d(static_cast<def::coord>(i),
						       static_cast<def::coord>(geom::ncols(img) - 1 +
									       img.border())),
					       geom::ncols(img) + 2 * img.border(),
					       dpoint2d(0, 1));
	    }
	}

	template <class I, class F>
	inline
	void
	generic_filter_(trait::image::dimension::three_d,
			Image<I>& img_, const F& coef, int dir)
	{
	  I& img = exact(img_);
	  typedef mln_site(I) S; // Help g++-2.95.

	  mln_precondition(dir < S::dim);

	  if (dir == 0)
	    {
	      // Apply on slices.
	      for (unsigned j = 0; j < geom::nrows(img); ++j)
		for (unsigned k = 0; k < geom::ncols(img); ++k)
		  recursivefilter_<mln_value(I)>(img, coef,
						 point3d(static_cast<def::coord>(-img.border()),
							 static_cast<def::coord>(j),
							 static_cast<def::coord>(k)),
						 point3d(static_cast<def::coord>(geom::nslis(img) - 1 +
										 img.border()),
							 static_cast<def::coord>(j),
							 static_cast<def::coord>(k)),
						 geom::nslis(img) + 2 *
						 img.border(),
						 dpoint3d(1, 0, 0));
	    }


	  if (dir == 1)
	    {
	      // Apply on rows.
	      for (unsigned i = 0; i < geom::nslis(img); ++i)
		for (unsigned k = 0; k < geom::ncols(img); ++k)
		  recursivefilter_<mln_value(I)>(img, coef,
						 point3d(static_cast<def::coord>(i),
							 static_cast<def::coord>(-img.border()),
							 static_cast<def::coord>(k)),
						 point3d(static_cast<def::coord>(i),
							 static_cast<def::coord>(geom::nrows(img) - 1 +
										 img.border()),
							 static_cast<def::coord>(k)),
						 geom::nrows(img) + 2 *
						 img.border(),
						 dpoint3d(0, 1, 0));
	    }

	  if (dir == 2)
	    {
	      // Apply on columns.
	      for (unsigned i = 0; i < geom::nslis(img); ++i)
		for (unsigned j = 0; j < geom::nrows(img); ++i)
		  recursivefilter_<mln_value(I)>(img, coef,
						 point3d(static_cast<def::coord>(i),
							 static_cast<def::coord>(j),
							 static_cast<def::coord>(-img.border())),
						 point3d(static_cast<def::coord>(i),
							 static_cast<def::coord>(j),
							 static_cast<def::coord>(geom::ncols(img) -
										 1 + img.border())),
						 geom::ncols(img) + 2 *
						 img.border(),
						 dpoint3d(0, 0, 1));
	    }
	}



	template <class I, class F, class O>
	inline
	void
	generic_filter_common_(trait::value::nature::floating,
			       const Image<I>& in,
			       const F& coef,
			       double sigma,
			       Image<O>& out)
	{
	  typedef mln_site(I) S; // Help g++-2.95.

	  mln_ch_value(O, double) work_img(exact(in).domain());
	  data::paste(in, work_img);
	  extension::adjust_fill(work_img, 4, 0);

	  // On tiny sigma, Derich algorithm doesn't work.
	  // It is the same thing that to convolve with a Dirac.
	  if (sigma > 0.006)
	    for (int i = 0; i < S::dim; ++i)
	      generic_filter_(mln_trait_image_dimension(I)(),
			      work_img, coef, i);

	  // We don't need to convert work_img
	  data::paste(work_img, out);
	}

	template <class I, class F, class O>
	inline
	void
	generic_filter_common_(trait::value::nature::floating,
			       const Image<I>& in,
			       const F& coef,
			       double sigma,
			       Image<O>& out,
			       int dir)
	{
	  mln_ch_value(O, double) work_img(exact(in).domain());
	  data::paste(in, work_img);
	  extension::adjust_fill(work_img, 4, 0);

	  // On tiny sigma, Derich algorithm doesn't work.
	  // It is the same thing that to convolve with a Dirac.
	  if (sigma > 0.006)
	    generic_filter_(mln_trait_image_dimension(I)(),
			    work_img, coef, dir);

	  // We don't need to convert work_img
	  data::paste(work_img, out);
	}


	template <class I, class F, class O>
	inline
	void
	generic_filter_common_(trait::value::nature::scalar,
			       const Image<I>& in,
			       const F& coef,
			       double sigma,
			       Image<O>& out)
	{
	  typedef mln_site(I) S; // Help g++-2.95.

	  mln_ch_value(O, double) work_img(exact(in).domain());
	  data::paste(in, work_img);
	  extension::adjust_fill(work_img, 4, 0);

	  // On tiny sigma, Derich algorithm doesn't work.
	  // It is the same thing that to convolve with a Dirac.
	  if (sigma > 0.006)
	    for (int i = 0; i < S::dim; ++i)
	      generic_filter_(mln_trait_image_dimension(I)(),
			      work_img, coef, i);

	  // Convert work_img into result type
	  data::paste(data::stretch(mln_value(I)(), work_img), out);
	}

	template <class I, class F, class O>
	inline
	void
	generic_filter_common_(trait::value::nature::scalar,
			       const Image<I>& in,
			       const F& coef,
			       double sigma,
			       Image<O>& out,
			       int dir)
	{
	  mln_ch_value(O, double) work_img(exact(in).domain());
	  data::paste(in, work_img);
	  extension::adjust_fill(work_img, 4, 0);

	  // On tiny sigma, Derich algorithm doesn't work.
	  // It is the same thing that to convolve with a Dirac.
	  if (sigma > 0.006)
	    generic_filter_(mln_trait_image_dimension(I)(),
			    work_img, coef, dir);

	  // Convert work_img into result type
	  data::paste(data::stretch(mln_value(I)(), work_img), out);
	}



	template <class I, class F, class O>
	inline
	void
	generic_filter_common_(trait::value::nature::vectorial,
			       const Image<I>& in,
			       const F& coef,
			       double sigma,
			       Image<O>& out)
	{
	  typedef mln_site(I) S; // Help g++-2.95.

	  // typedef algebra::vec<3, double> vec3f;
	  // mln_ch_value(O, vec3f) work_img(exact(in).domain());
	  // FIXME : paste does not work (rgb8 -> vec3f).
	  data::paste(in, out);

	  // On tiny sigma, Derich algorithm doesn't work.
	  // It is the same thing that to convolve with a Dirac.
	  if (sigma > 0.006)
	    for (int i = 0; i < S::dim; ++i)
	      generic_filter_(mln_trait_image_dimension(I)(),
			      out, coef, i);
	}

	template <class I, class F, class O>
	inline
	void
	generic_filter_common_(trait::value::nature::vectorial,
			       const Image<I>& in,
			       const F& coef,
			       double sigma,
			       Image<O>& out,
			       int dir)
	{
	  // typedef algebra::vec<3, double> vec3f;
	  // mln_ch_value(O, vec3f) work_img(exact(in).domain());
	  // FIXME : paste does not work (rgb8 -> vec3f).
	  data::paste(in, out);

	  // On tiny sigma, Derich algorithm doesn't work.
	  // It is the same thing that to convolve with a Dirac.
	  if (sigma > 0.006)
	    generic_filter_(mln_trait_image_dimension(I)(),
			    out, coef, dir);
	}


# endif // ! MLN_INCLUDE_ONLY


    } // end of namespace mln::linear::gaussian::impl

  } // end of namespace mln::linear::gaussian

} // end of namespace mln::linear

} // end of namespace mln


#endif // ! MLN_LINEAR_GAUSSIAN_IMPL_HH
