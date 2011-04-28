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

#ifndef MLN_MORPHO_ATTRIBUTE_SHARPNESS_HH
# define MLN_MORPHO_ATTRIBUTE_SHARPNESS_HH

/// \file
///
/// Define an accumulator that computes the sharpness of a
/// component. (sharpness = volume of the component / volume of box bounding the component.

# include <mln/core/concept/accumulator.hh>
# include <mln/accu/internal/base.hh>
# include <mln/morpho/attribute/volume.hh>
# include <mln/morpho/attribute/height.hh>


namespace mln
{

  // Forward declaration.
  namespace morpho {
    namespace attribute {
      template <typename I>
      struct sharpness;
    }
  }


  // Traits.

  namespace trait
  {

    template <typename I>
    struct accumulator_< morpho::attribute::sharpness<I> >
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

      /// Sharpness accumulator class.
      ///
      /// The parameter \p I is the image type on which the accumulator
      /// of pixels is built.
      template <typename I>
      struct sharpness
	: public mln::accu::internal::base< double, sharpness<I> >
      {
	typedef mln_value(I) argument;

	sharpness();

	/// Manipulators.
	/// \{
	void init();

	void take(const mln_value(I)& v);
	void take(const sharpness<I>& other);

	void take_as_init_(const mln_value(I)& v);
	/// \}

	/// Get the value of the accumulator.
	double to_result() const;

	/// Check whether this accu is able to return a result.
	/// Always true here.
	bool is_valid() const;

	/// Give the area of the component.
	unsigned area() const;

	/// Give the height.
	unsigned height() const;

	/// Give the volume of the component.
	unsigned volume() const;

      protected:
	/// The height of the component.
	typename mln::morpho::attribute::height<I> height_;
	/// The volume of the component.
	typename mln::morpho::attribute::volume<I> volume_;
      };



# ifndef MLN_INCLUDE_ONLY

      template <typename I>
      inline
      sharpness<I>::sharpness()
      {
	init();
      }

      template <typename I>
      inline
      void
      sharpness<I>::init()
      {
	volume_.init();
      }

      template <typename I>
      inline
      void
      sharpness<I>::take(const mln_value(I)& v)
      {
	if (! is_valid())
	  {
	    take_as_init_(v);
	    return;
	  }
	volume_.take(v);
	height_.take(v);
      }

      template <typename I>
      inline
      void
      sharpness<I>::take(const sharpness<I>& other)
      {
	mln_invariant(is_valid());
	volume_.take(other.volume_);
	height_.take(other.height_);
      }

      template <typename I>
      inline
      void
      sharpness<I>::take_as_init_(const mln_value(I)& v)
      {
	volume_.take_as_init_(v);
	height_.take_as_init_(v);
      }

      template <typename I>
      inline
      double
      sharpness<I>::to_result() const
      {
	double d = 0;
	if (height_.to_result() != 0)
	  {
	    d = (double) volume_.to_result()  /
	      (double)((volume_.area()  -  1) * (height_.to_result() + 1) + 1);
	  }
	mln_postcondition(d >= 0 && d <= 1);
	return d;
      }

      template <typename I>
      inline
      unsigned
      sharpness<I>::area() const
      {
	return volume_.area();
      }

      template <typename I>
      inline
      unsigned
      sharpness<I>::volume() const
      {
	return volume_.to_result();
      }

      template <typename I>
      inline
      unsigned
      sharpness<I>::height() const
      {
	return height_.to_result();
      }

      template <typename I>
      inline
      bool
      sharpness<I>::is_valid() const
      {
	return volume_.is_valid() && height_.is_valid();
      }

# endif // ! MLN_INCLUDE_ONLY

    } // end of namespace mln::morpho::attribute

  } // end of namespace mln::morpho

} // end of namespace mln


#endif // ! MLN_MORPHO_ATTRIBUTE_SHARPNESS_HH
