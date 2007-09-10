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
 */

# include <mln/core/concept/point_iterator.hh>


namespace mln
{

  /*! \brief A generic forward iterator on points by lines.
   *
   * The parameter \c P is the type of points.
   */
  template <typename P>
  class line_piter_ : public Point_Iterator< line_piter_<P> >
  {
  public:

    /// Space dimension.
    enum { dim = P::dim };

    /// Point_Site associated type.
    typedef P psite;

    /// Point associated type.
    typedef P point;

    /// Dpoint associated type.
    typedef mln_dpoint(P) dpoint;

    /// Coordinate associated type.
    typedef mln_coord(P) coord;

    /*! \brief Constructor.
     *
     * \param[in] b A box.
     */
    line_piter_(const box_<P>& b);

    /// Convertion to point.
    operator P() const;

    /// Address of the point.
    const P* pointer_() const;

    /// Give the i-th coordinate.
    coord operator[](unsigned i) const;

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
    P p_, nop_;
  };


# ifndef MLN_INCLUDE_ONLY


  // line_piter_<P>

  template <typename P>
  line_piter_<P>::line_piter_(const box_<P>& b)
    : b_(b)
  {
    nop_ = b_.pmax();
    if (dim == 1)
      ++nop_[0];
    else
    {
      nop_[0] = 0;
      ++nop_[1];
    }
    invalidate();
  }

  template <typename P>
  line_piter_<P>::operator P() const
  {
    return p_;
  }

  template <typename P>
  const P*
  line_piter_<P>::pointer_() const
  {
    return & p_;
  }

  template <typename P>
  mln_coord(P)
  line_piter_<P>::operator[](unsigned i) const
  {
    mln_invariant(p_[0] == 0);
    assert(i < dim);
    return p_[i];
  }

  template <typename P>
  bool
  line_piter_<P>::is_valid() const
  {
    return p_ != nop_;
  }

  template <typename P>
  void
  line_piter_<P>::invalidate()
  {
    p_ = nop_;
  }

  template <typename P>
  void
  line_piter_<P>::start()
  {
    p_ = b_.pmin();
  }

  template <typename P>
  void
  line_piter_<P>::next_()
  {
    for (int c = 1; c < dim; ++c)
    {
      if (p_[c] != b_.pmax()[c])
      {
	++p_[c];
	break;
      }
      p_[c] = b_.pmin()[c];
    }
    if (p_ == b_.pmin())
      p_ = nop_;
  }
  

# endif // ! MLN_INCLUDE_ONLY

} // end of namespace mln


#endif // ! MLN_CORE_LINE_PITER_HH
