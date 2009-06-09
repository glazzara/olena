// Copyright (C) 2008, 2009 EPITA Research and Development Laboratory (LRDE)
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

#ifndef MLN_CORE_ROUTINE_EXTEND_HH
# define MLN_CORE_ROUTINE_EXTEND_HH

/// \file
///
/// Definition of a morpher that extends the extended domain of an image.
///
/// \todo Use the 'instant' mechanism.
/// \todo Deal with ambiguities.
/// \todo Check that there is no extension yet (except "extendable").

# include <mln/core/image/dmorph/extension_ima.hh>
# include <mln/core/image/dmorph/extension_fun.hh>
# include <mln/core/image/dmorph/extension_val.hh>



namespace mln
{


  /// Routines for domain extension with a function.

  template <typename I, typename F>
  extension_fun<const I, F>
  extend(const Image<I>& ima, const Function_v2v<F>& fun);

  template <typename I, typename F>
  extension_fun<I, F>
  extend(Image<I>& ima, const Function_v2v<F>& fun);


  /// Routines for domain extension with an image.

  template <typename I, typename J>
  extension_ima<const I, const J>
  extend(const Image<I>& ima, const Image<J>& ext);


  /// Routines for domain extension with a value.

  template <typename I>
  extension_val<const I>
  extend(const Image<I>& ima, const mln_value(I)& val);

  template <typename I>
  extension_val<I>
  extend(Image<I>& ima, const mln_value(I)& val);



# ifndef MLN_INCLUDE_ONLY


  // With a function.

  template <typename I, typename F>
  inline
  extension_fun<const I, F>
  extend(const Image<I>& ima, const Function_v2v<F>& fun)
  {
    mlc_converts_to(mln_result(F), mln_value(I))::check();
    extension_fun<const I, F> tmp(exact(ima), exact(fun));
    return tmp;
  }

  template <typename I, typename F>
  inline
  extension_fun<I, F>
  extend(Image<I>& ima, const Function_v2v<F>& fun)
  {
    mlc_converts_to(mln_result(F), mln_value(I))::check();
    extension_fun<I, F> tmp(exact(ima), exact(fun));
    return tmp;
  }


  // With an image.

  template <typename I, typename J>
  extension_ima<const I, const J>
  extend(const Image<I>& ima, const Image<J>& ext)
  {
    mlc_converts_to(mln_value(J), mln_value(I))::check();
    extension_ima<const I, const J> tmp(exact(ima), exact(ext));
    return tmp;
  }


  // With a value.

  template <typename I>
  inline
  extension_val<const I>
  extend(const Image<I>& ima, const mln_value(I)& val)
  {
    extension_val<const I> tmp(exact(ima), val);
    return tmp;
  }

  template <typename I>
  inline
  extension_val<I>
  extend(Image<I>& ima, const mln_value(I)& val)
  {
    extension_val<I> tmp(exact(ima), val);
    return tmp;
  }


# endif // ! MLN_INCLUDE_ONLY

} // end of namespace mln


#endif // ! MLN_CORE_ROUTINE_EXTEND_HH
