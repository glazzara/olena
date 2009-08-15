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

#ifndef MLN_ACCU_STAT_MAX_H_HH
# define MLN_ACCU_STAT_MAX_H_HH

/// \file
///
/// Define a generic max accumulator class based on histogram.

# include <mln/accu/internal/base.hh>
# include <mln/core/concept/meta_accumulator.hh>
# include <mln/accu/histo.hh>
# include <mln/util/pix.hh>


namespace mln
{

  // Forward declaration.
  namespace accu {
    namespace stat {
      template <typename V> struct max_h;
    }
  }


  // Traits.

  namespace trait
  {

    template <typename V>
    struct accumulator_< accu::stat::max_h<V> >
    {
      typedef accumulator::has_untake::yes   has_untake;
      typedef accumulator::has_set_value::no has_set_value;
      typedef accumulator::has_stop::no      has_stop;
      typedef accumulator::when_pix::use_v   when_pix;
    };

  } // end of namespace mln::trait


  namespace accu
  {

    namespace meta
    {

      namespace stat
      {

	/// Meta accumulator for max.
	struct max_h : public Meta_Accumulator< max_h >
	{
	  template <typename T>
	  struct with
	  {
	    typedef accu::stat::max_h<T> ret;
	  };
	};

      } // end of namespace mln::accu::meta::stat

    } // end of namespace mln::accu::meta


    namespace stat
    {

      /// \brief Generic max function based on histogram over a value set with
      /// type \c V.
      ///
      /// \ingroup modaccuvalues
      //
      template <typename V>
      struct max_h : public mln::accu::internal::base< const V&, max_h<V> >
      {
	typedef V argument;

	max_h();

	/// Manipulators.
	/// \{
	void init();
	void   take(const argument& t);
	void   take_as_init_(const argument& t);
	void   take(const max_h<V>& other);
	void untake(const argument& t);
	/// \}

	unsigned card() const { return h_.sum(); }

	/// Get the value of the accumulator.
	const argument& to_result() const;

	const accu::histo<V>& histo() const;

	/// Check whether this accu is able to return a result.
	/// Always true here.
	bool is_valid() const;

	void debug_print_() const;

      protected:

	mutable accu::histo<V> h_;
	const value::set<V>& s_; // derived from h_

	mutable unsigned sum_; // number of taken values > t_ 
	mutable unsigned i_; // the current max index ('current' means 'last known')
	mutable argument t_; // the current max argument

	mutable bool valid_; // validity of the current indent / argument
	                     // when valid_ is false, an update of i_ and t_ is required

	// Dev note: we can have at the same time (sum_ == 0) and
	// (valid_ == false) because of the 'untake' method.

	// Auxiliary methods
	void update_() const;
	void go_minus_() const;
	void go_plus_() const;

	void invariant_() const;
      };


      template <typename I> struct max_h< util::pix<I> >;

# ifndef MLN_INCLUDE_ONLY

      template <typename V>
      inline
      void
      max_h<V>::invariant_() const
      {
	// valid_ => (sum_ == 0)
	mln_invariant(! valid_ || (sum_ == 0));
      }

      template <typename V>
      inline
      max_h<V>::max_h()
	: h_(),
	  s_(h_.vset())
      {
	init();
	invariant_();
      }

      template <typename V>
      inline
      void
      max_h<V>::take(const argument& t)
      {
	if (h_.sum() == 0)
	  {
	    this->take_as_init_(t);
	    return;
	  }
	h_.take(t);
	if (t > t_)
	  {
	    ++sum_;
	    valid_ = false;
	  }
	invariant_();
      }

      template <typename V>
      inline
      void
      max_h<V>::take(const max_h<V>& other)
      {
	// h_
	h_.take(other.h_);
	for (unsigned i = this->card() - 1; i > i_; --i)
	  sum_ += other.h_[i];
	if (valid_ && sum_ != 0)
	  valid_ = false;
	// FIXME: Optimize.
	invariant_();
      }

      template <typename V>
      inline
      void
      max_h<V>::untake(const argument& t)
      {
	mln_precondition(h_(t) != 0);
	h_.untake(t);
	if (h_.sum() == 0)
	  {
	    init();
	    return;
	  }
	if (t > t_)
	  {
	    mln_invariant(sum_ >= 1);
	    --sum_;
	    valid_ = false;
	  }
	else
	  if (t == t_ && h_[i_] == 0)
	    valid_ = false;
	invariant_();
      }

      template <typename V>
      inline
      void
      max_h<V>::update_() const
      {
	if (sum_ != 0)
	  go_plus_();
	else
	  if (h_[i_] == 0)
	    go_minus_();
	valid_ = true;

	mln_postcondition(sum_ == 0);
	mln_postcondition(h_[i_] != 0);
	for (unsigned j = i_ + 1; j < h_.nvalues(); ++j)
	  mln_postcondition(h_[j] == 0);
      }

      template <typename V>
      inline
      void
      max_h<V>::go_plus_() const
      {
	do
	  {
	    ++i_;
	    if (h_[i_] != 0)
	      sum_ -= h_[i_];
	  }
	while (sum_ != 0);
	t_ = s_[i_];
      }

      template <typename V>
      inline
      void
      max_h<V>::go_minus_() const
      {
	do
	  --i_;
	while (h_[i_] == 0);
	t_ = s_[i_];
      }

      template <typename V>
      inline
      void
      max_h<V>::init()
      {
	h_.init();
	sum_ = 0;
	i_ = mln_min(argument);
	t_ = s_[i_];
	valid_ = true;
      }

      template <typename V>
      inline
      void
      max_h<V>::take_as_init_(const argument& t)
      {
	h_.take(t);
	sum_ = 0;
	i_ = s_.index_of(t);
	t_ = t;
	valid_ = true;
      }

      template <typename V>
      inline
      const typename max_h<V>::argument&
      max_h<V>::to_result() const
      {
	if (! valid_)
	  update_();
	invariant_();
	return t_;
      }

      template <typename V>
      inline
      const accu::histo<V>&
      max_h<V>::histo() const
      {
	return h_;
      }

      template <typename V>
      inline
      bool
      max_h<V>::is_valid() const
      {
	return true;
      }

      template <typename V>
      inline
      void
      max_h<V>::debug_print_() const
      {
	std::cout << "h={" << h_ << "} h.sum = " << h_.sum() << ' ';
	std::cout << "sum=" << sum_ << ' '
		  << "valid=" << valid_ << ' '
		  << "i=" << i_ << ' '
		  << "t=" << t_ << std::endl;
      }

      template <typename V>
      inline
      std::ostream& operator<<(std::ostream& ostr, const max_h<V>& m)
      {
	return ostr << m.to_result();
      }

# endif // ! MLN_INCLUDE_ONLY

    } // end of namespace mln::accu::stat

  } // end of namespace mln::accu

} // end of namespace mln


#endif // ! MLN_ACCU_STAT_MAX_H_HH
