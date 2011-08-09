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

#ifndef SCRIBO_UTIL_COMPONENT_PRECISE_OUTLINE_HH
# define SCRIBO_UTIL_COMPONENT_PRECISE_OUTLINE_HH

/*!
 * \file
 *
 * \brief Define a function which finds a precise and isothetic
 * outline.
 *
 */


# include <mln/io/pbm/load.hh>
# include <mln/literal/colors.hh>
# include <mln/io/ppm/save.hh>
# include <mln/data/convert.hh>
# include <mln/opt/at.hh>
# include <mln/extension/fill.hh>

# include <iostream>

#include <mln/io/pgm/save.hh>
#include <mln/data/wrap.hh>
#include <mln/data/convert.hh>


namespace scribo
{

  namespace util
  {

    using namespace mln;


    template <typename I>
    mln::p_array<point2d>
    component_precise_outline(const Image<I>& input_, const mln_value(I)& id,
			      bool compress_points);

    template <typename I>
    mln::p_array<point2d>
    component_precise_outline(const Image<I>& input_, const mln_value(I)& id);

    template <typename I>
    mln::p_array<point2d>
    component_precise_outline(const Image<I>& input_);


# ifndef MLN_INCLUDE_ONLY

    namespace internal
    {


      static const int offset[4][8][2] =
      {
	{ { -1, 0 }, { 0, -1 }, { -1, -1 }, { 1, 0 }, { 1, -1 }, { 0, 1 }, {
	    1, 1 }, { -1, 1 } },
	{ { 0, -1 }, { 1, 0 }, { 1, -1 }, { 0, 1 }, { 1, 1 }, { -1, 0 }, {
	    -1, 1 }, { -1, -1 } },
	{ { 1, 0 }, { 0, 1 }, { 1, 1 }, { -1, 0 }, { -1, 1 }, { 0, -1 }, {
	    -1, -1 }, { 1, -1 } },
	{ { 0, 1 }, { -1, 0 }, { -1, 1 }, { 0, -1 }, { -1, -1 }, { 1, 0 }, {
	    1, -1 }, { 1, 1 } }
      };


      template <typename I>
      void
      find_first_point(const I& input,
		       point2d& p,
		       const mln_value(I)& id)
      {
	const mln::def::coord
	  mid_row = geom::min_row(input) + (geom::nrows(input) >> 1);

	for (mln::def::coord i = geom::min_col(input);
	     i <= geom::max_col(input); ++i)
	{
	  if (opt::at(input, mid_row, i) == id)
	  {
	    p.row() = mid_row;
	    p.col() = i;
	    break;
	  }
	}
      }

      template <typename I>
      void
      left_up(int& direction,
	      const I& input,
	      const point2d& cur_pt,
	      const mln_value(I)& id)
      {
	const point2d p2(cur_pt.row() + offset[direction][5][1],
			 cur_pt.col() + offset[direction][5][0]);
	const point2d p3(cur_pt.row() + offset[direction][7][1],
			 cur_pt.col() + offset[direction][7][0]);

	if ((input(p2) != id) && (input(p3) == id))
	{
	  direction = 3;
	  return;
	}
      }

      inline
      void
      left_up_after(int& direction,
		    const unsigned i)
      {
	if (i == 3 || i == 4)
	  direction = 1;
	else if (i == 5 || i == 6)
	  direction = 2;
	else if (i == 7)
	  direction = 3;
      }

      template <typename I>
      void
      right_up(int& direction,
	       const I& input,
	       const point2d& cur_pt,
	       const mln_value(I)& id)
      {
	const point2d p1(cur_pt.row() + offset[direction][0][1],
			 cur_pt.col() + offset[direction][0][0]);
	const point2d p2(cur_pt.row() + offset[direction][5][1],
			 cur_pt.col() + offset[direction][5][0]);
	const point2d p3(cur_pt.row() + offset[direction][7][1],
			 cur_pt.col() + offset[direction][7][0]);

	if ((input(p2) != id) && ((input(p1) == id) || (input(p3) == id)))
	{
	  direction = 0;
	  return;
	}
      }

      inline
      void
      right_up_after(int& direction,
		     const unsigned i)
      {
	if (i == 3 || i == 4)
	  direction = 2;
	else if (i == 5 || i == 6)
	  direction = 3;
	else if (i == 7)
	  direction = 0;
      }

      template <typename I>
      void
      right_down(int& direction,
		 const I& input,
		 const point2d& cur_pt,
		 const mln_value(I)& id)
      {
	const point2d p2(cur_pt.row() + offset[direction][5][1],
			 cur_pt.col() + offset[direction][5][0]);
	const point2d p3(cur_pt.row() + offset[direction][7][1],
			 cur_pt.col() + offset[direction][7][0]);

	if ((input(p2) != id) && (input(p3) == id))
	{
	  direction = 1;
	  return;
	}
      }

      inline
      void
      right_down_after(int& direction,
		       const unsigned i)
      {
	if (i == 3 || i == 4)
	  direction = 3;
	else if (i == 5 || i == 6)
	  direction = 0;
	else if (i == 7)
	  direction = 1;
      }

      template <typename I>
      void
      left_down(int& direction,
		const I& input,
		const point2d& cur_pt,
		const mln_value(I)& id)
      {
	const point2d p1(cur_pt.row() + offset[direction][0][1],
			 cur_pt.col() + offset[direction][0][0]);
	const point2d p2(cur_pt.row() + offset[direction][5][1],
			 cur_pt.col() + offset[direction][5][0]);
	const point2d p3(cur_pt.row() + offset[direction][7][1],
			 cur_pt.col() + offset[direction][7][0]);

	if ((input(p2) != id) && ((input(p1) == id) || (input(p3) == id)))
	{
	  direction = 2;
	  return;
	}
      }

      inline
      void
      left_down_after(int& direction,
		      const unsigned i)
      {
	if (i == 3 || i == 4)
	  direction = 0;
	else if (i == 5 || i == 6)
	  direction = 1;
	else if (i == 7)
	  direction = 2;
      }


      template <typename I>
      void
      find_next_point(const I& input,
		      point2d& cur_pt,
		      int& direction,
		      const mln_value(I)& id)
      {
	unsigned i = 0;
	point2d tmp;

	switch (direction)
	{
	  case 0: left_up(direction, input, cur_pt, id); break;
	  case 1: right_up(direction , input, cur_pt, id); break;
	  case 2: right_down(direction, input, cur_pt, id); break;
	  case 3: left_down(direction, input, cur_pt, id); break;
	}

	for (; i < 8; ++i)
	{
	  tmp = point2d(cur_pt.row() + offset[direction][i][1],
			cur_pt.col() + offset[direction][i][0]);

	  if (input(tmp) == id)
	    break;
	}

	// Should not happen
	if (i == 8)
	  return;

	switch (direction)
	{
	  case 0: left_up_after(direction, i); break;
	  case 1: right_up_after(direction , i); break;
	  case 2: right_down_after(direction, i); break;
	  case 3: left_down_after(direction, i); break;
	}

	cur_pt = tmp;
      }

      inline
      void
      filter_points(const mln::p_array<point2d>& points,
		    mln::p_array<point2d>& waypoints)
      {
	const unsigned nelements = points.nsites();

	if (nelements > 2)
	{
	  const point2d* first_pt = & points[0];
	  const point2d* last_pt = first_pt;
	  waypoints.append(*first_pt);
	  unsigned i = 1;
	  const point2d* cur_pt = & points[i];
	  bool has_changed = false;

	  while (i < nelements)
	  {
	    has_changed = false;

	    while (cur_pt->row() == first_pt->row()
		   || cur_pt->col() == first_pt->col())
	    {
	      has_changed = true;
	      ++i;
	      if (i == nelements)
		break;

	      const point2d* llast_pt = last_pt;
	      last_pt = cur_pt;
	      cur_pt = & points[i];

	      if (llast_pt->col() == last_pt->col() && last_pt->col() == cur_pt->col())
	      {
		if (llast_pt->row() == cur_pt->row())
		  break;
	      }
	      else if(llast_pt->row() == last_pt->row() && last_pt->row() == cur_pt->row())
	      {
		if (llast_pt->col() == cur_pt->col())
		  break;
	      }
	    }

	    if (!has_changed)
	    {
	      ++i;
	      last_pt = cur_pt;
	      if (i < nelements)
		cur_pt = & points[i];
	    }

	    if (i == nelements)
	      break;

	    waypoints.append(*last_pt);
	    first_pt = last_pt;
	    last_pt = first_pt;
	  }

	  waypoints.append(*cur_pt);
	}
	else
	{
	  for (unsigned i = 0; i < nelements; ++i)
	    waypoints.append(points[i]);
	}
      }
    } // end of namespace scribo::util::internal


    template <typename I>
    mln::p_array<point2d>
    component_precise_outline(const Image<I>& input_, const mln_value(I)& id,
			      bool compress_points)
    {
      trace::entering("scribo::util::component_precise_outline");

      const I& input = exact(input_);
      typedef mln_site(I) P;

      extension::adjust_fill(input, 3, 0);

      mln::p_array<P> points;
      points.reserve(std::max(geom::ncols(input), geom::nrows(input)));

      point2d start_pt;
      int direction = 0;

      internal::find_first_point(input, start_pt, id);

      P cur_pt = start_pt;

      internal::find_next_point(input, cur_pt, direction, id);
      points.append(cur_pt);

      while (cur_pt != start_pt)
      {
	internal::find_next_point(input, cur_pt, direction, id);
	points.append(cur_pt);
      }

      internal::find_next_point(input, cur_pt, direction, id);

      const std::vector<point2d>& vec_points = points.hook_std_vector_();

      if (std::find(vec_points.begin(),
		    vec_points.end(), cur_pt) == vec_points.end())
      {
	points.append(cur_pt);

	while (cur_pt != start_pt)
	{
	  internal::find_next_point(input, cur_pt, direction, id);
	  points.append(cur_pt);
	}
      }

      if (compress_points)
      {
	mln::p_array<P> waypoints;
	internal::filter_points(points, waypoints);

	trace::exiting("scribo::util::component_precise_outline");
	return waypoints;
      }

      trace::exiting("scribo::util::component_precise_outline");
      return points;
    }

    template <typename I>
    mln::p_array<point2d>
    component_precise_outline(const Image<I>& input, const mln_value(I)& id)
    {
      return component_precise_outline(input, id, true);
    }

    template <typename I>
    mln::p_array<point2d>
    component_precise_outline(const Image<I>& input)
    {
      return component_precise_outline(input, true, true);
    }

# endif // ! MLN_INCLUDE_ONLY

  } // end of namespace scribo::util

} // end of namespace scribo

#endif // ! SCRIBO_UTIL_COMPONENT_PRECISE_OUTLINE_HH
