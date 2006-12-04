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
    typedef morpher::slice<Image> self_t;

    typedef oln_deduce_vtype(Image, grid, dimvalue) orig_dimvalue_type;
    typedef mlc::uint_< mlc_value(orig_dimvalue_type) - 1 > dimvalue_type;

    typedef oln_vtype(Image, fwd_piter) orig_fwd_piter;
    typedef oln_vtype(Image, bkd_piter) orig_bkd_piter;
    typedef oln_vtype(Image, fwd_qiter) orig_fwd_qiter;
    typedef oln_vtype(Image, bkd_qiter) orig_bkd_qiter;

  public:
    typedef typename
    type_fun::slice_topo< oln_vtype(Image, topo) >::ret topo_type;

    typedef typename point_< mlc_value(dimvalue_type),
			     oln_deduce_vtype(Image, point, coord) >::ret point_type;
    typedef point_type psite_type;
    // FIXME: A better (when overriding is involved) is oln_deferred_vtype(self_t, point).

    typedef oln_is_computed(Image) is_computed_type;
    typedef oln_value(Image) value_type;
    // rvalue_type: see below.
    // lvalue_type: see below.

    // fwd_piter_type: see oln/core/iterator_vtypes.hh.
    // bkd_piter_type: see oln/core/iterator_vtypes.hh.

    // fwd_qiter_type: see oln/core/iterator_vtypes.hh.
    // bkd_qiter_type: see oln/core/iterator_vtypes.hh.

    typedef oln::morpher::tag::slice morpher_type;

    // Final.
    typedef oln_fwd_piter(Image) piter_type;
    typedef oln_rvalue(Image)    rvalue_type;
    typedef oln_lvalue(Image)    lvalue_type;

    typedef typename type_fun::slice_iterator<orig_fwd_piter>::ret fwd_piter_type;
    typedef typename type_fun::slice_iterator<orig_bkd_piter>::ret bkd_piter_type;
    typedef typename type_fun::slice_iterator<orig_fwd_qiter>::ret fwd_qiter_type;
    typedef typename type_fun::slice_iterator<orig_bkd_qiter>::ret bkd_qiter_type;
    // FIXME: What about niters on morpher::slice?
  };



  namespace morpher
  {

    template <typename Image>
    class slice : public image_entry< slice<Image> >
    {
      typedef slice<Image> self_t;

      typedef oln_vtype(self_t, topo)   topo_t;
      typedef oln_vtype(self_t, rvalue) rvalue_t;
      typedef oln_vtype(self_t, lvalue) lvalue_t;
      typedef oln_vtype(self_t, point)  point_t;

      typedef oln_vtype(Image,  topo)  orig_topo_t;
      typedef oln_vtype(Image,  point) orig_point_t;

      typedef oln_deduce_vtype(Image, point, coord) coord_t;


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
