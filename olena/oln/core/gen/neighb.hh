// Copyright (C) 2001, 2003, 2004, 2005, 2006, 2007 EPITA Research and
// Development Laboratory
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

#ifndef OLN_CORE_GEN_NEIGHB_HH
# define OLN_CORE_GEN_NEIGHB_HH

# include <oln/core/internal/dpoints_impl.hh>
# include <oln/core/internal/neighborhood_base.hh>
# include <oln/core/internal/op_image_plus_nbh.hh>


namespace oln
{


  // Op.
  oln_decl_op_plus(Image, Neighborhood);



  // Fwd decl.
  template <typename Dp> class neighb_;


  // Super type.
  template <typename Dp>
  struct super_trait_< neighb_<Dp> >
  {
    typedef neighb_<Dp> current__;
    typedef internal::neighborhood_base_<current__> ret;
  };


  /// Virtual types.
  template <typename Dp>
  struct vtypes< neighb_<Dp> >
  {
    typedef stc_type(Dp, point) point;
  };


  /// Generic classical neighborhood class.

  template <typename Dp>
  class neighb_ : public internal::neighborhood_base_< neighb_<Dp> >,
		  public internal::dpoints_impl_<Dp>
  {
  public:
    
    neighb_();

    neighb_<Dp>& take(const Dp& dp);

  }; // end of class oln::neighb_<Dp>



# ifndef OLN_INCLUDE_ONLY

  template <typename Dp>
  neighb_<Dp>::neighb_()
  {
  }

  template <typename Dp>
  neighb_<Dp>& neighb_<Dp>::take(const Dp& dp)
  {
    this->take_( dp);
    this->take_(-dp);
    return *this;
  }

# endif
  

} // end of namespace oln


#endif // ! OLN_CORE_GEN_NEIGHB_HH
