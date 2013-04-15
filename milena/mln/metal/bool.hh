// Copyright (C) 2007, 2008, 2009, 2012 EPITA Research and Development
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

#ifndef MLN_METAL_BOOL_HH
# define MLN_METAL_BOOL_HH

/// \file
///
/// Definition of a Boolean value type.

# include <string>


# define mlc_bool(B) mln::metal::bool_<( B )>

// The macro above is very convenient for static checks on Boolean
// expressions, e.g., "mlc_bool(m == n)::check();"



namespace mln
{

  namespace metal
  {

    // Fwd decl.
    template <bool b> struct bool_;


    /*!
      \internal
      \brief "true" type.
    */
    template <>
    struct bool_< true >
    {
      typedef bool_<true> check_t;
      static const bool value = true;
      typedef bool_<true> eval;
      static void check();
      std::string name() const { return "true"; }
    };

    typedef bool_<true> true_;


    /*!
      \internal
      \brief "false" type.
    */
    template <>
    struct bool_< false >
    {
      typedef bool_<false> check_not_t;
      static const bool value = false;
      typedef bool_<false> eval;
      static void check_not();
      std::string name() const { return "false"; }
    };

    typedef bool_<false> false_;


# ifndef MLN_INCLUDE_ONLY

    inline
    void true_::check()
    {
    }

    inline
    void false_::check_not()
    {
    }

# endif // ! MLN_INCLUDE_ONLY

  } // end of namespace mln::metal

} // end of namespace mln


# include <mln/metal/bexpr.hh>


#endif // ! MLN_METAL_BOOL_HH
