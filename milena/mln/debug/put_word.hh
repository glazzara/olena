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

#ifndef MLN_DEBUG_PUT_WORD_HH
# define MLN_DEBUG_PUT_WORD_HH

/// \file
///
/// Write a word in a 2D image of characters.

# include <string>
# include <mln/core/image/image2d.hh>


namespace mln
{

  namespace debug
  {

    /*! \brief Put the \p word starting at location \p word_start in
      the image \p inout.
      \ingroup mlndebug
    */
    inline
    void
    put_word(image2d<char>& inout, const point2d& word_start,
	     const std::string& word);


# ifndef MLN_INCLUDE_ONLY

    inline
    void
    put_word(image2d<char>& inout, const point2d& word_start,
	     const std::string& word)
    {
      mln_precondition(word.length() != 0);
      mln_precondition(inout.has(word_start));

      point2d word_end = word_start;

      word_end.last_coord() = static_cast<def::coord>(word_end.last_coord() + word.length() - 1);

      if (! inout.has(word_end))
	trace::warning("Cannot write text outside the image domain");

      const unsigned n = word.length();
      point2d p = word_start;
      for (unsigned i = 0; i < n && inout.has(p); ++i)
	{
	  inout(p) = word[i];
	  p += right;
	}
    }

# endif // ! MLN_INCLUDE_ONLY

  } // end of namespace mln::debug

} // end of namespace mln


#endif // ! MLN_DEBUG_PUT_WORD_HH
