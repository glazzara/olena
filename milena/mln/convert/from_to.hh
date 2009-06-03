// Copyright (C) 2008, 2009 EPITA Research and Development Laboratory
// (LRDE)
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

/// \file mln/convert/from_to.hh
///
/// \brief General conversion procedure between two objects.
///
/// \todo Test the effectiveness of guards.
///
/// \todo Dispatch code in appropriate files.
///
/// \todo Find a solution for g++-2.95 and '...'; see FIXME below.

# include <mln/convert/impl/all.hh>
# include <mln/convert/from_to.hxx>

# include <mln/metal/abort.hh>
# include <mln/metal/converts_to.hh>
# include <mln/metal/is.hh>
# include <mln/metal/is_a.hh>


namespace mln
{

  // Forward declarations.
  template <typename E> struct Object;
  template <typename E> struct Value;
  template <typename E> struct Site_Set;
  template <typename E> struct Image;

  namespace convert
  {

    template <typename F, typename T>
    void
    from_to(const F& from, T& to);


# ifndef MLN_INCLUDE_ONLY

    namespace internal
    {

      // Dispatch to specific implementation.

      // Image -> Site_Set.
      template <typename I, typename S>
      inline
      void
      from_to_dispatch(const Image<I>& from, Site_Set<S>& to)
      {
	mlc_is(mln_trait_site_set_contents(S),
	    mln::trait::site_set::contents::dynamic)::check();
	mln_precondition(exact(from).is_valid());
	mln::convert::impl::from_image_to_site_set(from, to);
      }


      // Site_Set -> Image.
      template <typename S, typename I>
      inline
      void
      from_to_dispatch(const Site_Set<S>& from, Image<I>& to)
      {
	mlc_converts_to(mln_site(S), mln_site(I))::check(); // FIXME: Is it too restrictive?
	mln_precondition(exact(from).is_valid());
	mln::convert::impl::from_site_set_to_image(from, to);
      }


      // Value -> Value
      template <typename F, typename T>
      inline
      void
      from_to_dispatch(const Value<F>& from, Value<T>& to)
      {
	mln::convert::impl::from_value_to_value(from, to);
      }



      // Dispatch related to convertible objects.

      // F -> T
      // if F convertible to T.
      template <typename F, typename T>
      inline
      void
      from_to_dispatch(metal::true_,
		       const Object<F>& from, Object<T>& to)
      {
	exact(to) = exact(from);
      }


      // F is NOT convertible to T.
      template <typename F, typename T>
      inline
      void
      from_to_dispatch(metal::false_,
		       const Object<F>& from, Object<T>& to)
      {
	over_load::from_to_(exact(from), exact(to));
      }



      // Default dispatch if the two arguments are objects.

      // Object -> Object
      template <typename F, typename T>
      inline
      void
      from_to_dispatch(const Object<F>& from, Object<T>& to)
      {
	typedef mlc_converts_to(F, T) F_converts_to_T; // FIXME: HERE we've got a problem with g++-2.95.
	internal::from_to_dispatch(F_converts_to_T(),
				   exact(from), exact(to));
      }



      // Dispatch entry points.
      // Check whether arguments are an object or not.

      // Builtin -> Builtin
      template <typename F, typename T>
      inline
      void
      from_to_dispatch(metal::false_,  const F& from,
		       metal::false_,  T&	to)
      {
	over_load::from_to_(from, to);
      }


      // Object -> Builtin
      template <typename F, typename T>
      inline
      void
      from_to_dispatch(metal::true_,  const F& from,
		       metal::false_, T&       to)
      {
	over_load::from_to_(exact(from), to);
      }


      // Builtin -> Object
      template <typename F, typename T>
      inline
      void
      from_to_dispatch(metal::false_, const F& from,
		       metal::true_,  T&       to)
      {
	over_load::from_to_(from, exact(to));
      }

      // Object -> Object
      template <typename F, typename T>
      inline
      void
      from_to_dispatch(metal::true_,  const F& from,
		       metal::true_,  T&       to)
      {
	internal::from_to_dispatch(exact(from), exact(to));
      }


    } // end of namespace mln::convert::internal


    namespace over_load
    {


      // Object -> Object (F not convertible towards T)
      // No conversion exists!
      template <typename F, typename T>
      void
      from_to_(const Object<F>&, Object<T>&)
      {
	// This particular from-to is not defined!
	//
	// Either this conversion is meaningless or an overload is
	// missing.
	mlc_abort(F)::check();
      }


      // Object -> Object
      template <typename T>
      inline
      void
      from_to_(const Object<T>& from, Object<T>& to)
      {
	exact(to) = exact(from);
      }

      template <typename T>
      inline
      void
      from_to_(const T& from, T& to)
      {
	to = from;
      }


    } // end of namespace mln::convert::over_load



    // Facade

    template <typename F, typename T>
    inline
    void
    from_to(const F& from, T& to)
    {
      typedef mlc_is_a(F, Object) F_is_object;
      typedef mlc_is_a(T, Object) T_is_object;
      internal::from_to_dispatch(F_is_object(), from,
				 T_is_object(), to);
    }


# endif // ! MLN_INCLUDE_ONLY

  } // end of namespace mln::convert

} // end of namespace mln


#endif // ! MLN_CONVERT_FROM_TO_HH
