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

#ifndef MLN_ACCU_SHAPE_VOLUME_HH
# define MLN_ACCU_SHAPE_VOLUME_HH

/// \file
/// Define an accumulator that computes the volume of a
/// component through one of its pixels.

/*!
This accumulator uses an mln::util::pix (pixel) to update the
reference level, area and volume information of the component.

The class mln/accu/volume is not a general-purpose accumulator;
it is used to implement volume-based connected filters.
\see mln::morpho::closing::volume
\see mln::morpho::opening::volume
*/

# include <mln/accu/internal/base.hh>
# include <mln/core/concept/meta_accumulator.hh>
# include <mln/math/diff_abs.hh>

# include <mln/util/pix.hh>
# include <mln/literal/zero.hh>

namespace mln
{

  namespace accu
  {

    namespace shape
    {

      /// \brief Volume accumulator class.
      ///
      /// The parameter \p I is the image type on which the accumulator
      /// of pixels is built.
      ///
      /// \ingroup modaccuimages
      template <typename I>
      struct volume
	: public mln::accu::internal::base< unsigned , volume<I> >
      {
	/// The accumulated data type.
	///
	/// The volume of component is represented by the volume of its
	/// root pixel.  See mln::morpho::closing_volume and
	/// mln::morpho::opening_volume for actual uses of this
	/// accumulator.
	/// FIXME: Replaced by mln::morpho::attribute::volume
	typedef util::pix<I> argument;
	/// The value type associated to the pixel type.
	typedef typename argument::value value;

	volume();

	/// Manipulators.
	/// \{
	void init();
	void take(const argument& pixel);
	void take(const volume<I>& other);

	/// Force the value of the counter to \a v.
	void set_value(unsigned v);
	/// \}

	/// Get the value of the accumulator.
	unsigned to_result() const;

	/// Check whether this accu is able to return a result.
	/// Always true here.
	bool is_valid() const;

      protected:
	/// The reference level (the level of the component's root).
	value ref_level__;
	/// The area of the component.
	unsigned area__;
	/// The volume of the component.
	unsigned volume_;
      };


    } // end of mln::accu::shape


    namespace meta
    {

      namespace shape
      {

	/// Meta accumulator for volume.

	struct volume : public Meta_Accumulator< volume >
	{
	  template <typename I>
	  struct with
	  {
	    typedef accu::shape::volume<I> ret;
	  };
	};

      } // end of namespace mln::accu::meta::shape

    } // end of namespace mln::accu::meta

# ifndef MLN_INCLUDE_ONLY

    namespace shape
    {

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
	ref_level__ = literal::zero;
	volume_ = 0;
	area__ = 0;
      }

      template <typename I>
      inline
      void
      volume<I>::take(const argument& pixel)
      {
	/* FIXME: Growing a component using this particular `take'
	routine won't work, since the update does not take care of
	the reference level to compute the new volume.  Maybe we
	could distinguish two cases:

	1. the empty accumulator case (which corresponds to the
	following code);
	2. the non-empty accumulator case (which sohuld act as in
	`take(const volume<I>& other)').

	Currently, the implementation is only valid if the
	accumulator was initialy empty before the call to
        `take(const argument&)'.  */
	ref_level__ = pixel.v();
	++area__;
	++volume_;
      }

      template <typename I>
      inline
      void
      volume<I>::take(const volume<I>& other)
      {
	area__ += other.area__;
	/* FIXME: Is it `t.area__' or `area__' ? Théo said it was
	the latter, but both the ISMM 2005 paper and Olena 0.11 use
	the former.  */
	volume_ +=
	  other.volume_  +
	  other.area__ * mln::math::diff_abs(other.ref_level__, ref_level__);
	// Member ref_level__ is not touched.
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
      void
      volume<I>::set_value(unsigned v)
      {
	volume_ = v;
	// Reset the other members.
	ref_level__ = literal::zero;
	area__ = 0;
      }

      template <typename I>
      inline
      bool
      volume<I>::is_valid() const
      {
	return true;
      }

    } // end of namespace mln::accu::shape

# endif // ! MLN_INCLUDE_ONLY

  } // end of namespace mln::accu

} // end of namespace mln


#endif // ! MLN_ACCU_SHAPE_VOLUME_HH
