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

#ifndef MLN_TRAIT_IMAGE_PROPS_HH
# define MLN_TRAIT_IMAGE_PROPS_HH

/*! \file mln/trait/image/props.hh
 *
 * \brief Properties of image classes.
 */

# include <string>
# include <mln/trait/value/kind.hh>


namespace mln
{

  namespace trait
  {

    namespace image
    {

      // FIXME: For the record:

      //     template <typename I>
      //     struct undefined_image_
      //     {
      //       typedef undef category; // primary, domain_morpher, value_morpher,
      //                               // or identity_morpher < morpher

      //       // related to I::value
      //       typedef undef kind;     // color, gray, binary < logic < label, data
      //       typedef undef quant;    // low or high
      //       typedef undef value;    // scalar, vectorial, structed, pointer
      //       // FIXME: Make the difference between homogeneous and heterogeneous vectors...

      //       // related to I::pset
      //       typedef undef access;   // random, browsing
      //       typedef undef space;    // one_d, two_d, three_d
      //       typedef undef size;     // huge or regular
      //       typedef undef support;  // irregular, aligned < regular

      //       // global
      //       typedef undef border;   // none, { stored, computed } < some
      //       typedef undef data;     // raw < linear < stored, computed
      //       typedef undef io;       // read_only < read, write_only < write, read_write < both read'n write
      //       typedef undef speed;    // slow, fast, or fastest
      //     };

      // FIXME: Add nice pictures of those hierarchies.
    


      struct category
      {
	struct any {};
	struct primary : any { std::string name() const { return "category::primary"; } };
	struct morpher : any {};
	struct domain_morpher
	  : morpher { std::string name() const { return "category::domain_morpher"; } };
	struct value_morpher
	  : morpher { std::string name() const { return "category::value_morpher"; } };
	struct identity_morpher
	  : morpher { std::string name() const { return "category::identity_morpher"; } };
      };


      typedef mln::trait::value::kind kind; // Fetch the 'kind' structure from value traits.


      struct data
      {
	struct any {};
	struct computed : any  { std::string name() const { return "data::computed"; } };
	struct stored   : any  { std::string name() const { return "data::stored"; } };
	struct linear
	  : stored { std::string name() const { return "data::linear"; } };
	struct raw
	  : linear { std::string name() const { return "data::raw"; } };
      };

      struct quant
      {
	struct any {};
	struct low  : any { std::string name() const { return "quant::low"; } };
	struct high : any { std::string name() const { return "quant::high"; } };
      };

      struct value
      {
	struct any {};
	struct scalar    : any { std::string name() const { return "value::scalar"; } };
	struct vectorial : any { std::string name() const { return "value::vectorial"; } };
	struct structed  : any { std::string name() const { return "value::structed"; } };
	struct pointer   : any { std::string name() const { return "value::pointer"; } };

	struct fixme // So FIXME!
	  : any { std::string name() const { return "space::fixme"; } };
      };


      struct access
      {
	struct any {};
	struct random   : any { std::string name() const { return "access::random"; } };
	struct browsing : any { std::string name() const { return "access::browsing"; } };
      };

      struct space
      {
	struct any {};
	struct one_d    : any { std::string name() const { return "space::one_d"; } };
	struct two_d    : any { std::string name() const { return "space::two_d"; } };
	struct three_d  : any { std::string name() const { return "space::three_d"; } };

	struct fixme_ // So FIXME!
	  : any { std::string name() const { return "space::fixme_"; } };
      };

      struct size
      {
	struct any {};
	struct huge     : any { std::string name() const { return "size::huge"; } };
	struct regular  : any { std::string name() const { return "size::regular"; } };
      };

      struct support
      {
	struct any {};
	struct irregular : any { std::string name() const { return "support::irregular"; } };
	struct regular   : any { std::string name() const { return "support::regular"; } };
	struct aligned
	  : regular { std::string name() const { return "support::aligned"; } };

	struct fixme_ // So FIXME!
	  : any { std::string name() const { return "support::fixme_"; } };
      };

      struct border
      {
	struct any {};
	struct none     : any  { std::string name() const { return "border::none"; } };
	struct some     : any  { std::string name() const { return "border::some"; } };
	struct stored   : some { std::string name() const { return "border::stored"; } };
	struct computed : some { std::string name() const { return "border::computed"; } };
      };

      struct io
      {
	struct any {};
	struct read  : virtual any {};
	struct write : virtual any {};
	struct read_only
	  : read        { std::string name() const { return "io::read_only"; } };
	struct write_only
	  : write       { std::string name() const { return "io::write_only"; } };
	struct read_write
	  : read, write { std::string name() const { return "io::read_write"; } };
      };

      struct speed
      {
	struct any {};
	struct slow : any { std::string name() const { return "speed::slow"; } };
	struct fast : any { std::string name() const { return "speed::fast"; } };
	struct fastest
	  : fast { std::string name() const { return "speed::fastest"; } };
      };


    } // end of namespace mln::trait::image

  } // end of namespace mln::trait

} // end of namespace mln


#endif // ! MLN_TRAIT_IMAGE_PROPS_HH
