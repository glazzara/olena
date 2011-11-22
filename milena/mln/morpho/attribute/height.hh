// Copyright (C) 2007, 2008, 2009, 2011 EPITA Research and Development
// Laboratory (LRDE)
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

#ifndef MLN_MORPHO_ATTRIBUTE_HEIGHT_HH
# define MLN_MORPHO_ATTRIBUTE_HEIGHT_HH

/// \file
/// Define an accumulator that computes the height of a
/// component through one of its pixels.

# include <mln/accu/internal/base.hh>

# include <mln/util/pix.hh>
# include <mln/math/diff_abs.hh>
# include <mln/math/min.hh>
# include <mln/math/max.hh>


namespace mln
{

  // Forward declaration.
  namespace morpho {
    namespace attribute {
      template <typename I> struct height;
    }
  }


  // Traits.

  namespace trait
  {

    template <typename I>
    struct accumulator_< morpho::attribute::height<I> >
    {
      typedef accumulator::has_untake::no    has_untake;
      typedef accumulator::has_set_value::no has_set_value;
      typedef accumulator::has_stop::no      has_stop;
      typedef accumulator::when_pix::use_v   when_pix;
    };

  } // end of namespace mln::trait


  namespace morpho
  {

    namespace attribute
    {

      /// Height accumulator class.
      ///
      /// The parameter \p I is the image type on which the accumulator
      /// of pixels is built.
      template <typename I>
      struct height
	: public mln::accu::internal::base< unsigned , height<I> >
      {
	typedef mln_value(I) argument;

	height();

	/// Manipulators.
	/// \{
	void init();
	void take(const mln_value(I)& v);
	void take(const util::pix<I>& v);
	void take(const height<I>& other);
	void take_as_init_(const mln_value(I)& v);
	void take_as_init_(const util::pix<I>& px);
	/// \}

	/// Check whether this accu is able to return a result.
	/// Always true here.
	bool is_valid() const;

	/// Get the value of the accumulator.
	unsigned to_result() const;

	/// Get base & current level of the accumulator.
	unsigned base_level() const;
	unsigned current_level() const;


      protected:
	/// The reference level in the component.
	unsigned ref_;
	/// The current level in the component.
	unsigned cur_;
	/// Mark of initialization
	bool initialized_;
      };


# ifndef MLN_INCLUDE_ONLY

      template <typename I>
      inline
      height<I>::height()
	: initialized_ (false)
      {
      }

      template <typename I>
      inline
      void
      height<I>::init()
      {
	mln_invariant(0);
      }

      template <typename I>
      inline
      void
      height<I>::take(const mln_value(I)& v)
      {
	if (!is_valid ())
	  {
	    take_as_init_(v);
	  }
	cur_ = v;
      }

      template <typename I>
      inline
      void
      height<I>::take(const util::pix<I>& px)
      {
	take(px.v());
      }

      template <typename I>
      inline
      void
      height<I>::take(const height<I>& other)
      {
	mln_invariant(((ref_ <= cur_) && (other.ref_ <= other.cur_))
		      || ((ref_ >= cur_) && (other.ref_ >= other.cur_)));

	if (!is_valid())
	  {
	    ref_ = other.ref_;
	    cur_ = other.cur_;
	  }
	else if (ref_ < cur_)
	  {
	    // Values are increasing.
	    ref_ = math::min(ref_, other.ref_);
	    cur_ = math::max(cur_, other.cur_);
	  }
	else
	  {
	    // Values are decreasing.
	    ref_ = math::max(ref_, other.ref_);
	    cur_ = math::min(cur_, other.cur_);
	  }
      }

      template <typename I>
      inline
      void
      height<I>::take_as_init_(const mln_value(I)& v)
      {
	cur_ = ref_ = v;
	initialized_ = true;
      }

      template <typename I>
      inline
      void
      height<I>::take_as_init_(const util::pix<I>& px)
      {
	take_as_init_(px.v());
      }


      template <typename I>
      inline
      unsigned
      height<I>::to_result() const
      {
	mln_invariant(is_valid());

	return math::diff_abs(ref_, cur_);
      }

      template <typename I>
      inline
      unsigned
      height<I>::base_level() const
      {
	return ref_;
      }

      template <typename I>
      inline
      unsigned
      height<I>::current_level() const
      {
	return cur_;
      }


      template <typename I>
      inline
      bool
      height<I>::is_valid() const
      {
	return initialized_;
      }

# endif // ! MLN_INCLUDE_ONLY

    } // end of namespace mln::morpho::attribute

  } // end of namespace mln::morpho

} // end of namespace mln


#endif // ! MLN_MORPHO_ATTRIBUTE_HEIGHT_HH
