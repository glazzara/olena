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
  template <typename point_t> class bbox_;
  template <typename point_t> class bbox_fwd_piter_;
  template <typename point_t> class bbox_bkd_piter_;


  // Super type declaration.
  template <typename point_t>
  struct set_super_type< bbox_<point_t> >
  {
    typedef bbox_<point_t> self_t;
    typedef point_set_entry<self_t> ret;
  };


  /// Virtual types associated to oln::bbox_<point_t>.
  template <typename point_t>
  struct vtypes< bbox_<point_t> >
  {
    typedef point_t point_type;
    typedef bbox_fwd_piter_<point_t> fwd_piter_type;
    typedef bbox_bkd_piter_<point_t> bkd_piter_type;

    typedef bbox_<point_t> bbox_type;

    typedef mlc::true_ is_random_accessible_type;
    typedef mlc::true_ has_know_size_type;
    typedef mlc::true_ is_connected_type;
  };


  /// Bounding box class based on a point class.
  template <typename point_t>
  class bbox_ : public point_set_entry< bbox_<point_t> >,
		private mlc::assert_< mlc_is_a(point_t, abstract::point) >
  {
    // Make self_type public so that is can be used to define a
    // virtual type of oln::topo_bbox_ and oln::topo_lbbox_.
  public:
    typedef bbox_<point_t> self_type;

  private:
    typedef point_set_entry<self_type> super_t;

    typedef oln_type_of(point_t, coord) coord_t;

    typedef oln_type_of(point_t, dim) dim;
    enum { n = mlc_value(dim) };

  public:
      
    bbox_();

    bbox_(const point_t& pmin, const point_t& pmax);

    bbox_(const bbox_<point_t>& rhs);

    bbox_<point_t>& operator=(const bbox_<point_t>& rhs);

    unsigned impl_npoints() const;

    bool impl_has(const point_t& p) const;

    void flush();

    void init_with(const point_t& p);

    void update_with(const point_t& p);

    void take(const point_t& p);

    // FIXME: Add "update : (rhs : exact)"

    bool includes(const bbox_<point_t>& rhs) const;

    bool impl_is_valid() const;

    void impl_print(std::ostream& ostr) const;

  protected:

    bool is_valid_;

  }; // end of class oln::bbox_<point_t>



# ifndef OLN_INCLUDE_ONLY

  template <typename point_t>
  bbox_<point_t>::bbox_()
  {
    flush();
  }

  template <typename point_t>
  bbox_<point_t>::bbox_(const point_t& pmin, const point_t& pmax)
  {
    this->pmin_ = pmin;
    this->pmax_ = pmax;
    is_valid_ = true;

    for (unsigned i = 0; i < n; ++i)
      precondition(pmax[i] >= pmin[i]);
  }

  template <typename point_t>
  bbox_<point_t>::bbox_(const bbox_<point_t>& rhs)
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

  template <typename point_t>
  bbox_<point_t>& bbox_<point_t>::operator=(const bbox_<point_t>& rhs)
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
      
  template <typename point_t>
  unsigned bbox_<point_t>::impl_npoints() const
  {
    unsigned count = 1;
    for (unsigned i = 0; i < n; ++i)
      count *= this->len(i);
    return count;
  }

  template <typename point_t>
  bool bbox_<point_t>::impl_has(const point_t& p) const
  {
    precondition(is_valid_);
    for (unsigned i = 0; i < n; ++i)
      if (p[i] < this->pmin_[i] or p[i] > this->pmax_[i])
	return false;
    return true;
  }

  template <typename point_t>
  void bbox_<point_t>::flush()
  {
    is_valid_ = false;
  }

  template <typename point_t>
  void bbox_<point_t>::init_with(const point_t& p)
  {
    precondition(not is_valid_);
    this->pmin_ = p;
    this->pmax_ = p;
    is_valid_ = true;
  }

  template <typename point_t>
  void bbox_<point_t>::update_with(const point_t& p)
  {
    precondition(is_valid_);
    for (unsigned i = 0; i < n; ++i)
      if (p[i] < this->pmin_[i])
	this->pmin_[i] = p[i];
      else if (p[i] > this->pmax_[i])
	this->pmax_[i] = p[i];
  }

  template <typename point_t>
  void bbox_<point_t>::take(const point_t& p)
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

  template <typename point_t>
  bool bbox_<point_t>::includes(const bbox_<point_t>& rhs) const
  {
    precondition(is_valid_ and rhs.is_valid());
    for (unsigned i = 0; i < n; ++i)
      if (rhs.pmin()[i] < this->pmin_[i] or rhs.pmax()[i] > this->pmax_[i])
	return false;
    return true;
  }

  template <typename point_t>
  bool bbox_<point_t>::impl_is_valid() const
  {
    return is_valid_;
  }

  template <typename point_t>
  void bbox_<point_t>::impl_print(std::ostream& ostr) const
  {
    ostr << "{ pmin=" << this->pmin_
	 << ", pmax=" << this->pmax_
	 << ", valid=" << is_valid_
	 << " }";
  }

# endif


} // end of namespace oln


#endif // ! OLN_CORE_GEN_BBOX_HH
