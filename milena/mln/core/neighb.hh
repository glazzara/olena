// Copyright (C) 2007, 2008, 2009 EPITA Research and Development
// Laboratory (LRDE)
//
// This file is part of the Milena Library.  This library is free
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

#ifndef MLN_CORE_NEIGHB_HH
# define MLN_CORE_NEIGHB_HH

/// \file
///
/// Definition of a window-to-neighborhood adapter.
///
///
/// \todo See if the impl of from_to is fine.  What about removing the
/// origin?  etc.

# include <mln/core/internal/neighborhood_base.hh>
# include <mln/core/internal/site_relative_iterator_base.hh>
# include <mln/core/internal/neighb_niter_impl.hh>


namespace mln
{

  // Forward declarations.
  template <typename W> class neighb_fwd_niter;
  template <typename W> class neighb_bkd_niter;
  template <typename W> class neighb;


  namespace convert
  {

    namespace over_load
    {

      template <typename W>
      void
      from_to_(const mln::neighb<W>& from, W& to);

      template <typename W>
      void
      from_to_(const W& from, mln::neighb<W>& to);

    } // end of namespace mln::convert::over_load

  } // end of namespace mln::convert



  /// Adapter class from window to neighborhood.

  template <typename W>
  class neighb : public internal::neighborhood_base< W, neighb<W> >,
		 private mlc_is_a(W, Window)::check_t
  {
  public:

    /// Forward site iterator associated type.
    typedef neighb_fwd_niter<W> fwd_niter;

    /// Backward site iterator associated type.
    typedef neighb_bkd_niter<W> bkd_niter;

    /// Site iterator associated type.
    typedef fwd_niter niter;


    /// Constructor without argument.
    neighb();

    /// Constructor from a window \p win.
    neighb(const W& win);

    /// Get the corresponding window.
    const W& win() const;

    /// Change the corresponding window.
    void change_window(const W& new_win);


    /// \internal Hook to the window.
    W& hook_win_();

  private:

    W win_;
  };



  // neighb_fwd_niter<W>

  template <typename W>
  class neighb_fwd_niter
    : public internal::site_relative_iterator_base< neighb<W>,
						    neighb_fwd_niter<W> >,
      public internal::neighb_niter_impl<W, neighb_fwd_niter<W> >
  {
  public:

    /// Constructor without argument.
    neighb_fwd_niter();

    template <typename P>
    neighb_fwd_niter(const neighb<W>& nbh, const P& c);

    /// Test the iterator validity.
    bool is_valid_() const;

    /// Invalidate the iterator.
    void invalidate_();

    /// Start an iteration.
    void do_start_();

    /// Go to the next point.
    void do_next_();

    /// Compute the current psite.
    mln_psite(W) compute_p_() const;

  protected:

    mln_fwd_qiter(W) i_;
  };



// neighb_bkd_niter<W>

template <typename W>
class neighb_bkd_niter
  : public internal::site_relative_iterator_base< neighb<W>,
						  neighb_bkd_niter<W> >,
    public internal::neighb_niter_impl<W, neighb_fwd_niter<W> >
{
public:

  /// Constructor without argument.
  neighb_bkd_niter();

  template <typename P>
  neighb_bkd_niter(const neighb<W>& nbh, const P& c);

  /// Test the iterator validity.
  bool is_valid_() const;

  /// Invalidate the iterator.
  void invalidate_();

  /// Start an iteration.
  void do_start_();

  /// Go to the next point.
  void do_next_();

  /// Compute the current psite.
  mln_psite(W) compute_p_() const;

protected:

  mln_bkd_qiter(W) i_;
};



# ifndef MLN_INCLUDE_ONLY

  // neighb<W>

  template <typename W>
  inline
  neighb<W>::neighb()
  {
  }

  template <typename W>
  inline
  neighb<W>::neighb(const W& win)
  {
    change_window(win);
  }

  template <typename W>
  inline
  const W&
  neighb<W>::win() const
  {
    return win_;
  }

  template <typename W>
  inline
  void
  neighb<W>::change_window(const W& new_win)
  {
    mln_precondition(new_win.is_neighbable_());
    win_ = new_win;
  }

  template <typename W>
  inline
  W&
  neighb<W>::hook_win_()
  {
    return win_;
  }


  // mln::convert::from_to

  namespace convert
  {

    namespace over_load
    {

      template <typename W>
      void
      from_to_(const mln::neighb<W>& from, W& to)
      {
        to = from.win();
      }

      template <typename W>
      void
      from_to_(const W& from, mln::neighb<W>& to)
      {
        to.change_window(from);
      }

    } // end of namespace mln::convert::over_load

  } // end of namespace mln::convert


  // neighb_fwd_niter<W>

  template <typename W>
  inline
  neighb_fwd_niter<W>::neighb_fwd_niter()
  {
  }

  template <typename W>
  template <typename P>
  inline
  neighb_fwd_niter<W>::neighb_fwd_niter(const neighb<W>& nbh, const P& c)
  {
    this->change_target(nbh);
    this->center_at(c);
    i_.center_at(c); // Always before change_target for this kind of iter.
    i_.change_target(nbh.win());
  }

  template <typename W>
  inline
  bool
  neighb_fwd_niter<W>::is_valid_() const
  {
    return i_.is_valid();
  }

  template <typename W>
  inline
  void
  neighb_fwd_niter<W>::invalidate_()
  {
    i_.invalidate();
  }

  template <typename W>
  inline
  void
  neighb_fwd_niter<W>::do_start_()
  {
    i_.start();
  }

  template <typename W>
  inline
  void
  neighb_fwd_niter<W>::do_next_()
  {
    i_.next();
  }

  template <typename W>
  inline
  mln_psite(W)
  neighb_fwd_niter<W>::compute_p_() const
  {
    return i_.compute_p_();
  }


  // neighb_bkd_niter<W>

  template <typename W>
  inline
  neighb_bkd_niter<W>::neighb_bkd_niter()
  {
  }

  template <typename W>
  template <typename P>
  inline
  neighb_bkd_niter<W>::neighb_bkd_niter(const neighb<W>& nbh, const P& c)
  {
    this->change_target(nbh);
    this->center_at(c);
    i_.center_at(c); // Always before change_target for this kind of iter.
    i_.change_target(nbh.win());
  }

  template <typename W>
  inline
  bool
  neighb_bkd_niter<W>::is_valid_() const
  {
    return i_.is_valid();
  }

  template <typename W>
  inline
  void
  neighb_bkd_niter<W>::invalidate_()
  {
    i_.invalidate();
  }

  template <typename W>
  inline
  void
  neighb_bkd_niter<W>::do_start_()
  {
    i_.start();
  }

  template <typename W>
  inline
  void
  neighb_bkd_niter<W>::do_next_()
  {
    i_.next();
  }

  template <typename W>
  inline
  mln_psite(W)
  neighb_bkd_niter<W>::compute_p_() const
  {
    return i_.compute_p_();
  }

# endif // ! MLN_INCLUDE_ONLY

} // end of namespace mln


#endif // ! MLN_CORE_NEIGHB_HH
