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

#ifndef OLN_CORE_INTERNAL_OP_IMAGE_SUCH_AS_FP2B_HH
# define OLN_CORE_INTERNAL_OP_IMAGE_SUCH_AS_FP2B_HH

# include <oln/core/internal/op_image_restricted_to_pset.hh>
# include <oln/core/internal/op_pset_such_as_fp2b.hh>


namespace oln
{


# define current \
     special_op_< stc::is<Image>, I, such_as, stc::is<Function_p2b>, F >


  // Fwd decl.
  namespace internal { template <typename I, typename F> class current; }


  // Super type.
  template <typename I, typename F>
  struct super_trait_< internal::current >
  {
    typedef internal::image_extension_< op_<I, such_as, F> > ret;
  };


# define super \
    super_trait_< internal::current >::ret


  // Virtual types.
  template <typename I, typename F>
  struct vtypes< internal::current >
  {
    typedef op_< const oln_pset(I), such_as, F > S__;
    typedef op_< I, restricted_to, const S__ > delegatee;

    typedef internal::singleton<delegatee> data;

    typedef oln_point(I) point;
    typedef oln_possible_lvalue(I) lvalue;
    typedef op_<oln_plain(I), such_as, F> plain;
    typedef op_<pl::rec<I>,   such_as, F> skeleton;
  };


  namespace internal
  {

    /// Implementation class for the result of "Image I | Function_p2b F".

    template <typename I, typename F>
    class current : public super
    {
    public:

      stc_using(delegatee);
      stc_using(data);

      delegatee& impl_image();
      const delegatee& impl_image() const;

    protected:
      special_op_();
      special_op_(I& ima, F& f);
    };


# ifndef OLN_INCLUDE_ONLY

    template <typename I, typename F>
    current::special_op_()
    {
    }

    template <typename I, typename F>
    current::special_op_(I& ima, F& f)
    {
      this->data_ = new data( ima | (ima.points() | f) );
    }

    template <typename I, typename F>
    typename current::delegatee&
    current::impl_image()
    {
      assert(this->has_data());
      return this->data_->value;
    }

    template <typename I, typename F>
    const typename current::delegatee&
    current::impl_image() const
    {
      assert(this->has_data());
      return this->data_->value;
    }

# endif // ! OLN_INCLUDE_ONLY

  } // end of namespace oln::internal


  // Fixme: Activate init


# undef super
# undef current


} // end of namespace oln


#endif // ! OLN_CORE_INTERNAL_OP_IMAGE_SUCH_AS_FP2B_HH
