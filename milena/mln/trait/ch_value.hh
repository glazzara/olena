// Copyright (C) 2007, 2008, 2009 EPITA Research and Development
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

#ifndef MLN_TRAIT_CH_VALUE_HH
# define MLN_TRAIT_CH_VALUE_HH

/// \file
///
/// Definition of the "change value" image trait.

# include <mln/tag/skeleton.hh>
# include <mln/trait/image_from_grid.hh>
# include <mln/trait/ch_function_value.hh>


# define mln_ch_value(I, V)  typename mln::trait::ch_value< I, V >::ret
# define mln_ch_value_(I, V)          mln::trait::ch_value< I, V >::ret



namespace mln
{

  // Forward declarations.
  namespace algebra { template <unsigned n, typename T> class vec; }
  template <typename G, typename F> class p_edges;
  template <typename G, typename F> class p_vertices;
  template <typename P, typename V, typename G> class vertex_image;
  template <typename P, typename V, typename G> class edge_image;
  template <typename I> class labeled_image;
  namespace pw { template <typename F, typename S> class image; }



  namespace trait
  {

    // Forward declaration.
    template <typename I, typename V> struct ch_value;


    namespace impl
    {

      // Declaration.
      template <typename I, typename V> struct ch_value_;

      template <typename I, typename V>
      struct ch_value_< tag::image_<I>,  V  >
      {
	typedef mln_ch_value(I, V) ret;
      };

      template < template <class> class M, typename T,
		 typename V >
      struct ch_value_<  M< tag::value_<T> >,  V  >
      {
	typedef M< V > ret;
      };

      template < template <class> class M, typename I,
		 typename V >
      struct ch_value_<  M< tag::image_<I> >,  V  >
      {
	typedef M< mln_ch_value(I, V) > ret;
      };

      template < template <class, class> class M, typename T, typename I,
		 typename V >
      struct ch_value_<  M< tag::value_<T>, tag::image_<I> >,  V  >
      {
	typedef mln_ch_value(I, V) ret;
      };

      template < template <class, class> class M, typename P, typename T,
		 typename V >
      struct ch_value_<  M< tag::psite_<P>, tag::value_<T> >,  V  >
      {
	typedef M< P, V > ret;
      };

      template < template <class, class> class M, typename I1, typename I2,
		 typename V >
      struct ch_value_<  M< tag::image_<I1>, tag::image_<I2> >,  V  >
      {
	typedef M< mln_ch_value(I1, V), mln_ch_value(I2, V) > ret;
      };

      template < template <class, class> class M, typename I, typename E,
		 typename V >
      struct ch_value_<  M< tag::image_<I>, tag::ext_<E> >,  V  >
      {
	typedef M< mln_ch_value(I, V), E > ret;
      };

      // For mln::complex_image<D, G, T>.
      template < template <unsigned, class, class> class M,
		 unsigned D, typename G, typename T, typename V >
      struct ch_value_<  M< D, tag::psite_<G>, tag::value_<T> >,  V  >
      {
	typedef M< D, G, V > ret;
      };

      // For mln::neighb::image<I, N>.
      template < template <class, class> class M, typename I, typename N,
		 typename V >
      struct ch_value_<  M< tag::image_<I>, tag::neighb_<N> >,  V  >
      {
	typedef M < mln_ch_value(I, V), N > ret;
      };

      template < template <class, class> class M, typename I, typename S,
		 typename V >
      struct ch_value_<  M< tag::image_<I>, tag::domain_<S> >,  V  >
      {
	typedef M< mln_ch_value(I, V), S > ret;
      };

      template < template <class, class> class M, typename F, typename S,
		 typename V >
      struct ch_value_<  M< tag::function_<F>, tag::domain_<S> >,  V  >
      {
        // FIXME: what about S::site having no grid?
        typedef mln_deduce(S, site, grid) grid;
        typedef typename image_from_grid< grid, V >::ret ret;
      };


      // Graph image based on p_edges
      template < typename F,
	         typename G, typename FP,
		 typename V >
      struct ch_value_<  pw::image< tag::function_<F>, tag::domain_<p_edges<G, FP> > >,  V  >
      {
	typedef pw::image< mln_ch_function_value(F, V), p_edges<G, FP> > ret;
      };

      // Graph image based on p_vertices
      template < typename F,
	         typename G, typename FP,
		 typename V >
      struct ch_value_<  pw::image< tag::function_<F>, tag::domain_<p_vertices<G, FP> > >,  V  >

      {
	typedef pw::image< mln_ch_function_value(F, V), p_vertices<G, FP> > ret;
      };

      // Vertex Image
      template < typename P, typename V1, typename G, typename V2>
      struct ch_value_< vertex_image< tag::psite_<P>,
				      tag::value_<V1>,
				      tag::graph_<G> >,
			V2 >
      {
	typedef vertex_image< P, V2, G > ret;
      };

      // Edge Image
      template < typename P, typename V1, typename G, typename V2>
      struct ch_value_< edge_image< tag::psite_<P>,
				    tag::value_<V1>,
				    tag::graph_<G> >,
			V2 >
      {
	typedef edge_image< P, V2, G > ret;
      };

      // Labeled image
      template < typename I, typename V>
      struct ch_value_< labeled_image< tag::image_<I> >,
			V >
      {
	typedef mln_ch_value(I,V) ret;
      };


      template < template <class, class> class M, typename T, typename S,
		 typename V >
      struct ch_value_<  M< tag::value_<T>, tag::domain_<S> >,  V  >
      {
        // FIXME: what about S::site having no grid?
        typedef mln_deduce(S, site, grid) grid;
        typedef typename image_from_grid< grid, V >::ret ret;
      };


      template < template <class, class> class M, typename I, typename F,
		 typename V >
      struct ch_value_<  M< tag::image_<I>, tag::function_<F> >,  V  >
      {
	typedef M< mln_ch_value(I, V), F > ret;
      };

    } // end of namespace mln::trait::impl


    template <typename I, typename V>
    struct ch_value
    {
      typedef typename I::skeleton skeleton;
      typedef typename impl::ch_value_<skeleton, V>::ret ret;
    };

  } // end of namespace mln::trait

} // end of namespace mln


#endif // ! MLN_TRAIT_CH_VALUE_HH
