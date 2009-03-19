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

#ifndef MLN_MORPHO_TREE_ACCUMULATOR_ARG_MAX_HH_
# define MLN_MORPHO_TREE_ACCUMULATOR_ARG_MAX_HH_

/// \file mln/morpho/tree/accumulator/arg_max.hh
///
/// Define an accumulator that performs arg max.


# include <mln/core/concept/image.hh>
# include <mln/accu/internal/base.hh>
# include <mln/util/pix.hh>

namespace mln {
  namespace accumulator {

	template <typename I>
	class arg_max : public mln::accu::internal::base<const mln_psite(I)&, arg_max<I> >
	{
	  typedef mln::accu::internal::base<const mln_psite(I)&, arg_max<I> > super_;

	public:
	  typedef typename util::pix<I> argument;

	  /// Constructor
	  arg_max(const Image<I>& ima);
	  /// Destructor
	  ~arg_max();

	  /// Manipulators.
	  /// \{
	  void init();

	  void take(const argument& pix);
	  void take(const arg_max<I>& other);

	  void take_as_init(const argument& pix);
	  /// \}

	  /// Get the value of the accumulator.
	  const mln_psite(I)& to_result() const;

	  /// Check whether the accumulator is able to give a result.
	  bool is_valid() const;

	private:
	  const I& ima_;
	  mln_psite(I)* p_;
	};

# ifndef MLN_INCLUDE_ONLY

	template <typename I>
	inline
	arg_max<I>::arg_max(const Image<I>& ima) :
	  ima_ (exact(ima)),
	  p_ (0)
	{
	}

	template <typename I>
	inline
	arg_max<I>::~arg_max()
	{
	  delete p_;
	}


	template <typename I>
	inline
	void
	arg_max<I>::init()
	{
	}

	template <typename I>
	inline
	void
	arg_max<I>::take(const argument& pix)
	{
	  if (!is_valid())
	    {
	      take_as_init(pix);
	      return;
	    }

	  if (pix.v() > ima_(*p_))
	    *p_ = pix.p();
	}

 	template <typename I>
	inline
	void
	arg_max<I>::take(const arg_max<I>& other)
	{
	  mln_invariant(other.is_valid());

	  if (other.ima_(*other.p_) > ima_(*p_))
	    *p_ = *other.p_;
	}

    	template <typename I>
	inline
	void
	arg_max<I>::take_as_init(const argument& pix)
	{
	  p_ = new mln_psite(I)(pix.p());
	}

	template <typename I>
	inline
	const mln_psite(I)&
	arg_max<I>::to_result() const
	{
	  mln_invariant(p_ != 0);
	  return *p_;
	}

	template <typename I>
	inline
	bool
	arg_max<I>::is_valid() const
	{
	  return p_ != 0;
	}


# endif // ! MLN_INCLUDE_ONLY

      }  // end of namespace mln::accumulator
    }  // end of namespace mln

#endif /* ! MLN_MORPHO_TREE_ACCUMULATOR_ARG_MAX_HH_ */
