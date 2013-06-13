// Copyright (C) 2013 EPITA Research and Development Laboratory (LRDE)
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

#ifndef MLN_DEBUG_TRACE_HH
# define MLN_DEBUG_TRACE_HH

/// \file
///
/// Definition of the function entering/exiting tracing mechanism.

# include <ctime>

# include <iostream>
# include <string>
# include <stack>

# include <mln/core/contract.hh>

# define mln_trace(S)				\
  mln::debug::trace mln_trace_(S);		\
  (void) mln_trace_;

# define mln_trace_warning(S)			\
  mln::debug::trace::warning(S);

namespace mln
{

  namespace debug
  {

    /*! \brief Trace function calls.

      This class is meant to be instantiated at the beginning of each
      routine in olena. It keep trace of the function call backtrace
      and remember the time in each function.

      This class is intended to be used through the mln_trace macro as
      follows:

      \code
      namespace mln
      {
        void my_function()
	{
          mln_trace("mln::my_function");

	  /// Your code
	  /// ....
	}
      }
      \endcode

      \warning mln_trace() declares a local variable (named
      mln_trace_), so do not use mln_trace twice in the same scope to
      avoid duplicate declarations.
     */
    class trace
    {
    public:

      trace(const std::string& scope);
      ~trace();

      static void resume();
      static void stop();
      static void warning(const std::string& message);

      /// Public attributes
      ///@{
      static bool quiet;
      static unsigned tab;
      static bool full_trace;
      ///@}

    private:
      static std::stack<std::clock_t> start_times_;
      static std::stack<std::string> scopes_;
      static unsigned max_tab_;
      static bool is_quiet_;
    };


# ifndef MLN_INCLUDE_ONLY


    // Initialization of static attributes.
#  ifndef MLN_WO_GLOBAL_VARS

    bool trace::quiet = true;
    unsigned trace::tab = 0;
    bool trace::full_trace = false;

    std::stack<std::clock_t> trace::start_times_;
    std::stack<std::string>  trace::scopes_;
    unsigned trace::max_tab_ = 0;
    bool trace::is_quiet_ = quiet;

#  endif // ! MLN_WO_GLOBAL_VARS



    // Implementations.

    inline
    trace::trace(const std::string& scope)
    {
      if (quiet)
	return;

      start_times_.push(std::clock());
      scopes_.push(scope);

      if ((tab != 0) && (max_tab_ == tab))
	std::cout << std::endl;

      for (unsigned i = 0; i < tab; ++i)
	std::cout << "  ";
      std::cout << scope << " {";

      max_tab_ = ++tab;
    }


    inline
    trace::~trace()
    {
      if (quiet)
	return;

      std::string scope = scopes_.top();
      scopes_.pop();

      bool has_inner_trace = (max_tab_ == tab);
      --tab;

      if (!has_inner_trace)
        for (unsigned i = 0; i < tab; ++i)
	  std::cout << "  ";

      std::cout << "} ";

      if (!has_inner_trace)
	std::cout << scope << " ";

      mln_assertion(! start_times_.empty());
      std::clock_t now = std::clock();

      if (start_times_.top() > now)
	std::cerr
	  << "Warning: Clock skew detected (start time in the future)."
	  << std::endl;

      if (start_times_.top() < now)
	{
	  std::cout
	    << "- "
	    << ((float(now) - float(start_times_.top())) / CLOCKS_PER_SEC)
	    << "s ";
	}

      start_times_.pop();

      if (has_inner_trace || (max_tab_ - tab > 1))
	std::cout << std::endl;
    }


    inline
    void trace::resume()
    {
       quiet = is_quiet_;
    }


    inline
    void trace::stop()
    {
      is_quiet_ = quiet;
      if (!full_trace)
	quiet = true;
    }


    inline
    void trace::warning(const std::string& message)
    {
      std::cerr << "Warning: " << message << std::endl;
    }

# endif // ! MLN_INCLUDE_ONLY

  } // end of namespace mln::debug

} // end of namespace mln


#endif // ! MLN_DEBUG_TRACE_HH
