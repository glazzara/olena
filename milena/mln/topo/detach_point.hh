// Copyright (C) 2010, 2011 EPITA Research and Development Laboratory (LRDE)
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

#ifndef MLN_TOPO_DETACH_POINT_HH
# define MLN_TOPO_DETACH_POINT_HH

/// \file
/// \brief Detaching a point from a binary image.

# include <mln/metal/equal.hh>

# include <mln/core/image/image2d.hh>
# include <mln/core/alias/point2d.hh>

namespace mln
{

  namespace topo
  {
    
    /// \brief Functor detaching a point from a binary image.
    template <typename I>
    class detach_point
    {
    public:
      /// Build a functor.
      detach_point();

      /// Build a functor, and assign an image to it.
      ///
      /// \param  ima     The image.
      detach_point(Image<I>& ima);

      /// Set the underlying image.
      void set_image(Image<I>& ima);

      /// \brief Detach point \a p from the image.
      void operator()(const mln_psite(I)& p) const;

    private:
      /// The image.
      I* ima_;
    };



# ifndef MLN_INCLUDE_ONLY

    template <typename I>
    inline
    detach_point<I>::detach_point()
      : ima_(0)
    {
      // Ensure I is a binary image type.
      /* FIXME: Not compatible with proxy/morphers on values.  */
      mlc_equal(mln_value(I), bool)::check();
    }

    template <typename I>
    inline
    detach_point<I>::detach_point(Image<I>& ima)
      : ima_(exact(&ima))
    {
      // Ensure I is a binary image type.
      /* FIXME: Not compatible with proxy/morphers on values.  */
      mlc_equal(mln_value(I), bool)::check();
    }

    template <typename I>
    inline
    void
    detach_point<I>::set_image(Image<I>& ima)
    {
      ima_ = exact(&ima);
    }

    template <typename I>
    inline
    void
    detach_point<I>::operator()(const mln_psite(I)& p) const
    {
      (*ima_)(p) = false;
    }

# endif // MLN_INCLUDE_ONLY

  } // end of namespace mln::topo

} // end of namespace mln

#endif // ! MLN_TOPO_DETACH_POINT_HH
