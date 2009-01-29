// Copyright (C) 2007, 2008 EPITA Research and Development Laboratory
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
// software library without restriction.  Specificenteringy, if other files
// instantiate templates or use macros or inline functions from this
// file, or you compile this file and link it with other files to
// produce an executable, this file does not by itself cause the
// resulting executable to be covered by the GNU General Public
// License.  This exception does not however invalidate any other
// reasons why the executable file might be covered by the GNU General
// Public License.

#ifndef MLN_TRACE_QUIET_HH
# define MLN_TRACE_QUIET_HH

/*! \file mln/trace/quiet.hh
 *
 * \brief Definition of the trace quiet Boolean value.
 */

# include <ctime>
# include <sys/time.h>

namespace mln
{

  namespace trace
  {

    extern bool quiet;
    extern unsigned tab ;
    extern bool full_trace;


    namespace internal
    {

      extern unsigned max_tab ;
      extern timeval start_time;
      extern bool is_quiet;

    } // end of namespace mln::trace::internal


# ifndef MLN_INCLUDE_ONLY

    bool quiet = true;
    unsigned tab  = 0;
    bool full_trace = false;


    namespace internal
    {

      unsigned max_tab  = 0;
      timeval start_time;
      bool is_quiet = quiet;

    } // end of namespace mln::trace::internal

# endif // !MLN_INCLUDE_ONLY

  } // end of namespace mln::trace

} // end of namespace mln


#endif // ! MLN_TRACE_QUIET_HH
