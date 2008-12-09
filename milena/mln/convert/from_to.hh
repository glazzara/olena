// Copyright (C) 2008 EPITA Research and Development Laboratory (LRDE)
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
/// General conversion procedure between two objects.
///
/// \todo Test the effectiveness of guards.
/// \todo Add fwd decls.
/// \todo Dispatch code in appropriate files.


# include <mln/core/concept/object.hh>
# include <mln/core/concept/gpoint.hh>
# include <mln/value/concept/all.hh>

# include <mln/convert/impl/all.hh>

# include <mln/algebra/vec.hh>
# include <mln/metal/is.hh>
# include <mln/metal/abort.hh>



namespace mln
{

  namespace convert
  {


    /// Guard.
    template <typename F, typename T>
    void
    from_to(const Object<F>& from, Object<T>& to);

    template <typename T>
    void
    from_to(const float& from, Object<T>& to);


    template <typename T>
    void
    from_to(const int& from, Object<T>& to);



# ifndef MLN_INCLUDE_ONLY


    // Guard.
    template <typename F, typename T>
    void
    from_to(const Object<F>&, Object<T>&)
    {
      // This particular from-to is not defined!
      //
      // Either this conversion is meaningless or an overload is
      // missing.
      mlc_abort(F)::check();
    }


    // Image -> Site_Set.
    template <typename I, typename S>
    inline
    void
    from_to(const Image<I>& from, Site_Set<S>& to)
    {
      mlc_is(mln_trait_site_set_contents(S),
	     mln::trait::site_set::contents::dynamic)::check();
      mln_precondition(exact(from).has_data());
      mln::convert::impl::from_image_to_site_set(from, to);
    }

    // Value -> Value
    template <typename F, typename T>
    inline
    void
    from_to(const Value<F>& from, Value<T>& to)
    {
      mln::convert::impl::from_value_to_value(from, to);
    }

    // float -> Object
    template <typename T>
    inline
    void
    from_to(const float& from, Object<T>& to)
    {
      mlc_is_a(T, mln::value::Floating)::check();
      exact(to) = from;
    }

    // int -> Object
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
