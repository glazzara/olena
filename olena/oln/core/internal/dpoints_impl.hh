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

#ifndef OLN_CORE_INTERNAL_DPOINTS_IMPL_HH
# define OLN_CORE_INTERNAL_DPOINTS_IMPL_HH

# include <set>
# include <vector>
# include <iterator>
# include <ostream>
# include <oln/core/concept/dpoint.hh>


namespace oln
{

  namespace internal
  {

    /// Implementation for classes storing a set of dpoints.

    template <typename Dp>
    class dpoints_impl_
    {
    public:

      unsigned impl_size() const;
      const Dp& operator[](unsigned i) const;
      const std::vector<Dp>& dpoints() const;
      bool has(const Dp& dp) const;

    protected:

      dpoints_impl_();
      void take_(const Dp& dp);

      std::vector<Dp> v_;

    private:

      void update_();
      std::set<Dp> s_;

    }; // end of class oln::internal::dpoints_impl_<Dp>


    template <typename Dp>
    std::ostream& operator<<(std::ostream& ostr,
			     const dpoints_impl_<Dp>& dps)
    {
      ostr << "[ ";
      unsigned n = dps.impl_size();
      for (unsigned i = 0; i < n; ++i)
	ostr << dps[i] << (i == n - 1 ? " ]" : ", ");
      return ostr;
    }


# ifndef OLN_INCLUDE_ONLY

    // public:

    template <typename Dp>
    unsigned
    dpoints_impl_<Dp>::impl_size() const
    {
      return v_.size();
    }

    template <typename Dp>
    const Dp&
    dpoints_impl_<Dp>::operator[](unsigned i) const
    {
      precondition(i < v_.size());
      return v_[i];
    }

    template <typename Dp>
    const std::vector<Dp>&
    dpoints_impl_<Dp>::dpoints() const
    {
      return v_;
    }

    template <typename Dp>
    bool
    dpoints_impl_<Dp>::has(const Dp& dp) const
    {
      return s_.find(dp) != s_.end();
    }

    // protected:

    template <typename Dp>
    dpoints_impl_<Dp>::dpoints_impl_()
    {
      mlc::assert_< mlc_is_a(Dp, Dpoint) >::check(); // FIXME: Add err msg.
    }

    template <typename Dp>
    void
    dpoints_impl_<Dp>::take_(const Dp& dp)
    {
      s_.insert(dp);
      update_();
    }

    // private:

    template <typename Dp>
    void
    dpoints_impl_<Dp>::update_()
    {
      v_.clear();
      std::copy(s_.begin(), s_.end(), std::back_inserter(v_));
    }

# endif // ! OLN_INCLUDE_ONLY

  } // end of namespace oln::internal

} // end of namespace oln


#endif // ! OLN_CORE_INTERNAL_DPOINTS_IMPL_HH
