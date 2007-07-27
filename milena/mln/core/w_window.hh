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

#ifndef MLN_CORE_W_WINDOW_HH
# define MLN_CORE_W_WINDOW_HH

/*! \file mln/core/w_window.hh
 *
 * \brief Definition of the generic weighted window class mln::w_window_.
 */

# include <map>

# include <mln/core/concept/weighted_window.hh>
# include <mln/core/window.hh>


namespace mln
{

  // Fwd decl.
  template <typename It, typename W> struct with_w_;



  /*! \brief Generic w_window class.
   *
   * This type of w_window is just like a set of delta-points.  The
   * parameter \c D is the type of delta-points; the parameter \c W is
   * the type of weights.
   */
  template <typename D, typename W>
  struct w_window_ : public Weighted_Window< w_window_<D,W> >
  {
    /// Point associated type.
    typedef mln_point(D) point;

    /// Dpoint associated type.
    typedef D dpoint;

    /// Point_Iterator type to browse the points of a generic w_window.
    typedef with_w_< dpoints_fwd_piter<D>, W > fwd_qiter;

    /// Point_Iterator type to browse the points of a generic w_window.
    typedef with_w_< dpoints_bkd_piter<D>, W > bkd_qiter;


    /// Constructor without argument.
    w_window_();

    /// Give the corresponding window.
    const window_<D>& window() const;

    /// Give the symmetrical w_window.
    w_window_<D,W> sym_() const;

    /// Give the \p i-th weight.
    W weight(unsigned i) const;

    /// Give access to the vector of weights.
    const std::vector<W>& weights() const;

    /// Give access to the vector of delta-points.
    const std::vector<D>& vect() const;

    /// Insert a delta-point \p d and its associated weight \p w.
    w_window_<D,W>& insert(const D& d, const W& w);

    /// Give the number of elements (pairs of delta-point/weight).
    unsigned nelements() const;

    // FIXME: Rename as dpoint(i); same in mln/core/dpoints_pixter.hh:174.
    const D& element(unsigned i) const;

  protected:
    
    window_<D>     win_;
    std::vector<W> wei_;
  };


  /// Print a weighted window \p wwin into an output stream \p ostr.
  template <typename D, typename W>
  std::ostream& operator<<(std::ostream& ostr, const w_window_<D,W>& wwin);



  /// Decorator to handle weights for iterators on window points.
  template <typename It, typename W>
  struct with_w_ : public It
  {

    template <typename Ds, typename P>
    with_w_(const Ds& ds, const P& p);

    W weight() const;

  protected:
    const std::vector<W> wei_;
  };



# ifndef MLN_INCLUDE_ONLY


  // with_w_<It,W>

  template <typename It, typename W>
  template <typename Ds, typename P>
  with_w_<It,W>::with_w_(const Ds& ds,
			 const P&  p)
    : It(ds, p),
      wei_(ds.weights())
  {
  }
  
  template <typename It, typename W>
  W
  with_w_<It,W>::weight() const
  {
    mln_precondition(this->i_ < wei_.size());
    return wei_[this->i_];
  }


  // w_window_<D,W>

  template <typename D, typename W>
  std::ostream& operator<<(std::ostream& ostr, const w_window_<D,W>& wwin)
  {
    ostr << '[';
    for (unsigned i = 0; i < wwin.window().nelements(); ++i)
      ostr << wwin.vect()[i] << ':' << wwin.weight(i) << ' ';
    return ostr << ']';
  }

  template <typename D, typename W>
  w_window_<D,W>::w_window_()
  {
  }

  template <typename D, typename W>
  const window_<D>&
  w_window_<D,W>::window() const
  {
    return win_;
  }

  template <typename D, typename W>
  const std::vector<D>&
  w_window_<D,W>::vect() const
  {
    return win_.vect();
  }

  template <typename D, typename W>
  const std::vector<W>&
  w_window_<D,W>::weights() const
  {
    return wei_;
  }

  template <typename D, typename W>
  unsigned
  w_window_<D,W>::nelements() const
  {
    mln_invariant(wei_.size() == win_.nelements());
    return win_.nelements();
  }

  template <typename D, typename W>
  const D&
  w_window_<D,W>::element(unsigned i) const
  {
    mln_precondition(i < wei_.size());
    mln_invariant(wei_.size() == win_.nelements());
    return win_.element(i);
  }

  template <typename D, typename W>
  W
  w_window_<D,W>::weight(unsigned i) const
  {
    mln_precondition(i < wei_.size());
    mln_invariant(wei_.size() == win_.nelements());
    return wei_[i];
  }

  template <typename D, typename W>
  w_window_<D,W>&
  w_window_<D,W>::insert(const D& d, const W& w)
  {
    if (win_.has(d))
      // no-op
      return *this;

    // memorization: d_i -> w_i
    std::map<D, W> memo_wei_;
    for (unsigned i = 0; i < win_.nelements(); ++i)
      memo_wei_[win_.element(i)] = wei_[i];

    // insertion of d and w:
    win_.insert(d);
    memo_wei_[d] = w;

    // re-mapping: w_i <- d_i
    wei_.resize(win_.nelements());
    for (unsigned i = 0; i < win_.nelements(); ++i)
      wei_[i] = memo_wei_[win_.element(i)];

    return *this;
  }


  template <typename D, typename W>
  w_window_<D,W>
  w_window_<D,W>::sym_() const
  {
    // FIXME
    return *this;
  }

# endif // ! MLN_INCLUDE_ONLY

} // end of namespace mln


#endif // ! MLN_CORE_W_WINDOW_HH
