// Copyright (C) 2009 EPITA Research and Development Laboratory
// (LRDE)
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


#ifndef SCRIBO_TABLE_INTERNAL_ALIGN_LINES_HH
# define SCRIBO_TABLE_INTERNAL_ALIGN_LINES_HH

/// \file scribo/table/internal/align_lines.hh
///
/// Align table lines verticaly or horizontaly.


# include <mln/core/site_set/box.hh>
# include <mln/util/array.hh>
# include <mln/util/set.hh>

# include <scribo/core/macros.hh>


namespace scribo
{

  namespace table
  {

    namespace internal
    {

      /// Align table lines bboxes according to a given dimension.
      ///
      /// \return A list of the resulting aligned cols. Each integer is actually
      ///		a col number.
      /*
      **
      **	  0 1 3 4 5 6
      **	  ------------	  -------
      **       0 |- - - - -   |	 | {0,1} |
      **       1 |        - - |	 | {0,1} |
      **       2 |            |	 | {1}	 |
      **       3 |	      |	 |	 |
      **       4 |	      |	 | {2}	 |
      **       5 |- -	      |	 | {2}	 |
      **       6 |	      |	 | {2}	 |
      **
      ** \p hboxes contains all the table lines bboxes. Each bbox is
      ** associated with an id, its location in the array.
      **
      ** For each bbox, its id is marked in a vector. The location is defined,
      ** according to the given parameter \p dim, either by the row or the col
      ** value of the pmin site.
      **
      ** Ids are then propagated in the vector according a small delta value.
      ** if bbox ids are at the same location in the vector, their related bboxes
      ** are likely to be on the same line.
      **
      ** Finally, iterate over the vector until all bboxes have been treated.
      ** For each iteration, the set with a specific number of elements is found
      ** and all bboxes referenced in this set are aligned on the same row or col.
      **
      */
      template <typename P>
      util::array<int>
      align_lines(unsigned nsites,
		  int min_coord,
		  int max_coord,
		  util::array<box<P> >& line_boxes,
		  unsigned dim);



# ifndef MLN_INCLUDE_ONLY


      template <typename P>
      util::array<int>
      align_lines(unsigned nsites,
		  int min_coord,
		  int max_coord,
		  util::array<box<P> >& line_boxes,
		  unsigned dim)
      {
	trace::entering("scribo::internal::align_lines");

	mln_precondition(nsites > 0);

	util::array< util::set<unsigned> > lines;
	lines.resize(nsites);

	// Map components with actual lines.
	for_all_components(i, line_boxes)
	{
	  int minline = line_boxes[i].pmin()[dim] - 5;
	  minline = (minline < min_coord ? min_coord : minline);
	  int maxline = line_boxes[i].pmax()[dim] + 5;
	  maxline = (maxline > max_coord ? max_coord : maxline);

	  for (int line = minline;
	      line <= maxline; ++line)
	    lines[line].insert(i);
	}

	// Init box2line
	util::array<int> box2line;
	box2line.resize(line_boxes.nelements());
	for_all_elements(i, box2line)
	  box2line[i] = -1;

	// Find the line with the highest element count.
	unsigned max_nelts = 0;
	for_all_elements(i, lines)
	  if (max_nelts < lines[i].nelements())
	    max_nelts = lines[i].nelements();

	// Aligning lines
	// FIXME: not optimal... Make it faster!
	// We may do too much iterations (while loop) and some of them may
	// be done for nothing...
	util::array<int> newlines;
	while (max_nelts > 0)
	{
	  for_all_elements(i, lines)
	    if (lines[i].nelements() == max_nelts)
	    {
	      accu::mean<unsigned> mean;
	      for_all_elements(j, lines[i])
		if (box2line[lines[i][j]] == -1)
		  mean.take(line_boxes[lines[i][j]].center()[dim]);

	      if (mean.is_valid())
	      {
		for_all_elements(j, lines[i])
		  if (box2line[lines[i][j]] == -1)
		  {
		    line_boxes[lines[i][j]].pmin()[dim] = mean.to_result();
		    line_boxes[lines[i][j]].pmax()[dim] = mean.to_result();
		    box2line[lines[i][j]] = mean.to_result();
		  }
		newlines.append(mean.to_result());
	      }
	    }
	  --max_nelts;
	}

	trace::exiting("scribo::internal::align_lines");
	return newlines;
      }


# endif // ! MLN_INCLUDE_ONLY


    } // end of namespace scribo::table::internal

  } // end of namespace scribo::table

} // end of namespace scribo

#endif // ! SCRIBO_TABLE_INTERNAL_ALIGN_LINES_HH
