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



namespace mln
{

  namespace win
  {

    // Forward declaration.
    template <unsigned n, typename W, typename F>  class multiple_qiter;


    template <unsigned n, typename W, typename F>
    class multiple : public internal::window_base< mln_dpsite(W), multiple<n,W,F> >
    {
    public:

      typedef mln_dpsite(W) dpsite;
      typedef  mln_psite(W)  psite;
      typedef   mln_site(W)   site;

      typedef multiple_qiter<n,W,F> fwd_qiter;
      typedef multiple_qiter<n,W,F> bkd_qiter;
      typedef multiple_qiter<n,W,F> qiter;

      multiple()
	: f_()
      {
      }

      multiple(const F& f)
	: f_(f)
      {
      }

      bool is_empty() const
      {
	bool res = false;
	for (unsigned i = 0; i < n; ++i)
	  if (win_[i].is_empty())
	    return true;
	return res;
      }

      void set_window(unsigned i, const W& win)
      {
	mln_precondition(i < n);
	win_[i] = win;
      }

      const W& window(unsigned i) const
      {
	mln_precondition(i < n);
	return win_[i];
      }

      const F& fun() const
      {
	return f_;
      }

    private:
      W win_[n];
      F f_;
    };



    template <unsigned n, typename W, typename F>
    class multiple_qiter 
      : public internal::site_relative_iterator_base< multiple<n,W,F>,
						      multiple_qiter<n,W,F> >
    {
    public:

      multiple_qiter()
      {
      }

      template <typename P>
      multiple_qiter(const multiple<n,W,F>& w, const P& c)
      {
	this->change_target(w);
	this->center_at(c);
      }

      /// Test the iterator validity.
      bool is_valid_() const
      {
	return i_ != -1;
      }

      /// Invalidate the iterator.
      void invalidate_()
      {
	i_ = -1;
      }

      /// Start an iteration.
      void do_start_()
      {
	i_ = 0;
      }

      /// Go to the next point.
      void do_next_()
      {
	++i_;
      }

      /// Compute the current psite.
      mln_psite(W) compute_p_() const
      {
	unsigned w = this->s_->fun()(*this->c_);
	return *this->c_ + this->s_->window(w).std_vector()[i_];
      }

    private:
      int i_;
    };



  } // end of namespace mln::win

} // end of namespace mln


#endif // ! MLN_WIN_MULTIPLE_HH
