// Copyright (C) 2009 EPITA Research and Development Laboratory (LRDE)
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

#ifndef SCRIBO_CORE_OBJECT_LINKS_HH
# define SCRIBO_CORE_OBJECT_LINKS_HH

/// \file
///
/// \brief Object links representation.
///
/// \fixme Should not inherit from util::array.

# include <mln/util/array.hh>

# include <scribo/core/component_set.hh>


namespace scribo
{

  using namespace mln;

  /// \brief Object group representation.
  //
  template <typename L>
  class object_links
    : public mln::util::array<unsigned>
  {
    typedef mln::util::array<unsigned> super_t;

  public:
    object_links();
    object_links(const component_set<L>& components);
    object_links(const component_set<L>& components, unsigned value);

    const component_set<L>& component_set_() const;

    bool is_valid() const;

  private:
    component_set<L> components_;
  };


# ifndef MLN_INCLUDE_ONLY

  template <typename L>
  object_links<L>::object_links()
  {
  }

  template <typename L>
  object_links<L>::object_links(const component_set<L>& components)
    : super_t(static_cast<unsigned>(components.nelements()) + 1),
      components_(components)
  {

  }

  template <typename L>
  object_links<L>::object_links(const component_set<L>& components,
				unsigned value)
    : super_t(static_cast<unsigned>(components.nelements()) + 1, value),
      components_(components)
  {

  }


  template <typename L>
  const component_set<L>&
  object_links<L>::component_set_() const
  {
    return components_;
  }


  template <typename L>
  bool
  object_links<L>::is_valid() const
  {
    return components_.is_valid() && components_.nelements() == (this->size() - 1);
  }


# endif // ! MLN_INCLUDE_ONLY


} // end of namespace scribo


#endif // ! SCRIBO_CORE_OBJECT_LINKS_HH
