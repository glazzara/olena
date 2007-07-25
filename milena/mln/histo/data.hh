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

#ifndef MLN_HISTO_DATA_HH
# define MLN_HISTO_DATA_HH

/*! \file mln/histo/data.hh
 *
 * \brief Define a generic histogram class.
 */

# include <vector>
# include <algorithm>

# include <mln/core/concept/value_set.hh>


namespace mln
{

  namespace histo
  {


    /*! Generic histogram class over a value set with type \c S.
     */
    template <typename S>
    struct data
    {
      typedef mln_value(S) value;

      data(const Value_Set<S>& s);

      void clear();

      std::size_t operator()(const value& v) const;
      std::size_t& operator()(const value& v);

      const std::vector<std::size_t>& vect() const;
      const S& vset() const;
      std::size_t operator[](unsigned i) const;
      
    protected:

      const S& s_;
      std::vector<std::size_t> h_;
    };


    template <typename S>
    std::ostream& operator<<(std::ostream& ostr, const data<S>& h);



# ifndef MLN_INCLUDE_ONLY

    template <typename S>
    data<S>::data(const Value_Set<S>& s)
      : s_(exact(s)),
	h_(s_.nvalues(), 0)
    {
      clear();
    }

    template <typename S>
    void
    data<S>::clear()
    {
      std::fill(h_.begin(), h_.end(), 0);
    }

    template <typename S>
    std::size_t
    data<S>::operator()(const value& v) const
    {
      return h_[s_.index_of(v)];
    }

    template <typename S>
    std::size_t&
    data<S>::operator()(const value& v)
    {
      return h_[s_.index_of(v)];
    }

    template <typename S>
    const S&
    data<S>::vset() const
    {
      return s_;
    }

    template <typename S>
    std::size_t
    data<S>::operator[](unsigned i) const
    {
      mln_precondition(i < s_.nvalues());
      return h_[i];      
    }

    template <typename S>
    const std::vector<std::size_t>&
    data<S>::vect() const
    {
      return h_;
    }

    template <typename S>
    std::ostream& operator<<(std::ostream& ostr, const data<S>& h)
    {
      mln_viter(S) v(h.vset());
      for_all(v)
	if (h(v) != 0)
	  ostr << v << ':' << h(v) << ' ';
      return ostr;
    }

# endif // ! MLN_INCLUDE_ONLY

  } // end of namespace mln::histo

} // end of namespace mln


#endif // ! MLN_HISTO_DATA_HH
