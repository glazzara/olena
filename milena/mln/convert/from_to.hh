// Copyright (C) 2008 EPITA Research and Development Laboratory
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

#ifndef MLN_CONVERT_FROM_TO_HH
# define MLN_CONVERT_FROM_TO_HH

/*! \file mln/convert/from_to.hh
 *
 * \brief General conversion procedure between two objects.
 *
 * \todo Prefer a static check that fails in the "unknown" case.
 *
 * \todo Use 'round' instead of static_cast in vec->Gpoint.
 */

# include <mln/core/concept/object.hh>
# include <mln/core/concept/gpoint.hh>
# include <mln/value/concept/all.hh>
# include <mln/value/rgb.hh>

# include <mln/convert/impl/all.hh>

# include <mln/algebra/vec.hh>
# include <mln/metal/is.hh>



namespace mln
{

  namespace convert
  {


    /// Conversion of an object \p from towards an object \p to.
    template <typename F, typename T>
    inline
    void
    from_to(F& from, Object<T>& to); // See below an explanation of this signature.


    template <typename T>
    inline
    void
    from_to(const float& from, Object<T>& to);


    template <typename T>
    inline
    void
    from_to(const int& from, Object<T>& to);


    // Technical note: 

    // In order to remain as general as possible (for instance to be
    // able to pass a C array to 'from'), the signature cannot be
    // "const F& from."  It cannot embed the strong typing "Object<*>"
    // neither.


# ifndef MLN_INCLUDE_ONLY

    namespace impl
    {

      // Default (F -> Object) means "unknown conversion".
      template <typename F, typename T>
      void
      from_to_(const F& from, Object<T>& to);


      // Image -> Site_Set.
      template <typename I, typename S>
      inline
      void
      from_to_(const Image<I>& from, Site_Set<S>& to)
      {
	mlc_is(mln_trait_site_set_contents(S),
	       mln::trait::site_set::contents::dynamic)::check();
	mln_precondition(exact(from).has_data());
	mln::convert::impl::from_image_to_site_set(from, to);
      }

      // algebra::vec -> Gpoint.
      template <unsigned n, typename T, typename P>
      inline
      void
      from_to_(const algebra::vec<n,T>& from, Gpoint<P>& to_)
      {
	mlc_bool(P::dim == n)::check();
	P& to = exact(to_);
	for (unsigned i = 0; i < n; ++i)
	  to[i] = static_cast< typename P::coord >(from[i]);
      }

      // algebra::vec -> rgb.
      template <typename T, unsigned m>
      inline
      void
      from_to_(const algebra::vec<3,T>& from, value::rgb<m>& to_)
      {
        value::rgb<m>& to = exact(to_);
        algebra::vec<3, unsigned> tmp;
	for (unsigned i = 0; i < 3; ++i)
          tmp[i] = static_cast<unsigned>(from[i]);

        to = value::rgb<m>(tmp);
      }


      template <typename F, typename T>
      inline
      void
      from_to_(const Value<F>& from, Value<T>& to)
      {
	mln::convert::impl::from_value_to_value(from, to);
      }

    } // end of namespace mln::convert::impl



    // Facades.


    template <typename F, typename T>
    inline
    void
    from_to(F& from, Object<T>& to)
    {
      trace::entering("convert::from_to");
      mlc_equal(F, mln_exact(F))::check();
      impl::from_to_(from, exact(to));
      trace::exiting("convert::from_to");
    }


    template <typename T>
    inline
    void
    from_to(const float& from, Object<T>& to)
    {
      mlc_is_a(T, mln::value::Floating)::check();
      exact(to) = from;
    }


    template <typename T>
    inline
    void
    from_to(const int& from, Object<T>& to)
    {
      mlc_is_a(T, mln::value::Integer)::check();
      exact(to) = from;
    }


# endif // ! MLN_INCLUDE_ONLY

  } // end of namespace mln::convert

} // end of namespace mln


#endif // ! MLN_CONVERT_FROM_TO_HH
