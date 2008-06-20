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

#ifndef MLN_CORE_LINE_PITER_HH
# define MLN_CORE_LINE_PITER_HH

/*! \file mln/core/line_piter.hh
 *
 * \brief Definition of iterators on points by lines.
 *
 * \todo Rename as box_line_piter.
 */

# include <mln/core/internal/site_iterator_base.hh>
# include <mln/core/box.hh>

namespace mln
{

  /*! \brief A generic forward iterator on points by lines.
   *
   * The parameter \c P is the type of points.
   */
  template <typename P>
  class line_piter_ :
    public internal::site_iterator_base_< P, line_piter_<P> >
  {
    typedef line_piter_<P> self_;
    typedef internal::site_iterator_base_< P, self_ > super_;
  public:

    // Make definitions from super class available.
    enum { dim = super_::dim };

    /*! \brief Constructor.
     *
     * \param[in] b A box.
     */
    line_piter_(const box_<P>& b);

    /// Conversion to point.
    operator P() const;

    /// Reference to the corresponding point.
    const P& to_point() const;

    /// Give the i-th coordinate.
    mln_coord(P) operator[](unsigned i) const;

    /// Test the iterator validity.
    bool is_valid() const;

    /// Invalidate the iterator.
    void invalidate();

    /// Start an iteration.
    void start();

    /// Go to the next point.
    void next_();

  private:
    const box_<P>& b_;
    P p_;
    bool is_valid_;
  };


# ifndef MLN_INCLUDE_ONLY


  // line_piter_<P>

  template <typename P>
  inline
  line_piter_<P>::line_piter_(const box_<P>& b)
    : b_(b)
  {
    invalidate();
  }

  template <typename P>
  inline
  line_piter_<P>::operator P() const
  {
    return to_point();
  }

  template <typename P>
  inline
  const P&
  line_piter_<P>::to_point() const
  {
    mln_precondition(is_valid());
    return p_;
  }

  template <typename P>
  inline
  mln_coord(P)
  line_piter_<P>::operator[](unsigned i) const
  {
    mln_precondition(is_valid());
    mln_precondition(i < dim);

    mln_invariant(p_[dim - 1] == b_.pmin()[dim - 1]);
    return p_[i];
  }

  template <typename P>
  inline
  bool
  line_piter_<P>::is_valid() const
  {
    return is_valid_;
  }

  template <typename P>
  inline
  void
  line_piter_<P>::invalidate()
  {
    is_valid_= false;
  }

  template <typename P>
  inline
  void
  line_piter_<P>::start()
  {
    p_ = b_.pmin();
    is_valid_ = true;
  }

  template <typename P>
  inline
  void
  line_piter_<P>::next_()
  {
    mln_precondition(is_valid());

    // Do we want this run for image in 3d?
    for (int c = dim - 2; c >= 0; --c)
    {
      if (p_[c] != b_.pmax()[c])
	++p_[c];
      else
	p_[c] = b_.pmin()[c];
    }
    if (p_ == b_.pmin())
      invalidate();
  }


# endif // ! MLN_INCLUDE_ONLY

} // end of namespace mln


#endif // ! MLN_CORE_LINE_PITER_HH
