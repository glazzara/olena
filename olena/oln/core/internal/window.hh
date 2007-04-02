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

#ifndef OLN_CORE_INTERNAL_WINDOW_HH
# define OLN_CORE_INTERNAL_WINDOW_HH

# include <oln/core/internal/window_base.hh>
# include <oln/core/internal/dpoints_impl.hh>
# include <oln/core/gen/dpoints_piter.hh>


namespace oln
{


  // Fwd decl.
  namespace internal { template <typename Exact> class window_; }


  // Super type.
  template <typename Exact>
  struct super_trait_< internal::window_<Exact> >
  {
    typedef internal::window_base_<Exact> ret;
  };


  // Virtual types.
  template <typename Exact>
  struct vtypes< internal::window_<Exact> >
  {
    typedef stc_deferred(point) point__;
    typedef stc::final< oln_dpoint(point__) >         dpoint;
    typedef stc::final< dpoints_fwd_piter_<point__> > fwd_qiter;
    typedef stc::final< dpoints_bkd_piter_<point__> > bkd_qiter;
  };

  namespace internal
  {

    /// Base implementation class for regular window classes.

    template <typename Exact>
    class window_ : public window_base_<Exact>,
		    public dpoints_impl_< stc_deferred(dpoint) >
    {
      typedef window_base_<Exact> super;
    public:

      stc_using(point);
      typedef typename point::dpoint dpoint;

      window_();
      Exact& take(const dpoint& dp);
      Exact impl_op_unary_minus_() const;
      
    }; // end of class oln::internal::window_<Exact>


# ifndef OLN_INCLUDE_ONLY

    template <typename Exact>
    window_<Exact>::window_()
    {
    }

    template <typename Exact>
    Exact&
    window_<Exact>::take(const typename window_<Exact>::dpoint& dp)
    {
      this->take_( dp);
      return exact(*this);
    }
    
    template <typename Exact>
    Exact
    window_<Exact>::impl_op_unary_minus_() const
    {
      Exact tmp;
      for (unsigned i = 0; i < this->size(); ++i)
	tmp.take(- this->v_[i]);
      return tmp;
    }

  } // end of namespace oln::internal

# endif // ! OLN_INCLUDE_ONLY

} // end of namespace oln


#endif // ! OLN_CORE_INTERNAL_WINDOW_HH
