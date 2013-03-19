// Copyright (C) 2009, 2013 EPITA Research and Development Laboratory
// (LRDE)
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

#ifndef SCRIBO_MAKE_DEBUG_FILENAME_HH
# define SCRIBO_MAKE_DEBUG_FILENAME_HH

/// \file
///
/// Construct and returns a formated output file name.

# include <sstream>
# include <mln/debug/trace.hh>

namespace scribo
{

  namespace make
  {

    /// Construct and returns a formated output file name:
    ///
    ///	    `input_filename`_`id`_`name`
    /// \sa scribo::make::internal::debug_filename_prefix
    std::string
    debug_filename(const std::string& name);


    namespace internal
    {

      /// Set the default debug filename prefix.
      extern std::string debug_filename_prefix;

    } // end of namespace scribo::make::internal

# ifndef MLN_INCLUDE_ONLY


#  ifndef MLN_WO_GLOBAL_VARS

    namespace internal
    {

      std::string debug_filename_prefix = "";

    } // end of namespace scribo::make::internal

#  endif // ! MLN_WO_GLOBAL_VARS


    inline
    std::string
    debug_filename(const std::string& name)
    {
      static int file_id = 1;

      std::ostringstream os;

      if (! internal::debug_filename_prefix.empty())
	os << internal::debug_filename_prefix << "_";
      else
	mln_trace_warning("You may like to set a default filename prefix.");

      if (file_id < 10)
	os << "0";

      os << file_id++
	 << "_"
	 << name;
      return os.str();
    }


# endif // ! MLN_INCLUDE_ONLY


  } // end of namespace scribo::make

} // end of namespace scribo


#endif // ! SCRIBO_MAKE_DEBUG_FILENAME_HH
