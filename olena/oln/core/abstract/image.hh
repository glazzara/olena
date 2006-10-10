// Copyright (C) 2001, 2003, 2004, 2005, 2006 EPITA Research and
// Development Laboratory
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

#ifndef OLN_CORE_ABSTRACT_IMAGE_HH
# define OLN_CORE_ABSTRACT_IMAGE_HH

# include <oln/core/typedefs.hh>
# include <oln/core/abstract/fwd_decls.hh>
# include <oln/core/automatic/image.hh>


namespace oln
{

  /*! \namespace oln::abstract
  ** \brief oln::abstract namespace.
  */
  namespace abstract {


    /*! \class abstract::image<E>
    **
    ** The abstract::image class is the base class from whom derives
    ** every concrete image class.  Basically, an image is a set of
    ** points and a set of values associated with those points.
    **
    ** Parameter E is the exact type of image.
    */


    template <typename E>
    struct image : public virtual stc::any__simple<E>,
		   public virtual oln::type,
		   public automatic::get_impl<image, E>
    {

    public:

      struct decl
      {
// 	oln_virtual_typedef(topo);
// 	oln_virtual_typedef(grid);

// 	oln_virtual_typedef(coord);

// 	oln_virtual_typedef(psite);
// 	oln_virtual_typedef(point);

// // 	oln_virtual_typedef(piter);
// // 	oln_virtual_typedef(fwd_piter);
// // 	oln_virtual_typedef(bkd_piter);

// 	oln_virtual_typedef(value);
// 	oln_virtual_typedef(rvalue);

// 	oln_virtual_typedef(concrete);

// 	oln_virtual_typedef(morpher);

	decl();
      };


    private:

      /// Typedefs.
      typedef oln_type_of(E, topo)   topo_t;
      typedef oln_type_of(E, psite)  psite_t;
      typedef oln_type_of(E, rvalue) rvalue_t;


    public:

      /*------------------*
       ! abstract methods !
       *------------------*/

      /*! \brief Return the topological information about the current
      **  image.  Nota bene: this method is abstract-like.it is a
      **  pseudo-abstract method.
      **
      ** \return An object deriving from abstract::topo.  Ex: if the
      ** image is an image2d<something>, the returned object is a
      ** topo2d.
      */

      const topo_t& topo() const;


      /*! \brief Gives access to the value stored at \a p in the
      ** current image.
      */

      rvalue_t operator()(const psite_t& p) const;

    protected:

      /*! \brief Constructor (protected, empty).
      */
      image();

      /*! \brief Destructor.
      */
      virtual ~image();

    };


# ifndef OLN_INCLUDE_ONLY

    template <typename E>
    image<E>::decl::decl()
    {
// 	  mlc::assert_< mlc_is_a(topo,      abstract::topology)           >::check();
// 	  mlc::assert_< mlc_is_a(grid,      abstract::grid)               >::check();
// 	  mlc::assert_< mlc_is_a(piter,     abstract::iterator_on_points) >::check();
// 	  mlc::assert_< mlc_is_a(fwd_piter, abstract::iterator_on_points) >::check();
// 	  mlc::assert_< mlc_is_a(bkd_piter, abstract::iterator_on_points) >::check();
	  // FIXME: Rec.
	  // mlc::assert_< mlc_is_a(concrete,  abstract::image)              >::check();
    }

    template <typename E>
    image<E>::image()
    {
    }

    template <typename E>
    image<E>::~image()
    {
      decl();
    }

    template <typename E>
    const typename image<E>::topo_t&
    image<E>::topo() const
    {
      return this->exact().impl_topo();
    }

    template <typename E>
    typename image<E>::rvalue_t
    image<E>::operator()(const typename image<E>::psite_t& p) const
    {
      return this->exact().impl_op_read(p);
    }

# endif



  } // end of namespace oln::abstract

} // end of namespace oln


# include <oln/core/abstract/image/hierarchies.hh>


#endif // ! OLN_CORE_ABSTRACT_IMAGE_HH
