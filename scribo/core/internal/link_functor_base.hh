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

#ifndef SCRIBO_PRIMITIVE_LINK_INTERNAL_LINK_FUNCTOR_BASE_HH
# define SCRIBO_PRIMITIVE_LINK_INTERNAL_LINK_FUNCTOR_BASE_HH

/// \file
///
/// Base class for link functors.

# include <mln/core/concept/image.hh>

# include <mln/math/abs.hh>

# include <mln/util/array.hh>
# include <mln/util/couple.hh>

# include <scribo/core/concept/link_functor.hh>

# include <scribo/core/object_image.hh>
# include <scribo/core/object_links.hh>

# include <scribo/primitive/internal/init_link_array.hh>


namespace scribo
{

  namespace internal
  {

    /// \brief Base class for link functors.
    template <typename L, typename E>
    class link_functor_base : public Link_Functor<E>
    {
    public:

      typedef L support;

      link_functor_base(const object_image(L)& objects);

      const object_links<L>& links() const;

      unsigned link(unsigned object) const;
      const object_image(L)& objects() const;

    protected:
      object_links<L> links_;
      const object_image(L) objects_;
    };



# ifndef MLN_INCLUDE_ONLY

    template <typename L, typename E>
    inline
    link_functor_base<L,E>::link_functor_base(const object_image(L)& objects)
      : links_(objects, static_cast<unsigned>(objects.nlabels()) + 1),
	objects_(objects)
    {
      primitive::internal::init_link_array(links_);
    }

    template <typename L, typename E>
    inline
    const object_links<L>&
    link_functor_base<L,E>::links() const
    {
      return links_;
    }

    template <typename L, typename E>
    inline
    unsigned
    link_functor_base<L,E>::link(unsigned object) const
    {
      return links_[object];
    }

    template <typename L, typename E>
    inline
    const object_image(L)&
    link_functor_base<L,E>::objects() const
    {
      return objects_;
    }

# endif // ! MLN_INCLUDE_ONLY


  } // end of namespace scribo::internal

} // end of namespace scribo


#endif // ! SCRIBO_PRIMITIVE_LINK_INTERNAL_LINK_FUNCTOR_BASE_HH
