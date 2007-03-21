// Copyright (C) 2001, 2003, 2004, 2005, 2006, 2007 EPITA Research and
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

#ifndef OLN_CORE_CONCEPT_IMAGE_HH
# define OLN_CORE_CONCEPT_IMAGE_HH

# include <cstddef>
# include <oln/core/equipment.hh>


/*


Image
  |
  |
  |
  o -- Mutable_Image
  |      |
  |      + -- Fast_Image
  |
  |
  |
  o -- + -- Image_1D
  |    |
  |    + -- Image_2D
  |    |      |
  |    |      + -- Image_2D_rec
  |    |      |
  |    |      + -- Image_2D_hex
  |    |      |
  |    |      + -- Image_2D_tri
  |    |
  |    + -- Image_3D
  |
  |
  |
  o -- Point_Wise_Accessible_Image
  |      |
  |      |      Image_2D
  |      |        |
  |      |        |
  |      + -- Point_Wise_Accessible_Image_2D
  |      |      |
  |      |      |      Mutable_Image
  |      |      |        |
  |      |      |        |
  |      |      + -- Point_Wise_Mutable_Image_2D
  |      |
  |      + ...
  |
  |
  |
  o -- + -- Plain_Image
  |    |
  |    + -- Computed_Image
  |
  |
  |
  o -- + -- Gray_Level_Image
  |    |
  |    + -- Color_Image
  |    |
  |    + -- Label_Image
  |    |      |
  |    |      + -- Binary_Image
  |    |      |
  |    |      + -- String_Image
  |    |
  |    + -- Deformation_Field_Image
  |    |
  |    + -- Data_Image
  |
  |
  |
 ...

*/


namespace oln
{


  /// Concept-class "Image".

  template <typename Exact>
  struct Image : public virtual Any<Exact>,
		 public automatic::get_impl<Image, Exact>
  {
    stc_typename(grid);

    stc_typename(point);
    stc_typename(psite);

    stc_typename(value);
    stc_typename(rvalue);

    stc_typename(piter);
    stc_typename(fwd_piter);
    stc_typename(bkd_piter);

    stc_typename(box);
    stc_typename(pset);

    // stc_typename(output); // FIXME: Uncomment!
    stc_typename(plain);

    bool owns_(const psite& p) const;
    rvalue operator()(const psite& p) const;

    pset points() const;

    // With final impl:
    box  bbox() const;

  protected:
    Image();
  };


  /// Concept-class "Image_with_Nbh".

  template <typename Exact>
  struct Image_with_Nbh : public virtual Image<Exact>,
			  public automatic::get_impl<Image_with_Nbh, Exact>
  {
    stc_typename(nbh);

    stc_typename(fwd_niter);
    stc_typename(bkd_niter);
    typedef fwd_niter niter;

    nbh nbhood() const;

  protected:
    Image_with_Nbh();
  };


  /// Concept-class "Image_with_Border".

  template <typename Exact>
  struct Image_with_Border : public virtual Image<Exact>,
			     public automatic::get_impl<Image_with_Border, Exact>
  {
    unsigned border() const;
  protected:
    Image_with_Border();
  };


  /// Concept-class "Mutable_Image".

  template <typename Exact>
  struct Mutable_Image : public virtual Image<Exact>,
			 public automatic::get_impl<Mutable_Image, Exact>
  {
    stc_using_from(Image, psite);
    using Image<Exact>::operator();

    stc_typename(lvalue);
    lvalue operator()(const psite& p);

  protected:
    Mutable_Image();
  };


  /// Concept-class "Fast_Image".

  template <typename Exact>
  struct Fast_Image : public virtual Mutable_Image<Exact>,
		      public automatic::get_impl<Fast_Image, Exact>
  {
    stc_using_from(Image, rvalue);
    stc_using_from(Image, lvalue);

    stc_typename(index);

    rvalue operator[](index i) const;
    lvalue operator[](index i);
    std::size_t npoints() const;
    // FIXME: ...

  protected:
    Fast_Image();
 };


  /// Concept-class "Point_Wise_Accessible_Image".

  template <typename Exact>
  struct Point_Wise_Accessible_Image : public virtual Image<Exact>,
				       public automatic::get_impl<Point_Wise_Accessible_Image, Exact>
  {
    stc_using_from(Image, point);

    bool has(const point& p) const;

  protected:
    Point_Wise_Accessible_Image();
  };


  /// Concept-class "Value_Wise_Accessible_Image".

  template <typename Exact>
  struct Value_Wise_Accessible_Image : public virtual Image<Exact>,
				       public automatic::get_impl<Value_Wise_Accessible_Image, Exact>
  {
    stc_typename(vsite);
    stc_typename(rvaluep);
    rvaluep value_(const vsite& v) const;

  protected:
    Value_Wise_Accessible_Image();
 };


  /// Concept-class "Value_Wise_Mutable_Image".

  template <typename Exact>
  struct Value_Wise_Mutable_Image : public virtual Value_Wise_Accessible_Image<Exact>,
				    public automatic::get_impl<Value_Wise_Mutable_Image, Exact>
  {
    stc_using_from(Value_Wise_Accessible_Image, vsite);
    using Value_Wise_Accessible_Image<Exact>::value;

    stc_typename(lvaluep);
    lvaluep value_(const vsite& v);

  protected:
    Value_Wise_Mutable_Image();
  };


  /// Concept-class "Image_1D".

  template <typename Exact>
  struct Image_1D : public virtual Image<Exact>,
		    public automatic::get_impl<Image_1D, Exact>
  {
    stc_typename(coord);

  protected:
    Image_1D();
  };


  /// Concept-class "Image_2D".

  template <typename Exact>
  struct Image_2D : public virtual Image<Exact>,
		    public automatic::get_impl<Image_2D, Exact>
  {
    stc_typename(coord);

    // final
    coord min_row() const;
    coord max_row() const;
    coord min_col() const;
    coord max_col() const;

  protected:
    Image_2D();
  };


  /// Concept-class "Image_3D".

  template <typename Exact>
  struct Image_3D : public virtual Image<Exact>,
		    public automatic::get_impl<Image_3D, Exact>
  {
    stc_typename(coord);

  protected:
    Image_3D();
  };


  /// Concept-class "Point_Wise_Accessible_Image_2D".

  template <typename Exact>
  struct Point_Wise_Accessible_Image_2D : public virtual Point_Wise_Accessible_Image<Exact>,
					  public virtual Image_2D<Exact>,
					  public automatic::get_impl<Point_Wise_Accessible_Image_2D, Exact>
  {
    stc_using_from(Point_Wise_Accessible_Image, point);
    stc_using_from(Point_Wise_Accessible_Image, rvalue);
    stc_using_from(Image_2D, coord);

    bool   has_at(coord row, coord col) const;
    rvalue     at(coord row, coord col) const;

    // default
    bool   impl_has_at(coord row, coord col) const;
    rvalue     impl_at(coord row, coord col) const;

  protected:
    Point_Wise_Accessible_Image_2D();
  };


  /// Concept-class "Point_Wise_Mutable_Image_2D".

  template <typename Exact>
  struct Point_Wise_Mutable_Image_2D : public virtual Point_Wise_Accessible_Image_2D<Exact>,
				       public virtual Mutable_Image<Exact>,
				       public automatic::get_impl<Point_Wise_Mutable_Image_2D, Exact>
  {
    stc_using_from(Point_Wise_Accessible_Image_2D, point);
    stc_using_from(Point_Wise_Accessible_Image_2D, coord);
    stc_using_from(Mutable_Image, lvalue);
    using Point_Wise_Accessible_Image_2D<Exact>::at;

    lvalue at(coord row, coord col);

    // default
    lvalue impl_at(coord row, coord col);

  protected:
    Point_Wise_Mutable_Image_2D();
  };



  /// Concept-class "Gray_Level_Image".

  template <typename Exact>
  struct Gray_Level_Image : public virtual Image<Exact>
  {
  protected:
    Gray_Level_Image();
  };

  /// Concept-class "Color_Image".

  template <typename Exact>
  struct Color_Image : public virtual Image<Exact>
  {
  protected:
    Color_Image();
  };

  /// Concept-class "Label_Image".

  template <typename Exact>
  struct Label_Image : public virtual Image<Exact>
  {
  protected:
    Label_Image();
  };

  /// Concept-class "Binary_Image".

  template <typename Exact>
  struct Binary_Image : public Label_Image<Exact>
  {
  protected:
    Binary_Image();
  };

  /// Concept-class "String_Image".

  template <typename Exact>
  struct String_Image : public Label_Image<Exact>
  {
  protected:
    String_Image();
  };

  /// Concept-class "Deformation_Field_Image".

  template <typename Exact>
  struct Deformation_Field_Image : public virtual Image<Exact>
  {
  protected:
    Deformation_Field_Image();
  };

  /// Concept-class "Data_Image".

  template <typename Exact>
  struct Data_Image : public virtual Image<Exact>
  {
  protected:
    Data_Image();
  };




# ifndef OLN_INCLUDE_ONLY

  // -----------------------------------   Image<Exact>

  template <typename Exact>
  bool
  Image<Exact>::owns_(const typename Image<Exact>::psite& p) const
  {
    return exact(this)->impl_owns_(p);
  }

  template <typename Exact>
  typename Image<Exact>::rvalue
  Image<Exact>::operator()(const typename Image<Exact>::psite& p) const
  {
    precondition(this->owns_(p));
    return exact(this)->impl_read(p);
  }

  template <typename Exact>
  typename Image<Exact>::box
  Image<Exact>::bbox() const
  {
    return this->points().bbox(); // Final impl.
  }

  template <typename Exact>
  typename Image<Exact>::pset
  Image<Exact>::points() const
  {
    return exact(this)->impl_points();
  }

  template <typename Exact>
  Image<Exact>::Image()
  {
  }

  // -----------------------------------   Image_with_Nbh<Exact>

  template <typename Exact>
  typename Image_with_Nbh<Exact>::nbh
  Image_with_Nbh<Exact>::nbhood() const
  {
    return exact(this)->impl_nbhood();
  }

  template <typename Exact>
  Image_with_Nbh<Exact>::Image_with_Nbh()
  {
  }

  // -----------------------------------   Image_with_Border<Exact>

  template <typename Exact>
  unsigned
  Image_with_Border<Exact>::border() const
  {
    return exact(this)->impl_border();
  }

  template <typename Exact>
  Image_with_Border<Exact>::Image_with_Border()
  {
  }

  // -----------------------------------   Mutable_Image<Exact>

  template <typename Exact>
  typename Mutable_Image<Exact>::lvalue
  Mutable_Image<Exact>::operator()(const typename Mutable_Image<Exact>::psite& p)
  {
    precondition(this->owns_(p));
    return exact(this)->impl_read_write(p);
  }

  template <typename Exact>
  Mutable_Image<Exact>::Mutable_Image()
  {
  }

  // -----------------------------------   Point_Wise_Accessible_Image<Exact>

  template <typename Exact>
  bool
  Point_Wise_Accessible_Image<Exact>::has(const typename Point_Wise_Accessible_Image<Exact>::point& p) const
  {
    return exact(this)->impl_has(p);
  }

  template <typename Exact>
  Point_Wise_Accessible_Image<Exact>::Point_Wise_Accessible_Image()
  {
  }

  // -----------------------------------   Fast_Image<Exact>

  template <typename Exact>
  typename Fast_Image<Exact>::rvalue
  Fast_Image<Exact>::operator[](typename Fast_Image<Exact>::index i) const
  {
    return exact(this)->impl_index_read(i);
  }

  template <typename Exact>
  typename Fast_Image<Exact>::lvalue
  Fast_Image<Exact>::operator[](typename Fast_Image<Exact>::index i)
  {
    return exact(this)->impl_index_read_write(i);
  }

  template <typename Exact>
  std::size_t
  Fast_Image<Exact>::npoints() const
  {
    return exact(this)->impl_npoints();
  }

  template <typename Exact>
  Fast_Image<Exact>::Fast_Image()
  {
  }

  // -----------------------------------   Value_Wise_Accessible_Image<Exact>

  template <typename Exact>
  typename Value_Wise_Accessible_Image<Exact>::rvaluep
  Value_Wise_Accessible_Image<Exact>::value_(const typename Value_Wise_Accessible_Image<Exact>::vsite& v) const
  {
    return exact(this)->impl_value_read(v);
  }

  template <typename Exact>
  Value_Wise_Accessible_Image<Exact>::Value_Wise_Accessible_Image()
  {
  }

  // -----------------------------------   Value_Wise_Mutable_Image<Exact>

  template <typename Exact>
  typename Value_Wise_Mutable_Image<Exact>::lvaluep
  Value_Wise_Mutable_Image<Exact>::value_(const typename Value_Wise_Accessible_Image<Exact>::vsite& v)
  {
    return exact(this)->impl_value_read_write(v);
  }

  template <typename Exact>
  Value_Wise_Mutable_Image<Exact>::Value_Wise_Mutable_Image()
  {
  }

  // -----------------------------------   Image_1D<Exact>

  template <typename Exact>
  Image_1D<Exact>::Image_1D()
  {
  }

  // -----------------------------------   Image_2D<Exact>

  template <typename Exact>
  Image_2D<Exact>::Image_2D()
  {
  }

  template <typename Exact>
  typename Image_2D<Exact>::coord
  Image_2D<Exact>::min_row() const
  {
    return this->bbox().pmin().row();
  }

  template <typename Exact>
  typename Image_2D<Exact>::coord
  Image_2D<Exact>::max_row() const
  {
    return this->bbox().pmax().row();
  }

  template <typename Exact>
  typename Image_2D<Exact>::coord
  Image_2D<Exact>::min_col() const
  {
    return this->bbox().pmin().col();
  }

  template <typename Exact>
  typename Image_2D<Exact>::coord
  Image_2D<Exact>::max_col() const
  {
    return this->bbox().pmax().col();
  }

  // -----------------------------------   Image_3D<Exact>

  template <typename Exact>
  Image_3D<Exact>::Image_3D()
  {
  }

  // -----------------------------------   Point_Wise_Accessible_Image_2D<Exact>

  template <typename Exact>
  bool
  Point_Wise_Accessible_Image_2D<Exact>::has_at(coord row, coord col) const
  {
    return exact(this)->impl_has_at(row, col);
  }

  template <typename Exact>
  typename Point_Wise_Accessible_Image_2D<Exact>::rvalue
  Point_Wise_Accessible_Image_2D<Exact>::at(coord row, coord col) const
  {
    return exact(this)->impl_at(row, col);
  }

  template <typename Exact>
  bool
  Point_Wise_Accessible_Image_2D<Exact>::impl_has_at(coord row, coord col) const
  {
    typename Point_Wise_Accessible_Image_2D<Exact>::point p(row, col);
    return this->has(p);
  }

  template <typename Exact>
  typename Point_Wise_Accessible_Image_2D<Exact>::rvalue
  Point_Wise_Accessible_Image_2D<Exact>::impl_at(coord row, coord col) const
  {
    typename Point_Wise_Accessible_Image_2D<Exact>::point p(row, col);
    return this->operator()(p);
  }

  template <typename Exact>
  Point_Wise_Accessible_Image_2D<Exact>::Point_Wise_Accessible_Image_2D()
  {
  }

  // -----------------------------------   Point_Wise_Mutable_Image_2D<Exact>

  template <typename Exact>
  typename Point_Wise_Mutable_Image_2D<Exact>::lvalue
  Point_Wise_Mutable_Image_2D<Exact>::at(coord row, coord col)
  {
    return exact(this)->impl_at(row, col);
  }

  template <typename Exact>
  typename Point_Wise_Mutable_Image_2D<Exact>::lvalue
  Point_Wise_Mutable_Image_2D<Exact>::impl_at(coord row, coord col)
  {
    typename Point_Wise_Mutable_Image_2D<Exact>::point p(row, col);
    return this->operator()(p);
  }

  template <typename Exact>
  Point_Wise_Mutable_Image_2D<Exact>::Point_Wise_Mutable_Image_2D()
  {
  }

  // -----------------------------------   Gray_Level_Image<Exact>

  template <typename Exact>
  Gray_Level_Image<Exact>::Gray_Level_Image()
  {
  }

  // -----------------------------------   Color_Image<Exact>

  template <typename Exact>
  Color_Image<Exact>::Color_Image()
  {
  }

  // -----------------------------------   Label_Image<Exact>

  template <typename Exact>
  Label_Image<Exact>::Label_Image()
  {
  }

  // -----------------------------------   Binary_Image<Exact>

  template <typename Exact>
  Binary_Image<Exact>::Binary_Image()
  {
  }

  // -----------------------------------   String_Image<Exact>

  template <typename Exact>
  String_Image<Exact>::String_Image()
  {
  }

  // -----------------------------------   Deformation_Field_Image<Exact>

  template <typename Exact>
  Deformation_Field_Image<Exact>::Deformation_Field_Image()
  {
  }

  // -----------------------------------   Data_Image<Exact>

  template <typename Exact>
  Data_Image<Exact>::Data_Image()
  {
  }


# endif // OLN_INCLUDE_ONLY

} // end of namespace oln


#endif // ! OLN_CORE_CONCEPT_IMAGE_HH
