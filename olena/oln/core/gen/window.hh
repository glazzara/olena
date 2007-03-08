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

#ifndef OLN_CORE_GEN_WINDOW_HH
# define OLN_CORE_GEN_WINDOW_HH

# include <oln/core/internal/window_base.hh>
# include <oln/core/internal/dpoints_impl.hh>
# include <oln/core/gen/dpoints_piter.hh>


namespace oln
{


  // Fwd decl.
  template <typename Dp> class window_;


  // Super type.
  template <typename Dp>
  struct super_trait_< window_<Dp> >
  {
    typedef window_<Dp> current__;
    typedef internal::window_base_<current__> ret;
  };


  /// Virtual types.
  template <typename Dp>
  struct vtypes< window_<Dp> >
  {
    typedef stc_type(Dp, point)       point;
    typedef dpoints_fwd_piter_<point> fwd_qiter;
    typedef dpoints_bkd_piter_<point> bkd_qiter;
  };


  /// Generic classical windoworhood class.

  template <typename Dp>
  class window_ : public internal::window_base_< window_<Dp> >,
		  public internal::dpoints_impl_<Dp>
  {
  public:
    
    window_();

    window_<Dp>& take(const Dp& dp);

    window_<Dp> impl_op_unary_minus_() const;

  }; // end of class oln::window_<Dp>



# ifndef OLN_INCLUDE_ONLY

  template <typename Dp>
  window_<Dp>::window_()
  {
  }

  template <typename Dp>
  window_<Dp>&
  window_<Dp>::take(const Dp& dp)
  {
    this->take_( dp);
    return *this;
  }

  template <typename Dp>
  window_<Dp>
  window_<Dp>::impl_op_unary_minus_() const
  {
    window_<Dp> tmp;
    for (unsigned i = 0; i < this->size(); ++i)
      tmp.take(- this->v_[i]);
    return tmp;
  }

# endif
  

} // end of namespace oln


#endif // ! OLN_CORE_GEN_WINDOW_HH
