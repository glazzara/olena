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

# include <mlc/unconst.hh>
# include <oln/core/concept/neighborhood.hh>
# include <oln/core/gen/op.hh>
# include <oln/core/gen/dpoints_piter.hh>
# include <oln/core/internal/image_base.hh>


namespace oln
{

# define current \
     special_op_< stc::is<Image>, I, plus, stc::is<Neighborhood>, N >

  // Instant value.
  oln_decl_instant_value(nbh);


  /// Fwd decls.
  template <typename Exact> struct Image;
  template <typename Exact> struct Neighborhood;


  /// Super type.
  template <typename I, typename N>
  struct super_trait_< internal::current >
  {
    typedef internal::image_extension_< op_<I, plus, N> > ret;
  };


  /// Virtual types.
  template <typename I, typename N>
  struct vtypes< internal::current >
  {
    typedef op_<I, plus, N> Exact;
    typedef stc_type(I, point) point__;

    typedef I delegatee;
    typedef internal::pair<I,N> data;

    typedef N nbh;
    typedef dpoints_fwd_piter_<point__> fwd_niter;
    typedef dpoints_bkd_piter_<point__> bkd_niter;

    typedef op_<oln_plain(I), plus, N> plain;
    typedef op_<pl::rec<I>,   plus, N> skeleton;
  };


  namespace internal
  {
    
    /// Implementation class the result of "Image I + Neighborhood N".
    
    template <typename I, typename N>
    class current
      :
      public internal::image_extension_< op_<I, plus, N> >,
      private mlc::assert_< mlc_is_not_a(I, Image_with_Nbh) > // FIXME: Add err msg.
    {
      typedef internal::image_extension_< op_<I, plus, N> > super;
    public:
      stc_using(nbh);
      stc_using(data);
      stc_using(delegatee);

      delegatee& impl_image();
      const delegatee& impl_image() const;

      nbh  impl_nbhood() const;
      nbh& impl_nbhood();

    protected:
      special_op_();
      special_op_(I& ima, N& n);
    };

  } // end of namespace oln::internal


  // prepare

  template <typename I, typename N, typename D>
  bool prepare(internal::current& target, with_t, const D& dat);

  // init

  template <typename I, typename N, typename D>
  bool init_(internal::current* target, const D& dat);

  template <typename N, typename I>
  bool init_(Neighborhood<N>* this_,
	     const internal::current& dat);

  template <typename I, typename N>
  bool init_(Image<I>* this_,
	     const internal::current& dat);



# ifndef OLN_INCLUDE_ONLY

  namespace internal
  {

    template <typename I, typename N>
    current::special_op_()
    {
    }

    template <typename I, typename N>
    current::special_op_(I& ima, N& n)
    {
      this->data_ = new data(ima, n);
    }

    template <typename I, typename N>
    typename current::delegatee&
    current::impl_image()
    {
      assert(this->has_data());
      return this->data_->first;
    }

    template <typename I, typename N>
    const typename current::delegatee&
    current::impl_image() const
    {
      assert(this->has_data());
      return this->data_->first;
    }

    template <typename I, typename N>
    typename current::nbh
    current::impl_nbhood() const
    {
      assert(this->has_data());
      return this->data_->second;
    }
    
  } // end of namespace oln::internal


  // init

  template <typename N, typename I>
  bool init_(Neighborhood<N>* this_,
	     const internal::current& data)
  {
    exact(*this_) = data.nbhood();
    return true;
  }

  template <typename I, typename N>
  bool init_(Image<I>* this_,
	     const internal::current& data)
  {
    exact(*this_) = data.image();
    return true;
  }

  template <typename I, typename N, typename D>
  bool init_(internal::current* this_, const D& dat)
  {
    precondition(not this_->has_data());
    this_->data__() = new typename op_<I, plus, N>::data;
    bool ima_ok = init(this_->data__()->first, with, dat);
    bool nbh_ok = init(this_->data__()->second, with, dat);
    postcondition(ima_ok);
    postcondition(nbh_ok);
    return ima_ok and nbh_ok;
  }

  // prepare

  template <typename I, typename N, typename D>
  bool prepare(internal::current& target, with_t, const D& dat)
  {
    precondition(not target.has_data());
    target.data__() = new typename op_<I, plus, N>::data;
    bool ima_ok = prepare(target.data__()->first, with, dat);
    bool nbh_ok = init(target.data__()->second, with, dat);
    postcondition(ima_ok);
    postcondition(nbh_ok);
    return ima_ok and nbh_ok;
  }

# endif // OLN_INCLUDE_ONLY

#  undef current

} // end of namespace oln


#endif // ! OLN_CORE_INTERNAL_OP_IMAGE_PLUS_NBH_HH
