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

#ifndef OLN_CORE_INTERNAL_OP_IMAGE_RESTRICTED_TO_PSET_HH
# define OLN_CORE_INTERNAL_OP_IMAGE_RESTRICTED_TO_PSET_HH

# include <oln/core/internal/image_base.hh>
# include <oln/core/concept/point_set.hh>
# include <oln/core/gen/op.hh>


namespace oln
{

# define current \
     special_op_< stc::is<Image>, I, restricted_to, stc::is<Point_Set>, S >


  // Super type.
  template <typename I, typename S>
  struct super_trait_< internal::current >
  {
    typedef internal::image_extension_< op_<I, restricted_to, S> > ret;
  };


  // Virtual types.
  template <typename I, typename S>
  struct vtypes< internal::current >
  {
    typedef op_<I, restricted_to, S> Exact;
    typedef stc_type(I, point) point__;

    typedef I delegatee;
    typedef internal::pair<I,S> data;

    typedef S pset;

    typedef op_<oln_plain(I), restricted_to, S> plain;
    typedef op_<pl::rec<I>,   restricted_to, S> skeleton;
  };


  namespace internal
  {
    
    /// Implementation class for the result of "Image I | Point_Set S".
    
    template <typename I, typename S>
    class special_op_< stc::is<Image>, I, restricted_to, stc::is<Point_Set>, S >
      :
      public internal::image_extension_< op_<I, restricted_to, S> >
    {
      typedef internal::image_extension_< op_<I, restricted_to, S> > super;
    public:
      stc_using(pset);
      stc_using(box);
      stc_using(psite);
      stc_using(data);
      stc_using(delegatee);

      delegatee& impl_image();
      const delegatee& impl_image() const;

      bool impl_owns_(const psite& p) const;
      const pset& impl_points() const;

    protected:
      special_op_();
      special_op_(I& ima, S& subset);
    };

  } // end of namespace oln::internal


  // init

  template <typename S, typename I>
  bool init_(Point_Set<S>* this_,
	     const internal::current& dat);

  template <typename I, typename S>
  bool init_(Image<I>* this_,
	     const internal::current& dat);

  template <typename I, typename S, typename D>
  bool init_(internal::current* this_, const D& dat);

  // prepare

  template <typename I, typename S, typename D>
  bool prepare(internal::current& target, with_t, const D& dat);




# ifndef OLN_INCLUDE_ONLY

  namespace internal
  {

    template <typename I, typename S>
    current::special_op_()
    {
    }

    template <typename I, typename S>
    current::special_op_(I& ima, S& subset)
    {
      precondition(subset <= ima.points());
      this->data_ = new data(ima, subset);
    }

    template <typename I, typename S>
    bool
    current::impl_owns_(const typename current::psite& p) const
    {
      assert(this->has_data());
      return this->has(p);
    }

    template <typename I, typename S>
    typename current::delegatee&
    current::impl_image()
    {
      assert(this->has_data());
      return this->data_->first;
    }

    template <typename I, typename S>
    const typename current::delegatee&
    current::impl_image() const
    {
      assert(this->has_data());
      return this->data_->first;
    }

    template <typename I, typename S>
    const typename current::pset&
    current::impl_points() const
    {
      assert(this->has_data());
      return this->data_->second;
    }
    
  } // end of namespace oln::internal



  // init

  template <typename S, typename I>
  bool init_(Point_Set<S>* this_,
	     const internal::current& data)
  {
    exact(*this_) = data.points();
    return true;
  }

  template <typename I, typename S>
  bool init_(Image<I>* this_,
	     const internal::current& data)
  {
    exact(*this_) = data.image();
    return true;
  }

  template <typename I, typename S, typename D>
  bool init_(internal::current* this_, const D& dat)
  {
    precondition(not this_->has_data());
    this_->data__() = new typename op_<I, restricted_to, S>::data;
    bool image_ok  = init(this_->data__()->first, with, dat);
    bool subset_ok = init(this_->data__()->second, with, dat);
    postcondition(image_ok);
    postcondition(subset_ok);
    postcondition(this_->points() <= this_->image().points());
    return image_ok and subset_ok;
  }


  // prepare

  template <typename I, typename S, typename D>
  bool prepare(internal::current& target, with_t, const D& dat)
  {
    precondition(not target.has_data());
    target.data__() = new typename op_<I, restricted_to, S>::data;
    bool image_ok  = prepare(target.data__()->first, with, dat);
    bool subset_ok = init(target.data__()->second, with, dat);
    postcondition(image_ok);
    postcondition(subset_ok);
    postcondition(target.points() <= target.image().points());
    return image_ok and subset_ok;
  }


# endif // ! OLN_INCLUDE_ONLY

#  undef current

} // end of namespace oln


#endif // ! OLN_CORE_INTERNAL_OP_IMAGE_RESTRICTED_TO_PSET_HH
