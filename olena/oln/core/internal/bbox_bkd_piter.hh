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

#ifndef OLN_CORE_INTERNAL_BBOX_BKD_PITER_HH
# define OLN_CORE_INTERNAL_BBOX_BKD_PITER_HH

# include <oln/core/abstract/iterator_on_points.hh>
# include <oln/core/abstract/point.hh>
# include <oln/core/gen/bbox.hh>


namespace oln
{


  // Forward declaration.
  namespace internal {
    template <typename E> class bbox_bkd_piter;
  }


  // Super type declaration.
  template <typename E>
  struct set_super_type< internal::bbox_bkd_piter<E> >
  {
    typedef abstract::iterator_on_points<E> ret;
  };



  namespace internal
  {

    /// Internal backward point iterator class; this class factors code.
    template <typename E>
    class bbox_bkd_piter : public abstract::iterator_on_points<E>
    {
      typedef oln_type_of(E, point) point_t;

    public:
    
      bbox_bkd_piter(const bbox_<point_t>& bb)
	: p_(),
	  bb_(bb)
      {
	nop_ = bb_.pmin();
	--nop_[0];
      }

      const bbox_<point_t>& bbox() const
      {
	return bb_;
      }

      void impl_start()
      {
	p_ = bb_.pmax();
	invariant(implies(p_ != nop_, bb_.has(p_)));
      }

      void impl_next()
      {
	invariant(implies(p_ != nop_, bb_.has(p_)));
	for (int i = point_t::n - 1; i >= 0; --i)
	  if (p_[i] == bb_.pmin(i))
	    p_[i] = bb_.pmax(i);
	  else
	    {
	      --p_[i];
	      break;
	    }
	if (p_ == bb_.pmax())
	  p_ = nop_;
      }

      void impl_invalidate()
      {
	invariant(implies(p_ != nop_, bb_.has(p_)));
	p_ = nop_;
      }

      bool impl_is_valid() const
      {
	invariant(implies(p_ != nop_, bb_.has(p_)));
	return p_ != nop_;
      }

      point_t impl_to_point() const
      {
	return p_;
      }

      const point_t* impl_point_adr() const
      {
	return &p_;
      }

    protected:

      point_t p_;
      bbox_<point_t> bb_;
      point_t nop_;

    }; // end of class oln::internal::bbox_bkd_piter<E>


  }  // end of namespace oln::internal

} // end of namespace oln


#endif // ! OLN_CORE_INTERNAL_BBOX_BKD_PITER_HH
