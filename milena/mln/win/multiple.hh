// Copyright (C) 2008, 2009, 2012 EPITA Research and Development
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

#ifndef MLN_WIN_MULTIPLE_HH
# define MLN_WIN_MULTIPLE_HH

/// \file
///
/// Definition of a multiple window.
///
/// \todo Implementation of the bkd_qiter (see FIXME).

# include <mln/core/window.hh>
# include <mln/core/internal/site_relative_iterator_base.hh>
# include <mln/util/array.hh>



namespace mln
{

  // Forward declarations.
  namespace win
  {
    template <typename W, typename F> class multiple;
    template <typename W, typename F> class multiple_qiter;
  }



  namespace trait
  {

    template <typename W, typename F>
    struct window_< win::multiple<W,F> >
    {
      typedef trait::window::size::fixed       size;
      typedef trait::window::support::regular  support;
      typedef trait::window::definition::n_ary definition;
    };

  } // end of namespace trait



  namespace win
  {


    /// \brief Multiple window.
    ///
    /// \ingroup modwinmulti
    template <typename W, typename F>
    class multiple

      : public internal::window_base< mln_dpsite(W), multiple<W,F> >,

	private metal::and_< mlc_is(mln_trait_window_size(W),
				    trait::window::size::fixed),
	                     mlc_is(mln_trait_window_support(W),
				    trait::window::support::regular) >::check_t
    {
    public:

      typedef mln_dpsite(W) dpsite;
      typedef  mln_psite(W)  psite;
      typedef   mln_site(W)   site;

      typedef multiple< window<dpsite>, F > regular;

      typedef multiple_qiter<W,F> fwd_qiter;
      typedef /* FIXME: */ multiple_qiter<W,F> bkd_qiter;
      typedef multiple_qiter<W,F> qiter;

      typedef W element;

      multiple();

      multiple(const F& f);

      bool is_empty() const;

      void set_window(unsigned i, const W& win);

      const W& window_(unsigned i) const;

      unsigned nwindows() const;

      const F& function() const;

      unsigned size() const;

      const mln_dpsite(W)& ith_dp_around(unsigned i, const mln_psite(W)& p) const;

      bool is_centered() const;

      bool is_symmetric() const;

      void sym();

      unsigned delta() const;

    private:

      util::array<W> win_;
      F f_;
    };


    template <typename W, typename F>
    class multiple_qiter
      : public internal::site_relative_iterator_base< multiple<W,F>,
						      multiple_qiter<W,F> >
    {
      typedef multiple_qiter<W,F> self_;
      typedef internal::site_relative_iterator_base< multiple<W,F>, self_ > super_;
    public:

      multiple_qiter();
      template <typename Pref>
      multiple_qiter(const multiple<W,F>& w, const Pref& c);

      /// Overridden to initialize size_.
      void change_target(const multiple<W,F>& w);

      /// Delegated initialization.
      template <typename Pref>
      void init_(const multiple<W,F>& w, const Pref& c);

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

    private:
      unsigned i_;
      unsigned size_;
    };



# ifndef MLN_INCLUDE_ONLY

    // win::multiple<W,F>

    template <typename W, typename F>
    inline
    multiple<W,F>::multiple()
      : f_()
    {
    }

    template <typename W, typename F>
    inline
    multiple<W,F>::multiple(const F& f)
      : f_(f)
    {
    }

    template <typename W, typename F>
    inline
    bool
    multiple<W,F>::is_empty() const
    {
      return win_.is_empty();
    }

    template <typename W, typename F>
    inline
    void
    multiple<W,F>::set_window(unsigned i, const W& win)
    {
      mln_precondition(i == win_.nelements());
      if (i >= 1)
	mln_precondition(win.size() == win_[0].size());
      win_.append(win);
    }

    template <typename W, typename F>
    inline
    const W&
    multiple<W,F>::window_(unsigned i) const
    {
      mln_precondition(i < win_.nelements());
      return win_[i];
    }

    template <typename W, typename F>
    inline
    unsigned
    multiple<W,F>::nwindows() const
    {
      return win_.nelements();
    }

    template <typename W, typename F>
    inline
    const F&
    multiple<W,F>::function() const
    {
      return f_;
    }

    template <typename W, typename F>
    inline
    unsigned
    multiple<W,F>::size() const
    {
      mln_precondition(win_.nelements() >= 2); // Multiple cannot be just 1 element.
      unsigned s = win_[0].size();
      for (unsigned i = 1; i < win_.nelements(); ++i)
	mln_precondition(win_[i].size() == s);
      return s;
    }

    template <typename W, typename F>
    inline
    bool
    multiple<W,F>::is_centered() const
    {
      mln_precondition(win_.nelements() >= 1);
      for (unsigned i = 0; i < win_.nelements(); ++i)
	if (! win_[i].is_centered())
	  return false;
      return true;
    }

    template <typename W, typename F>
    inline
    bool
    multiple<W,F>::is_symmetric() const
    {
      mln_precondition(win_.nelements() >= 1);
      for (unsigned i = 0; i < win_.nelements(); ++i)
	if (! win_[i].is_symmetric())
	  return false;
      return true;
    }


    template <typename W, typename F>
    inline
    void
    multiple<W,F>::sym()
    {
      mln_precondition(win_.nelements() >= 1);
      for (unsigned i = 0; i < win_.nelements(); ++i)
	win_[i].sym();
    }

    template <typename W, typename F>
    inline
    unsigned
    multiple<W,F>::delta() const
    {
      mln_precondition(win_.nelements() >= 1);
      unsigned d = win_[0].delta();
      for (unsigned i = 1; i < win_.nelements(); ++i)
	{
	  unsigned d_i = win_[i].delta();
	  if (d_i > d)
	    d = d_i;
	}
      return d;
    }

    template <typename W, typename F>
    inline
    const mln_dpsite(W)&
    multiple<W,F>::ith_dp_around(unsigned i, const mln_psite(W)& p) const
    {
      mln_precondition(f_(p) < win_.nelements());
      mln_precondition(i < win_[f_(p)].size());
      return win_[f_(p)].dp(i);
    }


    // win::multiple_qiter<W,F>

    template <typename W, typename F>
    inline
    multiple_qiter<W,F>::multiple_qiter()
    {
    }

    template <typename W, typename F>
    template <typename Pref>
    inline
    multiple_qiter<W,F>::multiple_qiter(const multiple<W,F>& w, const Pref& c)
    {
      init_(w, c);
    }

    template <typename W, typename F>
    template <typename Pref>
    inline
    void
    multiple_qiter<W,F>::init_(const multiple<W,F>& w, const Pref& c)
    {
      this->center_at(c);
      // We have to first change the center so that 'invalidate' can
      // work when changing the target.
      change_target(w);
    }

    template <typename W, typename F>
    inline
    void
    multiple_qiter<W,F>::change_target(const multiple<W,F>& w)
    {
      size_ = w.size();
      this->super_::change_target(w);
    }

    template <typename W, typename F>
    inline
    bool
    multiple_qiter<W,F>::is_valid_() const
    {
      return i_ < size_;
    }

    template <typename W, typename F>
    inline
    void
    multiple_qiter<W,F>::invalidate_()
    {
      i_ = size_;
    }

    template <typename W, typename F>
    inline
    void
    multiple_qiter<W,F>::do_start_()
    {
      i_ = 0;
    }

    template <typename W, typename F>
    inline
    void
    multiple_qiter<W,F>::do_next_()
    {
      ++i_;
    }

    template <typename W, typename F>
    inline
    mln_psite(W)
    multiple_qiter<W,F>::compute_p_() const
    {
      return *this->c_ + this->s_->ith_dp_around(i_, *this->c_);
    }

# endif // ! MLN_INCLUDE_ONLY


  } // end of namespace mln::win

} // end of namespace mln


#endif // ! MLN_WIN_MULTIPLE_HH
