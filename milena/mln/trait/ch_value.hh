// Copyright (C) 2007, 2008 EPITA Research and Development Laboratory (LRDE)
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

#ifndef MLN_TRAIT_CH_VALUE_HH
# define MLN_TRAIT_CH_VALUE_HH

/*! \file mln/trait/ch_value.hh
 *
 * \brief Definition of the "change value" image trait.
 */

# include <mln/tag/skeleton.hh>
# include <mln/trait/image_from_mesh.hh>


# define mln_ch_value(I, V) typename mln::trait::ch_value< I, V >::ret



namespace mln
{

  // Fwd decl.
  namespace metal { template <unsigned n, typename T> class vec; }




  namespace trait
  {

    // Fwd decl.
    template <typename I, typename V> struct ch_value;


    namespace impl
    {

      // Decl.
      template <typename I, typename V> struct ch_value_;

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

      // For mln::value::stack_image<n,I>.
      template < template <unsigned, class> class M, unsigned n, typename I,
		 typename V >
      struct ch_value_<  M< n, tag::image_<I> >,  V  >
      {
	/* FIXME: The code used to read

	     typedef algebra::vec<n, V> value;
	     typedef mln_ch_value(I, value) ret;

	   here.  But this is wrong IMHO (Roland).  Changing the value
	   type of a stack image (in fact, a vectorial image) shall
	   alter the *value type* of the image, not the type of the
	   *components* of the vector.  Hence the current definition.
	 */
	typedef mln_ch_value(I, V) ret;
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
      struct ch_value_<  M< tag::image_<I>, tag::pset_<S> >,  V  >
      {
	typedef M< mln_ch_value(I, V), S > ret;
      };

      template < template <class, class> class M, typename F, typename S,
		 typename V >
      struct ch_value_<  M< tag::function_<F>, tag::pset_<S> >,  V  >
      {
	typedef typename S::mesh mesh;
	// FIXME: from_psite instead? coord=int!?
	typedef typename image_from_mesh< mesh, V >::ret ret;
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
