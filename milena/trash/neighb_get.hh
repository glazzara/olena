// Copyright (C) 2008 EPITA Research and Development Laboratory (LRDE)
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

#ifndef MLN_NEIGHB_GET_HH
# define MLN_NEIGHB_GET_HH

/// \file mln/neighb/get.hh
/// \brief Define function that returns the neighborhood assocoatied
/// to an image.

# include <mln/trait/images.hh>
# include <mln/neighb/image.hh>


/// Shortcuts to access the neighborhood type associated to I.
/// \{
# define mln_neighb(T)     typename mln::neighb::get_helper<I>::type
# define mln_neighb_(T)             mln::neighb::get_helper<I>::type
/// \}


namespace mln
{

  namespace neighb
  {

    // Fwd decl.
    template <typename I>
    struct get_helper;

    template <typename I>
    typename get_helper<I>::type
    get(const Image<I>& ima);


    /// FIXME: Move this to mln/trait/neighb.hh?  And split
    /// neighborhood type deduction from neighborhood access (fun)?

    /// Get the (possible) neighborhood type associated to an image
    /// type \c I.
    template <typename I>
    struct get_helper
    {
      /// Select the neighborhood from the presence of the
      /// neighborhood and the category of the image.
      /// {

      // FIXME: Rename select_from_traits.

      template <typename image_neighb, typename image_category, typename I_>
      struct select_from_traits;

      /// Case where the image has a neighborhood and is a primary
      /// image.
      template <typename I_>
      struct select_from_traits<trait::image::neighb::some,
				trait::image::category::primary,
				I_>
      {
	typedef typename I_::neighb type;

	static const type&
	run(const I_& ima) { return ima.neighborhood(); }
      };

      /// Special case: neighb::image<I_, N_>.
      template <typename I_, typename N_>
      struct select_from_traits<trait::image::neighb::some,
				trait::image::category::morpher,
				mln::neighb::image<I_, N_> >
      {
	typedef typename neighb::image<I_, N_>::neighb type;

	static const type&
	run(const neighb::image<I_, N_>& ima) { return ima.neighborhood(); }
      };

      /// Case where the image has a neighborhood and is a morpher.
      template <typename I_>
      struct select_from_traits<trait::image::neighb::some,
				trait::image::category::morpher,
				I_>
      {
	typedef typename get_helper<typename I_::delegatee>::type type;

	static const type&
	run(const I_& ima) { return neighb::get( *ima.delegatee_() ); }
      };

      /// Case where the image has a neighborhood and is a identity morpher.
      template <typename I_>
      struct select_from_traits<trait::image::neighb::some,
				trait::image::category::identity_morpher,
				I_>
      {
	typedef typename get_helper<typename I_::delegatee>::type type;

	static const type&
	run(const I_& ima) { return neighb::get( *ima.delegatee_() ); }
      };

      /// Case where the image has a neighborhood and is a value morpher.
      template <typename I_>
      struct select_from_traits<trait::image::neighb::some,
				trait::image::category::value_morpher,
				I_>
      {
	typedef typename get_helper<typename I_::delegatee>::type type;

	static const type&
	run(const I_& ima) { return neighb::get( *ima.delegatee_() ); }	
      };

      /// \brief Default case: error.
      /// 
      /// Causes:
      /// \li the image has no neighborhood;
      /// \li the image category is of an invalid morpher type (e.g.,
      ///     a domain morpher);
      /// \li the trait for the neighborhood of the image is not
      ///     defined or ill-defined;
      /// \li the trait for the image category is not defined or
      ///     ill-defined.
      template <typename image_neighb, typename image_category, typename I_>
      struct select_from_traits
      {
	/// Nothing.
      };

      /// \}

      typedef select_from_traits< mln_trait_image_neighb(I),
				  mln_trait_image_category(I),
				  I > selection_;

      typedef typename selection_::type type;

      static const type& run(const I& ima) { return selection_::run(ima); }
    };



# ifndef MLN_INCLUDE_ONLY

    // Facade.

    template <typename I>
    inline
    typename get_helper<I>::type
    get(const Image<I>& ima)
    {
      trace::entering("neighb::get");

      mln_precondition(exact(ima).is_valid());
      typename neighb::get_helper<I>::type res =
	neighb::get_helper<I>::run(exact(ima));

      trace::exiting("neighb::get");
      return res;
    }

# endif // ! MLN_INCLUDE_ONLY

  } // end of namespace mln::neighb

} // end of namespace mln


#endif // ! MLN_NEIGHB_GET_HH
