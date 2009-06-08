// Copyright (C) 2007, 2008, 2009 EPITA Research and Development Laboratory
// (LRDE)
//
// This file is part of the Milena Library.  This library is free
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

#ifndef MLN_MORPHO_ATTRIBUTE_MEAN_HH
# define MLN_MORPHO_ATTRIBUTE_MEAN_HH

/// \file mln/morpho/attribute/mean.hh
///
/// Define an accumulator that computes the mean of a
/// component.
///

# include <mln/accu/internal/base.hh>
# include <mln/accu/stat/mean.hh>
# include <mln/util/pix.hh>

namespace mln {

  // Forward declaration.

  namespace morpho {
    namespace attribute {
      template <typename I> class mean;
    }
  }

  // Traits

  namespace trait {
    template <typename I>
    struct accumulator_< morpho::attribute::mean<I> >
    {
      typedef accumulator::has_untake::no	has_untake;
      typedef accumulator::has_set_value::yes	has_set_value;
      typedef accumulator::has_stop::no		has_stop;
      typedef accumulator::when_pix::use_v	when_pix;
    };
  }


  namespace morpho {

    namespace attribute {

      namespace internal {

	template <typename T, typename S>
	struct sum_set_value : public mln::accu::sum<T, S>
	{
	  void set_value(const S& s);
	};
	
      }

      /// Morphological (i.e. for pixel and pixel values) accumulator calculating mean.
      /// Valid for leveling algorithm.
      template <typename I>
      class mean: public mln::accu::internal::base<mln_sum(mln_value(I)), mean<I> >
      {
      public:
	typedef util::pix<I> argument;
	typedef mln_value(I) value;
	typedef mln_sum(value) result;

	mean();

	/// Manipulators.
	/// \{
	void init();
        void take(const value& v);
	void take(const argument& t);
	void take(const mean<I>& other);

	void set_value(const result& r);
	/// \}

        /// Get the value of the accumulator.
	result to_result() const;

	/// Check whether this accu is able to return a result.
	/// Always true here.
	bool is_valid() const;

      protected:
	
	accu::count<value> count_;
	internal::sum_set_value<value,result>   sum_;
      };
    
# ifndef MLN_INCLUDE_ONLY

      namespace internal {

	template <typename T, typename S>
	void
	sum_set_value<T, S>::set_value(const S& s)
	{
	  this->s_ = s;
	};
	
      }

      template <typename I>
      inline
      mean<I>::mean()
      {
	init();
      }

      template <typename I>
      inline
      void
      mean<I>::init()
      {
	count_.init();
	sum_.init();
      }
      
      template <typename I>
      inline
      void
      mean<I>::take(const value& t)
      {
	count_.take(t);
	sum_.take(t);
      }
      
      template <typename I>
      inline
      void
      mean<I>::take(const argument& t)
      {
	take(t.v());
      }
      
      template <typename I>
      inline
      void
      mean<I>::take(const mean<I>& other)
      {
	count_.take(other.count_);
	sum_.take(other.sum_);
      }
      
      template <typename I>
      void
      mean<I>::set_value(const result& r)
      {
	sum_.set_value(r * count_.to_result());
      };

      template <typename I>
      inline
      typename mean<I>::result
      mean<I>::to_result() const
      {
	unsigned n = count_.to_result();
	return sum_.to_result() / n;
      }
      
      template <typename I>
      inline
      bool
      mean<I>::is_valid() const
      {
	return count_.to_result() != 0;
      }
      
# endif // ! MLN_INCLUDE_ONLY
      
    } // end of namespace mln::morpho::attribute

  } // end of namespace mln::morpho

} // end of namespace mln

#endif /* ! MLN_MORPHO_ATTRIBUTE_MEAN_HH */
