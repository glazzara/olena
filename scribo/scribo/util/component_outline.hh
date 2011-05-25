// Copyright (C) 2011 EPITA Research and Development Laboratory (LRDE)
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

#ifndef SCRIBO_UTIL_COMPONENT_OUTLINE_HH
# define SCRIBO_UTIL_COMPONENT_OUTLINE_HH

/*!
 * \file
 *
 * \brief Define a function which finds a polygonal approximation
 * of a component's contour using the Douglas-Peucker algorithm.
 *
 */

# include <mln/core/concept/image.hh>
# include <mln/core/site_set/p_array.hh>
# include <mln/geom/all.hh>

# include <utility>
# include <cmath>

#include <mln/accu/shape/bbox.hh>

namespace scribo
{

  namespace util
  {

    /*! Finds the 'douglas-peucker' polygonal approximation.
     *
     * Assumes there's only one centered component.
     * More information about the algorithm can be found here :
     * http://en.wikipedia.org/wiki/Ramer-Douglas-Peucker_algorithm
     *
     * Returns an ordered vector of points defining the polygonal contour.
     *
     * \param[in] ima_ The component whose contour is sought.
     * \param[in] precision The max distance between the approximation
     * and the curve.
     */
    template <typename I>
    mln::p_array<mln::point2d>
    component_outline(const mln::Image<I>& ima, float precision);


# ifndef MLN_INCLUDE_ONLY

    namespace internal
    {
      typedef enum
      {
	DIR_RIGHT = 0,
	DIR_RIGHT_UP = 1,
	DIR_UP = 2,
	DIR_LEFT_UP = 3,
	DIR_LEFT = 4,
	DIR_LEFT_DOWN = 5,
	DIR_DOWN = 6,
	DIR_RIGHT_DOWN = 7,
	DIR_NONE = 8
      } e_dir;

      /// Give the point2d which is in the given dir relatively to p.
      static inline
      mln::point2d
      iterate_in_dir(const mln::point2d& p,
		     e_dir dir)
      {
	int dx = 0;
	int dy = 0;

	switch (dir)
	{
	  case DIR_RIGHT:
	    dx = 1;
	    break;
	  case DIR_RIGHT_UP:
	    dx = 1;
	    dy = -1;
	    break;
	  case DIR_UP:
	    dy = -1;
	    break;
	  case DIR_LEFT_UP:
	    dx = dy = -1;
	    break;
	  case DIR_LEFT:
	    dx = -1;
	    break;
	  case DIR_LEFT_DOWN:
	    dx = -1;
	    dy = 1;
	    break;
	  case DIR_DOWN:
	    dy = 1;
	    break;
	  case DIR_RIGHT_DOWN:
	    dx = dy = 1;
	    break;
	  default:
	    break;
	}

	return mln::point2d(p[0] + dx, p[1] + dy);
      }

      /// Decrement the given direction.
      static inline
      void
      dec_dir(e_dir* d)
      {
	*d = (e_dir) (((unsigned)*d + 7) & 7);
      }

      /// Increment the given direction (clockwise).
      static inline
      void
      inc_dir(e_dir* d)
      {
	*d = (e_dir) (((unsigned)*d + 9) & 7);
      }

      /// Inverse the given direction.
      static inline
      void
      inverse_dir(e_dir* d)
      {
	*d = (e_dir) (((unsigned)*d + 4) & 7);
      }

      /// Initialize the direction in which to find next points in the contour
      template <typename I>
      static inline
      e_dir
      find_first_dir(const I& ima,
		     const mln::point2d& p)
      {
	e_dir dir = DIR_NONE;

	mln::point2d n;
	do
	{
	  dec_dir(&dir);
	  n = iterate_in_dir(p, dir);
	}
	while (ima(n) != mln::literal::zero);

	while (ima(n) == mln::literal::zero)
	{
	  inc_dir(&dir);
	  n = iterate_in_dir(p, dir);
	}

	return (dir);
      }

      /// find the next point in the contour of the image
      template <typename I>
      static
      mln::point2d
      next_pt_in_contour(const I& ima,
			 const mln::point2d& ori,
			 e_dir* last_dir)
      {
	e_dir dir = (*last_dir);
	dec_dir(&dir);
	mln::point2d cur;

	do
	{
	  cur = iterate_in_dir(ori, dir);
	  dec_dir(&dir);
	}
	while (ima(cur) == mln::literal::zero);

	*last_dir = dir;
	inverse_dir(last_dir);
	return cur;
      }

      /// Give an array of the pixels on the contour
      /// Ordered so that two consecutive indexes correspond
      /// to two adjacent points.
      template <typename I>
      mln::p_array<mln::point2d>
      get_contour(const I&ima,
		  const mln::point2d& first)
      {
	mln::p_array<mln::point2d> contour;
	mln::point2d current;

	e_dir dir = find_first_dir(ima, first);

	contour.append(first);

	current = next_pt_in_contour(ima, first, &dir);
	
	while (current != first)
	  {
	    contour.append(current);
	    current = next_pt_in_contour(ima, current, &dir);
	  }

	return contour;
      }

      /// Give the next point in the contour array
      inline
      unsigned
      get_next(mln::p_array<mln::point2d>& contour,
	       unsigned i)
      {
	if (i + 1 == contour.nsites())
	  return 0;
	else
	  return i + 1;
      }

      /// Give The farthest point from line (begin, end) or NULL
      /// if that distance is inferior to the given precision. (bottleneck)
      inline
      unsigned
      find_farthest(mln::p_array<mln::point2d>& contour,
		    unsigned begin,
		    unsigned end,
		    float precision)
      {
	float d = 0.;
	float dmax = 0.;

	float a = contour[end][1] - contour[begin][1];
	float b = contour[begin][0] - contour[end][0];
	float c = contour[begin][1] * contour[end][0]
	  - contour[begin][0] * contour[end][1];
	float norm = sqrt ((a * a) + (b * b));

	unsigned max;
	unsigned cur;
	unsigned old;

	cur = begin;
	max = cur;

	while (cur != end)
	  {
	    old = cur;
	    cur = get_next(contour, cur);

	    d = a * contour[cur][0] + contour[cur][1] * b + c;
	    
	    if (d < 0)
	      d = -d;
	    
	    if (d >= dmax)
	      {
		dmax = d;
		max = cur;
	      }
	  }
	
	if (dmax >= precision * norm)
	  return max;
	else
	  return begin;
      }

      /// Split the line (begin, end) at the farthest point on the curve
      /// and apply itself recursively.
      void
      split_rec(unsigned begin,
		unsigned end,
		mln::p_array<mln::point2d>& l,
		mln::p_array<mln::point2d>& contour,
		float precision)
      {
	unsigned node = find_farthest(contour, begin, end, precision);

	if (node != begin)
	  {
	    split_rec(begin, node, l, contour, precision);

	    l.append(contour[node]);

	    split_rec(node, end, l, contour, precision);
	  }

      }

      /// Find (simply) 2 points on the contour of the component
      template <typename I>
      static inline
      std::pair<mln::point2d, mln::point2d>
      get_initials(const I& ima)
      {
	mln::point2d begin(mln::geom::min_row(ima),
			   mln::geom::max_col(ima)
			   - ima.ncols() / 2);
	mln::point2d end(mln::geom::max_row(ima),
			 mln::geom::max_col(ima)
			 - ima.ncols() / 2);

	while (ima(begin) == mln::literal::zero)
	  begin[0]++;

	while (ima(end) == mln::literal::zero)
	  end[0]--;

	return std::pair<mln::point2d, mln::point2d> (begin, end);
      }
    } // end of namespace internal




    template <typename I>
    mln::p_array<mln::point2d>
    component_outline(const mln::Image<I>& ima_,
		    float precision)
    {
      mln::trace::entering("scribo::component_outline");

      const I& ima = exact(ima_);

      mln_precondition(ima.is_valid());

      mln::accu::shape::bbox<mln::point2d> ab;
      mln_piter(I) p(ima.domain());
      for_all(p)
	ab.take(p);

      std::cout << ab.to_result() << std::endl;

      mln::p_array<mln::point2d> approx_contour;
      std::pair<mln::point2d, mln::point2d> initials =
	internal::get_initials(ima);

      mln::p_array<mln::point2d> contour;

      contour = internal::get_contour(ima, initials.first);

      std::cout << " - " << initials.first
		<< " - " << initials.first << std::endl;

      internal::split_rec(0,
			  contour.nsites() / 2,
			  approx_contour,
			  contour,
			  precision);
      internal::split_rec(contour.nsites() / 2,
			  0,
			  approx_contour,
			  contour,
			  precision);

      mln::trace::exiting("scribo::component_outline");

      return approx_contour;
    }

# endif // ! MLN_INCLUDE_ONLY

  } // end of namespace util

} // end of namespace scribo

#endif // ! SCRIBO_UTIL_COMPONENT_OUTLINE_HH
