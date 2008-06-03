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
    \brief Define an accumulator that computes a volume.
   
    \see mln::morpho::closing_volume
    \see mln::morpho::opening_volume  */

# include <mln/accu/internal/base.hh>
# include <mln/core/concept/meta_accumulator.hh>

# include <mln/literal/zero.hh>

namespace mln
{

  namespace accu
  {


    /* FIXME: We should probably ``inline'' the parameter T of the
       sole use of accu::volume_ (in volume closing and opening, where
       T = util::pix<I>), since volume is not as generic as
       accu::count_, for instance.  Hence, we would get rid of the
       FIXME of this file on the constraints on T.  */

    /// \brief Generic volume accumulator class.
    /// The parameter \p T is the type of value whose volume is computed.
    template <typename T>
    struct volume_
      : public mln::accu::internal::base_< std::size_t , volume_<T> >
    {
      typedef T argument;
      typedef std::size_t result; // FIXME: Up in Accumulator.

      volume_();

      void init();
      void take(const argument&);
      void take(const volume_<T>& other);

      std::size_t to_result() const;
      void set_value(std::size_t c);

    protected:
      // FIXME: This attributes expects a typedef `value' from T.
      typename argument::value height__;
      std::size_t area__;
      std::size_t volume__;
    };


    /// \brief Meta accumulator for volume.
    struct volume : public Meta_Accumulator< volume >
    {
      template <typename T>
      struct with
      {
	typedef volume_<T> ret;
      };
    };


# ifndef MLN_INCLUDE_ONLY

    template <typename T>
    inline
    volume_<T>::volume_()
    {
      init();
    }

    template <typename T>
    inline
    void
    volume_<T>::init()
    {
      height__ = literal::zero;
      volume__ = 0;
      volume__ = 0;
    }

    template <typename T>
    inline
    void
    volume_<T>::take(const argument& t)
    {
      /* FIXME: This accumulator will only work with types T providing
	 a method v() (e.g., a util::pix<I>).  */
      height__ = t.v();
      ++area__;
      ++volume__;
    }

    template <typename T>
    inline
    void
    volume_<T>::take(const volume_<T>& other)
    {
      // Member height__ is not touched.

      /* FIXME: This accumulator will only work with types T providing
	 a method v() (e.g., a util::pix<I>).  */
      area__ += other.area__;
      /* FIXME: Is it `t.area__' or `area__' ? Théo said it was
	 the latter, but both the ISMM 2005 paper and Olena 0.11 use
	 the former.  */
      volume__ +=
	other.volume__ + other.area__ * math::abs(other.height__ - height__);
    }

    template <typename T>
    inline
    std::size_t
    volume_<T>::to_result() const
    {
      return volume__;
    }

    template <typename T>
    inline
    void
    volume_<T>::set_value(std::size_t c)
    {
      volume__ = c;
      // FIXME: What about area__ and height__ ?
    }

# endif // ! MLN_INCLUDE_ONLY

  } // end of namespace mln::accu

} // end of namespace mln


#endif // ! MLN_ACCU_VOLUME_HH
