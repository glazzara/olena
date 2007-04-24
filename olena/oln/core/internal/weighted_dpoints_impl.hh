// Copyright (C) 2007 EPITA Research and Development Laboratory
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

#ifndef OLN_CORE_INTERNAL_WEIGHTED_DPOINTS_IMPL_HH
# define OLN_CORE_INTERNAL_WEIGHTED_DPOINTS_IMPL_HH

# include <set>
# include <vector>
# include <ostream>
# include <oln/core/concept/dpoint.hh>
# include <oln/core/internal/utils.hh>


namespace oln
{

  namespace internal
  {

    template <typename W, typename Dp>
    struct wdp_less_
    {
      bool operator()(const internal::pair<W,Dp>& lhs,
		      const internal::pair<W,Dp>& rhs) const
      {
	return lhs.second < rhs.second;
      }
    };

    /// Implementation for classes storing a set of weighted_dpoints.

    template <typename W, typename Dp>
    class weighted_dpoints_impl_
    {
    public:

      unsigned size() const;

      const W&  w(unsigned i) const;
      const Dp& dp(unsigned i) const;

      const std::vector<W>&  weights() const;
      const std::vector<Dp>& dpoints() const;
      bool has(const Dp& dp) const;

    protected:

      weighted_dpoints_impl_();
      void take_(const W& w, const Dp& dp);

      std::vector<W>  w_;
      std::vector<Dp> dp_;

    private:

      void update_();

      typedef std::set< internal::pair<W,Dp>, wdp_less_<W,Dp> > s_wdp_t;

      std::set<Dp> s_dp_;
      s_wdp_t      s_wdp_;

    }; // end of class oln::internal::weighted_dpoints_impl_<W,Dp>


    template <typename W, typename Dp>
    std::ostream& operator<<(std::ostream& ostr,
			     const weighted_dpoints_impl_<W,Dp>& wdps)
    {
      ostr << "[ ";
      unsigned n = wdps.size();
      for (unsigned i = 0; i < n; ++i)
	ostr << '{' << wdps.w(i) << ", " << wdps.dp(i) << (i == n - 1 ? "} ]" : "}, ");
      return ostr;
    }


# ifndef OLN_INCLUDE_ONLY

    // public:

    template <typename W, typename Dp>
    unsigned
    weighted_dpoints_impl_<W,Dp>::size() const
    {
      invariant(dp_.size() == w_.size());
      return dp_.size();
    }

    template <typename W, typename Dp>
    const W&
    weighted_dpoints_impl_<W,Dp>::w(unsigned i) const
    {
      precondition(i < w_.size());
      return w_[i];
    }

    template <typename W, typename Dp>
    const Dp&
    weighted_dpoints_impl_<W,Dp>::dp(unsigned i) const
    {
      precondition(i < dp_.size());
      return dp_[i];
    }

    template <typename W, typename Dp>
    const std::vector<W>&
    weighted_dpoints_impl_<W,Dp>::weights() const
    {
      return w_;
    }

    template <typename W, typename Dp>
    const std::vector<Dp>&
    weighted_dpoints_impl_<W,Dp>::dpoints() const
    {
      return dp_;
    }

    template <typename W, typename Dp>
    bool
    weighted_dpoints_impl_<W,Dp>::has(const Dp& dp) const
    {
      return s_dp_.find(dp) != s_dp_.end();
    }

    // protected:

    template <typename W, typename Dp>
    weighted_dpoints_impl_<W,Dp>::weighted_dpoints_impl_()
    {
      mlc::assert_< mlc_is_a(Dp, Dpoint) >::check(); // FIXME: Add err msg.
    }

    template <typename W, typename Dp>
    void
    weighted_dpoints_impl_<W,Dp>::take_(const W& w, const Dp& dp)
    {
      precondition(not this->has(dp));
      s_dp_.insert(dp);
      internal::pair<W,Dp> tmp(w, dp);
      s_wdp_.insert(tmp);
      update_();
    }

    // private:

    template <typename W, typename Dp>
    void
    weighted_dpoints_impl_<W,Dp>::update_()
    {
      w_.clear();
      dp_.clear();
      typename s_wdp_t::const_iterator i;
      for (i = s_wdp_.begin(); i != s_wdp_.end(); ++i)
	{
	  w_ .push_back(i->first);
	  dp_.push_back(i->second);
	}
    }

# endif // ! OLN_INCLUDE_ONLY

  } // end of namespace oln::internal

} // end of namespace oln


#endif // ! OLN_CORE_INTERNAL_WEIGHTED_DPOINTS_IMPL_HH
