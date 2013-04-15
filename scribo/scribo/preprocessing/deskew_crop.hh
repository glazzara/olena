// Copyright (C) 2010, 2011 EPITA Research and Development Laboratory
// (LRDE)
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

#ifndef SCRIBO_PREPROCESSING_DESKEW_CROP_HH
# define SCRIBO_PREPROCESSING_DESKEW_CROP_HH

/// \file
///
/// \brief Deskew a region of interest.

# include <scribo/preprocessing/deskew.hh>

namespace scribo
{

  namespace preprocessing
  {

    /*! \brief Deskew a region of interest.

      \param[in] crop_gl  A gray-level image.
      \param[in] input_gl A gray-level image.

      \return A deskewed binary image.

      Handles skew angles from -25 to +25 degrees.

      \p crop_gl and \p input_gl must be 2D images and must be
      identical (e.g. only the value differs).

      This algorithm is designed for images created from a region of
      interest (e.g. Not a full document).

     */
    template <typename I, typename J>
    mln_concrete(I)
    deskew(const Image<I>& crop_gl, const Image<I>& input_gl);

# ifndef MLN_INCLUDE_ONLY

    namespace internal
    {


      /*! \brief Compute the skew angle using a Hough transform.

	\param[in] input_bin  A binary image.
	\param[in] input_gray A gray-level image.
	\param[in] threshold
	\param[in] length

	\return A deskewed binary image.

	This algorithm tries to use only the sites located on the
	baselines (the most relevant sites for deskewing).
	A site is considered as relevant for this algorithm if :
	- \p threshold sites in the window of length \p length are set to True.
	- The bottom left, bottom and bottom right sites are set to False.

	Note: Increasing \p length value while keeping threshold value
	low increases the number of relevant sites.

	Handles skew angles from -25 to +25 degrees.

	\p input_bin and \p input_gray must be 2D images and must be
	identical (e.g. only the value differs).

	This algorithm is designed for images created from a region of
	interest (e.g. Not a full document).

       */
//       double
//       perform_deskew_crop(const image2d<bool>&	input_bin,
// 			  const image2d<value::int_u8>& input_gray,
// 			  int threshold,
// 			  int length)
//       {
// 	Hough hough(input_bin.ncols(), input_bin.nrows());
// 	int
// 	  mid = length >> 1,
// 	  max = 0;
// 	double pos = 0;

// 	for (int i = 1; i < input_bin.nrows() - 1; ++i)
// 	{
// 	  int
// 	    acc = 0,
// 	    begin = 0,
// 	    end = length;

// 	  for (int j = 0; j < length; ++j)
// 	    acc += opt::at(input_bin, i, j);

// 	  for (int j = mid + 1; end < input_bin.ncols(); ++j, ++end, ++begin)
// 	  {
// 	    acc += opt::at(input_bin, i, end) - opt::at(input_bin, i, begin);

// 	    if (acc > threshold && !opt::at(input_bin, i + 1, j) &&
// 		!opt::at(input_bin, i + 1, j - 1) && !opt::at(input_bin, i + 1, j + 1))
// 	    {
// 	      double gy = opt::at(input_gray, i - 1, j - 1) + 2 * opt::at(input_gray, i - 1, j) + opt::at(input_gray, i - 1, j + 1);
// 	      gy += - opt::at(input_gray, i + 1, j - 1) - 2 * opt::at(input_gray, i + 1, j) - opt::at(input_gray, i + 1, j + 1);

// 	      double gx = opt::at(input_gray, i - 1, j - 1) + 2 * opt::at(input_gray, i, j - 1) + opt::at(input_gray, i + 1, j - 1);
// 	      gx += - opt::at(input_gray, i - 1, j + 1) - 2 * opt::at(input_gray, i, j + 1) - opt::at(input_gray, i + 1, j + 1);

// 	      if (abs(gx) + abs(gy) >= 255)
// 		vote(j, i, deskew, &max, &pos);
// 	    }
// 	  }
// 	}

// 	return 90 - (pos + 650) / 10.0f;
//       }


    } // end of namespace scribo::preprocessing::internal


    // Facade


    template <typename I>
    mln_concrete(I)
    deskew(const Image<I>& crop_gl_, const Image<I>& input_gl_)
    {
      const I& crop_gl = exact(crop_gl_);
      const I& input_gl = exact(input_gl_);

      mln_trace("scribo::preprocessing::deskew_crop");
      mln_assertion(crop_gl.is_valid());
      mln_assertion(input_gl.is_valid());
      mlc_is(mln_domain(I), box2d)::check();
      mlc_is_not(mln_value(I), bool)::check();
      mlc_is_not_a(mln_value(I), value::Vectorial)::check();

      double angle = internal::perform_deskew(crop_gl);
      std::cout << angle << std::endl;

      mln_concrete(I) output = crop_gl;

      // FIXME: trick to make this routine faster for really small
      // angles (no impact on the results)
      if (angle > 0.5 || angle < -0.5)
	output = geom::rotate(crop_gl, - angle,
			      //mln_max(mln_value(I)),
			      extend(input_gl, mln_max(mln_value(I))),
			      make::box2d(crop_gl.nrows(),
					  crop_gl.ncols()));

      return output;
    }

# endif // ! MLN_INCLUDE_ONLY


  } // end of namespace scribo::preprocessing

} // end of namespace scribo


# endif // SCRIBO_PREPROCESSING_DESKEW_CROP_HH
