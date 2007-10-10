// Copyright (C) 2007 EPITA Research and Development Laboratory
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

#ifndef MLN_TRAIT_IMAGES_HH
# define MLN_TRAIT_IMAGES_HH

/*! \file mln/trait/images.hh
 *
 * \brief Some base trait types for images.
 *
 * \todo Split this file into many.
 */

# include <iostream>
# include <string>

# include <mln/trait/undef.hh>
# include <mln/trait/image/props.hh>
# include <mln/value/props.hh>

# include <mln/metal/bexpr.hh>
# include <mln/metal/equal.hh>
# include <mln/metal/if.hh>
# include <mln/metal/is_const.hh>


# define mln_trait_image_category(I) typename mln::trait::image_< I >::category

# define mln_trait_image_kind(I)     typename mln::trait::image_< I >::kind
# define mln_trait_image_quant(I)    typename mln::trait::image_< I >::quant
# define mln_trait_image_value(I)    typename mln::trait::image_< I >::value

# define mln_trait_image_access(I)   typename mln::trait::image_< I >::access
# define mln_trait_image_space(I)    typename mln::trait::image_< I >::space
# define mln_trait_image_size(I)     typename mln::trait::image_< I >::size
# define mln_trait_image_support(I)  typename mln::trait::image_< I >::support

# define mln_trait_image_border(I)   typename mln::trait::image_< I >::border
# define mln_trait_image_data(I)     typename mln::trait::image_< I >::data
# define mln_trait_image_io(I)       typename mln::trait::image_< I >::io
# define mln_trait_image_speed(I)    typename mln::trait::image_< I >::speed


// for io: I const => read_only, otherwise like I
# define mln_trait_image_io_from_(I) \
mlc_if( mlc_is_const(I), mln::trait::image::io::read_only, mln_trait_image_io(I) )

// for data: if raw or linear => stored, otherwise like I (i.e., either stored or computed)
#define mln_trait_image_data_from_(I) typename							\
mln::metal::if_< mln::metal::or_< mlc_equal( mln_trait_image_data(I),				\
					     mln::trait::image::data::raw),			\
                                  mlc_equal( mln_trait_image_data(I),				\
					     mln::trait::image::data::linear) >,		\
	         mln::trait::image::data::stored,						\
                 mln_trait_image_data(I) >::ret


namespace mln
{

  // Primitive types.
  template <typename T> struct image1d;
  template <typename T> struct image2d;
  template <typename T> struct image3d;
  namespace pw { template <typename F, typename S> struct image; }
  template <typename P, typename T> class rle_image;
  template <typename P, typename T> class sparse_image;

  // Morphers.
  template <typename I, typename F> struct image_if_base;
  template <typename I, typename D> class decorated_image;
  template <typename I, typename S> class sub_image;
  template <typename I> struct t_image;
  template <typename I> class safe_image;
  template <typename T, typename I> class cast_image_;
  namespace value { template <unsigned n, typename I> struct stack_image; }



  namespace trait
  {


    template <typename I>
    struct undefined_image_
    {
      typedef undef category; // primary, domain_morpher, value_morpher,
                              // or identity_morpher < morpher

      // related to I::value
      typedef undef kind;     // color, gray, binary < logic < label, data
      typedef undef quant;    // low or high
      typedef undef value;    // scalar, vectorial, structed, pointer
      // FIXME: Make the difference between homogeneous and heterogeneous vectors...

      // related to I::pset
      typedef undef access;   // random, browsing
      typedef undef space;    // one_d, two_d, three_d
      typedef undef size;     // huge or regular
      typedef undef support;  // irregular, aligned < regular

      // global
      typedef undef border;   // none, stored, computed
      typedef undef data;     // raw < linear < stored, computed
      typedef undef io;       // read_only < read, write_only < write, read_write < both read'n write
      typedef undef speed;    // slow, fast, or fastest
    };


    template <typename I>
    struct image_ : undefined_image_<I>
    {
    };


    template <typename T, typename I>
    struct default_image_ : undefined_image_<I>
    {
    private:
      typedef metal::bool_<( mln_value_card_(T) == 0 )> is_high_quant_;
    public:
      typedef mln_value_kind(T) kind;
      typedef mlc_if( is_high_quant_,
		      trait::image::quant::high,
		      trait::image::quant::low ) quant;
      // FIXME: typedef undef value;  // scalar, vectorial, structed

      // speed is fast by default (neither "fastest" nor "slow")
      typedef trait::image::speed::fast speed;
    };


    template <typename D, typename T, typename I>
    struct default_image_morpher_ : default_image_<T, I>
    {
      // value-related => delegation
      typedef typename image_<D>::kind  kind;
      typedef typename image_<D>::quant quant;
      typedef typename image_<D>::value value;

      // domain-related => delegation
      typedef typename image_<D>::access  access;
      typedef typename image_<D>::space   space;
      typedef typename image_<D>::size    size;
      typedef typename image_<D>::support support;

      // mostly global-related => delegation
      typedef typename image_<D>::border border;
      typedef typename image_<D>::data   data;
      typedef typename image_<D>::io     io;
    };


  } // end of namespace mln::trait

} // end of namespace mln


# include <mln/trait/image/print.hh>


#endif // ! MLN_TRAIT_IMAGES_HH
