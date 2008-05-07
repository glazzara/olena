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

#ifndef MLN_CORE_PSET_ARRAY_HH_
# define MLN_CORE_PSET_ARRAY_HH_


/*! \file mln/core/pset_array.hh
 *
 * \brief Definition of the pset array class.
 */

# include <mln/core/internal/point_set_base.hh>
# include <mln/core/internal/point_iterator_base.hh>

# include <vector>
# include <mln/accu/bbox.hh>
# include <mln/core/pset_array_psite.hh>




namespace mln
{

  // Forward declaration
  template <typename Pset> struct pset_array_fwd_piter_;


  /*! \brief Point Set Class based on a collection of Point Set
  **         This class is based on a std::vector.
  **         This class is parametrized by \p Pset which is the Point Set
  **         type of the collection.
  **
  **   The iterator type associated  to Pset must satisfy some constraints:
  **     * Iterator type must have a constructor which takes no argument
  **     * Iterator type must have a method
  **              void assign(const Pset&)
  **       which change the Pset associated to the iterator
  **
  **
  ** FIXME: Add a backward iterator.
  */
  template <typename Pset>
  class pset_array:
    public internal::point_set_base_<pset_array_psite<Pset>, pset_array<Pset> >
  {
  public:

    // Container
    typedef std::vector<Pset> container;

    typedef pset_array_fwd_piter_<Pset> fwd_piter;
    typedef pset_array_fwd_piter_<Pset> bkd_piter;

    typedef pset_array_psite<typename Pset::psite> psite;
    typedef typename Pset::point point;

    /// Constructor without arguments.
    pset_array();

    /// Test is \p p belongs to this point set.
    bool has(const psite& ps) const;

    /// Give the exact bounding box.
    const box_<point>& bbox() const;

    /// Give the number of points.
    typename std::size_t npoints() const;

    /// Give the number of pset
    typename std::size_t npsets() const;

    /// Insert a new Point Set in the Point Sets Collections
    void insert(const Pset& ps);

    /// Return the i-th pset of the Point Sets Colletion
    const Pset& operator[](unsigned i) const;
    Pset& operator[](unsigned i);

  protected:

    /// Number of points.
    typename std::size_t npoints_;

    /// Points container
    container con_;

    /// Exact bounding box.
    accu::bbox<point> fb_;

  };

# ifndef MLN_INCLUDE_ONLY

  template <typename Pset>
  inline
  pset_array<Pset>::pset_array() :
    npoints_(0)
  {
  }

  template <typename Pset>
  inline
  bool
  pset_array<Pset>::has(const typename pset_array<Pset>::psite& ps) const
  {
    return ps.to_index() < con_.size() &&
      con_[ps.to_index()].has(ps.to_psite());
  }

  template <typename Pset>
  inline
  const box_<typename pset_array<Pset>::point>&
  pset_array<Pset>::bbox() const
  {
    return fb_.to_result();
  }

  template <typename Pset>
  inline
  typename std::size_t
  pset_array<Pset>::npoints() const
  {
    return npoints_;
  }

  template <typename Pset>
  inline
  typename std::size_t
  pset_array<Pset>::npsets() const
  {
    return con_.size();
  }

  template <typename Pset>
  inline
  void
  pset_array<Pset>::insert(const Pset& ps)
  {
    // update box
    fb_.take(ps.bbox().pmin());
    fb_.take(ps.bbox().pmax());

    // update size
    npoints_ += ps.npoints();

    // update the container
    con_.push_back(ps);
  }

  template <typename Pset>
  inline
  const Pset&
  pset_array<Pset>::operator[](unsigned i) const
  {
    return con_[i];
  }

  template <typename Pset>
  inline
  Pset&
  pset_array<Pset>::operator[](unsigned i)
  {
    return con_[i];
  }

# endif // ! MLN_INCLUDE_ONLY


  /*! \brief Forward Iterator on a Pset array.
  **  \p Pset is the type of Point Set in the Point Sets collection.
  **
  */
  template <typename Pset>
  class pset_array_fwd_piter_ : public
  internal::point_iterator_base_<pset_array_psite<typename Pset::psite>,
				 pset_array_fwd_piter_<Pset> >
  {
  public:

    /// Access types
    typedef pset_array_psite<typename Pset::psite> psite;
    typedef typename Pset::point point;

    /// Constructor
    pset_array_fwd_piter_(const pset_array<Pset>& pset);

    /// Test the iterator validity.
    bool is_valid() const;

    /// Invalidate the iterator.
    void invalidate();

    /// Start an iteration.
    void start();

    /// Go to the next point.
    void next_();


    /// Convertion into a point.
    operator point () const;

    /// Reference to the corresponding point.
    const point& to_point() const;

    /// Convertion into a point-site.
    operator psite () const;

    /// Access to the current point coordinates.
    mln_coord(point) operator[](unsigned i) const;

  protected:

    /// Change the Point Set associated with the wrapped piter.
    void assign_piter_();

  private:

    typedef typename Pset::fwd_piter pset_fwd_piter_;

    const pset_array<Pset>* pset_;
    pset_fwd_piter_ piter_;
    unsigned pos_;
  };

# ifndef MLN_INCLUDE_ONLY

  template <typename Pset>
  pset_array_fwd_piter_<Pset>::pset_array_fwd_piter_(const pset_array<Pset>& pset) :
    pset_(&pset),
    piter_(),
    pos_(pset_->npsets())
  {
    invalidate();
  }

  template <typename Pset>
  inline
  bool
  pset_array_fwd_piter_<Pset>::is_valid() const
  {
    return pos_ < pset_->npsets();
  }

  template <typename Pset>
  inline
  void
  pset_array_fwd_piter_<Pset>::invalidate()
  {
    pos_ = pset_->npsets();
  }

  template <typename Pset>
  inline
  void
  pset_array_fwd_piter_<Pset>::assign_piter_()
  {
    piter_.assign((*pset_)[pos_]);
    piter_.start();
  }

  template <typename Pset>
  inline
  void
  pset_array_fwd_piter_<Pset>::start()
  {
    mln_precondition(pset_->npsets() > 0);

    pos_ = 0;
    assign_piter_();
  }

  template <typename Pset>
  inline
  void
  pset_array_fwd_piter_<Pset>::next_()
  {
    mln_precondition(is_valid());

    piter_.next();
    if (!piter_.is_valid())
    {
      ++pos_;
      if (is_valid())
	assign_piter_();
      else
	piter_.invalidate();
    }
  }

  template <typename Pset>
  pset_array_fwd_piter_<Pset>::operator
  typename pset_array_fwd_piter_<Pset>::point() const
  {
    return to_point();
  }

  template <typename Pset>
  const typename pset_array_fwd_piter_<Pset>::point&
  pset_array_fwd_piter_<Pset>::to_point() const
  {
    mln_precondition(is_valid());
    mln_precondition(piter_.is_valid());

    return piter_.to_point();
  }

  template <typename Pset>
  pset_array_fwd_piter_<Pset>::operator
  typename pset_array_fwd_piter_<Pset>::psite ()
    const
  {
    mln_precondition(is_valid());

    return pset_array_psite<typename Pset::psite>(piter_, pos_);
  }

  template <typename Pset>
  mln_coord_(typename pset_array_fwd_piter_<Pset>::point)
  pset_array_fwd_piter_<Pset>::operator[](unsigned i) const
  {
    mln_precondition(is_valid());

    return piter_[i];
  }


# endif // ! MLN_INCLUDE_ONLY


} // end of namespace mln


#endif // ! MLN_CORE_PSET_ARRAY_HH_
