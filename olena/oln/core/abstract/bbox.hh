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

#ifndef OLENA_CORE_ABSTRACT_BBOX_HH
# define OLENA_CORE_ABSTRACT_BBOX_HH

# include <mlc/value.hh>
# include <oln/core/type.hh>
# include <oln/core/typedefs.hh>
# include <oln/core/abstract/pset.hh>


namespace oln
{

  // Forward declaration.
  namespace abstract { template <typename E> class bbox; }
  template <typename point> class fwd_piter_;
  template <typename point> class bkd_piter_;


  // Super type declaration.
  template <typename E>
  struct set_super_type< abstract::bbox<E> >
  {
    typedef abstract::pset<E> ret;
  };



  template <typename E>
  struct ext_vtype< abstract::bbox<E>, typedef_::fwd_piter_type >
  {
  private:
    typedef oln_type_of(E, point) P;
  public:
    typedef fwd_piter_<P> ret;
  };

  template <typename E>
  struct ext_vtype< abstract::bbox<E>, typedef_::bkd_piter_type >
  {
  private:
    typedef oln_type_of(E, point) P;
  public:
    typedef bkd_piter_<P> ret;
  };


  namespace abstract
  {

    /// Abstract bbox (bounding box) class.
    template <typename E>
    class bbox : public abstract::pset<E>
    {
      typedef E exact_t;
      typedef oln_type_of(E, point) point_t;
      typedef oln_type_of(E, coord) coord_t;

      typedef oln_type_of(point_t, dim) dim;
      enum { n = mlc_value(dim) };

    public:
      
      unsigned npoints() const
      {
	unsigned count = 1;
	for (unsigned i = 0; i < n; ++i)
	  count *= size(i);
	return count;
      }

      const point_t& pmin() const
      {
	precondition(is_valid_);
	return pmin_;
      }

      coord_t pmin(unsigned i) const
      {
	precondition(is_valid_ and i < n);
	return pmin_[i];
      }

      const point_t& pmax() const
      {
	precondition(is_valid_);
	return pmax_;
      }

      coord_t pmax(unsigned i) const
      {
	precondition(is_valid_ and i < n);
	return pmax_[i];
      }

      unsigned size(unsigned i) const
      {
	precondition(is_valid_ and i < n);
	return pmax_[i] - pmin_[i] + 1;
      }

      void flush()
      {
	is_valid_ = false;
      }

      void init_with(const point_t& p)
      {
	precondition(not is_valid_);
	pmin_ = p;
	pmax_ = p;
	is_valid_ = true;
      }

      void update_with(const point_t& p)
      {
	precondition(is_valid_);
	for (unsigned i = 0; i < n; ++i)
	  if (p[i] < pmin_[i])
	    pmin_[i] = p[i];
	  else if (p[i] > pmax_[i])
	    pmax_[i] = p[i];
      }

      void take(const point_t& p)
      {
	if (not is_valid_)
	  {
	    init_with(p);
	    return;
	  }
	for (unsigned i = 0; i < n; ++i)
	  if (p[i] < pmin_[i])
	    pmin_[i] = p[i];
	  else if (p[i] > pmax_[i])
	    pmax_[i] = p[i];
      }

      // FIXME: Add "update : (rhs : exact)"

      bool has(const point_t& p) const
      {
	precondition(is_valid_);
	for (unsigned i = 0; i < n; ++i)
	  if (p[i] < pmin_[i] or p[i] > pmax_[i])
	    return false;
	return true;
      }

      bool includes(const exact_t& rhs) const
      {
	precondition(is_valid_ and rhs.is_valid());
	for (unsigned i = 0; i < n; ++i)
	  if (rhs.pmin()[i] < pmin_[i] or rhs.pmax()[i] > pmax_[i])
	    return false;
	return true;
      }

      bool is_valid() const
      {
	return is_valid_;
      }

      void print(std::ostream& ostr) const
      {
	ostr << "{ pmin=" << pmin_
	     << ", pmax=" << pmax_
	     << ", valid=" << is_valid_
	     << " }";
      }

      friend
      std::ostream& operator<<(std::ostream& ostr, const abstract::bbox<E>& bb)
      {
	bb.print(ostr);
	return ostr;
      }

      // FIXME: Add the scool code below.

//   invariant {
//     n > 1
//     is_valid => (pmax >= pmin) // FIXME: More.
//   }

//   (=) const : (rhs : exact) -> bool =
//   {
//     precondition(@is_valid and rhs.is_valid)
//     return @pmin = rhs.pmin and @pmax = rhs.pmax
//   }

//   (+=) : (dp : dpoint const ref) -> exact =
//   {
//     precondition(is_valid)
//     pmin += dp
//     pmax += dp
//   }

//   (-=) : (dp : dpoint const ref) -> exact =
//   {
//     precondition(is_valid)
//     pmin -= dp
//     pmax -= dp
//   }

    protected:

      point_t pmin_;
      point_t pmax_;
      bool is_valid_;
      
      bbox()
      {
	flush();
      }

      bbox(const point_t& pmin, const point_t& pmax)
	: pmin_(pmin),
	  pmax_(pmax)
      {
	for (unsigned i = 0; i < n; ++i)
	  precondition(pmax[i] >= pmin[i]);
	is_valid_ = true;
      }

      ~bbox()
      {
	flush();

	mlc::assert_defined_< point_t >::check();

// 	typedef oln_type_of(E, fwd_piter) fwd_piter_t;
// 	typedef oln_type_of(E, bkd_piter) bkd_piter_t;

// 	mlc::assert_defined_< fwd_piter_t >::check();
// 	mlc::assert_defined_< bkd_piter_t >::check();

// 	mlc::assert_< mlc::eq_< oln_type_of(fwd_piter_t, grid),
//                                 oln_type_of(point_t, grid) > >::check();

// 	mlc::assert_< mlc::eq_< oln_type_of(bkd_piter_t, grid),
//                                 oln_type_of(point_t, grid) > >::check();

      }

    }; // end of class oln::abstract::bbox<E>


  } // end of namespace oln::abstract


} // end of namespace oln


#endif // ! OLENA_CORE_ABSTRACT_BBOX_HH
