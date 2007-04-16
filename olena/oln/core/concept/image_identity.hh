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

#ifndef OLN_CORE_CONCEPT_IMAGE_IDENTITY_HH
# define OLN_CORE_CONCEPT_IMAGE_IDENTITY_HH

# include <oln/core/concept/image.hh>



namespace oln
{

  namespace behavior { struct identity; }


  namespace automatic
  {


    /// Concept-class "Image".

    template <typename Exact>
    struct set_impl< Image, behavior::identity, Exact > : public virtual Any<Exact>
    {
      stc_typename(psite);
      stc_typename(rvalue);
      stc_typename(box);
      stc_typename(pset);

      bool impl_owns_(const psite& p) const;
      rvalue impl_read(const psite& p) const;
      const pset& impl_points() const;
    };


    /// Concept-class "Image_with_Nbh".

    template <typename Exact>
    struct set_impl< Image_with_Nbh, behavior::identity, Exact > : public virtual Any<Exact>
    {
      stc_typename(nbh);
      const nbh& impl_nbhood() const;
      nbh& impl_nbhood();
    };


    /// Concept-class "Image_with_Border".

    template <typename Exact>
    struct set_impl< Image_with_Border, behavior::identity, Exact > : public virtual Any<Exact>
    {
      unsigned impl_border() const;
    };


    /// Concept-class "Mutable_Image".

    template <typename Exact>
    struct set_impl< Mutable_Image, behavior::identity, Exact > : public virtual Any<Exact>
    {
      stc_typename(psite);
      stc_typename(lvalue);
      stc_typename(value);
      lvalue impl_read_write(const psite& p);
      void impl_write(const psite& p, const value& v);
    };


    /// Concept-class "Fast_Image".

    template <typename Exact>
    struct set_impl< Fast_Image, behavior::identity, Exact > : public virtual Any<Exact>
    {
      stc_typename(rvalue);
      stc_typename(lvalue);
      stc_typename(index);

      rvalue impl_index_read(index i) const;
      lvalue impl_index_read_write(index i);
      std::size_t impl_npoints() const;
    };


    /// Concept-class "Value_Wise_Accessible_Image".

    template <typename Exact>
    struct set_impl< Value_Wise_Accessible_Image, behavior::identity, Exact > : public virtual Any<Exact>
    {
      stc_typename(vsite);
      stc_typename(rvaluep);
      rvaluep impl_value_read(const vsite& v) const;
    };


    /// Concept-class "Value_Wise_Mutable_Image".

    template <typename Exact>
    struct set_impl< Value_Wise_Mutable_Image, behavior::identity, Exact > : public virtual Any<Exact>
    {
      stc_typename(vsite);
      stc_typename(lvaluep);
      lvaluep impl_value_read_write(const vsite& v);
    };


    /// Concept-class "Image_1D".

    template <typename Exact>
    struct set_impl< Image_1D, behavior::identity, Exact > : public virtual Any<Exact>
    {
    };


    /// Concept-class "Image_2D".

    template <typename Exact>
    struct set_impl< Image_2D, behavior::identity, Exact > : public virtual Any<Exact>
    {
    };


    /// Concept-class "Image_3D".

    template <typename Exact>
    struct set_impl< Image_3D, behavior::identity, Exact > : public virtual Any<Exact>
    {
    };


    /// Concept-class "Point_Wise_Accessible_Image_2D".

    template <typename Exact>
    struct set_impl< Point_Wise_Accessible_Image_2D, behavior::identity, Exact > : public virtual Any<Exact>
    {
      // default code is provided in Point_Wise_Accessible_Image_2D<Exact>
    };


    /// Concept-class "Point_Wise_Mutable_Image_2D".

    template <typename Exact>
    struct set_impl< Point_Wise_Mutable_Image_2D, behavior::identity, Exact > : public virtual Any<Exact>
    {
      // default code is provided in Point_Wise_Mutable_Image_2D<Exact>
    };






# ifndef OLN_INCLUDE_ONLY


    /// Concept-class "Image".

    template <typename Exact>
    bool
    set_impl< Image, behavior::identity, Exact >::impl_owns_(const typename set_impl< Image, behavior::identity, Exact >::psite& p) const
    {
      return exact(this)->image().owns_(p);
    }

    template <typename Exact>
    typename set_impl< Image, behavior::identity, Exact >::rvalue
    set_impl< Image, behavior::identity, Exact >::impl_read(const typename set_impl< Image, behavior::identity, Exact >::psite& p) const
    {
      return exact(this)->image()(p);
    }

    template <typename Exact>
    const typename set_impl< Image, behavior::identity, Exact >::pset&
    set_impl< Image, behavior::identity, Exact >::impl_points() const
    {
      return exact(this)->image().points();
    }


    /// Concept-class "Image_with_Nbh".

    template <typename Exact>
    const typename set_impl< Image_with_Nbh, behavior::identity, Exact >::nbh&
    set_impl< Image_with_Nbh, behavior::identity, Exact >::impl_nbhood() const
    {
      return exact(this)->image().nbhood();
    }

    template <typename Exact>
    typename set_impl< Image_with_Nbh, behavior::identity, Exact >::nbh&
    set_impl< Image_with_Nbh, behavior::identity, Exact >::impl_nbhood()
    {
      return exact(this)->image().nbhood();
    }


    /// Concept-class "Image_with_Border".

    template <typename Exact>
    unsigned
    set_impl< Image_with_Border, behavior::identity, Exact >::impl_border() const
    {
      return exact(this)->image().border();
    }


    /// Concept-class "Mutable_Image".

# define current set_impl< Mutable_Image, behavior::identity, Exact >

    template <typename Exact>
    typename current::lvalue
    current::impl_read_write(const typename current::psite& p)
    { 
      return exact(this)->image().operator()(p);
    }

    template <typename Exact>
    void
    current::impl_write(const typename current::psite& p,
			const typename current::value& v)
    { 
      return exact(this)->image().write_(p, v);
    }

# undef current


    /// Concept-class "Fast_Image".

    template <typename Exact>
    typename set_impl< Fast_Image, behavior::identity, Exact >::rvalue
    set_impl< Fast_Image, behavior::identity, Exact >::impl_index_read(typename set_impl< Fast_Image, behavior::identity, Exact >::index i) const
    { 
      return exact(this)->image()[i];
    }

    template <typename Exact>
    typename set_impl< Fast_Image, behavior::identity, Exact >::lvalue
    set_impl< Fast_Image, behavior::identity, Exact >::impl_index_read_write(typename set_impl< Fast_Image, behavior::identity, Exact >::index i)
    {
      return exact(this)->image()[i];
    }
    
    template <typename Exact>
    std::size_t
    set_impl< Fast_Image, behavior::identity, Exact >::impl_npoints() const
    {
      return exact(this)->image().npoints();
    }


    /// Concept-class "Value_Wise_Accessible_Image".

    template <typename Exact>
    typename set_impl< Value_Wise_Accessible_Image, behavior::identity, Exact >::rvaluep
    set_impl< Value_Wise_Accessible_Image, behavior::identity, Exact >::impl_value_read(const typename set_impl< Value_Wise_Accessible_Image, behavior::identity, Exact >::vsite& v) const
    {
      return exact(this)->image().value(v);
    }


    /// Concept-class "Value_Wise_Mutable_Image".

    template <typename Exact>
    typename set_impl< Value_Wise_Mutable_Image, behavior::identity, Exact >::lvaluep
    set_impl< Value_Wise_Mutable_Image, behavior::identity, Exact >::impl_value_read_write(const typename set_impl< Value_Wise_Mutable_Image, behavior::identity, Exact >::vsite& v)
    { 
      return exact(this)->image().value(v);
    }


# endif


  } // end of namespace oln::automatic

} // end of namespace oln


#endif // ! OLN_CORE_CONCEPT_IMAGE_IDENTITY_HH
