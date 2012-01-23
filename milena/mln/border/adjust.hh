// Copyright (C) 2007, 2008, 2009, 2012 EPITA Research and Development
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

#ifndef MLN_BORDER_ADJUST_HH
# define MLN_BORDER_ADJUST_HH

/// \file
///
/// Define a function that adjusts the thickness of an image
/// virtual border.

# include <mln/border/resize.hh>


namespace mln
{

  namespace border
  {

    /*! \brief Adjust the virtual (outer) border of image \p ima so
     *  that its size is at least \p min_thickness.
     *
     * \param[in,out] ima The image whose border is to be adjusted.
     * \param[in] min_thickness The expected border minimum thickness.
     *
     * \pre \p ima has to be initialized.
     *
     * \warning If the image border is already larger than \p
     * min_thickness, this routine is a no-op.
     *
     * \ingroup mlnborderext
     */
    template <typename I>
    void adjust(const Image<I>& ima, unsigned min_thickness);


# ifndef MLN_INCLUDE_ONLY

    template <typename I>
    inline
    void adjust(const Image<I>& ima, unsigned min_thickness)
    {
      trace::entering("border::adjust");

      mln_precondition(exact(ima).is_valid());

      if (border::get(ima) < min_thickness)
	border::resize(ima, min_thickness);

      // We cannot run here a postcondition since we do not know if
      // ima (or an underlying image) has a border or not.

      trace::exiting("border::adjust");
    }

# endif // ! MLN_INCLUDE_ONLY

  } // end of namespace mln::border

} // end of namespace mln


#endif // ! MLN_BORDER_ADJUST_HH
