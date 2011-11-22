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

#ifndef MLN_MORPHO_ATTRIBUTE_VOLUME_HH
# define MLN_MORPHO_ATTRIBUTE_VOLUME_HH

/// \file
///
/// Define an accumulator that computes the volume of a
/// component.

# include <mln/accu/internal/base.hh>
# include <mln/math/diff_abs.hh>
# include <mln/util/pix.hh>


namespace mln
{

  // Forward declaration.
  namespace morpho {
    namespace attribute {
      template <typename I> struct volume;
    }
  }


  // Traits.

  namespace trait
  {

    template <typename I>
    struct accumulator_< morpho::attribute::volume<I> >
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

      /// Volume accumulator class.
      ///
      /// The parameter \p I is the image type on which the accumulator
      /// of pixels is built.
      template <typename I>
      struct volume
	: public mln::accu::internal::base< unsigned , volume<I> >
      {
	typedef mln_value(I) argument;

	volume();

	/// Manipulators.
	/// \{
	void init();

	void take(const mln_value(I)& v);
	void take(const util::pix<I>& px);
	void take(const volume<I>& other);

	void take_as_init_(const mln_value(I)& v);
	void take_as_init_(const util::pix<I>& px);
	/// \}

	/// Get the value of the accumulator.
	unsigned to_result() const;

	/// Check whether this accu is able to return a result.
	/// Always true here.
	bool is_valid() const;

	/// Give the area.
	unsigned area() const;

      protected:
	/// The current level.
	mln_value(I) cur_level_;
	/// The area of the component.
	unsigned area_;
	/// The volume of the component.
	unsigned volume_;
      };



# ifndef MLN_INCLUDE_ONLY

      template <typename I>
      inline
      volume<I>::volume()
      {
	init();
      }

      template <typename I>
      inline
      void
      volume<I>::init()
      {
	volume_ = 0;
      }

      template <typename I>
      inline
      void
      volume<I>::take(const mln_value(I)& v)
      {
	mln_invariant(volume_ != mln_max(unsigned));
	if (! is_valid())
	  {
	    take_as_init_(v);
	    return;
	  }
	++area_;
	volume_ += 1 + math::diff_abs(v, cur_level_);
	cur_level_ = v;
      }

      template <typename I>
      inline
      void
      volume<I>::take(const util::pix<I>& px)
      {
	mln_invariant(volume_ != mln_max(unsigned));
	take(px.v());
      }

      template <typename I>
      inline
      void
      volume<I>::take(const volume<I>& other)
      {
	mln_invariant(volume_ != mln_max(unsigned));
	area_ += other.area_;
	volume_ +=
	  other.volume_  +
	  other.area_ * math::diff_abs(other.cur_level_, cur_level_);
	// cur_level_ do not change.
      }

      template <typename I>
      inline
      void
      volume<I>::take_as_init_(const mln_value(I)& v)
      {
	cur_level_ = v;
	area_ = 1;
	volume_ = 1;
      }

      template <typename I>
      inline
      void
      volume<I>::take_as_init_(const util::pix<I>& px)
      {
	take_as_init_(px.v());
      }

      template <typename I>
      inline
      unsigned
      volume<I>::to_result() const
      {
	return volume_;
      }

      template <typename I>
      inline
      unsigned
      volume<I>::area() const
      {
	return area_;
      }

      template <typename I>
      inline
      bool
      volume<I>::is_valid() const
      {
	return volume_ != 0;
      }

# endif // ! MLN_INCLUDE_ONLY

    } // end of namespace mln::morpho::attribute

  } // end of namespace mln::morpho

} // end of namespace mln


#endif // ! MLN_MORPHO_ATTRIBUTE_VOLUME_HH
