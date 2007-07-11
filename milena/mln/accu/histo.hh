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

#ifndef MLN_ACCU_HISTO_HH
# define MLN_ACCU_HISTO_HH

/*! \file mln/value/histo.hh
 *
 * \brief Define a couple of generic histogram classes.
 */

# include <vector>
# include <algorithm>

# include <mln/core/concept/value_set.hh>
# include <mln/value/set.hh>


namespace mln
{

  namespace accu
  {


    /*! Generic histogram class over a value set with type \c S.
     */
    template <typename S>
    struct histo_on_set
    {
      histo_on_set(const Value_Set<S>& s);

      typedef mln_value(S) value;

      void   take(const value& v);
      void untake(const value& v);
      void init();

      std::size_t operator()(const value& v) const;
      std::size_t operator[](std::size_t i) const;
      std::size_t nvalues() const;
      std::size_t sum() const;

      const std::vector<std::size_t>& vec() const;

      const S& vset() const;
      
    protected:

      const S& s_;
      std::vector<std::size_t> h_;
      std::size_t sum_;
    };


    template <typename S>
    std::ostream& operator<<(std::ostream& ostr, const histo_on_set<S>& h);




    /*! Generic histogram class over the set of values of type \c T.
     */
    template <typename T>
    struct histo_on_type : public histo_on_set< value::set_<T> >
    {
      histo_on_type();
    };




# ifndef MLN_INCLUDE_ONLY


    // histo_on_set<S>

    template <typename S>
    histo_on_set<S>::histo_on_set(const Value_Set<S>& s)
      : s_(exact(s)),
	h_(exact(s).nvalues(), 0),
	sum_(0)
    {
    }

    template <typename S>
    void
    histo_on_set<S>::take(const value& v)
    {
      ++h_[s_.index_of(v)];
      ++sum_;
    }

    template <typename S>
    void
    histo_on_set<S>::untake(const value& v)
    {
      mln_precondition(h_[s_.index_of(v)] > 0);
      mln_precondition(sum_ > 0);
      --h_[s_.index_of(v)];
      --sum_;
    }

    template <typename S>
    void
    histo_on_set<S>::init()
    {
      std::fill(h_.begin(), h_.end(), 0);
      sum_ = 0;
    }

    template <typename S>
    std::size_t
    histo_on_set<S>::operator()(const value& v) const
    {
      return h_[s_.index_of(v)];
    }

    template <typename S>
    std::size_t
    histo_on_set<S>::operator[](std::size_t i) const
    {
      mln_precondition(i < s_.nvalues());
      return h_[i];
    }
    
    template <typename S>
    std::size_t
    histo_on_set<S>::nvalues() const
    {
      return s_.nvalues();
    }

    template <typename S>
    std::size_t
    histo_on_set<S>::sum() const
    {
      return sum_;
    }

    template <typename S>
    const std::vector<std::size_t>&
    histo_on_set<S>::vec() const
    {
      return h_;
    }
    
    template <typename S>
    const S&
    histo_on_set<S>::vset() const
    {
      return s_;
    }

    template <typename S>
    std::ostream& operator<<(std::ostream& ostr, const histo_on_set<S>& h)
    {
      mln_viter(S) v(h.vset());
      for_all(v)
	if (h(v) != 0)
	  ostr << v << ':' << h(v) << ' ';
      ostr << std::endl;
      return ostr;
    }


    // histo_on_type<T>

    template <typename T>
    histo_on_type<T>::histo_on_type()
      : histo_on_set< value::set_<T> >(value::set_<T>::the())
    {
    }
    


# endif // ! MLN_INCLUDE_ONLY

  } // end of namespace mln::accu

} // end of namespace mln


#endif // ! MLN_ACCU_HISTO_HH
