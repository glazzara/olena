// Copyright (C) 2007, 2008 EPITA Research and Development Laboratory
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

#ifndef MLN_ACCU_MEAN_RGB_PIX_HH
# define MLN_ACCU_MEAN_RGB_PIX_HH

/// \file mln/accu/mean_rgb_pix.hh
///
/// Define an accumulator that computes an rgb8 mean.

# include <mln/core/concept/meta_accumulator.hh>
# include <mln/accu/internal/base.hh>

# include <mln/value/rgb8.hh>
# include <mln/algebra/vec.hh>

# include <mln/util/pix.hh>
# include <mln/core/image/image2d.hh>


namespace mln
{


  namespace accu
  {


    image2d<value::rgb8> rgb_image_;


    /// Generic mean_rgb_pix accumulator class.
    ///
    template <typename Pix>
    struct mean_rgb_pix : public mln::accu::internal::base< value::rgb8, mean_rgb_pix<Pix> >
    {
      typedef Pix argument;

      mean_rgb_pix();

      /// Manipulators.
      /// \{
      void init();
      void take(const argument& t);
      void take(const mean_rgb_pix<Pix>& other);
      /// \}

//       void set_value(S v)
//       {
// 	s_ = v;
//       }

      /// Get the value of the accumulator.
      value::rgb8 to_result() const;

      /// Check whether this accu is able to return a result.
      /// Always true here.
      bool is_valid() const;

    protected:

      typedef algebra::vec<3,float> vec_t;
      vec_t s_;
      unsigned n_;
    };


# ifndef MLN_INCLUDE_ONLY

    template <typename Pix>
    inline
    mean_rgb_pix<Pix>::mean_rgb_pix()
    {
      init();
    }

    template <typename Pix>
    inline
    void
    mean_rgb_pix<Pix>::init()
    {
      s_ = vec_t::zero;
      n_ = 0;
    }

    template <typename Pix>
    inline
    void mean_rgb_pix<Pix>::take(const argument& px)
    {
      s_ += vec_t(rgb_image_(px.p()));
      ++n_;
    }

    template <typename Pix>
    inline
    void
    mean_rgb_pix<Pix>::take(const mean_rgb_pix<Pix>& other)
    {
      s_ += other.s_;
      n_ += other.n_;
    }

    template <typename Pix>
    inline
    value::rgb8
    mean_rgb_pix<Pix>::to_result() const
    {
      return algebra::vec<3, unsigned>(s_ / n_);
    }

    template <typename Pix>
    inline
    bool
    mean_rgb_pix<Pix>::is_valid() const
    {
      return true;
    }

# endif // ! MLN_INCLUDE_ONLY

  } // end of namespace mln::accu

} // end of namespace mln


#endif // ! MLN_ACCU_MEAN_RGB_PIX_HH
