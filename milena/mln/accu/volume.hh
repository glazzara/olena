// Copyright (C) 2007, 2008 EPITA Research and Development Laboratory (LRDE)
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

#ifndef MLN_ACCU_VOLUME_HH
# define MLN_ACCU_VOLUME_HH

/** \file mln/accu/volume.hh
    \brief Define an accumulator that computes the volume of a
    component through one of its pixels.

    This accumulator uses an mln::util::pix (pixel) to update the
    height, area and volume information of the component.

    The class mln/accu/volume_ is not a general-purpose accumulator;
    it is used to implement volume-based connected filters.
    \see mln::morpho::closing_volume
    \see mln::morpho::opening_volume  */

# include <mln/accu/internal/base.hh>
# include <mln/core/concept/meta_accumulator.hh>

# include <mln/util/pix.hh>
# include <mln/literal/zero.hh>

namespace mln
{

  namespace accu
  {

    /// \brief Volume accumulator class.
    ///
    /// The parameter \p I is the image type on which the accumulator
    /// of pixels is built.
    template <typename I>
    struct volume_
      : public mln::accu::internal::base_< std::size_t , volume_<I> >
    {
      /// \brief The accumulated data type.
      ///
      /// The volume of component is represented by the volume of its
      /// root pixel.  See mln::morpho::closing_volume and
      /// mln::morpho::opening_volume for actual uses of this
      /// accumulator.
      typedef util::pix<I> argument;
      typedef std::size_t result; // FIXME: Up in Accumulator.

      volume_();

      void init();
      void take(const argument&);
      void take(const volume_<I>& other);

      std::size_t to_result() const;
      void set_value(std::size_t c);

    protected:
      typename argument::value height__;
      std::size_t area__;
      std::size_t volume__;
    };


    /// \brief Meta accumulator for volume.
    struct volume : public Meta_Accumulator< volume >
    {
      template <typename I>
      struct with
      {
	typedef volume_<I> ret;
      };
    };


# ifndef MLN_INCLUDE_ONLY

    template <typename I>
    inline
    volume_<I>::volume_()
    {
      init();
    }

    template <typename I>
    inline
    void
    volume_<I>::init()
    {
      height__ = literal::zero;
      volume__ = 0;
      volume__ = 0;
    }

    template <typename I>
    inline
    void
    volume_<I>::take(const argument& t)
    {
      height__ = t.v();
      ++area__;
      ++volume__;
    }

    template <typename I>
    inline
    void
    volume_<I>::take(const volume_<I>& other)
    {
      area__ += other.area__;
      /* FIXME: Is it `t.area__' or `area__' ? Théo said it was
	 the latter, but both the ISMM 2005 paper and Olena 0.11 use
	 the former.  */
      volume__ +=
	other.volume__ + other.area__ * math::abs(other.height__ - height__);
      // Member height__ is not touched.
    }

    template <typename I>
    inline
    std::size_t
    volume_<I>::to_result() const
    {
      return volume__;
    }

    template <typename I>
    inline
    void
    volume_<I>::set_value(std::size_t c)
    {
      volume__ = c;
      // FIXME: What about area__ and height__ ?
    }

# endif // ! MLN_INCLUDE_ONLY

  } // end of namespace mln::accu

} // end of namespace mln


#endif // ! MLN_ACCU_VOLUME_HH
