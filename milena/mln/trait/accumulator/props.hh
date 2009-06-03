// Copyright (C) 2009 EPITA Research and Development Laboratory (LRDE)
//
// This file is part of the Milena Library.  This library is free
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

#ifndef MLN_TRAIT_ACCUMULATOR_PROPS_HH
# define MLN_TRAIT_ACCUMULATOR_PROPS_HH

/// \file mln/trait/accumulator/props.hh
///
/// Properties of accumulator classes.

# include <string>


namespace mln
{

  namespace trait 
  {

    namespace accumulator
    {

      // Basic accumulators properties.

      struct has_untake
      {
        struct any { protected: any() {}; };
        struct no : any { std::string name() const { return "has_untake::no"; } };
        struct yes : any { std::string name() const { return "has_untake::yes"; } };
      };

      struct has_stop
      {
        struct any { protected: any() {}; };
        struct no : any { std::string name() const { return "has_stop::no"; } };
        struct yes : any { std::string name() const { return "has_stop::yes"; } };
      };

      struct has_set_value
      {
        struct any { protected: any() {}; };
        struct no : any { std::string name() const { return "has_set_value::no"; } };
        struct yes : any { std::string name() const { return "has_set_value::yes"; } };
      };

      // Morphological accumulators properties
      struct when_pix
      {
        struct any { protected: any() {}; };
        struct use_v : any { std::string name() const { return "when_pix::use_v"; } };
        struct use_p : any { std::string name() const { return "when_pix::use_p"; } };
        struct use_pix  : any { std::string name() const { return "when_pix::use_pix"; } };
        struct use_none : any { std::string name() const { return "when_pix::use_none"; } };
        struct not_ok : any { std::string name() const { return "when_pix::not_ok"; } };
      };

    } // end of namespace mln::trait::accumulator

  } // end of namespace mln::trait

} // end of namespace mln


#endif // ! MLN_TRAIT_ACCUMULATOR_PROPS_HH
