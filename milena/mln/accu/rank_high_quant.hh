// Copyright (C) 2007, 2008, 2009 EPITA Research and Development Laboratory (LRDE)
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

#ifndef MLN_ACCU_RANK_HIGH_QUANT_HH
# define MLN_ACCU_RANK_HIGH_QUANT_HH

/// \file
///
/// Define an rank accumulator.

# include <vector>
# include <mln/accu/internal/base.hh>
# include <mln/core/concept/meta_accumulator.hh>
# include <mln/trait/value_.hh>
# include <mln/util/pix.hh>


namespace mln
{

  namespace accu
  {


    /// \brief Generic rank accumulator class.
    /*!
     * The parameter \c T is the type of values.
     *
     * \ingroup modaccuvalues
     */
    template <typename T>
    struct rank : public mln::accu::internal::base< const T&, rank<T> >
    {
      typedef T argument;

      rank(unsigned k, unsigned n);

      /// Manipulators.
      /// \{
      void init();
      void take_as_init_(const argument& t);
      void take(const argument& t);
      void take(const rank<T>& other);
      void sort();
      /// \}

      /// Get the value of the accumulator.
      const T& to_result() const;

      /// Check whether this accu is able to return a result.
      /// Always true here.
      bool is_valid() const;

    protected:

      std::vector<T> elts_;
      bool is_sorted_;
      unsigned k_; // 0 <= k_ < n
      unsigned n_;
    };


    template <typename I> struct rank< util::pix<I> >;


    namespace meta
    {

      /// Meta accumulator for rank.

      struct rank : public Meta_Accumulator< rank >
      {
	rank(unsigned k_, unsigned n_) : k(k_), n(n_) {}

	template <typename T>
	  struct with
	  {
	    typedef accu::rank<T> ret;
	  };

	unsigned k;
	unsigned n;
      };

    }


    template <typename T>
    rank<T> unmeta(const meta::rank& m, T)
    {
      rank<T> a(m.k, m.n);
      return a;
    }


# ifndef MLN_INCLUDE_ONLY

    template <typename T>
    inline
    rank<T>::rank(unsigned k, unsigned n)
      : k_(k),
	n_(n),
	is_sorted_(false)
    {
      mln_assertion(k_ < n_);
      init();
    }

    template <typename T>
    inline
    void
    rank<T>::init()
    {
      elts_.clear();
    }

    template <typename T>
    inline
    void rank<T>::take_as_init_(const argument& t)
    {
      elts_.push_back(t);
      is_sorted_ = false;
    }

    template <typename T>
    inline
    void rank<T>::take(const argument& t)
    {
      elts_.push_back(t);
      is_sorted_ = false;
    }

    template <typename T>
    inline
    void
    rank<T>::take(const rank<T>& other)
    {
      elts_.insert(elts_.end(),
		   other.elts_.begin(),
		   other.elts_.end());
      is_sorted_ = false;
    }

    template <typename T>
    inline
    const T&
    rank<T>::to_result() const
    {
      const_cast<rank<T>&>(*this).sort();

      if (n_ == elts_.size())
	return elts_[k_];
      else
	// FIXME : This alternative is used to handle images edges.
	return elts_[(elts_.size() * k_) / n_];
    }

    template <typename T>
    inline
    bool
    rank<T>::is_valid() const
    {
      return true;
    }

    template <typename T>
    inline
    void
    rank<T>::sort()
    {
      if (! is_sorted_)
      {
	is_sorted_ = true;
	std::sort(elts_.begin(), elts_.end());
      }
    }

# endif // ! MLN_INCLUDE_ONLY

  } // end of namespace mln::accu

} // end of namespace mln

#include <mln/accu/rank_bool.hh>

#endif // ! MLN_ACCU_RANK_HIGH_QUANT_HH
