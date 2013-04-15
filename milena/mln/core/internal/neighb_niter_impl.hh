// Copyright (C) 2008, 2009, 2012 EPITA Research and Development
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

#ifndef MLN_CORE_INTERNAL_NEIGHB_NITER_IMPL_HH
# define MLN_CORE_INTERNAL_NEIGHB_NITER_IMPL_HH

/// \file
///
/// Optional implementation for neigbh_niter.

# include <mln/core/macros.hh>
# include <mln/core/internal/force_exact.hh>


namespace mln
{

  // Forward declaration.
  template <typename P, typename W> class graph_window_base;
  template <typename G, typename S> class graph_elt_window;
  template <typename G, typename S, typename S2> class graph_elt_mixed_window;
  template <typename G, typename F, typename I> class graph_elt_window_if;
  template <typename G, typename F> class line_graph_elt_window;
  namespace util
  {
    template <typename G> class vertex;
    template <typename G> class edge;
  }


  namespace internal
  {

    /*!
      \internal
      \brief Default optional implementation.
    */
    template <typename W, typename E>
    struct neighb_niter_impl
    {

    };

    /*!
      \internal
      \brief Add more implementation for neighborhoods made from
      graph_window_base windows.

      FIXME: we need to redeclare the graph element interface.
      Here, a neighb niter iterator encapsulates a window qiter iterator.
      A window qiter iterator is a Proxy on a site P and can convert towards
      a graph element through its member element().

      The window qiter iterator cannot have an automatic conversion towards
      a graph element since there would be an ambiguity between this
      conversion and the conversion to a psite P, if P is also a graph
      element.
    */
    template <typename P, typename E>
    struct neighb_niter_impl_graph_window
    {
      unsigned id() const
      {
	return internal::force_exact<E>(*this).compute_p_().id();
      }

    };

    /*!
      \internal
      In this case, The site P is a util::vertex which means this iterator
      can automatically converts towards this type.
      There would be an ambiguity between util::vertex members and the one
      declared in neighb_niter_impl_graph_window<P,E> if this
      specialization did not exist.
    */
    template <typename G, typename E>
    struct neighb_niter_impl_graph_window< util::vertex<G>, E >
    {
    };

    /*!
      \internal
      In this case, The site P is a util::vertex which means this iterator
      can automatically converts towards this type.
      There would be an ambiguity between util::edge members and the one
      declared in neighb_niter_impl_graph_window<P,E> if this
      specialization did not exist.
    */
    template <typename G, typename E>
    struct neighb_niter_impl_graph_window< util::edge<G>, E >
    {
    };


    /*!
      \internal
      \brief Add more implementation for neighborhoods made from
      graph_window_base windows.
    */
    template <typename P, typename T, typename E>
    struct neighb_niter_impl< graph_window_base<P, T>, E >
      : neighb_niter_impl_graph_window<P,E>
    {
      typedef typename T::target S;

      // Tech. note: A copy is returned since it actually is a
      // temporary object; we cannot return a "const&" here for some
      // compilers badly kills the temporary object...
      mln_graph_element(S) element() const
      {
	return internal::force_exact<E>(*this).compute_p_().element();
      }

    };



    /*!
      \internal
      \brief Add more implementation for neighborhoods made from a
       graph_window_piter.
    */
    template <typename G, typename S, typename E>
    struct neighb_niter_impl<graph_elt_window<G,S>, E>
      : public neighb_niter_impl< graph_window_base< mln_result(S::fun_t),
						     graph_elt_window<G,S> >,
				  E >
    {

    };

    /*!
      \internal
      \brief Add more implementation for neighborhoods made from a
      line_graph_window_piter.
    */
    template <typename G, typename F, typename E>
    struct neighb_niter_impl<line_graph_elt_window<G,F>, E>
      : public neighb_niter_impl< graph_window_base< mln_result(F),
						     line_graph_elt_window<G, F> >,
				  E >
    {

    };


    /*!
      \internal
      \brief Add more implementation for neighborhoods made from a
      graph_window_if_piter.
    */
    template <typename G, typename S, typename I, typename E>
    struct neighb_niter_impl<graph_elt_window_if<G,S,I>, E>
      : public neighb_niter_impl< graph_window_base< mln_result(S::fun_t),
						     graph_elt_window_if<G,S,I> >,
				  E >
    {

    };


    /*!
      \internal
      \brief Add more implementation for neighborhoods made from a
      graph_window_piter.
    */
    template <typename G, typename S, typename S2, typename E>
    struct neighb_niter_impl<graph_elt_mixed_window<G,S,S2>, E>
      : public neighb_niter_impl< graph_window_base< mln_result(S2::fun_t),
						     graph_elt_mixed_window<G,S,S2> >,
				  E >
    {

    };


  } // end of namespace mln::internal

} // end of namespace mln

#endif // ! MLN_CORE_INTERNAL_NEIGHB_NITER_IMPL_HH
