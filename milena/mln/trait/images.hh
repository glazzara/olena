// Copyright (C) 2007, 2008, 2009, 2011 EPITA Research and Development
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

#ifndef MLN_TRAIT_IMAGES_HH
# define MLN_TRAIT_IMAGES_HH

/// \file
///
/// Some base trait types for images.
///
/// \todo Split this file into many.
///
/// \todo the 'nature' prop is not set yet in image types.
///
/// \todo The image list is not up-to-date (and move it elsewhere !)

# include <iostream>
# include <string>

# include <mln/trait/undef.hh>
# include <mln/trait/image/props.hh>
# include <mln/trait/value_.hh>

# include <mln/metal/bexpr.hh>
# include <mln/metal/equal.hh>
# include <mln/metal/if.hh>
# include <mln/metal/is_const.hh>



# define mln_trait_image_category(I) typename mln::trait::image_< I >::category
# define mln_trait_image_speed(I)    typename mln::trait::image_< I >::speed
# define mln_trait_image_size(I)     typename mln::trait::image_< I >::size

# define mln_trait_image_value_access(I)     typename mln::trait::image_< I >::value_access
# define mln_trait_image_value_storage(I)    typename mln::trait::image_< I >::value_storage
# define mln_trait_image_value_browsing(I)   typename mln::trait::image_< I >::value_browsing
# define mln_trait_image_value_io(I)         typename mln::trait::image_< I >::value_io
# define mln_trait_image_pw_io(I)            typename mln::trait::image_< I >::pw_io
# define mln_trait_image_vw_io(I)            typename mln::trait::image_< I >::vw_io
# define mln_trait_image_vw_set(I)           typename mln::trait::image_< I >::vw_set
# define mln_trait_image_value_alignment(I)  typename mln::trait::image_< I>::value_alignment

# define mln_trait_image_localization(I) typename mln::trait::image_< I >::localization
# define mln_trait_image_dimension(I)    typename mln::trait::image_< I >::dimension

# define mln_trait_image_ext_domain(I) typename mln::trait::image_< I >::ext_domain
# define mln_trait_image_ext_value(I)  typename mln::trait::image_< I >::ext_value
# define mln_trait_image_ext_io(I)     typename mln::trait::image_< I >::ext_io

# define mln_trait_image_kind(I)   typename mln::trait::image_< I >::kind
# define mln_trait_image_nature(I) typename mln::trait::image_< I >::nature
# define mln_trait_image_quant(I)  typename mln::trait::image_< I >::quant


// For value_io: I const => read_only, otherwise like I

# define mln_internal_trait_image_value_io_from(I)	\
							\
  mlc_if( mlc_is_const(I),				\
	  mln::trait::image::value_io::read_only,	\
	  mln_trait_image_value_io(I) )

# define mln_internal_trait_image_pw_io_from(I)         \
							\
  mlc_if( mlc_is_const(I),				\
	  mln::trait::image::pw_io::read,               \
	  mln_trait_image_pw_io(I) )


# define mln_internal_trait_image_vw_io_from(I)         \
							\
  mlc_if( mlc_is_const(I),				\
	  mln::trait::image::vw_io::read,               \
	  mln_trait_image_vw_io(I) )


# define mln_internal_trait_image_speed_from(I)						\
											\
  mlc_if( mlc_equal( mln_trait_image_speed(I), mln::trait::image::speed::fastest ),	\
	  mln::trait::image::speed::fast,						\
	  mln_trait_image_speed(I) )



namespace mln
{

  // Primitive types.
  template <typename T> struct image1d;
  template <typename T> class image2d;
  template <typename T> struct image3d;
  namespace pw { template <typename F, typename S> class image; }

  // Morphers.
  template <typename I, typename F> struct image_if_base;
  template <typename I, typename D> struct decorated_image;
  template <typename I, typename S> struct sub_image;
  template <typename I> class safe_image;
  template <typename T, typename I> struct cast_image_;
  namespace value { template <unsigned n, typename I> struct stack_image; }


  namespace trait
  {

    template <typename I>
    struct undefined_image_
    {
      // misc
      typedef undef category;
      typedef undef speed;
      typedef undef size;

      // value
      typedef undef vw_io;
      typedef undef vw_set;
      typedef undef value_alignment;
      typedef undef value_access;
      typedef undef value_storage;
      typedef undef value_browsing;
      typedef undef value_io;

      // site
      typedef undef pw_io;
      typedef undef localization;
      typedef undef dimension;

      // extended domain
      typedef undef ext_domain;
      typedef undef ext_value;
      typedef undef ext_io;

      // data (I::value)
      typedef undef kind;
      typedef undef nature;
      typedef undef quant;
    };


    template <typename I>
    struct image_ : undefined_image_<I>
    {
    };


    template <typename I>
    struct image_<const I> : image_<I>
    {
      // FIXME: TODO: io cannot contain "write"...
    };


    template <typename T, typename I>
    struct default_image_ : undefined_image_<I>
    {
    private:
      typedef mlc_equal(mln_trait_value_quant(T),
			trait::value::quant::high) is_high_quant_;
    public:
      typedef mln_trait_value_kind(T) kind;
      typedef mlc_if( is_high_quant_,
		      trait::image::quant::high,
		      trait::image::quant::low ) quant;
      // FIXME: typedef undef value;  // scalar, vectorial, structed

      // speed is fast by default (neither "fastest" nor "slow")
      typedef trait::image::speed::fast speed;
    };


    template <typename D, typename T, typename I>
    struct default_image_morpher : default_image_<T, I>
    {
      // misc => delegation except for 'category'
      typedef typename image_<D>::size  size;
      typedef mln_internal_trait_image_speed_from(D) speed; // un-fastest

      // Image geometry
      typedef typename image_<D>::localization localization;
      typedef typename image_<D>::dimension    dimension;

      // Memory organization
      typedef typename image_<D>::value_alignment  value_alignment;
      typedef typename image_<D>::value_access     value_access;
      typedef typename image_<D>::value_storage    value_storage;
      typedef typename image_<D>::value_browsing   value_browsing;
      // un-write when D is const
      typedef mln_internal_trait_image_value_io_from(D) value_io;

      // value wise access => delegation
      typedef mln_internal_trait_image_vw_io_from(D) vw_io;
      typedef typename image_<D>::vw_set             vw_set;


      // site wise access => delegation
      typedef mln_internal_trait_image_pw_io_from(D) pw_io;

      // extended domain => delegation
      typedef typename image_<D>::ext_domain ext_domain;
      typedef typename image_<D>::ext_value  ext_value;
      typedef typename image_<D>::ext_io     ext_io;

      // data (I::value) => delegation
      typedef typename image_<D>::nature nature;
      typedef typename image_<D>::kind   kind;
      typedef typename image_<D>::quant  quant;
    };


  } // end of namespace mln::trait

} // end of namespace mln


# include <mln/trait/image/print.hh>


#endif // ! MLN_TRAIT_IMAGES_HH
