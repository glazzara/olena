// Copyright (C) 2007 EPITA Research and Development Laboratory
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

#ifndef MLN_CORE_INIT_HH
# define MLN_CORE_INIT_HH

/*! \file mln/core/init.hh
 *
 * \brief Initialization of an image from another one.
 */

# include <mln/tag/init.hh>

# include <mln/core/concept/image.hh>
# include <mln/geom/bbox.hh>
# include <mln/border/get.hh>



namespace mln
{

  /*! Initialize the image \p target with data extracted from image \p model.
   *
   * \param[in, out] target The image to be initialized.
   * \param[in] model The image to provide data for the initialization.
   *
   * \pre (not target.has_data) and model.has_data
   */
  template <typename I, typename J>
  void init(Image<I>& target, const Image<J>& model);



  // Fwd decl.
  
  namespace impl
  {

    template <typename Subject, typename T, typename M>
    void init_(Subject, T& target, const Object<M>& model);

    // Easy and final versions.

    template <typename B, typename I>
    void init_(tag::bbox_t, B& b, const Image<I>& ima);

    template <typename I>
    void init_(tag::border_t, unsigned& bdr, const Image<I>& ima);

    template <typename I>
    void init_(tag::domain_t, mln_pset(I)& pset, const Image<I>& ima);

  } // end of namespace mln::impl



# ifndef MLN_INCLUDE_ONLY

  namespace impl
  {

    template <typename Subject, typename T, typename M>
    void init_(Subject, T&, const Object<M>&)
    {
      struct ERROR err_; // FIXME: Explicit msg.
    }

    // Easy impl.

    template <typename B, typename I>
    void init_(tag::bbox_t, B& b, const Image<I>& ima)
    {
      b = geom::bbox(ima);
    }

    template <typename I>
    void init_(tag::border_t, unsigned& bdr, const Image<I>& ima)
    {
      bdr = border::get(ima);
    }

    template <typename I>
    void init_(tag::domain_t, mln_pset(I)& pset, const Image<I>& ima)
    {
      pset = exact(ima).domain();
    }

  } // end of namespace mln::impl


  // Facade.

  template <typename I, typename J>
  void init(Image<I>& target, const Image<J>& model)
  {
    mln_precondition(! exact(target).has_data());
    mln_precondition(exact(model).has_data());

    impl::init_(tag::image, exact(target), exact(model));

    mln_postcondition(exact(target).has_data());
  }

# endif // ! MLN_INCLUDE_ONLY

} // end of namespace mln


#endif // ! MLN_CORE_INIT_HH
