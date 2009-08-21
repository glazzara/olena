// Copyright (C) 2007 EPITA Research and Development Laboratory (LRDE)
// Copyright (C) 2008 EPITA Research and Development Laboratory (LRDE)
// Copyright (C) 2009 EPITA Research and Development Laboratory (LRDE)
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

#ifndef MLN_ACCU_HISTO1D_HH
#define MLN_ACCU_HISTO1D_HH

/// \file
///
/// \brief Define an histogram as an accumulator which returns an image1d .
///
/// This implementation is the very easiest. It's the discrete one. What about
/// a continue version ? How finding the correct bin number ?? What about the
/// subsampling process ?? Shall we estimate the density ?? In this case, which
/// method shall we choose, kernel version or k-neareast-neighbours ??
///

#include <iostream>

#include <mln/accu/internal/base.hh>

#include <mln/core/macros.hh>
#include <mln/core/image/image1d.hh>

#include <mln/trait/value/comp.hh>

#include <mln/arith/plus.hh>

#include <mln/trace/entering.hh>
#include <mln/trace/exiting.hh>

#include <mln/value/ops.hh>

namespace mln
{
  
  namespace accu
  {
    
    namespace stat
    {
   
    // Forward declaration
      template <typename V>
      struct histo1d;

    } // end of namespace mln::accu::stat
  
  } // end of namespace mln::accu


  namespace trait
  {
    
    template <typename V>
    struct accumulator_< mln::accu::stat::histo1d<V> >
    {
      typedef accumulator::has_untake::no    has_untake;
      typedef accumulator::has_set_value::no has_set_value;
      typedef accumulator::has_stop::no      has_stop;
      typedef accumulator::when_pix::use_v   when_pix;
    };

    template <typename V>
    struct set_precise_binary_<op::eq, 
			       accu::stat::histo1d<V>,
			       accu::stat::histo1d<V> >
    {
      typedef bool ret;
    };
        
  } // end of namespace mln::trait

  namespace accu
  {
    
    namespace stat
    {      

      /// \brief Define an histogram which returns an image1d .  
      ///
      /// Param V defines the space in which we count the values.
      /// For instance, this histogram works image2d<gl8> or image1d<gl16>.
      /// The histogram count the occurrence of each value.
      ///
      /// \ingroup modaccuvalues

      template <typename V>
      struct histo1d : 
	public mln::accu::internal::base<image1d<unsigned>, histo1d<V> >
      {
	typedef V                 argument;
	typedef image1d<unsigned> result;
	typedef result            q_result;

	/// Constructors
	/// \{
	/// \brief Initialize the size of the resulting image1d.
	histo1d();
	/// \}

	
	/// Manipulators.
	/// \{
	/// \brief Initialize the histogram with zero value.
	void init();

	
	/// \brief Update the histogram with the RGB pixel t.
	/// \param[in] t a RGB pixel.
	void take(const argument& t);


	/// \brief Update the histogram with an other histogram.
	/// \param[in] other the other histogram.
	void take(const histo1d<V>& other);
	/// \}

	/// Accessors.
	/// \{
	/// \brief Return the histogram as an image1d.
	result to_result() const;
	operator result () const;
	/// \}

	/// \brief Check whethever this accumulator is able to return a result.
	/// Always in a safe state and then always return true;
	bool is_valid() const;

      protected:
	result  count_;
      };

      /// \brief Check wethever an histogram is equal to an other one.
      /// The comparaison states that all the bins share the same value.
      /// \param[in] histo1 the first histogram to compare with.
      /// \param[in] histo2 the second histogram.
      template <typename V>
      bool operator==(const histo1d<V>& histo1, const histo1d<V>& histo2);

#ifndef MLN_INCLUDE_ONLY




      template <typename V>
      inline
      histo1d<V>::histo1d()
      {
	trace::entering("mln::accu::stat::histo1d<V>::histo1d");
	typedef mln_trait_value_comp(V,0) comp;
	
	// comp keep a trace of the dimension of the theorical image.
	// mln_min(comp) --> 0
	// mln_max(comp) --> 2^(n-1) [127 for n=7][255 for n=8] ...
	count_.init_(box1d(point1d(mln_min(comp)),
			   point1d(mln_max(comp))));
	
	trace::exiting("mln::accu::stat::histo1d<V>::histo1d");
      }

      template <typename V>
      inline
      void histo1d<V>::init()
      {
	trace::entering("mln::accu::stat::histo1d<V>::init");

	data::fill(count_, 0);
	trace::exiting("mln::accu::stat::histo1d<V>::init");
      }

      template <typename V>
      inline
      void histo1d<V>::take(const argument& t)
      {
	trace::entering("mln::accu::stat::histo1d<V>::take");

	// Just convert a greyscale value (int_u8 like) to a position for an 
	// iterator on the resulting image.
	++count_(point1d(t));

	trace::exiting("mln::accu::stat::histo1d<V>::take");
      }

      
      template <typename V>
      inline
      void histo1d<V>::take(const histo1d<V>& other)
      {
	trace::entering("mln::accu::stat::histo1d<V>::take");

	count_ += other.count_;

	trace::exiting("mln::accu::stat::histo1d<V>::take");
      }
      
      template <typename V>
      inline
      typename histo1d<V>::result histo1d<V>::to_result() const
      {
	trace::entering("mln::accu::stat::histo1d<V>::to_result");

	trace::exiting("mln::accu::stat::histo1d<V>::to_result");
	return count_;
      }
      
      template <typename V>
      inline
      histo1d<V>::operator result() const
      {
	trace::entering("mln::accu::stat::histo1d<V>::operator result");

	trace::exiting("mln::accu::stat::histo1d<V>::operator result");
	return count_;
      }      

      template <typename V>
      inline
      bool histo1d<V>::is_valid() const
      {
	trace::entering("mln::accu::stat::histo1d<V>::is_valid");

	trace::exiting("mln::accu::stat::histo1d<V>::is_valid");
	return true;
      }

      template <typename V>
      bool operator==(const histo1d<V>& histo1, const histo1d<V>& histo2)
      {
	trace::entering("mln::accu::stat::operator==");

	bool  result                  = true;
	const image1d<unsigned>& res1 = histo1.to_result();
	const image1d<unsigned>& res2 = histo2.to_result();
	
	mln_precondition(res1.is_valid());
	mln_precondition(res2.is_valid());

	mln_piter(image1d<unsigned>)      p1(res1.domain());
	mln_piter(image1d<unsigned>)      p2(res2.domain());

	for_all_2(p1, p2)
	  result &= (res1(p1) == res2(p2));

	trace::exiting("mln::accu::stat::operator==");
	return result;
      }


#endif // ! MLN_INCLUDE_ONLY


    } // end of namespace mln::accu::stat

  } // end of namespace mln::accu

} // end of namespace mln

#endif // ! MLN_ACCU_HISTO1D_HH
