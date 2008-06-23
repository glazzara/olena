// Copyright (C) 2007, 2008 EPITA Research and Development Laboratory
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
 *
 * \todo Remove space_from_point and fwd decls at EOF.
 */

# include <string>
# include <mln/trait/undef.hh>
# include <mln/trait/value/kind.hh>



//  Properties of images.
//  =====================

// * misc

// category:     /any/
//                 |
//                 + -- primary
//                 |
//                 + -- /morpher/
//                        |
//                        + -- domain_morpher
//                        |
//                        + -- value_morpher
//                        |
//                        + -- identity_morpher

// speed:        /any/
//                 |
//                 + -- slow
//                 |
//                 + -- fast
//                       |
//                       + -- fastest

// size:         /any/
//                 |
//                 + -- regular
//                 |
//                 + -- huge

// * value

// value_access: /any/
//                 |
//                 + -- direct
//                 |
//                 + -- computed

// value_storage:/any/
//                 |
//                 + -- /organized/
//                 |      |
//                 |      + -- singleton
//                 |      |
//                 |      + -- one_block
//                 |      |
//                 |      + -- piecewise
//                 |
//                 + -- disrupted

// value_browsing:/any/
//                 |
//                 + -- site_wise_only
//                 |
//                 + -------- cell_wise
//                 |                 |
//                 + -- value_wise   |
//                              \    |
//                                -- + -- cell_and_value_wise

// value_io:     /any/
//                 |
//                 + -- read_only
//                 |
//                 + -- read_write

// * site localization

// localization: /any/
//                 |
//                 + -- none
//                 |
//                 + -- space
//                       |
//                       + -- /grid/
//                              |
//                              + -- isotropic_grid
//                              |     |
//                              |     + -- basic_grid
//                              |
//                              + -- anisotropic_grid

// dimension:    /any/
//                 |
//                 + -- none
//                 |
//                 + -- /some/
//                         |
//                         + -- one_d
//                         |
//                         + -- two_d
//                         |
//                         + -- three_d

// *  extended domain

// ext_domain:   /any/
//                 |
//                 + -- none
//                 |
//                 + -- /some/
//                        |
//                        + -- fixed
//                        |     |
//                        |     + -- infinite
//                        |
//                        + -- extendable

// ext_value:    /any/
//                 |
//                 + -- irrelevant
//                 |
//                 + -- single
//                 |
//                 + -- multiple

// ext_io:       /any/
//                 |
//                 + -- irrelevant
//                 |
//                 + -- read_only
//                 |
//                 + -- read_write

// * data (related to I::value)

// kind:         /any/
//                 |
//                 + -- color
//                 |
//                 + -- gray
//                 |
//                 + ----------- label
//                 |              |
//                 |              +-- named
//                 + -- /logic/   |
//                 |      |       |
//                 |      + -- /mvlogic/
//                 |      |      |
//                 |      |      + -- binary
//                 |      |      |
//                 |      |      + -- ternary
//                 |      |
//                 |      + -- fuzzy
//                 |
//                 + -- data
//                 |
//                 |
//                 + -- /map/
//                        |
//                        + -- distance

// nature:       /any/
//                 |
//                 + -- scalar
//                 |
//                 + -- vectorial
//                 |
//                 + -- structed
//                 |
//                 + -- pointer

// quant:        /any/
//                 |
//                 + -- low
//                 |
//                 + -- high


namespace mln
{

  namespace trait
  {

    namespace image
    {

// category:     /any/
//                 |
//                 + -- primary
//                 |
//                 + -- /morpher/
//                        |
//                        + -- domain_morpher
//                        |
//                        + -- value_morpher
//                        |
//                        + -- identity_morpher

      struct category
      {
	struct any { protected: any() {} };
	struct primary : any { std::string name() const { return "category::primary"; } };
	struct morpher : any { protected: morpher() {} };
	struct domain_morpher
	  : morpher { std::string name() const { return "category::domain_morpher"; } };
	struct value_morpher
	  : morpher { std::string name() const { return "category::value_morpher"; } };
	struct identity_morpher
	  : morpher { std::string name() const { return "category::identity_morpher"; } };
      };


// speed:        /any/
//                 |
//                 + -- slow
//                 |
//                 + -- fast
//                       |
//                       + -- fastest

      struct speed
      {
	struct any { protected: any() {} };
	struct slow : any { std::string name() const { return "speed::slow"; } };
	struct fast : any { std::string name() const { return "speed::fast"; } };
	struct fastest
	  : fast { std::string name() const { return "speed::fastest"; } };
      };


// size:         /any/
//                 |
//                 + -- regular
//                 |
//                 + -- huge

      struct size
      {
	struct any { protected: any() {} };
	struct huge     : any { std::string name() const { return "size::huge"; } };
	struct regular  : any { std::string name() const { return "size::regular"; } };
      };


// value_access: /any/
//                 |
//                 + -- direct
//                 |
//                 + -- computed

      struct value_access
      {
	struct any { protected: any() {} };
	struct direct   : any  { std::string name() const { return "value_access::direct"; } };
	struct computed : any  { std::string name() const { return "value_access::computed"; } };
      };


// value_storage:/any/
//                 |
//                 + -- /organized/
//                 |      |
//                 |      + -- singleton
//                 |      |
//                 |      + -- one_block
//                 |      |
//                 |      + -- piecewise
//                 |
//                 + -- disrupted

      struct value_storage
      {
	struct any { protected: any() {} };
	struct organized
	  : any { protected: organized() {} };
	struct singleton
	  : organized { std::string name() const { return "value_storage::singleton"; } };
	struct one_block
	  : organized { std::string name() const { return "value_storage::one_block"; } };
	struct piecewise
	  : organized { std::string name() const { return "value_storage::piecewise"; } };
	struct disrupted : any  { std::string name() const { return "value_storage::disrupted"; } };
      };


// value_browsing:/any/
//                 |
//                 + -- site_wise_only
//                 |
//                 + -------- cell_wise
//                 |                 |
//                 + -- value_wise   |
//                              \    |
//                                -- + -- cell_and_value_wise

      struct value_browsing
      {
	struct any { protected: any() {} };
	struct site_wise_only
	  : any { std::string name() const { return "value_browsing::site_wise_only"; } };
	struct cell_wise
	  : virtual any { std::string name() const { return "value_browsing::cell_wise"; } };
	struct value_wise
	  : virtual any { std::string name() const { return "value_browsing::value_wise"; } };
	struct cell_and_value_wise
	  : cell_wise,
	    value_wise
	{ std::string name() const { return "value_browsing::cell_and_value_wise"; } };
      };


// value_io:     /any/
//                 |
//                 + -- read_only
//                 |
//                 + -- read_write

      struct value_io
      {
	struct any { protected: any() {} };
	struct read_only
	  : any { std::string name() const { return "value_io::read_only"; } };
	struct read_write
	  : any { std::string name() const { return "value_io::read_write"; } };
      };


// localization: /any/
//                 |
//                 + -- none
//                 |
//                 + -- space
//                       |
//                       + -- /grid/
//                              |
//                              + -- isotropic_grid
//                              |     |
//                              |     + -- basic_grid
//                              |
//                              + -- anisotropic_grid

      struct localization
      {
	struct any { protected: any() {} };
	struct none  : any { std::string name() const { return "localization::none"; } };
	struct space : any { std::string name() const { return "localization::space"; } };
	struct grid  : space { protected: grid() {} };
	struct isotropic_grid
	  : grid { std::string name() const { return "localization::isotropic_grid"; } };
	struct basic_grid
	  : isotropic_grid { std::string name() const { return "localization::basic_grid"; } };
	struct anisotropic_grid
	  : grid { std::string name() const { return "localization::anisotropic_grid"; } };
      };


// dimension:    /any/
//                 |
//                 + -- none
//                 |
//                 + -- /some/
//                         |
//                         + -- one_d
//                         |
//                         + -- two_d
//                         |
//                         + -- three_d

      struct dimension
      {
	struct any { protected: any() {} };
	struct none     : any  { std::string name() const { return "dimension::none"; } };
	struct some
	  : any { protected: some() {} };
	struct one_d    : some { std::string name() const { return "dimension::one_d"; } };
	struct two_d    : some { std::string name() const { return "dimension::two_d"; } };
	struct three_d  : some { std::string name() const { return "dimension::three_d"; } };
      };


// ext_domain:   /any/
//                 |
//                 + -- none
//                 |
//                 + -- /some/
//                        |
//                        + -- fixed
//                        |     |
//                        |     + -- infinite
//                        |
//                        + -- extendable

      struct ext_domain
      {
	struct any { protected: any() {} };
	struct none     : any  { std::string name() const { return "ext_domain::none"; } };
	struct some
	  : any { protected: some() {} };
	struct extendable : some { std::string name() const { return "ext_domain::extendable"; } };
	struct fixed      : some { std::string name() const { return "ext_domain::fixed"; } };
	struct infinite  : fixed { std::string name() const { return "ext_domain::infinite"; } };
      };


// ext_value:    /any/
//                 |
//                 + -- irrelevant
//                 |
//                 + -- single
//                 |
//                 + -- multiple

      struct ext_value
      {
	struct any { protected: any() {} };
	struct irrelevant : any { std::string name() const { return "ext_value::irrelevant"; } };
	struct single     : any { std::string name() const { return "ext_value::single"; } };
	struct multiple   : any { std::string name() const { return "ext_value::multiple"; } };
      };


// ext_io:       /any/
//                 |
//                 + -- irrelevant
//                 |
//                 + -- read_only
//                 |
//                 + -- read_write

      struct ext_io
      {
	struct any { protected: any() {} };
	struct irrelevant
	  : any { std::string name() const { return "ext_io::irrelevant"; } };
	struct read_only
	  : any { std::string name() const { return "ext_io::read_only"; } };
	struct read_write
	  : any { std::string name() const { return "ext_io::read_write"; } };
      };


// kind:         /any/
//                 |
//                 + -- color
//                 |
//                 + -- gray
//                 |
//                 + ----------- label
//                 |              |
//                 |              +-- named
//                 + -- /logic/   |
//                 |      |       |
//                 |      + -- /mvlogic/
//                 |      |      |
//                 |      |      + -- binary
//                 |      |      |
//                 |      |      + -- ternary
//                 |      |
//                 |      + -- fuzzy
//                 |
//                 + -- data
//                 |
//                 |
//                 + -- /map/
//                        |
//                        + -- distance

      typedef mln::trait::value::kind kind; // Fetch the 'kind' structure from value traits.


// nature:       /any/
//                 |
//                 + -- scalar
//                 |
//                 + -- vectorial
//                 |
//                 + -- structed
//                 |
//                 + -- pointer

      struct nature
      {
	struct any { protected: any() {} };
	struct scalar    : any { std::string name() const { return "nature::scalar"; } };
	struct vectorial : any { std::string name() const { return "nature::vectorial"; } };
	struct structed  : any { std::string name() const { return "nature::structed"; } };
	struct pointer   : any { std::string name() const { return "nature::pointer"; } };
      };


// quant:        /any/
//                 |
//                 + -- low
//                 |
//                 + -- high

      struct quant
      {
	struct any { protected: any() {} };
	struct low  : any { std::string name() const { return "quant::low"; } };
	struct high : any { std::string name() const { return "quant::high"; } };
      };


    } // end of namespace mln::trait::image

  } // end of namespace mln::trait



  // FIXME: To be moved elsewhere?

  /// Compute the image::space trait from a point type.
  /// \{

  // Fwd decl. (used by trait::image::space_from_point).
  namespace grid
  {
    struct tick;
    struct square;
    struct hexa;
    struct cube;
  }

  template <typename M, typename C> struct point;
  typedef point<grid::tick, int> point1d;
  typedef point<grid::square, int> point2d;
  typedef point<grid::hexa, int> point2d_h;
  typedef point<grid::cube, int> point3d;

  namespace trait
  {
    namespace image
    {

      /// Function mapping a point type to the corresponding space trait.
      /// \{
      template <typename P>
      struct space_from_point
      { typedef undef ret; };

      template <>
      struct space_from_point<point1d>
      { typedef trait::image::dimension::one_d ret; };

      template <>
      struct space_from_point<point2d>
      { typedef trait::image::dimension::two_d ret; };

      template <>
      struct space_from_point<point3d>
      { typedef trait::image::dimension::three_d ret; };
      /// \}

    } // end of namespace mln::trait::image

  } // end of namespace mln::trait

} // end of namespace mln


#endif // ! MLN_TRAIT_IMAGE_PROPS_HH
