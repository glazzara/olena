// Copyright (C) 2011 EPITA Research and Development Laboratory (LRDE)
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

#ifndef SCRIBO_CORE_INTERNAL_DOC_SERIALIZER_HH
# define SCRIBO_CORE_INTERNAL_DOC_SERIALIZER_HH

/// \file
///
/// Concept for serializer visitors.

# include <scribo/core/concept/serialize_visitor.hh>

# include <scribo/core/document.hh>
# include <scribo/core/component_set.hh>
# include <scribo/core/component_info.hh>
# include <scribo/core/paragraph_set.hh>
# include <scribo/core/object_groups.hh>
# include <scribo/core/object_links.hh>
# include <scribo/core/line_links.hh>
# include <scribo/core/line_info.hh>

namespace scribo
{

  /// \brief Link functor concept.
  template <typename E>
  class doc_serializer : public SerializeVisitor<E>
  {
  public:
    // Visit overloads
    template <typename L>
    void visit(const document<L>& doc) const;

    template <typename L>
    void visit(const line_links<L>& llinks) const;

    template <typename L>
    void visit(const object_groups<L>& groups) const;

    template <typename L>
    void visit(const object_links<L>& links) const;

    template <typename L>
    void visit(const component_set<L>& comp_set) const;

    template <typename L>
    void visit(const component_info<L>& info) const;

    template <typename L>
    void visit(const paragraph_set<L>& parset) const;

    template <typename L>
    void visit(const line_info<L>& line) const;
  };


# ifndef MLN_INCLUDE_ONLY

  template <typename E>
  template <typename L>
  void
  doc_serializer<E>::visit(const document<L>& doc) const
  {
  }

  template <typename E>
  template <typename L>
  void
  doc_serializer<E>::visit(const line_links<L>& llinks) const
  {
  }

  template <typename E>
  template <typename L>
  void
  doc_serializer<E>::visit(const object_groups<L>& groups) const
  {
  }

  template <typename E>
  template <typename L>
  void
  doc_serializer<E>::visit(const object_links<L>& links) const
  {
  }

  template <typename E>
  template <typename L>
  void
  doc_serializer<E>::visit(const component_set<L>& comp_set) const
  {
  }

  template <typename E>
  template <typename L>
  void
  doc_serializer<E>::visit(const component_info<L>& info) const
  {
  }

  template <typename E>
  template <typename L>
  void
  doc_serializer<E>::visit(const paragraph_set<L>& parset) const
  {
  }

  template <typename E>
  template <typename L>
  void
  doc_serializer<E>::visit(const line_info<L>& line) const
  {
  }

# endif // ! MLN_INCLUDE_ONLY


} // end of namespace scribo

#endif // SCRIBO_CORE_INTERNAL_DOC_SERIALIZER_HH
