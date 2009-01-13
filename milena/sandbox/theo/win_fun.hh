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
// instantiate templates or use macros or infun functions from this
// file, or you compile this file and link it with other files to
// produce an executable, this file does not by itself cause the
// resulting executable to be covered by the GNU General Public
// License.  This exception does not however invalidate any other
// reasons why the executable file might be covered by the GNU General
// Public License.

#ifndef MLN_WIN_FUN_HH
# define MLN_WIN_FUN_HH

/*! \file mln/win/fun.hh
 *
 * \brief Definition of the mln::win::fun window.
 */

# include <mln/core/internal/window_base.hh>
# include <mln/core/internal/site_relative_iterator_base.hh>


namespace mln
{

  namespace win
  {

    // Fwd decl.
    template <typename F> class fun_piter;

 
    /*! \brief Window defined by a function.
     */
    template <typename F>
    struct fun : public internal::window_base< mln_deduce(F, result, dpsite),
					       fun<F> >
    {

      /// Site_Iterator type to browse a fun forward
      typedef fun_piter<F> fwd_qiter;

      /// Site_Iterator type to browse a fun backward
      typedef fun_piter<F> bkd_qiter;

      /// Same as fwd_qiter
      typedef fwd_qiter qiter;

      /// Constructor.
      fun(const F& f) : f(f) {}

      /// Test if the window is centered.
      bool is_centered() const  { return false; }

      /// Test if the window is empty.
      bool is_empty() const  { return false; }
	
      /// Test if the window is symmetric.
      bool is_symmetric() const { return false; }
	
      /// Give the maximum coordinate gap between the window
      unsigned delta() const { return 100; } // FIXME

      /// Apply a central symmetry to the target window.
      void sym() {}
		
      const F& f;
    };


    template <typename F>
    class fun_piter
      : public internal::site_relative_iterator_base< fun<F>,
						      fun_piter<F> >
    {
    public:

      typedef mln_result(F) W;

      /// Constructor.
      template <typename P>
      fun_piter(const fun<F>& win, const P& c)
	: f_(win.f)
      {
	q_.center_at(c);
	this->change_target(win);
	this->center_at(c);
      }

      /// Test the iterator validity.
      bool is_valid_() const
      {
	return this->c_ != 0 && q_.is_valid();
      }

      /// Invalidate the iterator.
      void invalidate_()
      {
	q_.invalidate();
      }

      /// Start an iteration.
      void do_start_()
      {
	q_.change_target(f_(*this->c_));
	q_.start();
      }

      /// Go to the next point.
      void do_next_()
      {
	q_.next();
      }

      /// Compute the current psite.
      mln_psite(W) compute_p_() const
      {
	return q_;
      }

    protected:

      const F& f_;
      mln_fwd_qiter(W) q_;
    };
 

# ifndef MLN_INCLUDE_ONLY

    // FIXME: Move code here.

    // MEMO: mln_fwd_qiter(mln_result(F))


# endif // ! MLN_INCLUDE_ONLY

  } // end of namespace mln::win

} // end of namespace mln



#endif // ! MLN_WIN_FUN_HH
