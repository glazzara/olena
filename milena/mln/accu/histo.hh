// Copyright (C) 2007, 2008 EPITA Research and Development Laboratory
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

/*! \file mln/accu/histo.hh
 *
 * \brief Define a generic histogram accumulator class.
 *
 * \todo Use histo::data instead of std::vector!
 */

# include <vector>
# include <algorithm>

# include <mln/core/concept/value_set.hh>
# include <mln/core/concept/meta_accumulator.hh>
# include <mln/accu/internal/base.hh>
# include <mln/value/set.hh>
# include <mln/histo/data.hh>

namespace mln
{

  namespace accu
  {


    /*!
     * \brief Generic histogram class over a value set with type \c V.
     */
    template <typename V>
    struct histo :
      public mln::accu::internal::base<const std::vector<std::size_t>& ,
				       histo<V> >
    {
      histo();

      typedef V argument;

      /// Manipulators.
      /// \{
      void   take(const argument& t);
      void   take(const histo<V>& other);
      void untake(const argument& t);
      void init();

      std::size_t operator()(const argument& t) const;
      std::size_t operator[](unsigned i) const;
      unsigned    nvalues() const;
      std::size_t sum() const;
      /// \}

      /// Get the value of the accumulator.
      /// \{
      const std::vector<std::size_t>& vect() const;
      const std::vector<std::size_t>& to_result() const;
      /// \}

      const value::set<V>& vset() const;

      /// Check whether this accu is able to return a result.
      /// Always true here.
      bool is_valid() const;

    protected:

      mln::histo::data<V> h_;
      std::size_t sum_;
    };

    template <typename V>
    std::ostream& operator<<(std::ostream& ostr, const histo<V>& h);

    namespace meta
    {

      /// Meta accumulator for histo.
      struct histo : public Meta_Accumulator< histo >
      {
	template <typename V>
	struct with
	{
	  typedef mln::accu::histo<V> ret;
	};
      };

    } // end of namespace mln::accu::meta




# ifndef MLN_INCLUDE_ONLY

    template <typename V>
    inline
    histo<V>::histo()
      : h_(),
	sum_(0)
    {
    }

    template <typename V>
    inline
    void
    histo<V>::take(const argument& t)
    {
      ++h_[h_.vset().index_of(t)];
      ++sum_;
    }

    template <typename V>
    inline
    void
    histo<V>::take(const histo<V>& other)
    {
      for (unsigned i = 0; i < h_.nvalues(); ++i)
	h_[i] += other.h_[i];
      sum_ += other.sum_;
    }

    template <typename V>
    inline
    void
    histo<V>::untake(const argument& t)
    {
      mln_precondition(h_[h_.vset().index_of(t)] > 0);
      mln_precondition(sum_ > 0);
      --h_[h_.vset().index_of(t)];
      --sum_;
    }

    template <typename V>
    inline
    void
    histo<V>::init()
    {
      h_.clear();
    }

    template <typename V>
    inline
    std::size_t
    histo<V>::operator()(const argument& t) const
    {
      return h_[h_.vset().index_of(t)];
    }

    template <typename V>
    inline
    std::size_t
    histo<V>::operator[](unsigned i) const
    {
      mln_precondition(i < h_.vset().nvalues());
      return h_[i];
    }

    template <typename V>
    inline
    unsigned
    histo<V>::nvalues() const
    {
      return h_.vset().nvalues();
    }

    template <typename V>
    inline
    std::size_t
    histo<V>::sum() const
    {
      return sum_;
    }

    template <typename V>
    inline
    const std::vector<std::size_t>&
    histo<V>::vect() const
    {
      return h_.vect();
    }

    template <typename V>
    inline
    const std::vector<std::size_t>&
    histo<V>::to_result() const
    {
      return this->vect();
    }

    template <typename V>
    inline
    const value::set<V>&
    histo<V>::vset() const
    {
      return h_.vset();
    }

    template <typename V>
    inline
    std::ostream& operator<<(std::ostream& ostr, const histo<V>& h)
    {
      mln_viter(value::set<V>) v(h.vset());
      for_all(v)
	if (h(v) != 0)
	  ostr << v << ':' << h(v) << ' ';
      return ostr;
    }

    template <typename V>
    inline
    bool
    histo<V>::is_valid() const
    {
      return true;
    }

# endif // ! MLN_INCLUDE_ONLY

  } // end of namespace mln::accu

} // end of namespace mln


#endif // ! MLN_ACCU_HISTO_HH
