// Copyright (C) 2009, 2010, 2012 EPITA Research and Development
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

/// \file
///
/// Rotate an image.

#ifndef MLN_GEOM_ROTATE_HH
# define MLN_GEOM_ROTATE_HH

# include <mln/core/concept/image.hh>
# include <mln/core/concept/site_set.hh>
# include <mln/core/concept/box.hh>

# include <mln/core/routine/extend.hh>

# include <mln/core/image/imorph/tr_image.hh>

# include <mln/accu/shape/bbox.hh>

# include <mln/data/paste.hh>

# include <mln/geom/bbox.hh>
# include <mln/geom/top_right.hh>
# include <mln/geom/bottom_left.hh>

# include <mln/extension/duplicate.hh>

# include <mln/fun/x2x/composed.hh>
# include <mln/fun/x2x/rotation.hh>
# include <mln/fun/x2x/translation.hh>

# include <mln/literal/zero.hh>

# include <mln/math/pi.hh>


namespace mln
{

  namespace geom
  {

    /*! \brief Perform a rotation from the center of an image.

      \param[in] input          An image.
      \param[in] angle          An angle in degrees.
      \param[in] extension      Function, image or value which will be used
                                as extension. This extension allows to map
				values to sites which where not part
				of the domain before the rotation.
      \param[in] output_domain  The domain of the output image. An
                                invalid domain, causes the routine
				to use a domain large enough to
				display the whole original image.

      \return An image with the same domain as \p input.

      \ingroup mlngeom
    */
    template <typename I, typename Ext, typename S>
    mln_concrete(I)
    rotate(const Image<I>& input, double angle,
	   const Ext& extension, const Site_Set<S>& output_domain);


    /*! \overload

      \ingroup mlngeom
     */
    template <typename I, typename Ext>
    mln_concrete(I)
    rotate(const Image<I>& input, double angle, const Ext& extension);


    /*! \overload
      Use literal::zero as default value for the extension.

      \ingroup mlngeom
    */
    template <typename I>
    mln_concrete(I)
    rotate(const Image<I>& input, double angle);


    /*! \brief Rotate a box.

      FIXME: the return type may be too generic and may lead to
      invalid covariance.

      \ingroup mlngeom
    */
    template <typename B>
    B
    rotate(const Box<B>& box_, double angle, const mln_site(B)& ref);

    /*! \overload

      The rotation center \p ref is set to box.pcenter().

      \ingroup mlngeom
    */
    template <typename B>
    B
    rotate(const Box<B>& box, double angle);



# ifndef MLN_INCLUDE_ONLY


    template <typename I, typename Ext, typename S>
    mln_concrete(I)
    rotate(const Image<I>& input_, double angle,
	   const Ext& extension_, const Site_Set<S>& output_domain_)
    {
      trace::entering("geom::rotate");

      const I& input = exact(input_);
      const S& output_domain = exact(output_domain_);
      const mln_exact(Ext)& extension = exact(extension_);

      // Do not check that output_domain_ is valid. If it is not,
      // further in this routine, we define a default domain.
      typedef mln_site(I) P;
      mln_precondition(input.is_valid());
      mln_precondition(angle >= -360.0f && angle <= 360.0f);
//      mlc_converts_to(mln_exact(Ext), mln_value(I))::check();
      mlc_is_a(S,Box)::check();
      // FIXME: A precondition is probably missing for the extension value.

      extension::duplicate(input);

      mln_site(I) c = geom::bbox(input).pcenter();
      typedef fun::x2x::translation<P::dim,double> trans_t;
      trans_t
	t(-1 * c.to_vec()),
	t_1(c.to_vec());

      typedef fun::x2x::rotation<P::dim,double> rot_t;
      rot_t rot(math::pi * angle / 180.f, literal::origin);

      typedef
	fun::x2x::composed<trans_t, fun::x2x::composed<rot_t, trans_t> >
	comp_transf_t;

      comp_transf_t comp_transf = compose(t_1, compose(rot, t));

      S b = output_domain;
      // Automatically adjusting the output domain if needed.
      if (!output_domain.is_valid())
	b = rotate(input.domain(), angle);

      typedef
	typename mln::internal::extension_type<const I,	mln_exact(Ext)>::result ext_t;

      typedef
	tr_image<mln_box(I), ext_t, comp_transf_t> tr_t;

      tr_t tr = transposed_image(b, ext_t(input, extension), comp_transf);


      mln_concrete(I) output;
      initialize(output, tr);

      data::paste(tr, output);

      trace::exiting("geom::rotate");
      return output;
    }


    template <typename I, typename Ext>
    mln_concrete(I)
    rotate(const Image<I>& input, double angle, const Ext& extension)
    {
      // Old versions of GCC (including Apple GCC 4.0.1) do not parse
      // correctly `mln_box(I)()'.  Hence, we need to typedef
      // `mln_box(I)' first.
      typedef mln_domain(I) domain_t;
      return rotate(input, angle, extension, domain_t());
    }


    template <typename I>
    mln_concrete(I)
    rotate(const Image<I>& input, double angle)
    {
      return rotate(input, angle, literal::zero);
    }


    template <typename B>
    B
    rotate(const Box<B>& box_, double angle, const mln_site(B)& ref)
    {
      trace::entering("geom::rotate");

      const B& box = exact(box_);

      typedef mln_site(B) P;
      mln_precondition(box.is_valid());
      mln_precondition(angle >= -360.0f && angle <= 360.0f);

      typedef fun::x2x::translation<P::dim,double> trans_t;
      trans_t
	t(-1 * ref.to_vec()),
	t_1(ref.to_vec());

      typedef fun::x2x::rotation<P::dim,double> rot_t;
      rot_t rot(math::pi * angle / 180.f, literal::origin);

      typedef
	fun::x2x::composed<trans_t, fun::x2x::composed<rot_t, trans_t> >
	comp_transf_t;

      comp_transf_t comp_transf = compose(t_1, compose(rot, t));

      accu::shape::bbox<P> accu;

      P
	top_right = geom::top_right(box),
	bot_left = geom::bottom_left(box);

      accu.take(P(comp_transf(box.pmin().to_vec())));
      accu.take(P(comp_transf(top_right.to_vec())));
      accu.take(P(comp_transf(bot_left.to_vec())));
      accu.take(P(comp_transf(box.pmax().to_vec())));

      B output = accu.to_result();

      trace::exiting("geom::rotate");
      return output;
    }


    template <typename B>
    B
    rotate(const Box<B>& box, double angle)
    {
      return rotate(box, angle, exact(box).pcenter());
    }



# endif // ! MLN_INCLUDE_ONLY


  } // end of namespace mln::geom

} // end of namespace mln


#endif // ! MLN_GEOM_ROTATE_HH
