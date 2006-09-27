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

#ifndef OLN_CORE_GEN_NEIGHB_HH
# define OLN_CORE_GEN_NEIGHB_HH

# include <set>
# include <vector>
# include <oln/core/neighborhood_entry.hh>


namespace oln
{


  // Forward declaration.
  template <typename dpoint> class neighb_;


  // Super type declaration.
  template <typename dpoint>
  struct set_super_type< neighb_<dpoint> >
  {
    typedef neighb_<dpoint> self_t;
    typedef neighborhood_entry<self_t> ret;
  };


  /// Virtual types associated to oln::neighb_<dpoint>.
  template <typename dpoint>
  struct vtypes< neighb_<dpoint> >
  {
    typedef oln_type_of(dpoint, grid) grid_type;
  };



  /// Abstract forward dpoint iterator class.
  template <typename dpoint>
  class neighb_ : public neighborhood_entry< neighb_<dpoint> >,
		  private mlc::assert_< mlc_is_a(dpoint, abstract::dpoint) >
  {
    typedef neighb_<dpoint> self_t;
    typedef neighborhood_entry<self_t> super_t;

  public:
    
    neighb_()
    {
    }

    self_t& add(const dpoint& dp)
    {
      s_.insert(dp);
      s_.insert(-dp);
      update_();
      return *this;
    }

    template <typename D>
    self_t& add(const abstract::dpoint<D>& dp)
    {
      return this->add(dp.exact());
    }

    unsigned card() const
    {
      return v_.size();
    }

    dpoint dp(unsigned i) const
    {
      precondition(i < v_.size());
      return v_[i];
    }

    // void print(std::ostream& ostr) const;
    // friend std::ostream& operator<<(std::ostream& ostr, const neighb_<dpoint>& nbh);

  protected:

    std::set<dpoint>    s_;
    std::vector<dpoint> v_;

    void update_()
    {
      v_.clear();
      std::copy(s_.begin(), s_.end(),
		std::back_inserter(v_));
    }

  }; // end of class oln::neighb_<dpoint>
  

} // end of namespace oln


#endif // ! OLN_CORE_GEN_NEIGHB_HH
