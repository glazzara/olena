// Copyright (C) 2007, 2008, 2009 EPITA Research and Development
// Laboratory (LRDE)
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

#ifndef MLN_CORE_MACROS_HH
# define MLN_CORE_MACROS_HH

/// \file mln/core/macros.hh
///
/// Definition of the set of Milena macros.


# define mln_deduce(T, A1, A2) typename T::A1::A2


// a

/// Shortcuts to access the argument type associated to T.
/// \{
# define mln_argument(T)      typename T::argument
# define mln_argument_(T)     T::argument
/// \}


// b

/// Shortcuts to access the element-backward-iterator type associated
/// to T.
/// \{
# define mln_bkd_eiter(T)   typename T::bkd_eiter
# define mln_bkd_eiter_(T)           T::bkd_eiter
/// \}

/// Shortcuts to access the bkd_niter type associated to T.
/// \{
# define mln_bkd_niter(T)  typename T::bkd_niter
# define mln_bkd_niter_(T)          T::bkd_niter
/// \}

/// Shortcuts to access the bkd_piter type associated to T.
/// \{
# define mln_bkd_piter(T)  typename T::bkd_piter
# define mln_bkd_piter_(T)          T::bkd_piter
/// \}

/// Shortcuts to access the bkd_qiter type associated to T.
/// \{
# define mln_bkd_qiter(T)  typename T::bkd_qiter
# define mln_bkd_qiter_(T)          T::bkd_qiter
/// \}

/// Shortcuts to access the bkd_viter type associated to T.
/// \{
# define mln_bkd_viter(T)  typename T::bkd_viter
# define mln_bkd_viter_(T)          T::bkd_viter
/// \}

/// Shortcuts to access the box type associated to T.
/// \{
# define mln_box(T)        typename mln::box<mln_psite(T)>
# define mln_box_(T)                mln::box<mln_psite(T)>
/// \}


// c

/// Shortcuts to access the coord type associated to T.
/// \{
# define mln_coord(T)      typename T::coord
# define mln_coord_(T)              T::coord
/// \}


// d

/// Shortcuts to access the delta type associated to T.
/// \{
# define mln_delta(T)      typename T::delta
# define mln_delta_(T)              T::delta
/// \}

/// Shortcuts to access the dpoint type associated to T.
/// \{
# define mln_dpoint(T)     typename T::dpoint
# define mln_dpoint_(T)             T::dpoint
/// \}

/// Shortcuts to access the dpsite type associated to T.
/// \{
# define mln_dpsite(T)     typename T::dpsite
# define mln_dpsite_(T)             T::dpsite
/// \}


// e

/// Shortcuts to access the element-iterator type associated to T.
/// \{
# define mln_eiter(T)      typename T::eiter
# define mln_eiter_(T)              T::eiter
/// \}

/// Shortcuts to access the graph type associated to T.
/// \{
# define mln_graph(T)         typename T::graph_t
# define mln_graph_(T)                 T::graph_t
/// \}

/// Shortcuts to access the element type associated to T.
/// \{
# define mln_element(T)        typename T::element
# define mln_element_(T)                T::element
/// \}

/// Shortcuts to access the graph element type associated to T.
/// \{
# define mln_graph_element(T)        typename T::graph_element
# define mln_graph_element_(T)                T::graph_element
/// \}

/// Shortcuts to access the encoding type associated to T.
/// \{
# define mln_enc(T)        typename T::enc
# define mln_enc_(T)                T::enc
/// \}

/// Shortcuts to access the equivalent type associated to T.
/// \{
# define mln_equiv(T)      typename T::equiv
# define mln_equiv_(T)              T::equiv
/// \}


// f

/// Shortcuts to access the element-forward-iterator type associated
/// to T.
/// \{
# define mln_fwd_eiter(T)   typename T::fwd_eiter
# define mln_fwd_eiter_(T)           T::fwd_eiter
/// \}

/// Shortcuts to access the fwd_niter type associated to T.
/// \{
# define mln_fwd_niter(T)  typename T::fwd_niter
# define mln_fwd_niter_(T)          T::fwd_niter
/// \}

/// Shortcuts to access the fwd_piter type associated to T.
/// \{
# define mln_fwd_piter(T)  typename T::fwd_piter
# define mln_fwd_piter_(T)          T::fwd_piter
/// \}

/// Shortcuts to access the fwd_qiter type associated to T.
/// \{
# define mln_fwd_qiter(T)  typename T::fwd_qiter
# define mln_fwd_qiter_(T)          T::fwd_qiter
/// \}

/// Shortcuts to access the fwd_viter type associated to T.
/// \{
# define mln_fwd_viter(T)  typename T::fwd_viter
# define mln_fwd_viter_(T)          T::fwd_viter
/// \}


// g

/// Shortcuts to access the geometry type associated to T.
/// \{
# define mln_geom(T)      typename T::geom
# define mln_geom_(T)              T::geom
/// \}

/// Shortcuts to access the grid type associated to T.
/// \{
# define mln_grid(T)      typename T::grid
# define mln_grid_(T)              T::grid
/// \}


// i

/// Shortcuts to access the insertion-element type associated to T.
/// \{
# define mln_i_element(T)  typename T::i_element
# define mln_i_element_(T)          T::i_element
/// \}

/// Shortcuts to access the image type associated to T.
/// \{
# define mln_image(T)      typename T::image
# define mln_image_(T)              T::image
/// \}

/// Shortcuts to access the invert type associated to T.
/// \{
# define mln_invert(T)	   typename T::invert
# define mln_invert_(T)             T::invert
/// \}


// l

/// Shortcuts to access the lvalue type associated to T.
/// \{
# define mln_lvalue(T)     typename T::lvalue
# define mln_lvalue_(T)             T::lvalue
/// \}


// m

/// Shortcuts to access the mesh type associated to T.
/// \{
# define mln_mesh(T)     typename T::mesh
# define mln_mesh_(T)             T::mesh
/// \}


// p

/// Shortcuts to access the type of point iterator (piter) associated to T.
/// \{
# define mln_piter(T)      typename T::piter
# define mln_piter_(T)              T::piter
/// \}

/// Shortcuts to access the type of point by line iterator
/// (line_piter) associated to T.
/// \{
# define mln_line_piter(T) typename T::line_piter
# define mln_line_piter_(T)         T::line_piter
/// \}

/// Shortcuts to access the domain type associated to T.
/// \{
# define mln_domain(T)       typename T::domain_t
# define mln_domain_(T)               T::domain_t
/// \}

/// Shortcuts to access the type of point set (pset) associated to T.
/// \{
# define mln_pset(T)       typename T::pset
# define mln_pset_(T)               T::pset
/// \}

/// Shortcuts to access the type of point site (psite) associated to T.
/// \{
# define mln_psite(T)      typename T::psite
# define mln_psite_(T)              T::psite
/// \}

/// Shortcuts to access the point type associated to T.
/// \{
# define mln_point(T)      typename T::point
# define mln_point_(T)              T::point
/// \}


// q

/// Shortcuts to access the qualified (const or mutable) value type
/// associated to T.
/// \{
# define mln_qlf_value(T)      typename mln::trait::qlf_value< T >::ret
# define mln_qlf_value_(T)              mln::trait::qlf_value< T >::ret
/// \}

/// Shortcuts to access the qiter type associated to T.
/// \{
# define mln_qiter(T)      typename T::fwd_qiter
# define mln_qiter_(T)              T::fwd_qiter
/// \}

/// Shortcuts to access the qualified-result type associated to T.
/// \{
# define mln_q_result(T)     typename T::q_result
# define mln_q_result_(T)    T::q_result
/// \}

/// Shortcuts to access the qualified-subject type associated to T.
/// \{
# define mln_q_subject(T)      typename T::q_subject
# define mln_q_subject_(T)              T::q_subject
/// \}


// n

/// Shortcuts to access the niter type associated to T.
/// \{
# define mln_niter(T)      typename T::fwd_niter
# define mln_niter_(T)              T::fwd_niter
/// \}


// r

/// Shortcuts to access the removal-element type associated to T.
/// \{
# define mln_r_element(T)  typename T::r_element
# define mln_r_element_(T)          T::r_element
/// \}

/// Shortcuts to access the regular type associated to T.
/// \{
# define mln_regular(T)     typename T::regular
# define mln_regular_(T)    T::regular
/// \}

/// Shortcuts to access the result type associated to T.
/// \{
# define mln_result(T)     typename T::result
# define mln_result_(T)    T::result
/// \}

/// Shortcuts to access the rvalue type associated to T.
/// \{
# define mln_rvalue(T)     typename T::rvalue
# define mln_rvalue_(T)             T::rvalue
/// \}


// s

/// Shortcuts to access the site type associated to T.
/// \{
# define mln_site(T)      typename T::site
# define mln_site_(T)              T::site
/// \}

// /// Shortcuts to access the subject type associated to T.
// /// \{
// # define mln_subject(T)      typename T::subject
// # define mln_subject_(T)              T::subject
// /// \}


// t

/// Shortcuts to access the target type associated to T.
/// \{
# define mln_target(T)      typename T::target
# define mln_target_(T)              T::target
/// \}



// u

/// Shortcuts to access the unmorph type associated to T.
/// \{
# define mln_unmorph(T)      typename T::unmorph
# define mln_unmorph_(T)              T::unmorph
/// \}



// v

/// Shortcuts to access the value type associated to T.
/// \{
# define mln_value(T)      typename T::value
# define mln_value_(T)              T::value
/// \}

/// Shortcuts to access the algebra::vector (vec) type associated to T.
/// \{
# define mln_vec(T)        typename T::vec
# define mln_vec_(T)                T::vec
/// \}

/// Shortcuts to access the type of value set (vset) associated to T.
/// \{
# define mln_vset(T)       typename T::t_eligible_values_set
# define mln_vset_(T)               T::t_eligible_values_set
/// \}

/// Shortcuts to access the viter type associated to T.
/// \{
# define mln_viter(T)      typename T::fwd_viter
# define mln_viter_(T)              T::fwd_viter
/// \}


// w

/// Shortcuts to access the weight type associated to T.
/// \{
# define mln_weight(T)     typename T::weight
# define mln_weight_(T)             T::weight
/// \}

/// Shortcuts to access the window type associated to T.
/// \{
# define mln_window(T)     typename T::window
# define mln_window_(T)             T::window
/// \}




// FIXME: Just include trait/all.hh !!!

# define mln_fwd_pixter(I)  typename mln::trait::fwd_pixter< I >::ret
# define mln_fwd_pixter_(I)          mln::trait::fwd_pixter< I >::ret

# define mln_bkd_pixter(I) typename mln::trait::bkd_pixter< I >::ret
# define mln_bkd_pixter_(I)         mln::trait::bkd_pixter< I >::ret

# define mln_pixter(I)  mln_fwd_pixter(I)
# define mln_pixter_(I) mln_fwd_pixter_(I)


# define mln_fwd_qixter(I, W) typename mln::trait::fwd_qixter< I, W >::ret
# define mln_fwd_qixter_(I, W)         mln::trait::fwd_qixter< I, W >::ret

# define mln_bkd_qixter(I, W) typename mln::trait::bkd_qixter< I, W >::ret
# define mln_bkd_qixter_(I, W)         mln::trait::bkd_qixter< I, W >::ret

# define mln_qixter(I, W)  mln_fwd_qixter(I, W)
# define mln_qixter_(I, W) mln_fwd_qixter_(I, W)


# define mln_fwd_nixter(I, N) typename mln::trait::fwd_nixter< I, N >::ret
# define mln_fwd_nixter_(I, N)         mln::trait::fwd_nixter< I, N >::ret

# define mln_bkd_nixter(I, N) typename mln::trait::bkd_nixter< I, N >::ret
# define mln_bkd_nixter_(I, N)         mln::trait::bkd_nixter< I, N >::ret

# define mln_nixter(I, N)  mln_fwd_nixter(I, N)
# define mln_nixter_(I, N) mln_fwd_nixter_(I, N)

/// Shortcuts to access the vertex iterator type associated to a graph G.
/// \{
# define mln_vertex_iter(G)	    typename G::vertex_iter
# define mln_vertex_iter_(G)		     G::vertex_iter
# define mln_vertex_fwd_iter(G)     typename G::vertex_fwd_iter
# define mln_vertex_fwd_iter_(G)             G::vertex_fwd_iter
# define mln_vertex_bkd_iter(G)     typename G::vertex_bkd_iter
# define mln_vertex_bkd_iter_(G)             G::vertex_bkd_iter
/// \}

/// Shortcuts to access the edge iterator type associated to a graph G.
/// \{
# define mln_edge_iter(G)	  typename G::edge_iter
# define mln_edge_iter_(G)		   G::edge_iter
# define mln_edge_fwd_iter(G)     typename G::edge_fwd_iter
# define mln_edge_fwd_iter_(G)             G::edge_fwd_iter
# define mln_edge_bkd_iter(G)     typename G::edge_bkd_iter
# define mln_edge_bkd_iter_(G)             G::edge_bkd_iter
/// \}

/// Shortcuts to access the vertex centered edge neighbors iterator type
/// associated to a graph G.
/// \{
# define mln_vertex_nbh_vertex_iter(G)	       typename G::vertex_nbh_vertex_iter
# define mln_vertex_nbh_vertex_iter_(G)			G::vertex_nbh_vertex_iter
# define mln_vertex_nbh_vertex_fwd_iter(G)     typename G::vertex_nbh_vertex_fwd_iter
# define mln_vertex_nbh_vertex_fwd_iter_(G)             G::vertex_nbh_vertex_fwd_iter
# define mln_vertex_nbh_vertex_bkd_iter(G)     typename G::vertex_nbh_vertex_bkd_iter
# define mln_vertex_nbh_vertex_bkd_iter_(G)             G::vertex_nbh_vertex_bkd_iter
/// \}

/// Shortcuts to access the vertex centered edge neighbors iterator type
/// associated to a graph G.
/// \{
# define mln_vertex_nbh_edge_iter(G)	     typename G::vertex_nbh_edge_iter
# define mln_vertex_nbh_edge_iter_(G)	              G::vertex_nbh_edge_iter
# define mln_vertex_nbh_edge_fwd_iter(G)     typename G::vertex_nbh_edge_fwd_iter
# define mln_vertex_nbh_edge_fwd_iter_(G)             G::vertex_nbh_edge_fwd_iter
# define mln_vertex_nbh_edge_bkd_iter(G)     typename G::vertex_nbh_edge_bkd_iter
# define mln_vertex_nbh_edge_bkd_iter_(G)             G::vertex_nbh_edge_bkd_iter
/// \}

/// Shortcuts to access the edge centered edge neighbors iterator type
/// associated to a graph G.
/// \{
# define mln_edge_nbh_edge_iter(G)	 typename G::edge_nbh_edge_iter
# define mln_edge_nbh_edge_iter_(G)		  G::edge_nbh_edge_iter
# define mln_edge_nbh_edge_fwd_iter(G)   typename G::edge_nbh_edge_fwd_iter
# define mln_edge_nbh_edge_fwd_iter_(G)           G::edge_nbh_edge_fwd_iter
# define mln_edge_nbh_edge_bkd_iter(G)   typename G::edge_nbh_edge_bkd_iter
# define mln_edge_nbh_edge_bkd_iter_(G)           G::edge_nbh_edge_bkd_iter
/// \}

#endif // ! MLN_CORE_MACROS_HH

