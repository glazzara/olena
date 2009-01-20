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

/// \file  tests/util/fibonacci_heap.cc
///
/// Definition of a generic vector class.

#include <mln/util/fibonacci_heap.hh>
#include <mln/core/alias/point2d.hh>


using mln::point2d;

point2d p[] = { point2d(4,5), point2d(3,4), point2d(3,2),
		point2d(1,6), point2d(2,3), point2d(3,5),
		point2d(2,4), point2d(7,2), point2d(9,6),
		point2d(7,3) };

point2d res_1[] = { point2d(1,6), point2d(2,3), point2d(2,4) };

point2d res_2[] = { point2d(53,4), point2d(5,4), point2d(1,4),
		    point2d(3,2),  point2d(3,4), point2d(3,5),
		    point2d(4,5),  point2d(7,2), point2d(7,3),
		    point2d(9,6) };


int main()
{
  using namespace mln;

  /// Init heap
  util::fibonacci_heap<int,point2d> heap;
  for (unsigned i = 0; i < 5; ++i)
    heap.push(3, p[i]);

  /// Init heap2
  util::fibonacci_heap<int,point2d> heap2;
  for (unsigned i = 5; i < 10; ++i)
    heap2.push(3, p[i]);


  /*!
  **
  **  | |     | |      |x|     | |
  **  |	|     |	|      |x|     | |
  **  |x|  +  |x|  =>  |x|  +  | |
  **  |x|     |x|      |x|     | |
  **   -       -        -       -
  **  heap   heap2     heap    heap2
  **
  */
  /// Merge heap and heap2.
  heap.push(heap2);

  /// Heap2 is now empty
  mln_assertion(heap2.is_empty());
  mln_assertion(heap2.nelements() == 0);

  /// Check if the front() is correct in heap
//  mln_assertion(heap.front() == res_1[0]);


  /*!
  **
  **  |x|     | |      | |     |x|
  **  |x|     |	|      | |     |x|
  **  |x|  +  | |  =>  | |  +  |x|
  **  |x|     | |      | |     |x|
  **   -       -        -       -
  **  heap   heap2     heap    heap2
  **
  */
  /// Heap2 is empty and heap is full of elements.
  /// Move the content of heap into heap2.
  heap2.push(heap);

  /// Heap is now empty
  mln_assertion(heap.is_empty());
  mln_assertion(heap.nelements() == 0);

  /// Check if the front() is correct
  mln_assertion(heap2.front() == res_1[0]);


  /// Extract and delete few front elements.
  for (unsigned i = 0; i < 3; ++i)
    mln_assertion(heap2.pop_front() == res_1[i]);


  /// Re-insert data after deletion...
  heap2.push(1, point2d(53,4));
  heap2.push(3, point2d(1,4));
  heap2.push(2, point2d(5,4));

  /// ... and try to extract and delete data again.
  unsigned i = 0;
  while (heap2.is_valid())
  {
    mln_assertion(heap2.pop_front() == res_2[i++]);
    mln_assertion(heap2.nelements() == (10 - i));
  }

  /// The heap must be empty.
  mln_assertion(heap2.is_empty());
  mln_assertion(heap2.nelements() == 0);



  /// Swap two variables thanks to a temporary.
  heap.push(3, point2d(2,4));
  heap2.push(3, point2d(53,4));
  util::fibonacci_heap<int,point2d> tmp = heap;
  heap = heap2;
  heap2 = tmp;

  mln_assertion(heap2.nelements() == 1);
  mln_assertion(heap.nelements() == 1);
  mln_assertion(tmp.nelements() == 0);


  /// Testing copy constructor.
  util::fibonacci_heap<int,point2d> tmp2(heap);

  mln_assertion(tmp2.nelements() == 1);
  mln_assertion(heap.nelements() == 0);

}
