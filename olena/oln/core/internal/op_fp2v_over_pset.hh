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

#ifndef OLN_CORE_INTERNAL_OP_FP2V_OVER_PSET
# define OLN_CORE_INTERNAL_OP_FP2V_OVER_PSET

# include <oln/core/concept/point_set.hh>
# include <oln/core/concept/function.hh>
# include <oln/core/internal/image_base.hh>
# include <oln/core/internal/f_pset_to_plain_image.hh>
# include <oln/core/gen/op.hh>


namespace oln
{

# define current \
     special_op_< stc::is<Function_p2v>, F, over, stc::is<Point_Set>, S >


  // Super type.
  template <typename F, typename S>
  struct super_trait_< internal::current >
  {
    typedef internal::primitive_image_< op_<F, over, S> > ret;
  };


  /// Virtual types.
  template <typename F, typename S>
  struct vtypes< internal::current >
  {
    typedef op_<F, over, S> Exact;

    typedef oln_point(S) point;
    typedef point psite;
    typedef S pset;

    typedef oln_coord(point) coord;

    typedef oln_result(F) value; // FIXME: Unconst, unref!
    typedef oln_result(F) rvalue;

    typedef internal::pair<F, S> data;

    typedef oln_f_pset_to_plain_image(S, value) plain;
    typedef void skeleton; // FIXME: check that this type is never accessed?
  };


  namespace internal
  {
    
    /// Implementation class for the result of "Function_p2v F / Point_Set S".
    
    template <typename F, typename S>
    class current : public internal::primitive_image_< op_<F, over, S> >
    {
      typedef internal::primitive_image_< op_<F, over, S> > super;
    public:
      stc_using(data);
      stc_using(point);
      stc_using(rvalue);

      bool impl_owns_(const point& p) const;
      rvalue impl_read(const point& p) const;
      const S& impl_points() const;

      const F& fun() const;

    protected:
      special_op_();
      special_op_(F& fun, S& pts);
    };

  } // end of namespace oln::internal



# ifndef OLN_INCLUDE_ONLY

  namespace internal
  {

    template <typename F, typename S>
    current::special_op_()
    {
    }

    template <typename F, typename S>
    current::special_op_(F& fun, S& pts)
    {
      this->data_ = new data(fun, pts);
    }

    template <typename F, typename S>
    bool
    current::impl_owns_(const typename current::point& p) const
    {
      assert(this->has_data());
      return this->data_->second.has(p);
    }

    template <typename F, typename S>
    typename current::rvalue
    current::impl_read(const typename current::point& p) const
    {
      assert(this->has_data());
      return this->data_->first(p);
    }

    template <typename F, typename S>
    const S&
    current::impl_points() const
    {
      assert(this->has_data());
      return this->data_->second;
    }

    template <typename F, typename S>
    const F&
    current::fun() const
    {
      assert(this->has_data());
      return this->data_->first;
    }
    
  } // end of namespace oln::internal

# endif // ! OLN_INCLUDE_ONLY

# undef current

} // end of namespace oln


#endif // ! OLN_CORE_INTERNAL_OP_FP2V_OVER_PSET_HH
