// Copyright (C) 2007, 2009 EPITA Research and Development Laboratory (LRDE)
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

namespace mln
{

  /*! \namespace mln::doc
   * \brief The namespace mln::doc is only for documentation purpose.
   *
   * Since concepts are not yet part of the C++ Standard, they are not
   * explicitly expressed in code.  Their documentation is handled by
   * their respective ghost class, located in this namespace.
   *
   * \warning The ghost classes located in mln::doc should not be used
   * by the client.
   */
  namespace doc
  {

    /*! \brief Documentation class for mln::Object.
     * \see mln::Object
     */
    template <typename E>
    struct Object
    {
    };

  } // end of namespace mln::doc

} // end of namespace mln
