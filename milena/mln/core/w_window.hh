// Copyright (C) 2007, 2008, 2009, 2012 EPITA Research and Development
// Laboratory (LRDE)
//
// This file is part of Olena.
//
// Olena is free software: you can redistribute it and/or modify it under
// the terms of the GNU General Public License as published by the Free
// Software Foundation, version 2 of the License.
//
// Olena is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
// General Public License for more details.
//
// You should have received a copy of the GNU General Public License
// along with Olena.  If not, see <http://www.gnu.org/licenses/>.
//
// As a special exception, you may use this file as part of a free
// software project without restriction.  Specifically, if other files
// instantiate templates or use macros or inline functions from this
// file, or you compile this file and link it with other files to produce
// an executable, this file does not by itself cause the resulting
// executable to be covered by the GNU General Public License.  This
// exception does not however invalidate any other reasons why the
// executable file might be covered by the GNU General Public License.

#ifndef MLN_CORE_W_WINDOW_HH
# define MLN_CORE_W_WINDOW_HH

/// \file
///
/// Definition of the generic weighted window class mln::w_window.
///
/// \todo Generalize W_Window -> Image.

# include <map>

# include <mln/core/internal/weighted_window_base.hh>
# include <mln/core/concept/image.hh>
# include <mln/core/site_set/box.hh>
# include <mln/core/window.hh>
# include <mln/core/dpsites_piter.hh>

# include <mln/value/ops.hh>
# include <mln/util/ord.hh>
# include <mln/geom/bbox.hh> // FIXME: We may have some dep trouble with this include.
# include <mln/literal/zero.hh>
# include <mln/convert/to.hh>


namespace mln
{

  // Forward declarations.
  template <typename D,  typename W> struct w_window;
  template <typename It, typename W> struct with_w_;


  namespace trait
  {

    template <typename D, typename W>
    struct window_< mln::w_window<D,W> > : window_< mln::window<D> >
    {
      // Same traits as its corresponding window.
    };

  } // end of namespace mln::trait


  /// Generic w_window class.
  ///
  /// This type of w_window is just like a set of delta-points.  The
  /// parameter \c D is the type of delta-points; the parameter \c W is
  /// the type of weights.
  ///
  template <typename D, typename W>
  struct w_window : public internal::weighted_window_base< mln::window<D>,
							   w_window<D,W> >
  {
    /// Dpsite associated type.
    typedef D dpsite;

    /// Weight associated type.
    typedef W weight;


    /// Site_Iterator type to browse (forward) the points of a generic w_window.
    typedef with_w_< dpsites_fwd_piter< w_window<D, W> >, W > fwd_qiter;

    /// Site_Iterator type to browse (backward) the points of a generic w_window.
    typedef with_w_< dpsites_bkd_piter< w_window<D, W> >, W > bkd_qiter;


    /// Constructor without argument.
    w_window();


    /// Insert a couple of weight \p w and delta-point \p d.
    w_window<D,W>& insert(const W& w, const D& d);


    /// Give the \p i-th weight.
    W w(unsigned i) const;

    /// Give access to the vector of weights.
    const std::vector<W>& weights() const;


    /// Give access to the vector of delta-points.
    const std::vector<D>& std_vector() const;

    /// Give the corresponding window.
    const mln::window<D>& win() const;


    /// Test if the window is symmetric.
    bool is_symmetric() const;

    /// Apply a central symmetry to the window.
    void sym();

    /// Clear this window.
    void clear();

  protected:

    mln::window<D> win_;
    std::vector<W> wei_;
  };


  /// Print a weighted window \p w_win into an output stream \p ostr.
  /// \relates mln::w_window
  template <typename D, typename W>
  std::ostream& operator<<(std::ostream& ostr, const w_window<D,W>& w_win);


  /// Equality test between two weighted windows \p lhs and \p rhs.
  /// \relates mln::w_window
  template <typename D, typename Wl, typename Wr>
  bool operator==(const w_window<D,Wl>& lhs, const w_window<D,Wr>& rhs);


  /// \internal Conversions: w_window -> image
  template <typename D, typename W, typename I>
  void from_to_(const w_window<D,W>& from, Image<I>& to);

  /// \internal Conversions: weight[] -> w_window
  template <typename V, unsigned S, typename D, typename W>
  void from_to_(const V (&weight)[S], w_window<D,W>& to);



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
  const std::vector<D>&
  w_window<D,W>::std_vector() const
  {
    return win_.std_vector();
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
    mln_invariant(wei_.size() == win_.size());
    return wei_[i];
  }

  template <typename D, typename W>
  inline
  w_window<D,W>&
  w_window<D,W>::insert(const W& w, const D& d)
  {
    mln_invariant(wei_.size() == win_.size());
    mln_precondition(! win_.has(d));

    if (w == W(0)) // FIXME: Implicit restriction "W scalar"...
      // no-op
      return *this;

    // memorization: d_i -> w_i
    std::map<D, W, util::ord<D> > memo_wei_;
    for (unsigned i = 0; i < win_.size(); ++i)
      memo_wei_[win_.dp(i)] = wei_[i];

    // insertion of d and w:
    win_.insert(d);
    memo_wei_[d] = w;

    // re-mapping: w_i <- d_i
    wei_.resize(win_.size());
    for (unsigned i = 0; i < win_.size(); ++i)
      wei_[i] = memo_wei_[win_.dp(i)];

    mln_invariant(wei_.size() == win_.size());
    return *this;
  }

  template <typename D, typename W>
  inline
  bool
  w_window<D,W>::is_symmetric() const
  {
    if (! win_.is_symmetric())
      return false;
    w_window<D,W> tmp;
    tmp.sym();
    return *this == tmp;
  }

  template <typename D, typename W>
  inline
  void
  w_window<D,W>::sym()
  {
    w_window<D,W> tmp;
    unsigned n = this->size();
    for (unsigned i = 0; i < n; ++i)
      tmp.insert(this->w(i), - this->dp(i));
    *this = tmp;
  }

  template <typename D, typename W>
  inline
  void
  w_window<D,W>::clear()
  {
    win_.clear();
    wei_.clear();
  }


  // operators

  template <typename D, typename W>
  inline
  std::ostream& operator<<(std::ostream& ostr, const w_window<D,W>& w_win)
  {
    ostr << '[';
    for (unsigned i = 0; i < w_win.win().size(); ++i)
      ostr << w_win.dp(i) << ':' << w_win.w(i) << ' ';
    return ostr << ']';
  }

  template <typename D, typename Wl, typename Wr>
  inline
  bool operator==(const w_window<D,Wl>& lhs, const w_window<D,Wr>& rhs)
  {
    if (lhs.size() != rhs.size())
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


  // Conversions

  template <typename D, typename W, typename I>
  void
  from_to_(const w_window<D,W>& w_win, Image<I>& ima_)
  {
    typedef mln_site(I) P;
    mlc_converts_to(D, mln_delta(P))::check();
    mlc_converts_to(W, mln_value(I))::check();

    I& ima = exact(ima_);
    mln_precondition(! ima.is_valid());
    mln_precondition(w_win.is_valid());

    ima.init_(geom::bbox(w_win));
    {
      // data::fill(ima, literal::zero) is:
      mln_value(I) zero = literal::zero;
      mln_piter(I) p(ima.domain());
      for_all(p)
	ima(p) = zero;
    }

    unsigned n = w_win.size();
    for (unsigned i = 0; i < n; ++i)
      ima(convert::to<P>(w_win.dp(i))) = w_win.w(i);
  }

  template <typename V, unsigned S, typename D, typename W>
  void
  from_to_(const V (&weight)[S], w_window<D,W>& to)
  {
    mlc_bool(S != 0)::check();
    mlc_converts_to(V, W)::check();
    enum { d = D::dim,
	   s = mlc_root(d,S)::value / 2 };
    metal::bool_<(mlc_pow_int(2 * s + 1, d) == S)>::check();
    to.clear();
    typedef mln_site(D) P;
    box<P> b(all_to(-s), all_to(+s));
    mln_fwd_piter(box<P>) p(b);
    unsigned i = 0;
    V zero = literal::zero;
    for_all(p)
    {
      if (weight[i] != zero)
	to.insert(weight[i], convert::to<D>(p));
      ++i;
    }

  }

# endif // ! MLN_INCLUDE_ONLY

} // end of namespace mln


# include <mln/make/w_window.hh>


#endif // ! MLN_CORE_W_WINDOW_HH
