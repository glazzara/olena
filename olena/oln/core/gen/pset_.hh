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

#ifndef OLN_CORE_GEN_PSET_HH
# define OLN_CORE_GEN_PSET_HH

# include <set>

# include <oln/core/point_set_entry.hh>
# include <oln/core/abstract/point.hh>


namespace oln
{

  // Forward declarations.
  template <typename point_t> class pset_;
  template <typename C> class fwd_piter_on_std_based_pset;
  template <typename C> class bkd_piter_on_std_based_pset;


  // Super type declaration.
  template <typename point_t>
  struct set_super_type< pset_<point_t> >
  {
    typedef pset_<point_t> self_t;
    typedef point_set_entry<self_t> ret;
  };


  /// Virtual types associated to oln::pset_<point_t>.
  template <typename point_t>
  struct vtypes< pset_<point_t> >
  {
    typedef point_t point_type;
    typedef fwd_piter_on_std_based_pset< std::set<point_t> > fwd_piter_type;
    typedef bkd_piter_on_std_based_pset< std::set<point_t> > bkd_piter_type;

    typedef mlc::false_ is_random_accessible_type;
    typedef mlc::true_  has_known_size_type;
    typedef mlc::false_ is_connected_type;
  };


  /// Bounding box class based on a point class.
  template <typename point_t>
  class pset_ : public point_set_entry< pset_<point_t> >,
		private mlc::assert_< mlc_is_a(point_t, abstract::point) >
  {
    typedef pset_<point_t> self_type;
    typedef point_set_entry<self_type> super_t;

  public:
      
    pset_();
    pset_(const pset_<point_t>& rhs);
    pset_<point_t>& operator=(const pset_<point_t>& rhs);

    unsigned impl_npoints() const;
    bool impl_is_valid() const;
    void impl_print(std::ostream& ostr) const;

    pset_<point_t>& insert(const point_t& p);

    const std::set<point_t>& container() const;
    std::set<point_t>& container();

  protected:

    std::set<point_t> sp_;

  }; // end of class oln::pset_<point_t>



# ifndef OLN_INCLUDE_ONLY

  template <typename point_t>
  pset_<point_t>::pset_()
  {
  }

  template <typename point_t>
  pset_<point_t>::pset_(const pset_<point_t>& rhs)
  {
    sp_ = rhs.sp_;
  }

  template <typename point_t>
  pset_<point_t>& pset_<point_t>::operator=(const pset_<point_t>& rhs)
  {
    sp_ = rhs.sp_;
    return *this;
  }
      
  template <typename point_t>
  unsigned pset_<point_t>::impl_npoints() const
  {
    return sp_.size();
  }

  template <typename point_t>
  bool pset_<point_t>::impl_is_valid() const
  {
    return true;
  }

  template <typename point_t>
  void pset_<point_t>::impl_print(std::ostream& ostr) const
  {
    typename std::set<point_t>::const_iterator it;
    ostr << "{ ";
    for (it = sp_.begin(); it != sp_.end(); ++it)
      ostr << *it << ' ';
    ostr << "}";
  }

  template <typename point_t>
  pset_<point_t>& pset_<point_t>::insert(const point_t& p)
  {
    precondition(sp_.find(p) == sp_.end());
    sp_.insert(p);
    return *this;
  }

  template <typename point_t>
  const std::set<point_t>& pset_<point_t>::container() const
  {
    return sp_;
  }

  template <typename point_t>
  std::set<point_t>& pset_<point_t>::container()
  {
    return sp_;
  }

# endif

} // end of namespace oln


# include <oln/core/gen/piter_on_std_based_pset.hh>


#endif // ! OLN_CORE_GEN_PSET_HH
