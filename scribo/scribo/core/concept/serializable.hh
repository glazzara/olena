// Copyright (C) 2011, 2013 EPITA Research and Development Laboratory
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

#ifndef SCRIBO_CORE_CONCEPT_SERIALIZABLE_HH
# define SCRIBO_CORE_CONCEPT_SERIALIZABLE_HH

/// \file
///
/// Concept for objects that can be serialized.

# include <mln/core/concept/object.hh>
# include <scribo/core/concept/serialize_visitor.hh>

namespace scribo
{

  /// \brief Concept for objects that can be serialized.
  template <typename E>
  class Serializable : public mln::Object<E>
  {
  public:

    /// Allow this object to be serialized by \p visitor.
    template <typename E2>
    void accept(const SerializeVisitor<E2>& visitor) const;
  };


# ifndef MLN_INCLUDE_ONLY

  template <typename E>
  template <typename E2>
  void
  Serializable<E>::accept(const SerializeVisitor<E2>& visitor) const
  {
    exact(visitor).visit(exact(*this));
  }

# endif // ! MLN_INCLUDE_ONLY


} // end of namespace scribo

#endif // SCRIBO_CORE_CONCEPT_SERIALIZABLE_HH
