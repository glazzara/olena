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

#ifndef RLE_PSET_HH_
# define RLE_PSET_HH_



# include <internal/pset_base.hh>
# include <internal/psite_base.hh>

# include <rle_psite.hh>
# include <p_run.hh>

# include <vector>
# include <utility>



namespace mln
{

  // Forward declaration
  template <typename P> struct rle_pset_fwd_piter_;

  template <typename P>
  class rle_pset :
    public internal::pset_base_< rle_psite<P>, rle_pset<P> >
  {
    typedef std::vector< p_run<P> >container;
  public:
    typedef rle_pset_fwd_piter_<P> fwd_piter;
    typedef fwd_piter bkd_piter;
    typedef fwd_piter piter;

    rle_pset();

    /// Test is \p p belongs to this point set.
    bool has(const rle_psite<P>& ps) const;

    typename std::size_t nsites() const;
    void insert(const p_run<P>& pr);


    unsigned nruns() const;
    unsigned range_len_(const P& p) const;

    const p_run<P>& operator[](unsigned i) const;

    const container& con() const;

  protected:

    typename std::size_t nsites_;
    container con_;

  };

# ifndef MLN_INCLUDE_ONLY

  template <typename P>
  inline
  rle_pset<P>::rle_pset() :
    nsites_(0)
  {
  }

  template <typename P>
  inline
  bool
  rle_pset<P>::has(const rle_psite<P>& ps) const
  {
    if (ps.p_of_run() < nruns()
	&& ps.p_in_run() < con_[ps.p_of_run()].length())
      return (ps == con_[ps.p_of_run()][ps.p_in_run()]);
    else
      return false;
  }

  template <typename P>
  inline
  typename std::size_t
  rle_pset<P>::nsites() const
  {
    return nsites_;
  }

  template <typename P>
  inline
  void
  rle_pset<P>::insert(const p_run<P>& pr)
  {
    /// This doesn't deal with recovering pset.
    con_.push_back(pr);
    nsites_ += pr.nsites();
  }

  template <typename P>
  inline
  unsigned
  rle_pset<P>::nruns() const
  {
    return con_.size();
  }

  template <typename P>
  inline
  unsigned
  rle_pset<P>::range_len_(const P& p) const
  {
    unsigned i;
    for (i = 0; i < con_.size(); ++i)
    {
      if (con_[i].first == p)
	return con_[i].nsites();
    }

    assert(i < con_.size());
    return (con_.size());
  }

  template <typename P>
  inline
  const p_run<P>&
  rle_pset<P>::operator[](unsigned i) const
  {
    return con_[i];
  }

  template <typename P>
  const typename rle_pset<P>::container&
  rle_pset<P>::con() const
  {
    return con_;
  }

# endif // ! MLN_INCLUDE_ONLY

  template <typename P, typename E>
  class rle_pset_piter_ : public internal::piter_base_<E, P, rle_psite<P> >
  {
  public:
    typedef P site;
    typedef rle_psite<P> psite;

    const P& to_site() const;

  protected:
    P p_;
    const rle_pset<P>& con_;
    rle_pset_piter_(const rle_pset<P>& pset);
  };


# ifndef MLN_INCLUDE_ONLY

  template <typename P, typename E>
  inline
  rle_pset_piter_<P, E>::rle_pset_piter_(const rle_pset<P>& pset) :
    con_(pset)
  {
  }

  template <typename P, typename E>
  inline
  const P&
  rle_pset_piter_<P, E>::to_site() const
  {
    assert(exact(this)->is_valid());
    return p_;
  }

# endif // ! MLN_INCLUDE_ONLY


  template <typename P>
  class rle_pset_fwd_piter_ :
    public rle_pset_piter_<P, rle_pset_fwd_piter_<P> >
  {
    typedef rle_pset_piter_<P, rle_pset_fwd_piter_<P> > super;
  public:

    rle_pset_fwd_piter_(const rle_pset<P>& pset);

    bool is_valid() const;
    void invalidate();
    void start();
    void next_();

    const rle_psite<P>& to_psite() const;

  protected:
    void update_();

    unsigned i_;
    p_run_fwd_piter_<P> it_;
    rle_psite<P> ps_;
  };


# ifndef MLN_INCLUDE_ONLY

  template <typename P>
  inline
  rle_pset_fwd_piter_<P>::rle_pset_fwd_piter_(const rle_pset<P>& pset) :
    super(pset),
    ps_(pset, 0, 0)
  {
    invalidate();
  }

  template <typename P>
  inline
  bool
  rle_pset_fwd_piter_<P>::is_valid() const
  {
    return i_ < this->con_.nruns();
  }

  template <typename P>
  inline
  void
  rle_pset_fwd_piter_<P>::invalidate()
  {
    i_ = this->con_.nruns();
  }

  template <typename P>
  inline
  void
  rle_pset_fwd_piter_<P>::start()
  {
    i_ = 0;
    it_.assign_run(this->con_[i_]);
    it_.start();
    this->p_ = it_;
    this->update_();
  }

  template <typename P>
  inline
  void
  rle_pset_fwd_piter_<P>::next_()
  {
    assert(this->is_valid());
    it_.next();
    if (!it_.is_valid())
    {
      ++i_;
      if (is_valid())
      {
	it_.assign_run(this->con_[i_]);
	it_.start();
      }
      else
	return;
    }
    this->update_();
  }

  template <typename P>
  void
  rle_pset_fwd_piter_<P>::update_()
  {
    this->p_ = it_;
    ps_.p_in_run() = it_.ind();
    ps_.p_of_run() = i_;
    this->ps_.update_();
  }

  template <typename P>
  const rle_psite<P>&
  rle_pset_fwd_piter_<P>::to_psite() const
  {
    return ps_;
  }

# endif // ! MLN_INCLUDE_ONLY


} // end of namespace mln


#endif // !RLE_PSET_HH_
