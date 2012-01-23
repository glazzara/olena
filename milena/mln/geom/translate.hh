// Copyright (C) 2009, 2012 EPITA Research and Development Laboratory
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

/// \file
///
/// Translate an image.

#ifndef MLN_GEOM_TRANSLATE_HH
# define MLN_GEOM_TRANSLATE_HH

# include <mln/core/concept/image.hh>
# include <mln/core/concept/site_set.hh>
# include <mln/core/concept/box.hh>

# include <mln/core/routine/extend.hh>
# include <mln/core/site_set/p_transformed.hh>
# include <mln/core/image/imorph/tr_image.hh>

# include <mln/data/fill.hh>

# include <mln/fun/x2x/translation.hh>

# include <mln/literal/zero.hh>

# include <mln/geom/bbox.hh>


namespace mln
{

  namespace geom
  {

    /*! \brief Perform a translation from the center of an image.

      \param[in] input          An image.
      \param[in] ref            The translation vector.
      \param[in] extension      Function, image or value which will be used
                                as extension. This extension allows to map
				values to sites which where not part
				of the domain before the translation.
      \param[in] output_domain  The domain of the output image. An
                                invalid domain, causes the routine
				to use the translated input_ domain.

      \return An image with the same domain as \p input.

      \ingroup mlngeom
     */
    template <typename I, typename V, typename Ext, typename S>
    mln_concrete(I)
    translate(const Image<I>& input,
	      const algebra::vec<mln_site_(I)::dim, V>& ref,
	      const Ext& extension, const Site_Set<S>& output_domain);


    /*! \overload

      \ingroup mlngeom
    */
    template <typename I, typename V, typename Ext>
    mln_concrete(I)
    translate(const Image<I>& input,
	      const algebra::vec<mln_site_(I)::dim, V>& ref,
	      const Ext& extension);


    /*! \overload
      Use literal::zero as default value for the extension.

      \ingroup mlngeom
    */
    template <typename I, typename V>
    mln_concrete(I)
    translate(const Image<I>& input,
 	      const algebra::vec<mln_site_(I)::dim, V>& ref);



# ifndef MLN_INCLUDE_ONLY


    template <typename I, typename V, typename Ext, typename S>
    mln_concrete(I)
    translate(const Image<I>& input_,
	      const algebra::vec<mln_site_(I)::dim, V>& ref,
	      const Ext& extension_, const Site_Set<S>& output_domain_)
    {
      trace::entering("geom::translate");

      const I& input = exact(input_);
      const S& output_domain = exact(output_domain_);
      const mln_exact(Ext)& extension = exact(extension_);
      //mlc_converts_to(mln_exact(Ext), mln_value(I))::check();

      mln_precondition(input.is_valid());
      mln_precondition(output_domain.is_valid());

      //mln_psite(I) c = geom::bbox(input).center();
      typedef fun::x2x::translation<mln_site_(I)::dim, V> trans_t;
      typedef p_transformed<mln_domain(I), trans_t> trans_domain_t;
      typedef tr_image<trans_domain_t, I, trans_t> tr_ima_t;

      trans_t t(ref);
      trans_domain_t d(input.domain(), t);
      tr_ima_t tr_ima(d, input, t);

      mln_concrete(I) output(output_domain);
      data::fill(output, extend(tr_ima, extension) | output_domain);

      trace::exiting("geom::translate");
      return output;
    }


    template <typename I, typename V, typename Ext>
    mln_concrete(I)
    translate(const Image<I>& input,
	      const algebra::vec<mln_site_(I)::dim, V>& ref,
	      const Ext& extension)
    {
      // Old versions of GCC (including Apple GCC 4.0.1) do not parse
      // correctly `mln_box(I)()'.  Hence, we need to typedef
      // `mln_box(I)' first.
      typedef mln_domain(I) domain_t;
      return translate(input, ref, extension, domain_t());
    }


    template <typename I, typename V>
    mln_concrete(I)
    translate(const Image<I>& input,
	      const algebra::vec<mln_site_(I)::dim, V>& ref)
    {
      return translate(input, ref, literal::zero);
    }


# endif // ! MLN_INCLUDE_ONLY


  } // end of namespace mln::geom

} // end of namespace mln


#endif // ! MLN_GEOM_TRANSLATE_HH
