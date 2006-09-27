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

#ifndef OLN_CORE_GEN_BBOX_HH
# define OLN_CORE_GEN_BBOX_HH

# include <oln/core/point_set_entry.hh>
# include <oln/core/abstract/point.hh>


namespace oln
{

  // Forward declarations.
  template <typename point> class bbox_;
  template <typename point> class bbox_fwd_piter_;
  template <typename point> class bbox_bkd_piter_;


  // Super type declaration.
  template <typename point>
  struct set_super_type< bbox_<point> >
  {
    typedef bbox_<point> self_t;
    typedef point_set_entry<self_t> ret;
  };


  /// Virtual types associated to oln::bbox_<point>.
  template <typename point>
  struct vtypes< bbox_<point> >
  {
    typedef point point_type;
    typedef bbox_fwd_piter_<point> fwd_piter_type;
    typedef bbox_bkd_piter_<point> bkd_piter_type;

    typedef bbox_<point> bbox_type;

    typedef mlc::true_ is_random_accessible_type;
    typedef mlc::true_ has_know_size_type;
    typedef mlc::true_ is_connected_type;
  };


  /// Bounding box class based on a point class.
  template <typename point>
  class bbox_ : public point_set_entry< bbox_<point> >,
		private mlc::assert_< mlc_is_a(point, abstract::point) >
  {
    typedef point point_t;
    typedef bbox_<point> self_t;
    typedef point_set_entry<self_t> super_t;

    typedef oln_type_of(point, coord) coord_t;

    typedef oln_type_of(point_t, dim) dim;
    enum { n = mlc_value(dim) };

  public:
      
    bbox_()
      {
	flush();
      }

    bbox_(const point_t& pmin, const point_t& pmax)
      {
	this->pmin_ = pmin;
	this->pmax_ = pmax;
	is_valid_ = true;

	for (unsigned i = 0; i < n; ++i)
	  precondition(pmax[i] >= pmin[i]);
      }

    bbox_(const self_t& rhs)
      {
	// FIXME: Remove these 3 lines?
	precondition(rhs.is_valid());
	for (unsigned i = 0; i < n; ++i)
	  precondition(rhs.pmax_[i] >= rhs.pmin_[i]);

	this->pmin_ = rhs.pmin_;
	this->pmax_ = rhs.pmax_;
        is_valid_ = rhs.is_valid_;

	for (unsigned i = 0; i < n; ++i)
	  postcondition(this->pmax_[i] >= this->pmin_[i]);
      }

    self_t& operator=(const self_t& rhs)
      {
	// FIXME: Remove these 3 lines?
	precondition(rhs.is_valid());
	for (unsigned i = 0; i < n; ++i)
	  precondition(rhs.pmax_[i] >= rhs.pmin_[i]);

	this->pmin_ = rhs.pmin_;
	this->pmax_ = rhs.pmax_;
        is_valid_ = rhs.is_valid_;

	for (unsigned i = 0; i < n; ++i)
	  postcondition(this->pmax_[i] >= this->pmin_[i]);

	return *this;
      }
      
    unsigned impl_npoints() const
      {
	unsigned count = 1;
	for (unsigned i = 0; i < n; ++i)
	  count *= this->len(i);
	return count;
      }

    bool impl_has(const point_t& p) const
      {
	precondition(is_valid_);
	for (unsigned i = 0; i < n; ++i)
	  if (p[i] < this->pmin_[i] or p[i] > this->pmax_[i])
	    return false;
	return true;
      }

    void flush()
      {
	is_valid_ = false;
      }

    void init_with(const point_t& p)
      {
	precondition(not is_valid_);
	this->pmin_ = p;
	this->pmax_ = p;
	is_valid_ = true;
      }

    void update_with(const point_t& p)
      {
	precondition(is_valid_);
	for (unsigned i = 0; i < n; ++i)
	  if (p[i] < this->pmin_[i])
	    this->pmin_[i] = p[i];
	  else if (p[i] > this->pmax_[i])
	    this->pmax_[i] = p[i];
      }

    void take(const point_t& p)
      {
	if (not is_valid_)
	  {
	    init_with(p);
	    return;
	  }
	for (unsigned i = 0; i < n; ++i)
	  if (p[i] < this->pmin_[i])
	    this->pmin_[i] = p[i];
	  else if (p[i] > this->pmax_[i])
	    this->pmax_[i] = p[i];
      }

    // FIXME: Add "update : (rhs : exact)"

    bool includes(const self_t& rhs) const
      {
	precondition(is_valid_ and rhs.is_valid());
	for (unsigned i = 0; i < n; ++i)
	  if (rhs.pmin()[i] < this->pmin_[i] or rhs.pmax()[i] > this->pmax_[i])
	    return false;
	return true;
      }

    bool impl_is_valid() const
      {
	return is_valid_;
      }

    void impl_print(std::ostream& ostr) const
      {
	ostr << "{ pmin=" << this->pmin_
	     << ", pmax=" << this->pmax_
	     << ", valid=" << is_valid_
	     << " }";
      }

  protected:

    bool is_valid_;

  }; // end of class oln::bbox_<point>


} // end of namespace oln


# include <oln/core/gen/bbox_fwd_piter.hh>
# include <oln/core/gen/bbox_bkd_piter.hh>


#endif // ! OLN_CORE_GEN_BBOX_HH
