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
 * \brief Definition of the generic weighted window class mln::w_window.
 */

# include <map>

# include <mln/core/window.hh>
# include <mln/core/concept/weighted_window.hh>
# include <mln/core/dpoints_piter.hh>

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
  struct w_window : public Weighted_Window< w_window<D,W> >
  {
    /// Point associated type.
    typedef mln_point(D) point;

    /// Dpoint associated type.
    typedef D dpoint;

    /// Weight associated type.
    typedef W weight;

    /// Window associated type.
    typedef mln::window<D> window;


    /// Site_Iterator type to browse (forward) the points of a generic w_window.
    typedef with_w_< dpoints_fwd_piter<D>, W > fwd_qiter;

    /// Site_Iterator type to browse (backward) the points of a generic w_window.
    typedef with_w_< dpoints_bkd_piter<D>, W > bkd_qiter;


    /// Constructor without argument.
    w_window();


    /// Insert a couple of weight \p w and delta-point \p d.
    w_window<D,W>& insert(const W& w, const D& d);


    /// Give the \p i-th weight.
    W w(unsigned i) const;

    /// Give access to the vector of weights.
    const std::vector<W>& weights() const;


    // Give the \p i-th delta-point.
    const D& dp(unsigned i) const;

    /// Give the number of delta-points.
    unsigned ndpoints() const;

    /// Give access to the vector of delta-points.
    const std::vector<D>& vect() const;

    /// Give the corresponding window.
    const mln::window<D>& win() const;


    /// Apply a central symmetry to the target window.
    w_window<D,W>& sym();

  protected:
    
    mln::window<D> win_;
    std::vector<W> wei_;
  };


  /* \brief Print a weighted window \p w_win into an output stream \p ostr.
   * \relates mln::w_window
   */
  template <typename D, typename W>
  std::ostream& operator<<(std::ostream& ostr, const w_window<D,W>& w_win);


  /* \brief Equality test between two weighted windows \p lhs and \p rhs.
   * \relates mln::w_window
   */
  template <typename D, typename Wl, typename Wr>
  bool operator==(const w_window<D,Wl>& lhs, const w_window<D,Wr>& rhs);



  /// Decorator to handle weights for iterators on window points.
  template <typename It, typename W>
  struct with_w_ : public It
  {

    template <typename Ds, typename P>
    with_w_(const Ds& ds, const P& p);

    W w() const;

  protected:
    const std::vector<W> wei_;
  };



# ifndef MLN_INCLUDE_ONLY


  // with_w_<It,W>

  template <typename It, typename W>
  template <typename Ds, typename P>
  inline
  with_w_<It,W>::with_w_(const Ds& ds,
			 const P&  p)
    : It(ds, p),
      wei_(ds.weights())
  {
  }
  
  template <typename It, typename W>
  inline
  W
  with_w_<It,W>::w() const
  {
    mln_precondition(this->i_ < wei_.size());
    return wei_[this->i_];
  }


  // w_window<D,W>

  template <typename D, typename W>
  inline
  w_window<D,W>::w_window()
  {
  }

  template <typename D, typename W>
  inline
  const mln::window<D>&
  w_window<D,W>::win() const
  {
    return win_;
  }

  template <typename D, typename W>
  inline
  const D&
  w_window<D,W>::dp(unsigned i) const
  {
    mln_precondition(i < win_.ndpoints());
    mln_invariant(wei_.size() == win_.ndpoints());
    return win_.dp(i);
  }

  template <typename D, typename W>
  inline
  unsigned
  w_window<D,W>::ndpoints() const
  {
    mln_invariant(wei_.size() == win_.ndpoints());
    return win_.ndpoints();
  }

  template <typename D, typename W>
  inline
  const std::vector<D>&
  w_window<D,W>::vect() const
  {
    return win_.vect();
  }

  template <typename D, typename W>
  inline
  const std::vector<W>&
  w_window<D,W>::weights() const
  {
    return wei_;
  }

  template <typename D, typename W>
  inline
  W
  w_window<D,W>::w(unsigned i) const
  {
    mln_precondition(i < wei_.size());
    mln_invariant(wei_.size() == win_.ndpoints());
    return wei_[i];
  }

  template <typename D, typename W>
  inline
  w_window<D,W>&
  w_window<D,W>::insert(const W& w, const D& d)
  {
    mln_invariant(wei_.size() == win_.ndpoints());
    mln_precondition(! win_.has(d));

    if (w == W(0)) // FIXME: Implicit restriction "W scalar"...
      // no-op
      return *this;

    // memorization: d_i -> w_i
    std::map<D, W> memo_wei_;
    for (unsigned i = 0; i < win_.ndpoints(); ++i)
      memo_wei_[win_.dp(i)] = wei_[i];

    // insertion of d and w:
    win_.insert(d);
    memo_wei_[d] = w;

    // re-mapping: w_i <- d_i
    wei_.resize(win_.ndpoints());
    for (unsigned i = 0; i < win_.ndpoints(); ++i)
      wei_[i] = memo_wei_[win_.dp(i)];

    mln_invariant(wei_.size() == win_.ndpoints());
    return *this;
  }

  template <typename D, typename W>
  inline
  w_window<D,W>&
  w_window<D,W>::sym()
  {
    w_window<D,W> tmp;
    for (unsigned i = 0; i < this->ndpoints(); ++i)
      tmp.insert(this->w(i), - this->dp(i));
    *this = tmp;
    return *this;
  }

  // operators

  template <typename D, typename W>
  inline
  std::ostream& operator<<(std::ostream& ostr, const w_window<D,W>& w_win)
  {
    ostr << '[';
    for (unsigned i = 0; i < w_win.win().ndpoints(); ++i)
      ostr << w_win.vect()[i] << ':' << w_win.w(i) << ' ';
    return ostr << ']';
  }

  template <typename D, typename Wl, typename Wr>
  inline
  bool operator==(const w_window<D,Wl>& lhs, const w_window<D,Wr>& rhs)
  {
    if (lhs.ndpoints() != rhs.ndpoints())
      return false;
    if (lhs.win() != rhs.win())
      return false;
    const std::vector<Wl>& wl = lhs.weights();
    const std::vector<Wr>& wr = rhs.weights();
    mln_assertion(wl.size() == wr.size());
    for (unsigned i = 0; i < wl.size(); ++i)
      if (wr[i] != wl[i])
	return false;
    return true;
  }

# endif // ! MLN_INCLUDE_ONLY

} // end of namespace mln


# include <mln/make/w_window.hh>


#endif // ! MLN_CORE_W_WINDOW_HH
