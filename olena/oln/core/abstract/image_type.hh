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

#ifndef OLENA_CORE_ABSTRACT_IMAGE_TYPE_HH
# define OLENA_CORE_ABSTRACT_IMAGE_TYPE_HH

# include <oln/core/abstract/image.hh>


/* Image ``type'' hierarchy (summary).


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
       | The selection of the super class(es) is made according to the value |
       |                      of type_of(I, value).                          |
       `---------------------------------------------------------------------'
           |              |         |       |         |        |         |
           o              o         o       o         o        o         o

                                        o 
                                        | 
                      /switch_<image_dimension_type, I>::ret/
                              (image type selector)
                                        ^
                                        |
                           /entry<abstract::image, I>/
                                        ^
                                        |
                                 image_entry<I>
                                        ^
                                        |
                                        I
                                (a concrete image)


  Default case: If no known value type is returned by `oln_type_of(I, value)',
  the entry is plugged to abstract::data_image<I>.  */


/*--------------------.
| Type abstractions.  |
`--------------------*/

namespace oln
{

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


    // -------------------- //
    // Conjunctions types.  //
    // -------------------- //

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

} // end of namespace oln


/*--------------.
| Type switch.  |
`--------------*/

namespace oln
{

  /// Switch on on the grid dimension.
  /// \{

  // ----------------------------------------------- //
  // Cases where the value type is an builtin type.  //
  // ----------------------------------------------- //

  /// Binary case.
  template <typename E>
  struct case_< image_hierarchy_wrt_type, E, 1 > :
    where_< mlc::eq_< oln_type_of(E, value), bool > >
  {
    // Definition of the super class corresponding to this case.
    typedef abstract::binary_image<E> ret;
  };

  /// Grey-level case.
  template <typename E>
  struct case_< image_hierarchy_wrt_type, E, 2 > :
    where_< mlc::or_list_< mlc::eq_<oln_type_of(E, value),          char>,
			   mlc::eq_<oln_type_of(E, value),   signed char>,
			   mlc::eq_<oln_type_of(E, value), unsigned char> > >
  {
    // Definition of the super class corresponding to this case
    // (abstract::grey_level_image_ is the conjunction of
    // abstract::grey_level_image and abstract::not_binary_image).
    typedef abstract::internal::grey_level_image_<E> ret;
  };


  // -------------- //
  // Default case.  //
  // -------------- //

  /// Default case: image of ``data''.
  template <typename E>
  struct default_case_< image_hierarchy_wrt_type, E >
  {
    // Definition of the super class corresponding to this case
    // (abstract::data_image_ is the conjunction of
    // abstract::data_image and abstract::not_binary_image).
    typedef abstract::internal::data_image_<E> ret;
  };

  /// \}

} // end of namespace oln


#endif // ! OLENA_CORE_ABSTRACT_IMAGE_TYPE_HH
