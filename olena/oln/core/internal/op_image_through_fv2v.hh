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

#ifndef OLN_CORE_INTERNAL_OP_IMAGE_THROUGH_FV2V_HH
# define OLN_CORE_INTERNAL_OP_IMAGE_THROUGH_FV2V_HH

# include <oln/core/internal/image_base.hh>
# include <oln/core/concept/function.hh>
# include <oln/core/gen/op.hh>


namespace oln
{


# define current \
     special_op_< stc::is<Image>, I, through, stc::is<Function_v2v>, F >


  // Fwd decl.
  namespace internal { template <typename I, typename F> class current; }


  // Super type.
  template <typename I, typename F>
  struct super_trait_< internal::current >
  {
    typedef internal::value_morpher_< op_<I, through, F> > ret;
  };


# define super \
    super_trait_< internal::current >::ret


  // Virtual types.
  template <typename I, typename F>
  struct vtypes< internal::current >
  {
    typedef I delegatee;
    typedef internal::pair<I,F> data;

    typedef oln_result(F) rvalue;
    typedef mlc_basic(rvalue) value;

    typedef typename mlc::if_< mlc::and_list_< mlc_is_not_const(I),
					       stc_is_found_type(I, lvalue),
					       mlc_is_reference(oln_result(F)),
					       mlc_is_not_const(oln_result(F)) >,
			       stc_find_type(I, lvalue),
			       stc::not_delegated >::ret lvalue;

    typedef op_<oln_plain(I), through, F> plain;
    typedef op_<pl::rec<I>,   through, F> skeleton;
  };


  namespace internal
  {

    /// Implementation class for the result of "Function_v2v F << Image I".

    template <typename I, typename F>
    class current : public super
    {
      typedef op_<I, through, F> Exact;
    public:

      stc_using(delegatee);
      stc_using(data);

      stc_using(psite);
      stc_using(value);
      stc_using(rvalue);
      typedef stc_find_type(Exact, lvalue) lvalue; // FIXME: macro; propagate in similar classes.

      delegatee& impl_image();
      const delegatee& impl_image() const;

      rvalue impl_read(const psite& p) const;
      lvalue impl_read_write(const psite& p);
      void impl_write(const psite& p, const value& v);

      const F& fun() const;

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
      this->data_ = new data(ima, f);
    }

    template <typename I, typename F>
    typename current::delegatee&
    current::impl_image()
    {
      assert(this->has_data());
      return this->data_->first;
    }

    template <typename I, typename F>
    const typename current::delegatee&
    current::impl_image() const
    {
      assert(this->has_data());
      return this->data_->first;
    }

    template <typename I, typename F>
    typename current::rvalue
    current::impl_read(const typename current::psite& p) const
    {
      assert(this->image().has_data());
      return this->fun()(this->image()(p));
    }

    template <typename I, typename F>
    typename current::lvalue
    current::impl_read_write(const typename current::psite& p)
    {
      assert(this->image().has_data());
      return this->fun()(this->image()(p));
    }

    template <typename I, typename F>
    void
    current::impl_write(const typename current::psite& p,
			const typename current::value& v)
    {
      assert(this->image().has_data());
      this->image()(p) = v;
    }

    template <typename I, typename F>
    const F&
    current::fun() const
    {
      assert(this->has_data());
      return this->data_->second;
    }

# endif // ! OLN_INCLUDE_ONLY

  } // end of namespace oln::internal


  // FIXME: Add init.


# undef super
# undef current


} // end of namespace oln


#endif // ! OLN_CORE_INTERNAL_OP_IMAGE_THROUGH_FV2V_HH
