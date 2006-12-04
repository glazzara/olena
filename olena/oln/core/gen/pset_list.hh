// Copyright (C) 2006 EPITA Research and Development Laboratory
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

#ifndef OLN_CORE_GEN_PSET_LIST_HH
# define OLN_CORE_GEN_PSET_LIST_HH

# include <list>

# include <oln/core/point_set_entry.hh>
# include <oln/core/abstract/point.hh>


namespace oln
{

  // Forward declarations.
  template <typename point_t> class pset_list;
  template <typename C> class fwd_piter_on_std_based_pset;
  template <typename C> class bkd_piter_on_std_based_pset;


  // Super type declaration.
  template <typename point_t>
  struct set_super_type< pset_list<point_t> >
  {
    typedef pset_list<point_t> self_t;
    typedef point_set_entry<self_t> ret;
  };


  /// Virtual types associated to oln::pset_list<point_t>.
  template <typename point_t>
  struct vtypes< pset_list<point_t> >
  {
    typedef point_t point_type;
    typedef fwd_piter_on_std_based_pset< std::list<point_t> > fwd_piter_type;
    typedef bkd_piter_on_std_based_pset< std::list<point_t> > bkd_piter_type;

    typedef mlc::false_ is_random_accessible_type;
    typedef mlc::true_  has_known_size_type;
    typedef mlc::false_ is_connected_type;
  };


  /// Bounding box class based on a point class.
  template <typename point_t>
  class pset_list : public point_set_entry< pset_list<point_t> >,
		   private mlc::assert_< mlc_is_a(point_t, abstract::point) >
  {
    typedef pset_list<point_t> self_type;
    typedef point_set_entry<self_type> super_t;

  public:
      
    pset_list();
    pset_list(const pset_list<point_t>& rhs);
    pset_list<point_t>& operator=(const pset_list<point_t>& rhs);

    unsigned impl_npoints() const;
    bool impl_is_valid() const;
    void impl_print(std::ostream& ostr) const;

    pset_list<point_t>& append(const point_t& p);
    pset_list<point_t>& prepend(const point_t& p);

    const std::list<point_t>& container() const;
    std::list<point_t>& container();

  protected:

    std::list<point_t> lp_;

  }; // end of class oln::pset_list<point_t>



# ifndef OLN_INCLUDE_ONLY

  template <typename point_t>
  pset_list<point_t>::pset_list()
  {
  }

  template <typename point_t>
  pset_list<point_t>::pset_list(const pset_list<point_t>& rhs)
  {
    lp_ = rhs.lp_;
  }

  template <typename point_t>
  pset_list<point_t>& pset_list<point_t>::operator=(const pset_list<point_t>& rhs)
  {
    lp_ = rhs.lp_;
    return *this;
  }
      
  template <typename point_t>
  unsigned pset_list<point_t>::impl_npoints() const
  {
    return lp_.size();
  }

  template <typename point_t>
  bool pset_list<point_t>::impl_is_valid() const
  {
    return true;
  }

  template <typename point_t>
  void pset_list<point_t>::impl_print(std::ostream& ostr) const
  {
    typename std::list<point_t>::const_iterator it;
    ostr << "{ ";
    for (it = lp_.begin(); it != lp_.end(); ++it)
      ostr << *it << ' ';
    ostr << "}";
  }

  template <typename point_t>
  pset_list<point_t>& pset_list<point_t>::append(const point_t& p)
  {
    precondition(std::find(lp_.begin(), lp_.end(), p)
		 == lp_.end());
    lp_.push_back(p);
    return *this;
  }

  template <typename point_t>
  pset_list<point_t>& pset_list<point_t>::prepend(const point_t& p)
  {
    precondition(std::find(lp_.begin(), lp_.end(), p)
		 == lp_.end());
    lp_.push_front(p);
    return *this;
  }

  template <typename point_t>
  const std::list<point_t>& pset_list<point_t>::container() const
  {
    return lp_;
  }

  template <typename point_t>
  std::list<point_t>& pset_list<point_t>::container()
  {
    return lp_;
  }

# endif

} // end of namespace oln


# include <oln/core/gen/piter_on_std_based_pset.hh>


#endif // ! OLN_CORE_GEN_PSET_LIST_HH
