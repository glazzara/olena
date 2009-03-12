// Copyright (C) 2007, 2008, 2009 EPITA Research and Development Laboratory
// (LRDE)
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

#ifndef MLN_MORPHO_TREE_ACCUMULATOR_MAX_HH_
# define MLN_MORPHO_TREE_ACCUMULATOR_MAX_HH_

/// \file mln/morpho/tree/accumulator/max.hh
///
/// Define an accumulator that returns iterator matching the max node value.


# include <mln/core/concept/image.hh>
# include <mln/accu/internal/base.hh>

namespace mln {
  namespace morpho {
    namespace tree{
      namespace accumulator {

	template <typename T, typename I>
	class max : public mln::accu::internal::base< mln_bkd_piter(T::nodes_t), max<T, I> >
	{
	  typedef mln::accu::internal::base< unsigned, max<T, I> > super_;

	public:
	  typedef mln_bkd_piter(T::nodes_t) argument;

	  /// Constructor
	  max(const Image<I>& f);

	  /// Manipulators.
	  /// \{
	  void init();

	  void take(const argument& it);
	  void take(const max<T, I>& other);

	  void take_as_init(const argument& it);
	  /// \}

	  /// Get the value of the accumulator.
	  mln_bkd_piter(T::nodes_t) to_result() const;

	  /// Check whether the accumulator is able to give a result.
	  bool is_valid() const;

	private:
	  const I& f_;
	  mln_bkd_piter(T::nodes_t) max_;
	};

# ifndef MLN_INCLUDE_ONLY

	template <typename T, typename I>
	inline
	max<T, I>::max(const Image<I>& f) :
	  f_ (exact(f))
	{
	}

	template <typename T, typename I>
	inline
	void
	max<T, I>::init()
	{
	}

	template <typename T, typename I>
	inline
	void
	max<T, I>::take(const argument& it)
	{
	  mln_invariant(it.is_valid());

	  if (!is_valid())
	    {
	      take_as_init(it);
	      return;
	    }

	  if (f_(it) > f_(max_))
	    max_ = it;
	}

 	template <typename T, typename I>
	inline
	void
	max<T, I>::take(const max<T, I>& other)
	{
	  mln_invariant(other.is_valid());

	  if (f_(other.max_) > f_(max_))
	    max_ = other.max_;
	}

    	template <typename T, typename I>
	inline
	void
	max<T, I>::take_as_init(const argument& it)
	{
	  mln_invariant(it.is_valid());
	  max_ = it;
	}

	template <typename T, typename I>
	inline
	mln_bkd_piter(T::nodes_t)
	max<T, I>::to_result() const
	{
	  return max_;
	}

	template <typename T, typename I>
	inline
	bool
	max<T, I>::is_valid() const
	{
	  return max_.is_valid();
	}


# endif // ! MLN_INCLUDE_ONLY

      }  // end of namespace mln::morpho::tree::accumulator
    }  // end of namespace mln::morpho::tree
  }  // end of namespace mln::morpho
} // end of namespace mln

#endif /* ! MLN_MORPHO_TREE_ACCUMULATOR_MAX_HH_ */
