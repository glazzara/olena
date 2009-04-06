// Copyright (C) 2008, 2009 EPITA Research and Development Laboratory (LRDE)
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

/// \file apps/statues/mesh-complex-skel.cc
/// \brief A program computing a skeleton of the surface of the
/// (triangle) mesh of a statue (by thinning), using a complex-based
/// image.

#include <cstdlib>
#include <cmath>

#include <algorithm>
#include <utility>
#include <iostream>

#include <mln/core/image/complex_image.hh>
#include <mln/core/image/complex_neighborhoods.hh>
#include <mln/core/image/image_if.hh>

#include <mln/core/site_set/p_set.hh>

#include <mln/pw/value.hh>
#include <mln/pw/cst.hh>

#include <mln/value/label_16.hh>
#include <mln/literal/white.hh>

#include <mln/core/routine/duplicate.hh>
#include <mln/labeling/regional_minima.hh>

#include <mln/io/off/load.hh>
#include <mln/io/off/save.hh>
/* FIXME: Remove as soon as mln::io::off::save is able to save a
   morphed mln::complex_image (i.e., seen through image_if).  */
#include "save_bin_alt.hh"


// FIXME: This is bad.  Remove as soon as all routines have been moved
// to the library.
using namespace mln;

// Fwd. decl.
template <typename I> struct is_simple_triangle;

// // A very simple and limited predicate for the simplicity of a point.
// template <typename I>
// class is_simple_triangle
//   : public mln::Function_p2b< is_simple_triangle<I> >
// {
// public:
//   /// Dimension of the image (and therefore of the complex).
//   static const unsigned D = I::dim;
//   /// Geometry of the image.
//   typedef mln_geom(I) G;
//   /// Psite type.
//   typedef mln::complex_psite<D, G> psite;

//   /// Result type of the functor.
//   typedef bool result;

//   is_simple_triangle()
//     : ima_(0), nbh_()
//   {
//   }

//   is_simple_triangle(const mln::Image<I>& ima)
//     : ima_(mln::exact(&ima)), nbh_()
//   {
//   }

//   /* FIXME: Rename as init() or something like this?  See how other
//      functors are written.  */
//   /// Set the underlying image.
//   void set_image(const mln::Image<I>& ima)
//   {
//     ima_ = mln::exact(&ima);
//   }

//   bool operator()(const psite& p) const
//   {
//     mln_precondition(ima_);
//     unsigned nneighbs = 0;
//     mln_niter(nbh_t) n(nbh_, p);
//     for_all(n)
//       if ((*ima_)(n))
// 	++nneighbs;
//     mln_invariant(n <= 3);
//     /* FIXME: Dummy: A triangle is considered simple if it is not
//        completely attached nor completely detached (i.e, if it has
//        more than 0 adjacent triangles but less than three).  This test
//        is obiously simplistic and wrong, but we'll use it as a first
//        approximation.  We should use the notion of free pair later.
//        And provide efficient, mask-based version as well.  */
//     return 0 < nneighbs && nneighbs < 3;
//   }

// private:
//   const I* ima_;

//   // FIXME: Make this type a parameter of the functor?
//   typedef mln::complex_lower_dim_connected_n_face_neighborhood<D, G> nbh_t;
//   nbh_t nbh_;
// };


// FIXME: Doc.
template <unsigned D, typename G>
bool
is_facet(const mln::complex_psite<D, G>& f)
{
  typedef mln::complex_higher_neighborhood<D, G> higher_adj_nbh_t;
  higher_adj_nbh_t higher_adj_nbh;
  mln_niter(higher_adj_nbh_t) n(higher_adj_nbh, f);
  for_all(n)
    // If the neighborhood is not empty, then F is included in a face
    // of higher dimension.
    return false;
  // Otherwise, F is a facet.
  return true;
}


/** \brief Compute the set of faces of the cell corresponding to the
    facet \a f.

    \pre \a f is a facet (it does not belong to any face of higher
    dimension)

    \return a set of faces containing the attachment.  */
template <unsigned D, typename G>
mln::p_set< complex_psite<D, G> >
cell(const complex_psite<D, G>& f)
{
  // Ensure the face F is a facet.
  mln_precondition(is_facet(f));

  typedef complex_psite<D, G> psite;
  typedef p_set<psite> faces_t;

  // Compute the cell F^HAT.
  faces_t f_hat;
  /* FIXME: We need a cell-iterator here
     (see https://trac.lrde.org/olena/ticket/162).  */
  typedef complex_m_face_neighborhood<D, G> m_faces_nbh_t;
  m_faces_nbh_t m_faces_nbh;
  mln_niter(m_faces_nbh_t) g(m_faces_nbh, f);
  for (unsigned m = 0; m < f.n(); ++m)
    {
      g.iter().set_m(m);
      for_all(g)
      {
	f_hat.insert(g);
// 	// FIXME: Debug.
// 	std::cerr << g << std::endl;
      }
    }
  f_hat.insert(f);
//   // FIXME: Debug.
//   std::cerr << f << std::endl;
  return f_hat;
}


/** \brief Compute the attachment of the cell corresponding to the
    facet \a f to the image \a ima.

    \pre ima is an image of Boolean values.

    \return a set of faces containing the attachment.

    We do not use the fomal definition of the attachment here (see
    couprie.08.pami).  We use the following (equivalent) definition:
    an N-face F in CELL is in the attachment of CELL to IMA if it is
    adjacent to at least an (N-1)-face or an (N+1)-face that does not
    belong to CELL.  */
template <unsigned D, typename G, typename V>
mln::p_set< complex_psite<D, G> >
attachment(const mln::complex_psite<D, G>& f,
	   const mln::complex_image<D, G, V>& ima)
{
  mlc_equal(V, bool)::check();

  typedef complex_psite<D, G> psite;
  typedef p_set<psite> faces_t;

  faces_t f_hat = cell(f);
//   // FIXME: Debug.
//   std::cerr << "f_hat.nsites() = " << f_hat.nsites() << std::endl;
  faces_t att_f;

  typedef complex_lower_higher_neighborhood<D, G> adj_nbh_t;
  adj_nbh_t adj_nbh;
  mln_piter(faces_t) g(f_hat);
  mln_niter(adj_nbh_t) n(adj_nbh, g);
  for_all(g)
    for_all(n)
    if (ima(n) && !f_hat.has(n))
      {
	att_f.insert(g);
	break;
      }
//   // FIXME: Debug.
//   std::cerr << "att_f.nsites() = " << att_f.nsites() << std::endl;
  return att_f;
}


/** \brief Compute the detachment of the cell corresponding to the
    facet \a f to the image \a ima.

    \pre ima is an image of Boolean values.

    \return a set of faces containing the detachment.

    We do not use the fomal definition of the detachment here (see
    couprie.08.pami).  We use the following (equivalent) definition:
    an N-face F in CELL is not in the detachment of CELL from IMA if
    it is adjacent to at least an (N-1)-face or an (N+1)-face that
    does not belong to CELL.  */
template <unsigned D, typename G, typename V>
mln::p_set< complex_psite<D, G> >
detachment(const mln::complex_psite<D, G>& f,
	   const mln::complex_image<D, G, V>& ima)
{
  mlc_equal(V, bool)::check();

  typedef complex_psite<D, G> psite;
  typedef p_set<psite> faces_t;

  faces_t f_hat = cell(f);
  // Initialize DETACH_F to F_HAT.
  faces_t detach_f = f_hat;

  typedef complex_lower_higher_neighborhood<D, G> adj_nbh_t;
  adj_nbh_t adj_nbh;
  mln_piter(faces_t) g(f_hat);
  mln_niter(adj_nbh_t) n(adj_nbh, g);
  for_all(g)
    for_all(n)
    if (ima(n) && !f_hat.has(n))
      {
	detach_f.remove(g);
	break;
      }
  return detach_f;
}


/** \brief A predicate for the simplicity of a point based on the
    collapse property of the attachment.

    The functor does not actually take a cell as input, but a face
    that is expected to be a 2-facet.  */
template <typename I>
class is_simple_cell
  : public mln::Function_p2b< is_simple_cell<I> >
{
public:
  /// Dimension of the image (and therefore of the complex).
  static const unsigned D = I::dim;
  /// Geometry of the image.
  typedef mln_geom(I) G;
  /// Psite type.
  typedef mln::complex_psite<D, G> psite;

  /// Result type of the functor.
  typedef bool result;

  is_simple_cell()
    : ima_(0)
  {
  }

  is_simple_cell(const mln::Image<I>& ima)
    : ima_(mln::exact(&ima))
  {
  }

  /* FIXME: Rename as init() or something like this?  See how other
     functors are written.  */
  /// Set the underlying image.
  void set_image(const mln::Image<I>& ima)
  {
    ima_ = mln::exact(&ima);
  }

  // Based on the algorithm A2 from couprie.08.pami.
  bool operator()(const psite& p) const
  {
//     // FIXME: Debug.
//     std::cerr << "is_simple_cell(" << p << ")" << std::endl;

    mln_precondition(ima_);

    typedef p_set<psite> faces_t;

    // Compute the attachment of the cell corresponding to P to he
    // domain of *IMA_.
    faces_t att = attachment(p, *ima_);

    // A cell with an empty attachment is not simple.
    /* FIXME: Why does p_set not provide an empty() predicate?  */
    if (att.nsites() == 0)
      return false;

    /* FIXME: This part could be moved to its own function/method
       (`is_collapsible').  Moreover, the code could be split: looking
       up for a free pair (g, h) could be performed in another
       routine.  */
    // Try to collapse the attachment (to a single point).
    {
      typedef complex_lower_neighborhood<D, G> lower_adj_nbh_t;
      typedef complex_higher_neighborhood<D, G> higher_adj_nbh_t;
      lower_adj_nbh_t lower_adj_nbh;
      higher_adj_nbh_t higher_adj_nbh;
      while (att.nsites() > 1)
	{

	  bool simple_pair_collapsed = false;
	  /* FIXME: The selection of G and H is probably suboptimal
	     (i.e., selecting G has a face of highest avalaible
	     dimension in CELL is probably smarter).  */
	  mln_piter(faces_t) g(att);
	  for_all(g)
	    /* G cannot have dimension 0, since we later look for an
	       adjacent face H of lower dimension.  */
	    if (static_cast<psite>(g).n() > 0)
	      {
		// Check whether G is a facet within ATT.
		bool g_is_facet = true;
		mln_niter(higher_adj_nbh_t) f(higher_adj_nbh, g);
		for_all(f)
		  if (att.has(f))
		    {
		      g_is_facet = false;
		      break;
		    }
		if (!g_is_facet)
		  break;

		// Look for a face H stricly included in G.
		bool gh_is_simple_pair = false;
		mln_niter(lower_adj_nbh_t) h(lower_adj_nbh, g);
		for_all(h)
		{
		  bool h_strictly_in_g = true;
		  if (att.has(h))
		    {
		      mln_niter(higher_adj_nbh_t) i(higher_adj_nbh, h);
		      for_all(i)
			if (i != g && att.has(i))
			  {
			    h_strictly_in_g = false;
			    break;
			  }
		    }
		  if (h_strictly_in_g)
		    {
		      gh_is_simple_pair = true;
		      att.remove(g);
		      att.remove(h);
		      mln_invariant(att.nsites() > 0);
		      break;
		    }
		} // for_all(h)

		// If a free pair (G, H) has been found and removed,
		// restart the free pair look up from the beginning.
		if (gh_is_simple_pair)
		  {
		    simple_pair_collapsed = true;
		    break;
		  }
	      } // for_all(g)

	  if (!simple_pair_collapsed)
	    // If no free pair (G, H) was found, then the attachment is
	    // not collapsible.
	    return false;

	} // while (att.nsites() > 1)

      mln_postcondition(att.nsites() == 1);
      mln_postcondition(att[0].n() == 0);
      // If the attachment is collapsible to a 0-face, then the cell
      // corresponding to the (face) P is simple.
      return true;
    }
  }

private:
  const I* ima_;
};


// FIXME: Doc.
template <unsigned D, typename G, typename V>
void
detach (const mln::complex_psite<D, G>f, mln::complex_image<D, G, V>& ima)
{
  mlc_equal(V, bool)::check();

  typedef mln::complex_psite<D, G> psite;
  typedef p_set<psite> faces_t;

  // Compute the detachment of P from IMA.
  faces_t detach = detachment(f, ima);
  // Detach all its faces from IMA.
#if 0
  // FIXME: Does not work yet?  Check again.
  data::fill(ima | detach, false);
#endif
  mln_piter(faces_t) p(detach);
  for_all(p)
    ima(p) = false;
}


// FIXME: Move this elsewhere.
// FIXME: Use a generic `I' instead of `mln::bin_2complex_image3df'.
// FIXME: Add a constraint/priority argument (K/P).
/** \brief Breadth-First Thinning.

    A non generic implementation of a binary skeleton on a triangle
    surface (mesh).

    \a N is the adjacency relation between triangles, but it probably
    does not make sense to use something other than an instance of
    mln::complex_lower_dim_connected_n_face_neighborhood if the image type is
    hard-coded as mln::bin_2complex_image3df.  */
template <typename N, typename F>
mln::bin_2complex_image3df
skeleton(const mln::bin_2complex_image3df& input,
	 const mln::Neighborhood<N>& nbh_,
	 mln::Function_p2b<F>& is_simple_)
{
  const N& nbh = exact(nbh_);
  F& is_simple = exact(is_simple_);

  typedef mln::bin_2complex_image3df I;
  typedef mln_psite(I) psite;

  I output = mln::duplicate(input);
  // Attach the work image to IS_SIMPLE.
  is_simple.set_image(output);

  typedef mln::p_set<psite> set_t;
  set_t set;

//   // FIXME: Debug.
//   unsigned count = 0;

  // Populate the set with candiate simple points.
  mln_piter(I) p(output.domain());
  for_all(p)
  {
    /* FIXME: Cheat!  We'd like to iterate on 2-cells only, but
       we cannot constrain the domain of the input using
       image_if (yet).  Hence this filter to restrict the
       dimension of the considered cells.  A less dirty
       approaach would be to use a constraint predicate (see
       comment above).  */
    if (p.unproxy_().n() != I::dim)
      continue;

//     // FIXME: Debug.
//     std::cerr << "skeleton: Iteration (for_all(p)) : "
// 	      << count++ << std::endl;

//     // FIXME: Debug.
//     std::cerr << "  output(p) = " << output(p) << std::endl;
//     std::cerr << "  is_simple(p) = " << is_simple(p) << std::endl;

    if (output(p) && is_simple(p))
      set.insert(p);
  }

//   // FIXME: Debug.
//   std::cerr << "skeleton: set.nsites() = " << set.nsites() << std::endl;

//   // FIXME: Debug.
//   count = 0;

  while (!set.is_empty())
    {
//       // FIXME: Debug.
//       mln_piter(set_t) ps(set);
//       std::cerr << "set = " << std::endl;
//       for_all(ps)
// 	std::cerr << "  " << static_cast<psite>(ps) << std::endl;
// 	std::cerr << std::endl;

//       // FIXME: Debug.
//       std::cerr << "skeleton: Iteration (while (!set.is_empty())) : "
// 		<< count++ << std::endl;

      set_t next_set;
      // FIXME: Use a piter on SET instead of this hand-made iteration.
      for (unsigned i = 0; i < set.nsites(); ++i)
	{
	  psite p = set[i];

	  /* FIXME: Cheat!  We'd like to iterate on 2-cells only, but
	     we cannot constrain the domain of the input using
	     image_if (yet).  Hence this filter to restrict the
	     dimension of the considered cells.  A less dirty
	     approaach would be to use a constraint predicate (see
	     comment above).  */
	  if (p.n() != I::dim)
	    continue;

	  /* FIXME: We compute the cell and attachment of P twice:
	     within is_simple and within detach.  How could we reuse
	     this elegantly, without breaking the genericity of the
	     skeleton algorithm?  */
	  if (is_simple(p))
	    {
	      // FIXME: `detach' could be a functor, as is `is_simple'.
	      detach(p, output);
	      mln_niter(N) n(nbh, p);
	      for_all(n)
	      {
		/* FIXME: Cheat!  We'd like to iterate on 2-cells only, but
		   we cannot constrain the domain of the input using
		   image_if (yet).  Hence this filter to restrict the
		   dimension of the considered cells.  A less dirty
		   approaach would be to use a constraint predicate (see
		   comment above).  */
		if (n.unproxy_().n() != I::dim)
		  continue;

		if (output.domain().has(n) && output(n) && is_simple(n))
		  next_set.insert(n);
	      }
	    }
	}
      set.clear();
      std::swap(set, next_set);
    }

  return output;
}


// Fwd. decl.
template <unsigned N> struct is_n_face;

/// A functor testing wheter a mln::complex_psite is an \N-face.
template <unsigned N>
struct is_n_face : public mln::Function_p2b< is_n_face<N> >
{
  typedef bool result;

  template <unsigned D, typename G>
  bool operator()(const mln::complex_psite<D, G>& p)
  {
    return p.n() == N;
  }
};


int main(int argc, char* argv[])
{
  if (argc != 4)
    {
      std::cerr << "usage: " << argv[0] << " input.off lambda output.off"
		<< std::endl;
      std::exit(1);
    }

  std::string input_filename = argv[1];
  // FIXME: Use lambda to filter the input.
#if 0
  unsigned lambda = atoi(argv[2]);
#endif
  std::string output_filename = argv[3];

  /*----------------.
  | Complex image.  |
  `----------------*/

  // Image type.
  typedef mln::float_2complex_image3df ima_t;
  // Dimension of the image (and therefore of the complex).
  static const unsigned D = ima_t::dim;
  // Geometry of the image.
  typedef mln_geom_(ima_t) G;

  ima_t input;
  mln::io::off::load(input, input_filename);

  /* FIXME: Workaround: Set maximal values on vertices and edges to
     exclude them from the set of minimal values.  */
  mln::p_n_faces_fwd_piter<D, G> v(input.domain(), 0);
  for_all(v)
    input(v) = mln_max(float);
  mln::p_n_faces_fwd_piter<D, G> e(input.domain(), 1);
  for_all(e)
    input(e) = mln_max(float);

  /*---------------.
  | Local minima.  |
  `---------------*/

  typedef mln::value::label_16 label_t;
  label_t nminima;

  typedef mln::complex_lower_dim_connected_n_face_neighborhood<D, G> nbh_t;
  nbh_t nbh;

  /* FIXME: We should use something like `ima_t | p_n_faces(2)' instead
     of `ima_t' here.  Or better: `input' should only associate data
     to 2-faces.  */
  mln_ch_value_(ima_t, label_t) minima =
    mln::labeling::regional_minima(input, nbh, nminima);

  /*-----------------------.
  | Initial binary image.  |
  `-----------------------*/

  typedef mln_ch_value_(ima_t, bool) bin_ima_t;
  bin_ima_t surface(input.domain());
  mln::data::fill(surface, true);
  // Dig ``holes'' in the surface surface by setting minima faces to false.
#if 0
  /* FIXME: The `is_n_face<2>' predicate is required because the
     domain of SURFACE also comprises 0-faces and 1-faces...  */
  mln::data::fill(surface
		  | is_n_face<2>()
		  | mln::pw::value(minima) != mln::pw::cst(mln::literal::zero),
		  false);
#endif
  /* FIXME: The code above does not compile (yet).  Probably because
     of an incompatibility between complex_image and image_if.  Use
     this hand-made filling for the moment.  */
  mln::p_n_faces_fwd_piter<D, G> t(input.domain(), 2);
  for_all(t)
    if (minima(t) != mln::literal::zero)
      surface(t) = false;

// #if 0
//   /* FIXME: Debug.  But alas, this does not work (yet).
//      Use workaround mln::io::off::save_bin_salt instead (bad!)  */
// # if 0
//   mln::io::off::save(surface | mln::pw::value(surface) == mln::pw::cst(true),
// 		     "surface.off");
// # endif
//   mln::io::off::save_bin_alt(surface, "surface.off");
// #endif

//   // FIXME: Debug.
//   typedef mln_ch_value_(ima_t, bool) bin_ima_t;
//   bin_ima_t surface;
//   mln::io::off::load(surface,
// 		     "/Users/roland/src/olena/milena/mesh/two-triangles.off");
//   // Set one of the two triangle to false.
//   p_n_faces_fwd_piter<D, G> t(surface.domain(), 2);
//   t.start();
//   mln_precondition(t.is_valid());
//   surface(t) = false;

  /*-----------.
  | Skeleton.  |
  `-----------*/

  is_simple_cell<bin_ima_t> is_simple_p;
  bin_ima_t skel = skeleton(surface, nbh, is_simple_p);

  /*---------.
  | Output.  |
  `---------*/

  /* FIXME: This does not work (yet).
     Use workaround mln::io::off::save_bin_salt instead (bad!)  */
#if 0
  mln::io::off::save(skel | mln::pw::value(skel) == mln::pw::cst(true),
		     output_filename);
#endif
  mln::io::off::save_bin_alt(skel, output_filename);
}
