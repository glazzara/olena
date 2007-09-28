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
 */

# include <iostream>
# include <string>

# include <mln/value/props.hh>
# include <mln/metal/if.hh>
# include <mln/metal/bool.hh>



# define mln_trait_image_data(I) typename mln::trait::image_< I >::data


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
      // related to I::value
      typedef undef kind;     // color, gray, binary < label, data
      typedef undef quant;    // low or high
      typedef undef value;    // scalar, vectorial, structed

      // related to I::pset
      typedef undef access;   // random, browse
      typedef undef space;    // one_d, two_d, three_d
      typedef undef size;     // huge or regular
      typedef undef support;  // irregular, aligned < regular

      // global
      typedef undef border;   // none, stored, computed
      typedef undef data;     // linear < stored, computed
      typedef undef io;       // read_only < read_write
      typedef undef speed;    // slow, fast, or fastest
    };


    template <typename I>
    struct image_ : undefined_image_<I>
    {
    };


    template <typename I>
    void print(std::ostream& ostr)
    {
      typedef image_<I> the;
      ostr << "{ "
	   << the::data().str() << ", "
	   << the::kind().str() << ", "
	   << the::quant().str() << ", "
	   << the::value().str() << ", "
	   << the::access().str() << ", "
	   << the::space().str() << ", "
	   << the::size().str() << ", "
	   << the::support().str() << ", "
	   << the::border().str() << ", "
	   << the::io().str() << ", "
	   << the::speed().str() << " }" << std::endl;
    }


    struct data
    {
      struct computed        { std::string str() const { return "data::computed"; } };
      struct stored          { std::string str() const { return "data::stored"; } };
      struct linear : stored { std::string str() const { return "data::linear"; } };
    };

    struct quant
    {
      struct low  { std::string str() const { return "quant::low"; } };
      struct high { std::string str() const { return "quant::high"; } };
    };

    struct value
    {
      struct scalar    { std::string str() const { return "value::scalar"; } };
      struct vectorial { std::string str() const { return "value::vectorial"; } };
      struct structed  { std::string str() const { return "value::structed"; } };
    };


    struct access
    {
      struct random    { std::string str() const { return "access::random"; } };
      struct iterative { std::string str() const { return "access::iterative"; } };
    };

    struct space
    {
      struct one_d   { std::string str() const { return "space::one_d"; } };
      struct two_d   { std::string str() const { return "space::two_d"; } };
      struct three_d { std::string str() const { return "space::three_d"; } };
    };

    struct size
    {
      struct huge    { std::string str() const { return "size::huge"; } };
      struct regular { std::string str() const { return "size::regular"; } };
    };

    struct support
    {
      struct irregular         { std::string str() const { return "support::irregular"; } };
      struct regular           { std::string str() const { return "support::regular"; } };
      struct aligned : regular { std::string str() const { return "support::aligned"; } };
    };

    struct border
    {
      struct none     { std::string str() const { return "border::none"; } };
      struct stored   { std::string str() const { return "border::stored"; } };
      struct computed { std::string str() const { return "border::computed"; } };
    };

    struct io
    {
      struct read_only  { std::string str() const { return "io::read_only"; } };
      struct read_write { std::string str() const { return "io::read_write"; } };
    };

    struct speed
    {
      struct slow    { std::string str() const { return "speed::slow"; } };
      struct fast    { std::string str() const { return "speed::fast"; } };
      struct fastest { std::string str() const { return "speed::fastest"; } };
    };




    template <typename I>
    struct default_image_ : undefined_image_<I>
    {
    private:
      typedef mln_value(I) T_;
      typedef metal::bool_<( mln_value_card_(T_) != 0 )> is_high_quant_;
    public:
      typedef mln_value_kind(T_) kind;
      typedef mlc_if( is_high_quant_, trait::quant::high, trait::quant::low ) quant;
      // FIXME: typedef undef value;  // scalar, vectorial, structed
    };


    template <typename I>
    struct default_image_morpher_ : default_image_<I>
    {
    private:
      typedef typename I::delegatee D_;
    public:

      // value-related => delegation
      typedef typename image_<D_>::kind  kind;
      typedef typename image_<D_>::quant quant;
      typedef typename image_<D_>::value value;

      // domain-related => delegation
      typedef typename image_<D_>::access  access;
      typedef typename image_<D_>::space   space;
      typedef typename image_<D_>::size    size;
      typedef typename image_<D_>::support support;

      // mostly global-related => delegation
      typedef typename image_<D_>::border border;
      typedef typename image_<D_>::data   data;
      typedef typename image_<D_>::io     io;

      // *but* speed is fast by default (not fastest!)
      typedef trait::speed::fast speed;
    };


  } // end of namespace mln::trait

} // end of namespace mln


#endif // ! MLN_TRAIT_IMAGES_HH
