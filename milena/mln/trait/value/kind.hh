// Copyright (C) 2007 EPITA Research and Development Laboratory
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

#ifndef MLN_TRAIT_VALUE_KIND_HH
# define MLN_TRAIT_VALUE_KIND_HH

/*! \file mln/trait/value/kind.hh
 *
 * \brief Kind of values (for use in images).
 */

# include <string>


namespace mln
{

  namespace trait
  {

    namespace value
    {

      struct kind
      {
	struct any            { std::string name() const { return "kind::any"; } };

	struct color  : any   { std::string name() const { return "kind::color"; } };
	struct gray   : any   { std::string name() const { return "kind::gray"; } };

	struct label  : any   { std::string name() const { return "kind::label"; } };
	struct logic  : label { std::string name() const { return "kind::logic"; } };
	struct binary : logic { std::string name() const { return "kind::binary"; } };

	struct data   : any   { std::string name() const { return "kind::data"; } };
      };

    } // end of namespace mln::trait::kind

  } // end of namespace mln::trait

} // end of namespace mln


#endif // ! MLN_TRAIT_VALUE_KIND_HH
