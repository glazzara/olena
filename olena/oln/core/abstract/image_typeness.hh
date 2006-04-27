// Copyright (C) 2005, 2006 EPITA Research and Development Laboratory
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

#ifndef OLENA_CORE_ABSTRACT_IMAGE_TYPENESS_HH
# define OLENA_CORE_ABSTRACT_IMAGE_TYPENESS_HH

# include <stc/entry.hh>
# include <oln/core/abstract/image.hh>
# include <oln/core/abstract/image_abstractions.hh>


/* Image ``typeness'' hierarchy (summary).


                                    /image<I>/
                                        ^
                                        |
           ,--------------+-------------+-------------+--------+---------.
           |              |             |             |        |         |
 /grey_level_image/ /color_image/ /label_image/ /data_image/  ...  /not_binary_
           ^              ^         ^       ^         ^        ^       image/
           |              |         |       |         |        |         ^
           |              |         |  /binary_image/ |        |         |
           |              |         |       ^         |        |         |
           |              |         |       |         |        |         |
       ,---------------------------------------------------------------------.
       | The selection of the super classe(s) is made according to the value |
       |                      of type_of(I, value).                          |
       `---------------------------------------------------------------------'
           |              |         |       |         |        |         |
           o              o         o       o         o        o         o

                                        o
                                        | 
                       stc::set_entry_node<I, typeness_tag>
                            (image typeness selector)
                                        ^
                                        |
                                 /image_entry<I>/
                                        ^
                                        |
                                        I
                                (a concrete image)

*/

namespace oln {

  namespace abstract
  {

    /// \brief Class of grey-level images.
    /// 
    /// An image that contains values whose type is \c int or
    /// <tt>unsigned char</tt>, or ntg::int_u<8> (...) is <b>not</b> a
    /// grey-level image.
    ///
    /// FIXME: Say more.
    template <typename E>
    struct grey_level_image : public virtual image<E>
    {
    protected:
      /// Constructor (protected, empty).
      grey_level_image() {}
    };

    /// Class of color images.
    template <typename E>
    struct color_image : public virtual image<E>
    {
    protected:
      /// Constructor (protected, empty).
      color_image() {}
    };

    /// \brief Class of images whose data are NOT Boolean values.
    ///
    /// However such images can derive from abstract::label_image (we
    /// can have labels that are not binary ones).
    template <typename E>
    struct not_binary_image : public virtual image<E>
    {
    protected:
      /// Constructor (protected, empty).
      not_binary_image() {}
    };

    /// \brief Class of images whose data are labels.
    /// 
    /// Such images do not support arithmetics.
    template <typename E>
    struct label_image : public virtual image<E>
    {
    protected:
      /// Constructor (protected, empty).
      label_image() {}
    };

    /// \brief Class of images whose data are Boolean values.
    ///
    /// Such images are also oln::abstract::label_image.
    template <typename E>
    struct binary_image : public label_image<E>
    {
    protected:
      /// Constructor (protected, empty).
      binary_image() {}
    };

    /// \brief Class of images whose data cannot be qualified of
    /// grey-levels, colors, or labels.
    template <typename E>
    struct data_image : public virtual image<E>
    {
    protected:
      /// Constructor (protected, empty).
      data_image() {}
    };


    /*---------------------.
    | Conjunctions types.  |
    `---------------------*/

    namespace internal
    {

      /// \brief Conjunctions types
      ///
      /// These types are helpers to express the inheritance relation
      /// ``is a X and not_binary_image''.  These abstractions are not
      /// to be used by client code (i.e., algorithms), they are only
      /// part of the inheritance machinery.
      //
      /// \{

      /// <em>Grey-level image</em> and <em>not binary image</em> abstraction.
      template <typename E>
      struct grey_level_image_ : public grey_level_image<E>,
				 public not_binary_image<E>
      {
      protected:
	/// Constructor (protected, empty).
	grey_level_image_() {}
      };

      /// <em>Label image</em> and <em>not binary image</em> abstraction.
      template <typename E>
      struct label_image_ : public label_image<E>,
			    public not_binary_image<E>
      {
      protected:
	/// Constructor (protected, empty).
	label_image_() {}
      };

      /// <em>Color image</em> and <em>not binary image</em> abstraction.
      template <typename E>
      struct color_image_ : public color_image<E>,
			    public not_binary_image<E>
      {
      protected:
	/// Constructor (protected, empty).
	color_image_() {}
      };

      /// <em>Data image</em> and <em>not binary image</em> abstraction.
      template <typename E>
      struct data_image_ : public data_image<E>,
			   public not_binary_image<E>
      {
      protected:
	/// Constructor (protected, empty).
	data_image_() {}
      };

    /// \}

    } // end of namespace oln::abstract::internal

  } // end of namespace oln::abstract


  /*--------------------.
  | Value type switch.  |
  `--------------------*/

  /// Case tag for the dimension.
  struct value_type_tag;
  
  /// Switch on on the grid dimension.
  /// \{

  // ----------------------------------------------- //
  // Cases where the value type is an builtin type.  //
  // ----------------------------------------------- //

  /// Binary case.
  template <typename value_type>
  struct case_<value_type_tag, value_type, 1> :
    public mlc::where_< mlc_eq(value_type, bool) >
  {
    // Definition of the super class corresponding to this case.
    typedef stc::abstraction_as_type<abstract::binary_image> super_type;
    // Definition of the external virtual type (same as the super
    // class in this case).
    typedef super_type image_typeness_type;
  };

  /// Grey-level case.
  template <typename value_type>
  struct case_<value_type_tag, value_type, 2> :
    public mlc::where_<
             mlc::or_list_< mlc_eq(value_type, char),
			    mlc_eq(value_type, unsigned char),
			    mlc_eq(value_type, unsigned char) > >
  {
    // Definition of the super class corresponding to this case
    // (abstract::grey_level_image_ is the conjunction of
    // abstract::grey_level_image and abstract::not_binary_image).
    typedef stc::abstraction_as_type<abstract::internal::grey_level_image_>
      super_type;
    // Definition of the external virtual type.
    typedef stc::abstraction_as_type<abstract::grey_level_image>
      image_typeness_type;
  };


  // -------------- //
  // Default case.  //
  // -------------- //

  /// Default case: image of ``data''.
  template <typename value_type>
  struct default_case_<value_type_tag, value_type>
  {
    // Definition of the super class corresponding to this case
    // (abstract::data_image_ is the conjunction of
    // abstract::data_image and abstract::not_binary_image).
    typedef stc::abstraction_as_type<abstract::internal::data_image_>
      super_type;
    // Definition of the external virtual type.
    typedef stc::abstraction_as_type<abstract::data_image>
      image_typeness_type;
  };

  /// \}

} // end of namespace oln


// Register the dimension switch/case for oln::abstract::image_entry.
namespace stc
{
  template <typename I>
  struct set_entry_node<I, oln::abstract::typeness_tag> :
    public oln::case_< oln::value_type_tag, oln_type_of(I, value) >::ret
      ::super_type::template instantiated_with<I>::ret
  {
  };
} // end of namespace stc


// FIXME: Is this external property really useful?
namespace oln
{
  /// Image ``typeness'' as an external vtype of abstract::image.
  template <typename I>
  struct ext_vtype< category::image,
		    abstract::image<I>,
		    // FIXME: Get rid of this typedef_:: qualifier.
		    typedef_::image_typeness_type >
  {
    // Use the same case as the one use in the inheritance-plugging
    // mechanism above, but retrieve the external vtype
    // (image_typename_type), not the super type.
    typedef
      typename oln::case_< oln::value_type_tag,
			   oln_type_of(I, value) >::ret::image_typeness_type
      ret;
  };
}

#endif // ! OLENA_CORE_ABSTRACT_IMAGE_TYPENESS_HH
