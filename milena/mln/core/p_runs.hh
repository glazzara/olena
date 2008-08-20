// Copyright (C) 2007, 2008 EPITA Research and Development Laboratory
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

#ifndef MLN_CORE_P_RUN_SET_HH
# define MLN_CORE_P_RUN_SET_HH

/*! \file mln/core/p_run_set.hh
 *
 * \brief Definition of a set of runs.
 *
 * \todo Zed: Test for unicity (see FIXMEs).
 *
 * \todo Write a p_set_of<S> class with inheritance features (for
 * nsites and bbox).
 */

# include <mln/accu/bbox.hh>
# include <mln/core/p_run.hh>
# include <mln/core/p_double.hh>
# include <mln/core/internal/piter_adaptor.hh>
# include <mln/util/set.hh>



namespace mln
{

  // Forward declaration.
  template <typename P> class p_run_set;


  namespace trait
  {

    template <typename P>
    struct site_set_< p_run_set<P> >
    {
      typedef trait::site_set::nsites::known     nsites;
      typedef trait::site_set::bbox::straight    bbox;
      typedef trait::site_set::contents::growing contents;
      typedef trait::site_set::arity::unique     arity;
    };

  } // end of namespace trait



  /*! \brief p_run_set is a mathematical set of runs.
   *
   * Parameter \c P is the type of the image point.
   */
  template <typename P>
  class p_run_set : public internal::site_set_base_< P, p_run_set<P> >
  {
    typedef p_run_set<P> self_;
    typedef util::set< p_run<P> > set_;
  public:

    /// Element associated type.
    typedef p_run<P> element;


    /// Psite associated type.
    typedef p_double_psite<self_, element> psite;

    /// Forward Site_Iterator associated type.
    typedef p_double_piter<self_,
			   mln_fwd_iter(set_),
			   mln_fwd_piter(p_run<P>)> fwd_piter;

    /// Backward Site_Iterator associated type.
    typedef p_double_piter<self_,
			   mln_bkd_iter(set_),
			   mln_bkd_piter(p_run<P>)> bkd_piter;

    /// Site_Iterator associated type.
    typedef fwd_piter piter;


    /// Constructor without arguments.
    p_run_set();


    /// Test if \p p belongs to this point set.
    bool has(const psite& p) const;

    /// Test if this set of runs is valid.
    bool is_valid() const;


    /// Box associated type.
    typedef const mln::box<P>& q_box;

    /// Give the exact bounding box.
    const box<P>& bbox() const;


    /// Give the number of points.
    unsigned nsites() const;

    /// Give the number of runs.
    unsigned nruns() const;

    /// Give the compression ratio: FIXME: explain...
    float zratio() const;


    /// Insertion element associated type.
    typedef p_run<P> i_element;

    /// Insert a run \p r.
    void insert(const p_run<P>& r);

    /// Insert a run from \p start to \p end.
    void insert(const P& start, const P& end);

    /// Insert a run defined by \p start with length \p len.
    void insert(const P& start, unsigned short len);

    /// Insert another set of runs.
    void insert(const p_run_set<P>& other);


    /// Return the i-th run.
    const p_run<P>& run(unsigned i) const;

    /// Return the size of this site set in memory.
    std::size_t memory_size() const;

    /// Hook to the set of runs.
    const util::set< p_run<P> >& hook_() const;


    // Required by p_double-related classes.
    const util::set< p_run<P> >& set_1_() const;
    template <typename I>
    const p_run<P>& set_2_(const I& it) const;


  protected:

    /// Number of points.
    unsigned nsites_;

    /// Bounding box accumulator.
    accu::bbox<P> b_;

    /// Set of runs.
    util::set< p_run<P> > run_;
  };



  template <typename P>
  std::ostream& operator<<(std::ostream& ostr, const p_run_set<P>& r);


  namespace util
  {

    template <typename P>
    struct less< p_run_set<P> >
    {
      bool operator()(const p_run_set<P>& lhs,
		      const p_run_set<P>& rhs) const;
    };

  } // end of namespace mln::util




# ifndef MLN_INCLUDE_ONLY

  template <typename P>
  inline
  p_run_set<P>::p_run_set()
    :
    nsites_(0)
  {
  }

  template <typename P>
  inline
  bool
  p_run_set<P>::has(const psite&) const
  {
    // FIXME
    return true;
  }

  template <typename P>
  inline
  bool
  p_run_set<P>::is_valid() const
  {
    // FIXME: A run of this set can be invalid...
    return true;
  }

  template <typename P>
  inline
  const box<P>&
  p_run_set<P>::bbox() const
  {
    return b_.to_result();
  }

  template <typename P>
  inline
  unsigned
  p_run_set<P>::nsites() const
  {
    return nsites_;
  }

  template <typename P>
  inline
  void
  p_run_set<P>::insert(const p_run_set<P>& other)
  {
    if (other.nsites() == 0)
      // No-op.
      return;
    nsites_ += other.nsites();
    b_.take(other.bbox());
    run_.insert(other.run_);
  }

  template <typename P>
  inline
  void
  p_run_set<P>::insert(const p_run<P>& r)
  {
    nsites_ += r.nsites();
    b_.take(r.bbox());
    run_.insert(r);
  }


// Previous code of 'insert' was: 
//
// {
//     typename std::vector<p_run<P> >::const_iterator iter = con_.vect().begin();
//     while (iter != con_.vect().end() && iter->first() < pr.first())
//       ++iter;

//     if (iter != con_.vect().begin())
//     {
//       typename std::vector<p_run<P> >::const_iterator prec = iter;
//       --prec;
//       bool equal = true;
//       for (int i = P::dim - 2; i >= 0; --i)
// 	if (!(equal = equal && (prec->first()[i] == pr.first()[i])))
// 	  break;
//       if (equal)
// 	mln_assertion(prec->first()[P::dim - 1] + (signed)prec->length()
// 		      <= pr.first()[P::dim - 1]);
//     }

//     if (iter != con_.vect().end())
//     {
//       bool equal = true;
//       for (int i = P::dim - 2; i >= 0; --i)
// 	if (!(equal = equal && ((*iter).first()[i] == pr.first()[i])))
// 	  break;
//       if (equal)
// 	mln_assertion(pr.first()[P::dim - 1] + (signed)pr.length()
// 		      <= iter->first()[P::dim - 1]);
//     }
//     con_.insert(pr);

//     // update box
//     fb_.take(pr.bbox().pmin());
//     fb_.take(pr.bbox().pmax());
//     // update size
//     npoints_ += pr.npoints();
// }


  template <typename P>
  inline
  void
  p_run_set<P>::insert(const P& start, const P& end)
  {
    mln_precondition(cut_(end) == cut_(start));
    mln_precondition(end.last_coord() >= start.last_coord());
    p_run<P> r(start, end);
    this->insert(r);
  }

  template <typename P>
  inline
  void
  p_run_set<P>::insert(const P& start, unsigned short len)
  {
    mln_precondition(len != 0);
    p_run<P> r(start, len);
    this->insert(r);
  }

  template <typename P>
  inline
  unsigned
  p_run_set<P>::nruns() const
  {
    return run_.nelements();
  }

  template <typename P>
  inline
  const p_run<P>&
  p_run_set<P>::run(unsigned i) const
  {
    mln_precondition(i < nruns());
    return run_[i];
  }

  template <typename P>
  inline
  std::size_t
  p_run_set<P>::memory_size() const
  {
    return run_.memory_size() + sizeof(nsites_) + sizeof(b_);
  }

  template <typename P>
  inline
  float
  p_run_set<P>::zratio() const
  {
    return
      float(memory_size()) /
      float(b_.to_result().nsites() * sizeof(P));
  }

  template <typename P>
  inline
  const util::set< p_run<P> >&
  p_run_set<P>::hook_() const
  {
    return run_;
  }

  template <typename P>
  inline
  const util::set< p_run<P> >&
  p_run_set<P>::set_1_() const
  {
    return run_;
  }

  template <typename P>
  template <typename I>
  inline
  const p_run<P>&
  p_run_set<P>::set_2_(const I& it) const
  {
    return it.element();
  }


  template <typename P>
  std::ostream& operator<<(std::ostream& ostr, const p_run_set<P>& r)
  {
    return ostr << r.hook_();
  }


  namespace util
  {

    template <typename P>
    inline
    bool
    less< p_run_set<P> >::operator()(const p_run_set<P>& lhs,
				     const p_run_set<P>& rhs) const
    {
      return op_less(lhs.run(0), rhs.run(0));
    }

  } // end of namespace mln::util


# endif // ! MLN_INCLUDE_ONLY

} // end of namespace mln


#endif // ! MLN_CORE_P_RUN_SET_HH
