// Copyright (C) 2009 EPITA Research and Development Laboratory (LRDE)
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

#ifndef MLN_TRANSFORMATION_ROTATE_HH
# define MLN_TRANSFORMATION_ROTATE_HH

# include <mln/core/routine/extend.hh>

# include <mln/core/image/imorph/tr_image.hh>

# include <mln/data/paste.hh>

# include <mln/geom/bbox.hh>

# include <mln/fun/x2x/composed.hh>
# include <mln/fun/x2x/rotation.hh>
# include <mln/fun/x2x/translation.hh>

# include <mln/literal/zero.hh>

# include <mln/math/pi.hh>


namespace mln
{

  namespace transformation
  {

    /// Perform a rotation from the center of an image.
    ///
    /// \param[in] input_     An image.
    /// \param[in] angle      An angle in degrees.
    /// \param[in] extension_ Function, image or value which will be used as
    ///			      extension. This extension allows to map values
    ///			      to sites which where not part of the domain
    ///			      before the rotation.
    ///
    /// \return An image with the same domain as \p input_.
    //
    template <typename I, typename Ext>
    mln_concrete(I)
    rotate(const Image<I>& input_, double angle, const Ext& extension_);


    /// \overload
    /// Use literal::zero as default value for the extension.
    template <typename I>
    mln_concrete(I)
    rotate(const Image<I>& input_, double angle);


# ifndef MLN_INCLUDE_ONLY


    template <typename I, typename Ext>
    mln_concrete(I)
    rotate(const Image<I>& input_, double angle, const Ext& extension_)
    {
      trace::entering("transformation::rotate");

      const I& input = exact(input_);

      mln_precondition(input.is_valid());
      mln_precondition(angle >= -360.0f && angle <= 360.0f);
      /// FIXME: A precondition is probably missing for the extension value.

      mln_site(I) c = geom::bbox(input).center();
      fun::x2x::translation<2,double>
	t(-1 * c.to_vec()),
	t_1(c.to_vec());

      typedef fun::x2x::rotation<2,double> rot_t;
      rot_t rot(math::pi * angle / 180.f, literal::origin);

      mln_concrete(I) output;
      initialize(output, input);
      data::paste(transposed_image(input.domain(),
				   extend(input, extension_),
				   compose(t_1, compose(rot, t))),
		  output);

      trace::exiting("transformation::rotate");
      return output;
    }


    template <typename I>
    mln_concrete(I)
    rotate(const Image<I>& input, double angle)
    {
      return rotate(input, angle, literal::zero);
    }


# endif // ! MLN_INCLUDE_ONLY


  } // end of namespace mln::transformation

} // end of namespace mln


#endif // ! MLN_TRANSFORMATION_ROTATE_HH
