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

#ifndef OLN_CORE_INTERNAL_OP_IMAGE_PLUS_NBH_HH
# define OLN_CORE_INTERNAL_OP_IMAGE_PLUS_NBH_HH

# include <oln/core/gen/op.hh>
# include <oln/core/gen/dpoints_piter.hh>
# include <oln/core/internal/image_base.hh>


namespace oln
{

  /// Fwd decls.
  template <typename Exact> struct Image;
  template <typename Exact> struct Neighborhood;


  /// Super type.
  template <typename I, typename N>
  struct super_trait_< internal::special_op_<stc::is<Image>, I, plus, stc::is<Neighborhood>, N> >
  {
    typedef internal::image_extension_< op_<I, plus, N> > ret;
  };


  /// Virtual types.
  template <typename I, typename N>
  struct vtypes< internal::special_op_<stc::is<Image>, I, plus, stc::is<Neighborhood>, N> >
  {
    typedef op_<I, plus, N> Exact;
    typedef stc_type(I, point) point__;

    typedef I delegatee;
    typedef internal::pair<I,N> data;

    typedef N nbh;
    typedef dpoints_fwd_piter_<point__> fwd_niter;
    typedef dpoints_bkd_piter_<point__> bkd_niter;
  };


  namespace internal
  {
    
    /// Implementation class the result of "Image I + Neighborhood N".
    
    template <typename I, typename N>
    class special_op_< stc::is<Image>, I, plus, stc::is<Neighborhood>, N >
      :
      public internal::image_extension_< op_<I, plus, N> >
    {
      typedef internal::image_extension_< op_<I, plus, N> > super;
    public:
      stc_using(nbh);
      stc_using(data);
      stc_using(delegatee);

      delegatee& impl_image()               { assert(this->has_data()); return this->data_->value1; }
      const delegatee& impl_image() const   { assert(this->has_data()); return this->data_->value1; }

      nbh impl_nbhood() const               { assert(this->has_data()); return this->data_->value2; }

    protected:
      special_op_(I& ima, N& n)
      {
	this->data_ = new data(ima, n);
      }
    };
    
    
  } // end of namespace oln::internal

} // end of namespace oln


#endif // ! OLN_CORE_INTERNAL_OP_IMAGE_PLUS_NBH_HH
