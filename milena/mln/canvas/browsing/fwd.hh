// Copyright (C) 2007, 2008, 2009, 2011 EPITA Research and Development
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

#ifndef MLN_CANVAS_BROWSING_FWD_HH
# define MLN_CANVAS_BROWSING_FWD_HH

/// \file
///
/// Canvas for forward browsing.

# include <mln/core/concept/browsing.hh>
# include <mln/core/concept/image.hh>


namespace mln
{

  namespace canvas
  {

    namespace browsing
    {

      /// Canvas for forward browsing
      /*!
       * This canvas browse all the points of an image 'input' of type
       * 'I' from left to right and from top to bottom
       *
       * The fonctor should provide (In addition of 'I' and 'input')
       * three methods :
       *   - init() : Will be called at the beginning.
       *   - next() : Will be called at each point 'p' (also provided by
       * the functor).
       *   - final(): Will be called at the end.
       *
       * F shall feature: \n
       * { \n
       *   --- as typedef: \n
       *   I; \n
       *   --as attributes: \n
       *   input; \n
       *   p; \n
       *   --- as method: \n
       *   void init(); \n
       *   void next(); \n
       *   void final(); \n
       * } \n
       *
       */
      struct fwd_t : public Browsing< fwd_t >
      {
	// This default constructor is needed for compilation with gcc
	// 4.6.0, gcc 4.6.1 and Clang.
	fwd_t();

	template <typename F>
	void operator()(F& f) const;
      };

      extern const fwd_t fwd;

# ifndef MLN_INCLUDE_ONLY

#  ifndef MLN_WO_GLOBAL_VARS

      const fwd_t fwd;

#  endif // ! MLN_WO_GLOBAL_VARS


      inline
      fwd_t::fwd_t()
      {
      }

      template <typename F>
      inline
      void
      fwd_t::operator()(F& f) const
      {
	trace::entering("canvas::browsing::fwd");
	mln_precondition(f.input.is_valid());
	typedef typename F::I I;
	mln_fwd_piter(I) p(f.input.domain());
	f.init();
	for_all(p)
	  {
	    f.p = p;
	    f.next();
	  }
	f.final();
	trace::exiting("canvas::browsing::fwd");
      }

# endif // ! MLN_INCLUDE_ONLY

    } // end of namespace mln::canvas::browsing

  } // end of namespace mln::canvas

} // end of namespace mln


#endif // ! MLN_CANVAS_BROWSING_FWD_HH
