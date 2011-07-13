// Copyright (C) 2008, 2009, 2011 EPITA Research and Development
// Laboratory (LRDE)
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

/// \brief Elements to compute 3D connectivity numbers.

#ifndef TOOLS_CONNECTIVITY_NUMBERS_3D_HH
# define TOOLS_CONNECTIVITY_NUMBERS_3D_HH

# include <cstdlib>

# include <string>
# include <vector>

# include <iostream>
# include <iomanip>

# include <mln/core/image/dmorph/image_if.hh>
# include <mln/core/image/image3d.hh>
# include <mln/core/alias/neighb3d.hh>
# include <mln/value/int_u8.hh>
# include <mln/value/int_u32.hh>

# include <mln/labeling/blobs.hh>
# include <mln/data/fill.hh>
# include <mln/debug/println.hh>


/* Connectivity numbers in 3D.

   N_26^*(A) =
    0  1  2     9 10 11    18 19 20
    3  4  5    12  * 14    21 22 23
    6  7  8    15 16 17    24 25 26

   N_18^*(A) =
   __  1 __     9 10 11    __ 19 __
    3  4  5    12  * 14    21 22 23
   __  7 __    15 16 17    __ 25 __

   N_6^*(A) =
   __ __ __    __ 10 __    __ __ __
   __  4 __    12  * 14    __ 22 __
   __ __ __    __ 16 __    __ __ __


   Note: The 6+-connectivity is the 6-connectivity associated to the
   18-connectivity, while the 6-connectivity (with no `+') is the
   6-connectivity associated to the 26-connectivity
   (bertrand.07.chap).


   Geodesic neighborhoods
     Careful, when N_26^*(A) is not included in X, this result is
     probably wrong (because all paths may not be valid).  In
     particular, this means that connectivity numbers computed on the
     borders of a 3D image (where a point has less than 26 neighbors)
     are wrong!  How can we deal with that?

   G_6(A,X)
     = N_6^2(A,X)
     = union(N_6(B) inter N_26^*(A) inter X s.t. B in N_6^1(A,X))
     = union(N_6(B) inter N_26^*(A) inter X s.t. B in N_6^*(A) inter X)
     = union(N_6(B) inter N_26^*(A) inter X s.t. B in {4, 10, 12, 14, 16, 22})
     = union( N_6(4) inter N_26^*(A) inter X,
             N_6(10) inter N_26^*(A) inter X,
             N_6(12) inter N_26^*(A) inter X,
             N_6(14) inter N_26^*(A) inter X,
             N_6(16) inter N_26^*(A) inter X,
             N_6(22) inter N_26^*(A) inter X)
     = union( {1, 3, 4, 5, 7}        inter X,
              {1, 9, 10 11, 19}      inter X,
              {3, 9, 12, 15, 21}     inter X,
              {5, 11, 14, 17, 23     inter X,
              {7, 15, 16, 17, 25}    inter X,
              {19, 21, 22, 23, 25}   inter X)
     = {1, 3, 4, 5, 7, 9, 10, 11, 12, 14, 15, 16, 17, 19, 21, 22, 23, 25}
       inter X
     = N_18^*(A) inter X

  Bottom line:
    G_6(A,X)  = N_18^*(A) inter X when N_26^*(A) in included in X
    G_6+(A,X) = N_26^*(A) inter X when N_26^*(A) in included in X
    G_18(A,X) = N_26^*(A) inter X when N_26^*(A) in included in X
    G_26(A,X) = N_26^*(A) inter X when N_26^*(A) in included in X
*/


/** Type of a connectivity number.

    Note: The maximun number of connected components in the
    neighborhood of a point is necessarily lower or equal to half the
    number of neighbors.  This number fits on an mln::value::int_u8
    when the dimension is lower or equal to 5.  */
typedef mln::value::int_u8 conn_number_t;
/// Type of a vector of connectivity numbers.
typedef std::vector<conn_number_t> conn_numbers_t;

/// Type of a 3D (neighborhood) configuration number (index).
typedef mln::value::int_u32 config_3d_t;

/* A function stating whether a point of the box (-1,-1-1)-(+1,+1,+1)
   is within the 18-c neighborhood of (0, 0, 0). */
bool
f_within_c18(const mln::point3d& p)
{
  return !(mln::math::abs(p.sli()) == 1
	   && mln::math::abs(p.row()) == 1
	   && mln::math::abs(p.col()) == 1);
}

// Associated functor.
struct within_c18 : mln::Function_v2b<within_c18>
{
  typedef bool result;
  bool operator()(const mln::point3d& p) const { return f_within_c18(p); }
};

/* FIXME: Concurrent accesses to the neighborhood (reference) returned
   by routines such as mln::c26() are not thread-safe.

   Short explanation

     This neighborhood is not thread-safe, because
     mln::util::set<T>::freeze_() is not thread-safe.

   Long explanation

     The neighborhood is built on a window object containing an
     mln::util::set of delta-points.  However, this set is implemented
     as a kind of variant containing an array and a set, with actual
     data in one of these containers or the other at any time.
     mln::util:set changes its internal representation when the nature
     of the current operation (read or write) changes with respect to
     the previously performed operation.  Alas, with concurrent
     accesses, this does not work as expected.

     The issue here is that mln::c26() (an similar routines) creates a
     neighborhood containing an mln::util::set where only write
     operations are performed at the beginning.  The next read
     operation (occurring when the neighborhood is browsed) may be
     done currently, and mln::util::set<T>::freeze_() may then be
     called concurrently.  As it has not been designed to support
     concurrent calls, it generally causes run-time errors (double
     deallocations, deallocation before allocation, etc.).

   Workaround

     A simple workaround is to prevent concurrent accesses, by copying
     the neighborhood returned by mln::c26() and other routines,
     instead of using it directly (i.e., as a shared reference).

     Another option would be to force-freeze the mln::util::set
     contained in the neighborhood `nbh', e.g. by calling
     `nbh.win().std_vector()'.  (We have to peform an indirect
     invocation, as mln::util::set<T>::freeze_() is a private method
     and cannot be called directly.)

  To do

     We must improve mln::util::set<T>::freeze_() and make it thread
     safe.  And maybe get rid of mln::c26() and others, or change
     their implementations.  */

/*----------------------------------------------------.
| Sequential computation of 3D connectivity numbers.  |
`----------------------------------------------------*/

/// Canevas to compute 3D connectivity numbers (in sequence)
///
/// \param f  A function computing the connectivity number for each
///           configuration (as a 3x3x3 3D image).
template <typename F>
conn_numbers_t
connectivity_numbers_3d(F f)
{
  using namespace mln;

  typedef image3d<bool> I;
  // B must be a model of mln::Box.
  typedef mln_domain(I) B;
  typedef mln_psite(I) P;

  B b = make::box3d(-1,-1,-1, 1,1,1);
  I ima(b, 0);
  P p(0, 0, 0);

  const unsigned dim = 3;
  const unsigned max_nneighbs = mlc_pow_int(3, dim) - 1;
  const unsigned nconfigs = mlc_pow_int(2, max_nneighbs);

  conn_numbers_t numbers(nconfigs, 0);

  typedef neighb3d N;
  N nbh = c26();

  for (config_3d_t i = 0; i < nconfigs; ++i)
    {
      /* Create the local i-th configuration around P.

	 Note that the value corresponding to P is always `false', to
	 prevent the connection of two components through P.  */
      data::fill(ima, false);
      config_3d_t tmp = i;
      mln_fwd_niter_(N) n(nbh, p);
      for_all(n)
      {
	if (tmp % 2)
	  ima(n) = true;
	tmp = tmp >> 1;
      }
      numbers[i] = f(ima);
    }
  return numbers;
}


// FIXME: Try to factor common parts among the 4 cases of
// 3D configurations below.

/*-------------------------------------------------------------.
| (6, 26) configurations: 6-connected foreground, 26-connected |
| background.                                                  |
`-------------------------------------------------------------*/

// FIXME: Factor these using mln/topo/connectivity_number_3d.hh

conn_number_t
connectivity_number_3d__6_26_one(const mln::image3d<bool>& ima)
{
  using namespace mln;
  typedef image3d<bool> I;
  typedef neighb3d N;
  typedef mln_psite_(I) P;
  P p(0, 0, 0);

  // Create a copy of mln::c6()'s own neighborhood to avoid
  // thread-unsafe accesses to this neighborhood (see the long
  // explanation above).
  mln::neighb3d nbh = c6();
  conn_number_t unused_nl;
  // Restrict the image to the 18-c neighborhood of P.
  image_if<image3d<conn_number_t>, within_c18> lab =
    labeling::blobs(ima | within_c18(), nbh, unused_nl);
  std::set<conn_number_t> s;
  mln_niter_(N) n(nbh, p);
  for_all(n)
    if (lab(n) != 0)
      s.insert(lab(n));
  return s.size();
}

conn_numbers_t
connectivity_numbers_3d__6_26()
{
  return connectivity_numbers_3d(connectivity_number_3d__6_26_one);
}


/*-------------------------------------------------------------.
| (26, 6) configurations: 26-connected foreground, 6-connected |
| background.                                                  |
`-------------------------------------------------------------*/

conn_number_t
connectivity_number_3d__26_6_one(const mln::image3d<bool>& ima)
{
  using namespace mln;

  // Create a copy of mln::c26()'s own neighborhood to avoid
  // thread-unsafe accesses to this neighborhood (see the long
  // explanation above).
  mln::neighb3d nbh = c26();
  conn_number_t n;
  labeling::blobs(ima, nbh, n);
  return n;
}

conn_numbers_t
connectivity_numbers_3d__26_6()
{
  return connectivity_numbers_3d(connectivity_number_3d__26_6_one);
}


/*---------------------------------------------------------------.
| (6+, 18) configurations: 6+-connected foreground, 18-connected |
| background.                                                    |
`---------------------------------------------------------------*/

conn_number_t
connectivity_number_3d__6p_18_one(const mln::image3d<bool>& ima)
{
  using namespace mln;
  typedef image3d<bool> I;
  typedef neighb3d N;
  typedef mln_psite_(I) P;
  P p(0, 0, 0);

  // Create a copy of mln::c6()'s own neighborhood to avoid
  // thread-unsafe accesses to this neighborhood (see the long
  // explanation above).
  mln::neighb3d nbh = c6();
  conn_number_t unused_nl;
  image3d<conn_number_t> lab = labeling::blobs(ima, nbh, unused_nl);
  std::set<conn_number_t> s;
  mln_niter_(N) n(nbh, p);
  for_all(n)
    if (lab(n) != 0)
      s.insert(lab(n));
  return s.size();
}

conn_numbers_t
connectivity_numbers_3d__6p_18()
{
  return connectivity_numbers_3d(connectivity_number_3d__6p_18_one);
}


/*---------------------------------------------------------------.
| (18, 6+) configurations: 18-connected foreground, 6+-connected |
| background.                                                    |
`---------------------------------------------------------------*/

conn_number_t
connectivity_number_3d__18_6p_one(const mln::image3d<bool>& ima)
{
  using namespace mln;
  typedef image3d<bool> I;
  typedef neighb3d N;
  typedef mln_psite_(I) P;
  P p(0, 0, 0);

  // Create a copy of mln::c18()'s own neighborhood to avoid
  // thread-unsafe accesses to this neighborhood (see the long
  // explanation above).
  mln::neighb3d nbh = c18();
  conn_number_t unused_nl;
  image3d<conn_number_t> lab = labeling::blobs(ima, nbh, unused_nl);
  std::set<conn_number_t> s;
  mln_niter_(N) n(nbh, p);
  for_all(n)
    if (lab(n) != 0)
      s.insert(lab(n));
  return s.size();
}

conn_numbers_t
connectivity_numbers_3d__18_6p()
{
  return connectivity_numbers_3d(connectivity_number_3d__18_6p_one);
}


/*----------------------.
| Helpers for drivers.  |
`----------------------*/

void
usage(const std::string& program)
{
  std::cerr <<
    "usage: " << program << " <nbhs>" << std::endl <<
    "where <nbhs> is one of these values:\n\n"
    "  `6_26'  :  6-c foreground,  26-c background\n"
    "  `26_6'  : 26-c foreground,  6-c background\n"
    "  `6p_18' : 6+-c foreground, 18-c background\n"
    "  `18_6p' : 18-c foreground, 6+-c background\n" << std::endl;
  std::exit(1);
}

void
display_connectivity_numbers(const conn_numbers_t& conn_numbers)
{
  for (size_t i = 0; i < conn_numbers.size(); ++i)
    {
      std::cout << std::setw(2) << conn_numbers[i] << ", ";
      if (! ((i + 1) % 4)) std::cout << " ";
      if (! ((i + 1) % 16)) std::cout << std::endl;
      if (! ((i + 1) % 64)) std::cout <<  std::endl;
    }
}

#endif // ! TOOLS_CONNECTIVITY_NUMBERS_3D_HH
