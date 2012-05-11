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

/// \file
///
/// \brief Various utilities for image extraction
///
/// \fixme To be cleanup

#ifndef SCRIBO_PRIMITIVE_EXTRACT_INTERNAL_UNION_HH
# define SCRIBO_PRIMITIVE_EXTRACT_INTERNAL_UNION_HH

# include <mln/core/image/image2d.hh>
# include <mln/border/fill.hh>


namespace scribo
{

  namespace primitive
  {

    namespace extract
    {

      namespace internal
      {

	using namespace mln;


	unsigned find_root(image2d<unsigned>& parent, unsigned x);


	void union_find(const image2d<bool>& input, bool lab,
			image2d<unsigned>& parent, image2d<unsigned>& area,
			unsigned& max_area);


# ifndef MLN_INCLUDE_ONLY

	inline
	unsigned find_root(image2d<unsigned>& parent, unsigned x)
	{
	  if (parent.element(x) == x)
	    return x;
	  return parent.element(x) = find_root(parent, parent.element(x));
	}


	inline
	void union_find(const image2d<bool>& input,
			bool lab,
			// output:
			image2d<unsigned>& parent,
			image2d<unsigned>& area,
			unsigned& max_area)
	{
	  const unsigned nrows = input.nrows(), ncols = input.ncols();

	  unsigned op, on, delta = input.delta_offset(dpoint2d(1, 0));

	  data::fill(parent, 0);
	  max_area = 0;

	  {

	    // row == 0 and col == 0

	    op = input.offset_of_point(point2d(0,0));
	    if (input.element(op) == lab)
	    {
	      area.element(op) = 1;
	      parent.element(op) = op;
	    }


	    // row = 0

	    for (unsigned col = 1; col < ncols; ++col)
	    {
	      on = op;
	      ++op;

	      if (input.element(op) != lab)
		continue;

	      if (input.element(on) == lab)
	      {
		unsigned& par_p = parent.element(op);
		par_p = find_root(parent, on);
		++area.element(par_p);
		if (area.element(par_p) > max_area)
		  max_area = area.element(par_p);
	      }
	      else
	      {
		area.element(op) = 1;
		parent.element(op) = op;
	      }

	    }
	  }


	  for (unsigned row = 1; row < nrows; ++row)
	  {

	    {
	      // col == 0

	      op = input.offset_of_point(point2d(row, 0));
	      on = op - delta;

	      if (input.element(op) == lab)
	      {
		if (input.element(on) == lab)
		{
		  unsigned& par_p = parent.element(op);
		  par_p = find_root(parent, on);
		  ++area.element(par_p);
		  if (area.element(par_p) > max_area)
		    max_area = area.element(par_p);
		}
		else
		{
		  area.element(op) = 1;
		  parent.element(op) = op;
		}
	      }

	    }

	    for (unsigned col = 1; col < ncols; ++col)
	    {
	      ++op;
	      ++on;

	      if (input.element(op) != lab)
		continue;

	      bool merge_ = false;

	      // up

	      if (input.element(on) == lab)
	      {
		unsigned& par_p = parent.element(op);
		par_p = find_root(parent, on);
		++area.element(par_p);
		if (area.element(par_p) > max_area)
		  max_area = area.element(par_p);
		merge_ = true;
	      }


	      // left

	      unsigned ol = op - 1;

	      if (input.element(ol) == lab)
	      {
		if (merge_)
		{
		  if (input.element(on - 1) != lab) // not already merged
		  {
		    unsigned r_op = parent.element(op), r_ol = find_root(parent, ol);
		    if (r_op != r_ol)
		    {
		      // do-union
		      if (r_op < r_ol)
		      {
			parent.element(r_ol) = r_op;
			area.element(r_op) += area.element(r_ol);
			if (area.element(r_op) > max_area)
			  max_area = area.element(r_op);
		      }
		      else
		      {
			parent.element(r_op) = r_ol;
			area.element(r_ol) += area.element(r_op);
			if (area.element(r_ol) > max_area)
			  max_area = area.element(r_ol);
		      }
		    }
		  }
		} // end of "if (merge)
		else
		{
		  unsigned& par_p = parent.element(op);
		  par_p = find_root(parent, ol);
		  ++area.element(par_p);
		  if (area.element(par_p) > max_area)
		    max_area = area.element(par_p);
		  merge_ = true;
		}
	      }


	      // finalization

	      if (merge_ == false)
	      {
		parent.element(op) = op;
		area.element(op) = 1;
	      }

	    }
	  }

	} // end of 'union_find'

# endif // ! MLN_INCLUDE_ONLY

      } // end of namespace scribo::primivite::extract::internal

    } // end of namespace scribo::primitive::extract

  } // end of namespace scribo::primitive

} // end of namespace scribo


#endif // ! SCRIBO_PRIMITIVE_EXTRACT_INTERNAL_UNION_HH
