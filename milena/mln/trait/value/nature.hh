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

#ifndef MLN_TRAIT_VALUE_NATURE_HH
# define MLN_TRAIT_VALUE_NATURE_HH

/*! \file mln/trait/value/nature.hh
 *
 * \brief Nature of values (for use in images).
 */

# include <string>


namespace mln
{

  namespace trait
  {

    namespace value
    {

      struct nature
      {
	struct any                 { std::string name() const { return "nature::any"; } };

	struct scalar     : any    { std::string name() const { return "nature::scalar"; } };
	struct integer    : scalar { std::string name() const { return "nature::integer"; } };
	struct floating   : scalar { std::string name() const { return "nature::floating"; } };

	struct vectorial  : any    { std::string name() const { return "nature::vectorial"; } };
	struct symbolic   : any    { std::string name() const { return "nature::symbolic"; } };
	struct structured : any    { std::string name() const { return "nature::structured"; } };

	struct unknown    : any    { std::string name() const { return "nature::unknown"; } };
      };

    } // end of namespace mln::trait::value

  } // end of namespace mln::trait

} // end of namespace mln


#endif // ! MLN_TRAIT_VALUE_NATURE_HH
