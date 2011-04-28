// Copyright (C) 2008, 2009, 2011 EPITA Research and Development
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

#ifndef MLN_CORE_IMAGE_CH_PITER_HH
# define MLN_CORE_IMAGE_CH_PITER_HH

/*!
 * \file
 *
 * \brief Definition of a morpher that changes the iterators.
 */

# include <mln/core/internal/image_identity.hh>



namespace mln
{

  // Forward declaration.
  template <typename I, typename Fwd> class ch_piter_image;


  namespace internal
  {

    /// \internal Data structure for \c mln::ch_piter_image<I,Fwd>.
    template <typename I, typename Fwd>
    struct data< ch_piter_image<I,Fwd> >
    {
      data(I& ima);
      I ima_;
    };

  } // end of namespace mln::internal


  namespace trait
  {

    template <typename I, typename Fwd>
    struct image_< ch_piter_image<I,Fwd> > : image_< I > // Same as I except...
    {
      // ...theis change.
      typedef trait::image::category::identity_morpher category;
    };

  } // end of namespace mln::trait



  // FIXME: Doc!

  template <typename I, typename Fwd>
  class ch_piter_image : public internal::image_identity< I,
							  mln_domain(I),
							  ch_piter_image<I,Fwd> >
  {
  public:

    /// Skeleton.
    typedef ch_piter_image< tag::image_<I>, Fwd > skeleton;

    typedef Fwd fwd_piter;
    typedef Fwd bkd_piter; // FIXME
    typedef fwd_piter piter;

    ch_piter_image();
    ch_piter_image(I& ima);

    // Deferred initialization.
    void init_(I& ima);
  };



  template <typename Fwd, typename I>
  ch_piter_image<I,Fwd>
  ch_piter(Image<I>& ima);



# ifndef MLN_INCLUDE_ONLY


  // internal::data< ch_piter_image<I,S> >

  namespace internal
  {

    template <typename I, typename Fwd>
    inline
    data< ch_piter_image<I,Fwd> >::data(I& ima)
      : ima_(ima)
    {
    }

  } // end of namespace mln::internal


  // ch_piter_image<I,Fwd>

  template <typename I, typename Fwd>
  inline
  ch_piter_image<I,Fwd>::ch_piter_image()
  {
  }

  template <typename I, typename Fwd>
  inline
  ch_piter_image<I,Fwd>::ch_piter_image(I& ima)
  {
    mln_precondition(ima.is_valid());
    init_(ima);
  }

  template <typename I, typename Fwd>
  inline
  void
  ch_piter_image<I,Fwd>::init_(I& ima)
  {
    mln_precondition(ima.is_valid());
    this->data_ = new internal::data< ch_piter_image<I,Fwd> >(ima);
  }


  // ch_piter

  template <typename Fwd, typename I>
  inline
  ch_piter_image<I,Fwd>
  ch_piter(Image<I>& ima)
  {
    ch_piter_image<I,Fwd> tmp(exact(ima));
    return tmp;
  }

# endif // ! MLN_INCLUDE_ONLY

} // end of namespace mln


#endif // ! MLN_CORE_IMAGE_CH_PITER_HH
