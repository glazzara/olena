// Copyright (C) 2007, 2008, 2009 EPITA Research and Development Laboratory
// (LRDE)
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

#ifndef MLN_ACCU_SHAPE_HEIGHT_HH
# define MLN_ACCU_SHAPE_HEIGHT_HH

/// \file
///
/// Define an accumulator that computes the height of a
/// component through one of its pixels.
/*
This accumulator uses an mln::util::pix (pixel) to update the
height information of the component.

The class mln/accu/height is not a general-purpose accumulator;
it is used to implement height-based connected filters.
\see mln::morpho::closing::height
\see mln::morpho::opening::height
*/

# include <mln/accu/internal/base.hh>
# include <mln/core/concept/meta_accumulator.hh>

# include <mln/util/pix.hh>
# include <mln/math/min.hh>
# include <mln/math/max.hh>

namespace mln
{

  namespace accu
  {

    namespace shape
    {

      /// \brief Height accumulator.
      ///
      /// The parameter \p I is the image type on which the accumulator
      /// of pixels is built.
      ///
      /// \ingroup modaccuimages
      //
      template <typename I>
      struct height
	: public mln::accu::internal::base< unsigned , height<I> >
      {
	/// The accumulated data type.
	///
	/// The height of component is represented by the height of its
	/// root pixel. See mln::morpho::closing_height and
	/// mln::morpho::opening_height for actual uses of this
	/// accumulator.
	/// FIXME: Replaced by mln::morpho::attribute::height
	typedef util::pix<I> argument;
	/// The value type associated to the pixel type.
	typedef typename argument::value value;

	height();

	/// Manipulators.
	/// \{
	void init();
	void take(const argument&);
	void take(const height<I>& other);

	/// Force the value of the counter to \a h.
	void set_value(unsigned h);
	/// \}

	/// Get the value of the accumulator.
	unsigned to_result() const;

	/// Check whether this accu is able to return a result.
	/// Always true here.
	bool is_valid() const;

      protected:
	/// The minimum level in the component.
	value min_level__;
	/// The maximum level in the component.
	value max_level__;
	/// The height of the component.
	unsigned height_;
      };


    } // end of mln::accu::shape


    namespace meta
    {

      namespace shape
      {

	/// Meta accumulator for height.
	struct height : public Meta_Accumulator< height >
	{
	  template <typename I>
	  struct with
	  {
	    typedef accu::shape::height<I> ret;
	  };
	};

      } // end of namespace mln::accu::meta::shape

    } // end of namespace mln::accu::meta


# ifndef MLN_INCLUDE_ONLY

    namespace shape
    {

      template <typename I>
      inline
      height<I>::height()
      {
	init();
      }

      template <typename I>
      inline
      void
      height<I>::init()
      {
	min_level__ = mln_max(value);
	max_level__ = mln_min(value);
	height_ = 0;
      }

      template <typename I>
      inline
      void
      height<I>::take(const argument& t)
      {
	min_level__ = mln::math::min(min_level__, t.v());
	max_level__ = mln::math::max(max_level__, t.v());
	height_ = max_level__ - min_level__;
      }

      template <typename I>
      inline
      void
      height<I>::take(const height<I>& other)
      {
	min_level__ = mln::math::min(min_level__, other.min_level__);
	max_level__ = mln::math::max(max_level__, other.max_level__);
	height_ = max_level__ - min_level__;
      }

      template <typename I>
      inline
      unsigned
      height<I>::to_result() const
      {
	return height_;
      }

      template <typename I>
      inline
      void
      height<I>::set_value(unsigned h)
      {
	height_ = h;
	// Reset the other members.
	min_level__ = mln_max(value);
	max_level__ = mln_min(value);
      }

      template <typename I>
      inline
      bool
      height<I>::is_valid() const
      {
	return true;
      }

    } // end of namespace mln::accu::shape

# endif // ! MLN_INCLUDE_ONLY

  } // end of namespace mln::accu

} // end of namespace mln


#endif // ! MLN_ACCU_SHAPE_HEIGHT_HH
