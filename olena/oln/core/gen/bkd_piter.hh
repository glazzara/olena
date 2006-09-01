// Copyright (C) 2001, 2003, 2004, 2005, 2006 EPITA Research and
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

#ifndef OLENA_CORE_GEN_BKD_PITER_HH
# define OLENA_CORE_GEN_BKD_PITER_HH

# include <oln/core/abstract/piter.hh>
# include <oln/core/abstract/point.hh>
# include <oln/core/gen/bbox.hh>


namespace oln
{


  // Forward declaration.
  template <typename point> class bkd_piter_;


  // Super type declaration.
  template <typename point>
  struct set_super_type< bkd_piter_<point> >
  {
    typedef bkd_piter_<point> self_t;
    typedef abstract::piter<self_t> ret;
  };


  /// Virtual types associated to oln::bbox_<point>.
  template <typename point>
  struct vtypes_< bkd_piter_<point> >
  {
    typedef point point_type;
    typedef oln_type_of(point, grid) grid_type;
  };



  /// Abstract forward point iterator class.
  template <typename point>
  class bkd_piter_ : public abstract::piter< bkd_piter_<point> >,
		     private mlc::assert_< mlc_is_a(point, abstract::point) >
  {
    typedef bkd_piter_<point> self_t;
    typedef abstract::piter<self_t> super_t;

    using super_t::p_;

  public:
    
    bkd_piter_(const bbox_<point>& bb)
      : bb_(bb)
      {
	nop_ = bb_.pmax();
	++nop_[0];

	psup_ = bb_.pmax().vec();
	for (unsigned i = 0; i < point::n; ++i)
	  ++psup_[i];
      }

    void impl_start()
      {
	p_ = bb_.pmin();
      }

    void impl_next()
      {
	assert(this->is_valid());
	++p_[0];
	unsigned i = 0;
	while (i + 1 < point::n)
	  {
	    if (p_[i] == psup_[i])
	      {
		p_[i] = bb_.pmin_[i];
		++p_[i+1];
	      }
	    ++i;
	  }
      }

//   def next  =
//   {
//     precondition { is_valid }
//   }

    void impl_invalidate()
      {
	assert(this->is_valid());
	p_ = nop_;
      }

    bool impl_is_valid() const
      {
	return p_ != nop_;
      }

  protected:

    bbox_<point> bb_;
    point nop_;
    point psup_;

//   invariant
//   {
//     p <= nop
//   }

//   make : [D : type] (data : D) = { @bb = data.bbox }
    
  }; // end of class oln::bkd_piter_<point>
  

} // end of namespace oln


#endif // ! OLENA_CORE_GEN_BKD_PITER_HH
