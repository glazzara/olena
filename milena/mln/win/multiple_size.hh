// Copyright (C) 2008, 2009, 2011, 2012 EPITA Research and Development
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

#ifndef MLN_WIN_MULTIPLE_SIZE_HH
# define MLN_WIN_MULTIPLE_SIZE_HH

/// \file
///
/// Definition of a multiple-size window.
///
/// \todo Use n for tests and code!!!

# include <mln/core/internal/window_base.hh>
# include <mln/core/internal/site_relative_iterator_base.hh>
# include <mln/util/array.hh>
# include <mln/metal/ands.hh>



namespace mln
{

  // Forward declarations.
  namespace win
  {
    template <unsigned n, typename W, typename F> class multiple_size;
    template <unsigned n, typename W, typename F> class multiple_size_qiter;
  }
  template <typename D> class window;



  namespace trait
  {

    template <unsigned n, typename W, typename F>
    struct window_< win::multiple_size<n,W,F> >
    {
      typedef trait::window::size::unknown     size;
      typedef trait::window::support::regular  support;
      typedef trait::window::definition::n_ary definition;
    };

  } // end of namespace trait



  namespace win
  {

    /// \brief Definition of a multiple-size window.
    ///
    /// \ingroup modwinmulti
    //
    template <unsigned n, typename W, typename F>
    class multiple_size

      : public internal::window_base< mln_dpsite(W), multiple_size<n,W,F> >,

	private metal::ands< mlc_bool(n > 1),
                             mlc_is(mln_trait_window_size(W),
				    trait::window::size::fixed),
	                     mlc_is(mln_trait_window_support(W),
				    trait::window::support::regular) >::check_t
    {
    public:

      typedef mln_dpsite(W) dpsite;
      typedef  mln_psite(W)  psite;
      typedef   mln_site(W)   site;

      typedef multiple_size< n, window<dpsite>, F > regular;

      typedef multiple_size_qiter<n,W,F> fwd_qiter;
      typedef multiple_size_qiter<n,W,F> bkd_qiter;
      typedef multiple_size_qiter<n,W,F> qiter;

      typedef W element;

      multiple_size();

      multiple_size(const F& f);

      bool is_empty() const;

      void set_window(unsigned i, const W& win);

      const W& window_(unsigned i) const;

      unsigned nwindows() const;

      const F& function() const;

      unsigned size_around(const mln_psite(W)& p) const;

      const mln_dpsite(W)& ith_dp_around(unsigned i, const mln_psite(W)& p) const;

      bool is_centered() const;

      bool is_symmetric() const;

      void sym();

      unsigned delta() const;

    private:

      util::array<W> win_;
      F f_;
    };


    template <unsigned n, typename W, typename F>
    class multiple_size_qiter
      : public internal::site_relative_iterator_base< multiple_size<n,W,F>,
						      multiple_size_qiter<n,W,F> >
    {
      typedef multiple_size_qiter<n,W,F> self_;
      typedef internal::site_relative_iterator_base< multiple_size<n,W,F>, self_ > super_;
    public:

      multiple_size_qiter();

      template <typename P>
      multiple_size_qiter(const multiple_size<n,W,F>& w, const P& c);

      template <typename P>
      void init_(const multiple_size<n,W,F>& w, const P& c);

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
      int i_;
      int size_() const;
    };



# ifndef MLN_INCLUDE_ONLY

    // win::multiple_size<n,W,F>

    template <unsigned n, typename W, typename F>
    inline
    multiple_size<n,W,F>::multiple_size()
      : f_()
    {
    }

    template <unsigned n, typename W, typename F>
    inline
    multiple_size<n,W,F>::multiple_size(const F& f)
      : f_(f)
    {
    }

    template <unsigned n, typename W, typename F>
    inline
    bool
    multiple_size<n,W,F>::is_empty() const
    {
      return win_.is_empty();
    }

    template <unsigned n, typename W, typename F>
    inline
    void
    multiple_size<n,W,F>::set_window(unsigned i, const W& win)
    {
      mln_precondition(i == win_.nelements());
      (void) i;
      win_.append(win);
    }

    template <unsigned n, typename W, typename F>
    inline
    const W&
    multiple_size<n,W,F>::window_(unsigned i) const
    {
      mln_precondition(i < win_.nelements());
      return win_[i];
    }

    template <unsigned n, typename W, typename F>
    inline
    unsigned
    multiple_size<n,W,F>::nwindows() const
    {
      return win_.nelements();
    }

    template <unsigned n, typename W, typename F>
    inline
    const F&
    multiple_size<n,W,F>::function() const
    {
      return f_;
    }

    template <unsigned n, typename W, typename F>
    inline
    bool
    multiple_size<n,W,F>::is_centered() const
    {
      mln_precondition(win_.nelements() >= 1);
      for (unsigned i = 0; i < win_.nelements(); ++i)
	if (! win_[i].is_centered())
	  return false;
      return true;
    }

    template <unsigned n, typename W, typename F>
    inline
    bool
    multiple_size<n,W,F>::is_symmetric() const
    {
      mln_precondition(win_.nelements() >= 1);
      for (unsigned i = 0; i < win_.nelements(); ++i)
	if (! win_[i].is_symmetric())
	  return false;
      return true;
    }

    template <unsigned n, typename W, typename F>
    inline
    void
    multiple_size<n,W,F>::sym()
    {
      mln_precondition(win_.nelements() >= 1);
      for (unsigned i = 0; i < win_.nelements(); ++i)
	win_[i].sym();
    }

    template <unsigned n, typename W, typename F>
    inline
    unsigned
    multiple_size<n,W,F>::delta() const
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

    template <unsigned n, typename W, typename F>
    inline
    unsigned
    multiple_size<n,W,F>::size_around(const mln_psite(W)& p) const
    {
      mln_precondition(win_.nelements() >= 2);
      mln_precondition(f_(p) < win_.nelements());
      return win_[f_(p)].size();
    }

    template <unsigned n, typename W, typename F>
    inline
    const mln_dpsite(W)&
    multiple_size<n,W,F>::ith_dp_around(unsigned i, const mln_psite(W)& p) const
    {
      mln_precondition(win_.nelements() >= 2);
      mln_precondition(f_(p) < win_.nelements());
      mln_precondition(i < win_[f_(p)].size());
      return win_[f_(p)].dp(i);
    }


    // win::multiple_size_qiter<n,W,F>

    template <unsigned n, typename W, typename F>
    inline
    multiple_size_qiter<n,W,F>::multiple_size_qiter()
    {
    }

    template <unsigned n, typename W, typename F>
    template <typename P>
    inline
    multiple_size_qiter<n,W,F>::multiple_size_qiter(const multiple_size<n,W,F>& w, const P& c)
    {
      init_(w, c);
    }

    template <unsigned n, typename W, typename F>
    template <typename P>
    inline
    void
    multiple_size_qiter<n,W,F>::init_(const multiple_size<n,W,F>& w, const P& c)
    {
      this->center_at(c);
      // We have to first change the center so that 'invalidate' can
      // work when changing the target.
      this->change_target(w);
    }


    template <unsigned n, typename W, typename F>
    inline
    bool
    multiple_size_qiter<n,W,F>::is_valid_() const
    {
      return i_ != -1 && i_ < size_();
    }

    template <unsigned n, typename W, typename F>
    inline
    void
    multiple_size_qiter<n,W,F>::invalidate_()
    {
      i_ = -1;
    }

    template <unsigned n, typename W, typename F>
    inline
    void
    multiple_size_qiter<n,W,F>::do_start_()
    {
      i_ = 0;
    }

    template <unsigned n, typename W, typename F>
    inline
    void
    multiple_size_qiter<n,W,F>::do_next_()
    {
      ++i_;
    }

    template <unsigned n, typename W, typename F>
    inline
    mln_psite(W)
    multiple_size_qiter<n,W,F>::compute_p_() const
    {
      return *this->c_ + this->s_->ith_dp_around(i_, *this->c_);
    }

    template <unsigned n, typename W, typename F>
    inline
    int
    multiple_size_qiter<n,W,F>::size_() const
    {
      return int(this->s_->size_around(*this->c_));
    }

# endif // ! MLN_INCLUDE_ONLY


  } // end of namespace mln::win

} // end of namespace mln


#endif // ! MLN_WIN_MULTIPLE_SIZE_HH
