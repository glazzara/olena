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

#ifndef MLN_PW_IMAGE_HH
# define MLN_PW_IMAGE_HH

/*! \file mln/pw/image.hh
 *
 * \brief Definition of an image class FIXME
 */

# include <mln/core/internal/image_base.hh>
# include <mln/core/concept/function.hh>
# include <mln/value/set.hh>


namespace mln
{

  // Fwd decl.
  namespace pw { template <typename F, typename S> struct image; }


  
  /*! \brief FIXME
   *
   */
  template <typename F, typename S>
  pw::image<F,S>
  operator | (const Function_p2v<F>& f, const Point_Set<S>& ps);



  namespace pw
  {

    /*! \brief FIXME
     *
     */
    template <typename F, typename S>
    class image : public internal::image_base_< S, image<F,S> >
    {
      typedef internal::image_base_< S, image<F,S> > super_;
    public:


      /// Skeleton.
      typedef image< tag::function<F>, tag::pset<S> > skeleton;


      // From super class.
      typedef mln_psite(super_) psite;


      /// Value associated type.
      typedef mln_result(F) value;

      /// Return type of read-only access.
      typedef mln_result(F) rvalue;

      /// Return type of read-write access.
      typedef void lvalue; // FIXME

      /// Value set associated type.
      typedef mln::value::set<mln_result(F)> vset;


      /// Constructor.
      image(const Function_p2v<F>& f, const Point_Set<S>& ps);


      /// Test if this image has been initialized.
      bool has_data() const;

      /// Test if a pixel value is accessible at \p p.
      bool owns_(const psite& p) const;

      /// Give the definition domain.
      const S& domain() const;

      /// Read-only access of pixel value at point site \p p.
      mln_result(F) operator()(const psite& p) const;

      /// Read-write access is present but disabled.
      void operator()(const psite&);

      /// Give the set of values of the image.
      const vset& values() const;

    protected:
      F f_;
      S pset_;
    };

  } // end of namespace mln::pw
  


# ifndef MLN_INCLUDE_ONLY

  template <typename F, typename S>
  pw::image<F,S>
  operator | (const Function_p2v<F>& f, const Point_Set<S>& ps)
  {
    pw::image<F,S> tmp(f, ps);
    return tmp;
  }

  namespace pw
  {

    template <typename F, typename S>
    image<F,S>::image(const Function_p2v<F>& f, const Point_Set<S>& ps)
      : f_(exact(f)),
	pset_(exact(ps))
    {
    }

    template <typename F, typename S>
    bool image<F,S>::has_data() const
    {
      return true;
    }

    template <typename F, typename S>
    bool image<F,S>::owns_(const psite& p) const
    {
      return pset_.has(p);
    }

    template <typename F, typename S>
    const S&
    image<F,S>::domain() const
    {
      return pset_;
    }

    template <typename F, typename S>
    mln_result(F)
      image<F,S>::operator()(const psite& p) const
    {
      mln_precondition(pset_.has(p));
      return f_(p);
    }

    template <typename F, typename S>
    void
    image<F,S>::operator()(const psite&)
    {
      mln_invariant(0); // FIXME: Turn into a compile-time error...
    }

    template <typename F, typename S>
    const mln::value::set<mln_result(F)>&
    image<F,S>::values() const
    {
      return vset::the();
    }

  } // end of namespace mln::pw
  
# endif // ! MLN_INCLUDE_ONLY

} // end of namespace mln


#endif // ! MLN_PW_IMAGE_HH
