// Copyright (C) 2007, 2008 EPITA Research and Development Laboratory
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

#ifndef MLN_CORE_CLOCK_NEIGHB2D_HH
# define MLN_CORE_CLOCK_NEIGHB2D_HH

/// \file mln/core/clock_neighb2d.hh
///
/// Definition of the mln::clock_neighb2d alias and of some classical
/// 2D clock_neighborhoods.

# include <cmath>
# include <mln/core/clock_neighb.hh>
# include <mln/core/alias/dpoint2d.hh>


namespace mln
{

  /// Type alias for a clock_neighborhood defined on the 2D square
  /// grid with integer coordinates.
  ///
  typedef clock_neighb<dpoint2d> clock_neighb2d;


  /// 4-connectivity clock_neighborhood on the 2D grid.
  ///
  /// - o -
  /// o x o
  /// - o -
  ///
  /// \return A clock_neighb2d.
  const clock_neighb2d cc4(dpoint2d& dp);


  /// 8-connectivity clock_neighborhood on the 2D grid.
  ///
  /// o o o
  /// o x o
  /// o o o
  ///
  /// \return A clock_neighb2d.
  const clock_neighb2d cc8(dpoint2d& dp);


//   /// Horizontal 2-connectivity clock_neighborhood on the 2D grid.
//   ///
//   /// - - -
//   /// o x o
//   /// - - -
//   ///
//   /// \return A clock_neighb2d.
//   ///
//   const clock_neighb2d& c2_row();


//   /// Vertical 2-connectivity clock_neighborhood on the 2D grid.
//   ///
//   /// - o -
//   /// - x -
//   /// - o -
//   ///
//   /// \return A clock_neighb2d.
//   ///
//   const clock_neighb2d& c2_col();



# ifndef MLN_INCLUDE_ONLY

  inline
  const clock_neighb2d cc4(dpoint2d& dp)
  {
    static bool flower = true;
    static clock_neighb2d it;
    if (flower)
      {
	unsigned begin = 0;
	std::vector<dpoint2d> v;
	v.push_back(dpoint2d(0, 1));
	v.push_back(dpoint2d(-1, 0));
	v.push_back(dpoint2d(0, -1));
	v.push_back(dpoint2d(1, 0));

	std::vector<dpoint2d>::const_iterator ite = v.begin ();
	for (; (ite != v.end ()) && (dp != *ite); ++ite, ++begin)
	  ;
	mln_assertion (ite != v.end ());
	for (unsigned i = begin; i < v.size(); ++i)
	  it.append(v[i]);
	for (unsigned i = 0; i < begin; ++i)
	  it.append(v[i]);

	flower = false;
      }
    return it;
  }


  inline
  const clock_neighb2d cc8(dpoint2d& dp)
  {
//     static bool flower = true;
    clock_neighb2d it;
//     if (flower)
//       {
	unsigned begin = 0;
	std::vector<dpoint2d> v;
	v.push_back(dpoint2d(0, 1));
	v.push_back(dpoint2d(-1, 1));
	v.push_back(dpoint2d(-1, 0));
	v.push_back(dpoint2d(-1, -1));
	v.push_back(dpoint2d(0, -1));
	v.push_back(dpoint2d(1, -1));
	v.push_back(dpoint2d(1, 0));
	v.push_back(dpoint2d(1, 1));

	std::vector<dpoint2d>::const_iterator ite = v.begin ();
	for (; (ite != v.end ()) && (dp != *ite); ++ite, ++begin)
	  ;
	//	mln_assertion (*ite != v.end ());
	for (unsigned i = begin; i < v.size(); ++i)
	  it.append(v[i]);
	for (unsigned i = 0; i < begin; ++i)
	  it.append(v[i]);

// 	flower = false;
//       }
    return it;
  }

//   const clock_neighb2d& c8()
//   {
//     static bool flower = true;
//     static clock_neighb2d it;
//     if (flower)
//       {
// 	it.insert(dpoint2d(0, 1));
// 	it.insert(dpoint2d(1,-1));
// 	it.insert(dpoint2d(1, 0));
// 	it.insert(dpoint2d(1, 1));
// 	flower = false;
//       }
//     return it;
//   }

//   const clock_neighb2d& c2_row()
//   {
//     static bool flower = true;
//     static clock_neighb2d it;
//     if (flower)
//       {
// 	it.insert(dpoint2d(0, 1));
// 	flower = false;
//       }
//     return it;
//   }

//   const clock_neighb2d& c2_col()
//   {
//     static bool flower = true;
//     static clock_neighb2d it;
//     if (flower)
//       {
// 	it.insert(dpoint2d(1, 0));
// 	flower = false;
//       }
//     return it;
//   }

# endif // ! MLN_INCLUDE_ONLY

} // end of namespace mln


#endif // ! MLN_CORE_CLOCK_NEIGHB2D_HH
