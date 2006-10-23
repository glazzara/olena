// Copyright (C) 2006 EPITA Research and Development Laboratory
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

#ifndef OLN_MORPHER_SLICE_HH
# define OLN_MORPHER_SLICE_HH

# include <cstdlib>

# include <mlc/int.hh>
# include <mlc/abort.hh>

# include <oln/core/image_entry.hh>
# include <oln/core/gen/bbox.hh>
# include <oln/core/type_fun/slice.hh>


namespace oln
{

  /*---------------.
  | slice<Image>.  |
  `---------------*/

  namespace morpher
  {
    // Forward declaration.
    template <typename Image> struct slice;

  } // end of namespace oln::morpher


  /// Super type.
  template <typename Image>
  struct set_super_type< morpher::slice<Image> >
  {
    typedef image_entry< morpher::slice<Image> > ret;
  };


  /// Virtual types associated to oln::morpher::slice<Image>
  /// \{
  template <typename Image>
  struct vtypes< morpher::slice<Image> >
  {
  private:
    typedef oln_deduce_type_of(Image, grid, dimvalue) orig_dimvalue_type;
    typedef mlc::uint_< mlc_value(orig_dimvalue_type) - 1 > dimvalue_type;

  public:
    typedef typename
    type_fun::slice_topo< oln_type_of(Image, topo) >::ret topo_type;

    typedef grid_<mlc_value(orig_dimvalue_type) - 1> grid_type;

    // psite_type: see below.
    typedef typename
    point_< mlc_value(dimvalue_type),
	    oln_deduce_type_of(Image, point, coord) >::ret
    point_type;

    // piter_type: see below.

    typedef oln_type_of(Image, value) value_type;
    // rvalue_type: see below.

    // fwd_piter_type: see oln/core/iterator_vtypes.hh.
    // bkd_piter_type: see oln/core/iterator_vtypes.hh.

    // fwd_qiter_type: see oln/core/iterator_vtypes.hh.
    // bkd_qiter_type: see oln/core/iterator_vtypes.hh.

    typedef oln::morpher::tag::slice morpher_type;

  };

  // Coord.
  template <typename Image>
  struct single_vtype< morpher::slice<Image>, typedef_::coord_type >
  {
    typedef oln_deduce_type_of(Image, point, coord) ret;
  };

  // Psite.
  template <typename Image>
  struct single_vtype< morpher::slice<Image>, typedef_::psite_type >
  {
  private:
    typedef morpher::slice<Image> self_t;
  public:
    typedef oln_type_of(self_t, point) ret;
  };

  /* FIXME: Should we keep the `piter' vtype, knowing that the macro
     `oln_piter' gives the `fwd_piter' vtype (and not the `piter'
     vtype)?  */
  // Piter.
  template <typename Image>
  struct single_vtype< morpher::slice<Image>, typedef_::piter_type >
  {
    typedef oln_type_of(Image, fwd_piter) ret;
  };

  // Rvalue.
  template <typename Image>
  struct single_vtype< morpher::slice<Image>, typedef_::rvalue_type >
  {
    typedef oln_type_of(Image, rvalue) ret;
  };

  // Lvalue.
  template <typename Image>
  struct single_vtype< morpher::slice<Image>, typedef_::lvalue_type >
  {
    typedef oln_type_of(Image, lvalue) ret;
  };
  /// \}


  namespace morpher
  {

    template <typename Image>
    class slice : public stc_get_supers(slice<Image>)
    {
      typedef slice<Image> self_t;

      typedef oln_type_of(self_t, topo)   topo_t;
      typedef oln_type_of(self_t, rvalue) rvalue_t;
      typedef oln_type_of(self_t, lvalue) lvalue_t;
      typedef oln_type_of(self_t, point)  point_t;

      typedef oln_type_of(Image,  topo)  orig_topo_t;
      typedef oln_type_of(Image,  point) orig_point_t;

      typedef oln_deduce_type_of(Image, point, coord) coord_t;


    public:
      // FIXME: Handle the constness.
      slice(const abstract::image_having_bbox<Image>& image,
	    unsigned slice_dim, coord_t slice_coord) :
	image_(image.exact()),
	slice_dim_(slice_dim),
	slice_coord_(slice_coord),
	topo_(type_fun::slice_topo<orig_topo_t>::convert(image.topo(),
							 slice_dim))
      {
      }

      const topo_t& impl_topo() const
      {
	return topo_;
      }

      rvalue_t impl_op_read(const point_t& p) const
      {
	precondition(topo_.has_large(p));
	orig_point_t q = unslice_point(p, slice_dim_, slice_coord_);
	return image_(q);
      }

      lvalue_t impl_op_readwrite(const point_t& p)
      {
	precondition(topo_.has_large(p));
	orig_point_t q = unslice_point(p, slice_dim_, slice_coord_);
	return image_(q);
      }

    protected:
      Image image_;
      unsigned slice_dim_;
      coord_t slice_coord_;

    private:
      topo_t topo_;
    };

  } // end of namespace oln::morpher

} // end of namespace oln

#endif // ! OLN_MORPHER_SLICE_HH
