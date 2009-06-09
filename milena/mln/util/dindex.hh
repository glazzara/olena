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

#ifndef MLN_UTIL_DINDEX_HH
# define MLN_UTIL_DINDEX_HH

/*! \file
 *
 * \brief Definition of a "delta-index" type.
 *
 * \todo Remove coord and dim, then make window<dindex> work.
 *
 * \todo Add inheritance to Object so add operators and traits. 
 */

# include <mln/util/index.hh>
# include <mln/literal/zero.hh>
# include <mln/literal/one.hh>


namespace mln
{

  namespace util
  {

    // Fwd decl.
    template <typename Tag> struct dindex_;


    /*! \brief Delta-index structure.
     *
     */
    template <typename Tag = void>
    struct dindex_ // : public Object< dindex_<Tag> >
    {
      typedef Object<void> category;

      typedef index_<Tag> psite;
      typedef index_<Tag> site;

      typedef int coord;
      enum { dim = 1 };

      int i_;

      dindex_()
      {
      }

      dindex_(int i)
	: i_(i)
      {
      }

      /// \{ Constructors/assignments with literals.
      dindex_(const literal::zero_t&)
	: i_(0)
      {
      }
      dindex_<Tag>& operator=(const literal::zero_t&)
      {
	i_ = 0;
	return *this;
      }

      dindex_(const literal::one_t&)
	: i_(1)
      {
      }
      dindex_<Tag>& operator=(const literal::one_t&)
      {
	i_ = 1;
	return *this;
      }
      /// \}

      bool operator<(const dindex_& rhs) const
      {
	return i_ < rhs.i_;
      }

      operator int() const
      {
	return i_;
      }
    };

    typedef dindex_<void> dindex;



# ifndef MLN_INCLUDE_ONLY

    // FIXME

# endif // ! MLN_INCLUDE_ONLY

  } // end of namespace mln::util

} // end of namespace mln


#endif // ! MLN_UTIL_DINDEX_HH
