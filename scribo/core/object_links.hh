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

# include <scribo/core/object_image.hh>


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
    object_links(const object_image(L)& objects);
    object_links(const object_image(L)& objects, unsigned n);
    object_links(const object_image(L)& objects, unsigned n, unsigned value);


    const void* objects_id_() const;
    const object_image(L)& object_image_() const;

  private:
    object_image(L) objects_;
  };


# ifndef MLN_INCLUDE_ONLY

  template <typename L>
  object_links<L>::object_links(const object_image(L)& objects)
    : objects_(objects)
  {

  }

  template <typename L>
  object_links<L>::object_links(const object_image(L)& objects, unsigned n)
    : super_t(n), objects_(objects)
  {

  }


  template <typename L>
  object_links<L>::object_links(const object_image(L)& objects,
				unsigned n, unsigned value)
    : super_t(n, value), objects_(objects)
  {

  }


  template <typename L>
  const void *
  object_links<L>::objects_id_() const
  {
    return objects_.id_();
  }

  template <typename L>
  const object_image(L)&
  object_links<L>::object_image_() const
  {
    return objects_;
  }


# endif // ! MLN_INCLUDE_ONLY


} // end of namespace scribo


#endif // ! SCRIBO_CORE_OBJECT_LINKS_HH
