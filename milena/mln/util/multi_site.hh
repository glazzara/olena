// Copyright (C) 2008 EPITA Research and Development Laboratory (LRDE)
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

#ifndef MLN_UTIL_MULTI_SITE_HH
# define MLN_UTIL_MULTI_SITE_HH

/// \file
/// \brief Definition of a multiple-site type.

# include <cstddef>

# include <vector>

# include <mln/core/concept/object.hh>

namespace mln
{

  namespace util
  {

    template <typename P>
    struct multi_site : public mln::Object< multi_site<P> >
    {
      // The type of a single site, called a location.
      typedef P location;
      /* FIXME: We should not need to define this typedef
	 (see. mln::internal::image_base's site `coord' typedef).  */
      typedef mln_coord(P) coord;

      typedef std::vector<P> container;
      typedef typename container::size_type size_type;
      typedef typename container::reference reference;
      typedef typename container::const_reference const_reference;

      /// Interface similar to std::vector.
      /// \{
      void push_back(const P& p);

      void reserve(size_type n);

      size_type size() const;

      reference operator[](size_type n);
      const_reference operator[](size_type n) const;

      const_reference front() const;
      reference front();
      /// \}

      container sites;
    };


    /* FIXME: Required by an assertion in mln::p_queue_fast<P>::has();
       shouldn't there be no requirements on sites?  */
    template <typename P>
    bool
    operator==(const multi_site<P>& lhs, const multi_site<P>& rhs);



# ifndef MLN_INCLUDE_ONLY

    template <typename P>
    void 
    multi_site<P>::push_back(const P& p)
    {
      sites.push_back(p);
    }

    template <typename P>
    void 
    multi_site<P>::reserve(size_type n)
    {
      sites.reserve(n);
    }

    template <typename P>
    typename multi_site<P>::size_type 
    multi_site<P>::size() const
    {
      return sites.size();
    }

    template <typename P>
    typename multi_site<P>::reference
    multi_site<P>::operator[](size_type n)
    {
      return sites[n];
    }

    template <typename P>
    typename multi_site<P>::const_reference
    multi_site<P>::operator[](size_type n) const
    {
      return sites[n];
    }

    template <typename P>
    typename multi_site<P>::const_reference
    multi_site<P>::front() const
    {
      mln_precondition(!sites.empty());
      return sites[0];
    }

    template <typename P>
    typename multi_site<P>::reference
    multi_site<P>::front()
    {
      mln_precondition(!sites.empty());
      return sites[0];
    }


    template <typename P>
    bool
    operator==(const multi_site<P>& lhs, const multi_site<P>& rhs)
    {
      return lhs.sites == rhs.sites;
    }

# endif // ! MLN_INCLUDE_ONLY

  } // end of mln::util

} // end of mln


#endif // ! MLN_UTIL_MULTI_SITE_HH
