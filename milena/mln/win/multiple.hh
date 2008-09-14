// Copyright (C) 2008 EPITA Research and Development Laboratory
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

#ifndef MLN_WIN_MULTIPLE_HH
# define MLN_WIN_MULTIPLE_HH

/*! \file mln/win/multiple.hh
 *
 * \brief Definition of a multiple window.
 */

# include <mln/core/internal/window_base.hh>
# include <mln/core/internal/site_relative_iterator_base.hh>
# include <mln/util/array.hh>



namespace mln
{

  namespace win
  {

    // Forward declaration.
    template <typename W, typename F>  class multiple_qiter;


    template <typename W, typename F>
    class multiple : public internal::window_base< mln_dpsite(W), multiple<W,F> >
    {
    public:

      typedef mln_dpsite(W) dpsite;
      typedef  mln_psite(W)  psite;
      typedef   mln_site(W)   site;

      typedef multiple_qiter<W,F> fwd_qiter;
      typedef multiple_qiter<W,F> bkd_qiter;
      typedef multiple_qiter<W,F> qiter;

      multiple();

      multiple(const F& f);

      bool is_empty() const;

      void set_window(unsigned i, const W& win);

      const W& window(unsigned i) const;

      unsigned size() const;

      unsigned size_around(const mln_psite(W)& p) const;

      const mln_dpsite(W)& ith_dp_around(unsigned i, const mln_psite(W)& p) const;

    private:

      util::array<W> win_;
      F f_;
      unsigned size_;
    };



    template <typename W, typename F>
    class multiple_qiter 
      : public internal::site_relative_iterator_base< multiple<W,F>,
						      multiple_qiter<W,F> >
    {
    public:

      multiple_qiter();

      template <typename P>
      multiple_qiter(const multiple<W,F>& w, const P& c);

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
      unsigned n_() const;
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
      return size_ == 0;
    }

    template <typename W, typename F>
    inline
    void
    multiple<W,F>::set_window(unsigned i, const W& win)
    {
      mln_precondition(i == win_.nelements());
      win_.append(win);
    }

    template <typename W, typename F>
    inline
    const W&
    multiple<W,F>::window(unsigned i) const
    {
      mln_precondition(i < win_.nelements());
      return win_[i];
    }

    template <typename W, typename F>
    inline
    unsigned
    multiple<W,F>::size() const
    {
      mln_precondition(win_.nelements() >= 1);
      unsigned s = win_[0].size();
      for (unsigned i = 1; i < win_.nelements(); ++i)
	mln_precondition(win_[i].size() == s);
      return s;
    }

    template <typename W, typename F>
    inline
    unsigned
    multiple<W,F>::size_around(const mln_psite(W)& p) const
    {
      mln_precondition(f_(p) < win_.nelements());
      return win_[f_(p)].size();
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
    template <typename P>
    inline
    multiple_qiter<W,F>::multiple_qiter(const multiple<W,F>& w, const P& c)
    {
      this->center_at(c);
      // We have to first change the center so that 'invalidate' can
      // work when changing the target.
      this->change_target(w);
    }

    template <typename W, typename F>
    inline
    bool
    multiple_qiter<W,F>::is_valid_() const
    {
      return i_ < n_();
    }

    template <typename W, typename F>
    inline
    void
    multiple_qiter<W,F>::invalidate_()
    {
      i_ = n_();
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

    template <typename W, typename F>
    inline
    unsigned
    multiple_qiter<W,F>::n_() const
    {
      return this->s_->size_around(*this->c_);
    }

# endif // ! MLN_INCLUDE_ONLY


  } // end of namespace mln::win

} // end of namespace mln


#endif // ! MLN_WIN_MULTIPLE_HH
