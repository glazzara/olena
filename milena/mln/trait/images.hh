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

/*! \file mln/core/trait/images.hh
 *
 * \brief Forward declarations of all image types.
 *
 * \todo Split this file into many.
 */

# include <iostream>
# include <string>

# include <mln/value/props.hh>
# include <mln/metal/all.hh>


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
mlc_if( mlc_is_const(I), mln::trait::io::read_only, mln_trait_image_io(I) )

// for data: if raw or linear => stored, otherwise like I (i.e., either stored or computed)
#define mln_trait_image_data_from_(I) typename					\
mln::metal::if_< mln::metal::or_< mlc_equal( mln_trait_image_data(I),		\
					     mln::trait::data::raw),		\
                                  mlc_equal( mln_trait_image_data(I),		\
					     mln::trait::data::linear) >,	\
	         mln::trait::data::stored,					\
                 mln_trait_image_data(I) >::ret


namespace mln
{

  // Primitive types.
  template <typename T> struct image1d_b;
  template <typename T> struct image2d_b;
  template <typename T> struct image3d_b;
  namespace pw { template <typename F, typename S> struct image; }
  template <typename P, typename T> class rle_image;
  template <typename P, typename T> class sparse_image;

  // Morphers.
  template <typename I, typename F> struct image_if;
  template <typename I, typename D> class decorated_image;
  template <typename I, typename S> class sub_image;
  template <typename I> struct t_image;
  template <typename I> class safe_image;
  template <typename T, typename I> class cast_image_;
  namespace value { template <unsigned n, typename I> struct stack_image; }



  namespace trait
  {

    struct undef { std::string str() const { return "undef"; } };


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



    struct category
    {
      struct any {};
      struct primary : any { std::string str() const { return "category::primary"; } };
      struct morpher : any {};
      struct domain_morpher
	: morpher { std::string str() const { return "category::domain_morpher"; } };
      struct value_morpher
	: morpher { std::string str() const { return "category::value_morpher"; } };
      struct identity_morpher
	: morpher { std::string str() const { return "category::identity_morpher"; } };
    };

    struct data
    {
      struct any {};
      struct computed : any  { std::string str() const { return "data::computed"; } };
      struct stored   : any  { std::string str() const { return "data::stored"; } };
      struct linear
	: stored { std::string str() const { return "data::linear"; } };
      struct raw
	: linear { std::string str() const { return "data::raw"; } };
    };

    struct quant
    {
      struct any {};
      struct low  : any { std::string str() const { return "quant::low"; } };
      struct high : any { std::string str() const { return "quant::high"; } };
    };

    struct value
    {
      struct any {};
      struct scalar    : any { std::string str() const { return "value::scalar"; } };
      struct vectorial : any { std::string str() const { return "value::vectorial"; } };
      struct structed  : any { std::string str() const { return "value::structed"; } };
      struct pointer   : any { std::string str() const { return "value::pointer"; } };

      struct fixme // So FIXME!
	: any { std::string str() const { return "space::fixme"; } };
    };


    struct access
    {
      struct any {};
      struct random   : any { std::string str() const { return "access::random"; } };
      struct browsing : any { std::string str() const { return "access::browsing"; } };
    };

    struct space
    {
      struct any {};
      struct one_d    : any { std::string str() const { return "space::one_d"; } };
      struct two_d    : any { std::string str() const { return "space::two_d"; } };
      struct three_d  : any { std::string str() const { return "space::three_d"; } };

      struct fixme // So FIXME!
	: any { std::string str() const { return "space::fixme"; } };
    };

    struct size
    {
      struct any {};
      struct huge     : any { std::string str() const { return "size::huge"; } };
      struct regular  : any { std::string str() const { return "size::regular"; } };
    };

    struct support
    {
      struct any {};
      struct irregular : any { std::string str() const { return "support::irregular"; } };
      struct regular   : any { std::string str() const { return "support::regular"; } };
      struct aligned
	: regular { std::string str() const { return "support::aligned"; } };

      struct fixme // So FIXME!
	: any { std::string str() const { return "support::fixme"; } };
    };

    struct border
    {
      struct any {};
      struct none     : any { std::string str() const { return "border::none"; } };
      struct stored   : any { std::string str() const { return "border::stored"; } };
      struct computed : any { std::string str() const { return "border::computed"; } };
    };

    struct io
    {
      struct any {};
      struct read  : virtual any {};
      struct write : virtual any {};
      struct read_only
	: read        { std::string str() const { return "io::read_only"; } };
      struct write_only
	: write       { std::string str() const { return "io::write_only"; } };
      struct read_write
	: read, write { std::string str() const { return "io::read_write"; } };
    };

    struct speed
    {
      struct any {};
      struct slow : any { std::string str() const { return "speed::slow"; } };
      struct fast : any { std::string str() const { return "speed::fast"; } };
      struct fastest
	: fast { std::string str() const { return "speed::fastest"; } };
    };



    template <typename I>
    void print(std::ostream& ostr)
    {
      typedef image_<I> the;
      ostr << "{ "
	   << typename the::category().str() << ", "
	   << typename the::data().str() << ", "
	   << typename the::kind().str() << ", "
	   << typename the::quant().str() << ", "
	   << typename the::value().str() << ", "
	   << typename the::access().str() << ", "
	   << typename the::space().str() << ", "
	   << typename the::size().str() << ", "
	   << typename the::support().str() << ", "
	   << typename the::border().str() << ", "
	   << typename the::io().str() << ", "
	   << typename the::speed().str() << " }" << std::endl;
    }



    template <typename T, typename I>
    struct default_image_ : undefined_image_<I>
    {
    private:
      typedef metal::bool_<( mln_value_card_(T) == 0 )> is_high_quant_;
    public:
      typedef mln_value_kind(T) kind;
      typedef mlc_if( is_high_quant_, trait::quant::high, trait::quant::low ) quant;
      // FIXME: typedef undef value;  // scalar, vectorial, structed

      // speed is fast by default (neither "fastest" nor "slow")
      typedef trait::speed::fast speed;
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


#endif // ! MLN_TRAIT_IMAGES_HH
