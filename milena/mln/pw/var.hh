// Copyright (C) 2007, 2008, 2009 EPITA Research and Development Laboratory (LRDE)
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

#ifndef MLN_PW_VAR_HH
# define MLN_PW_VAR_HH

/*! \file
 *
 * \brief FIXME.
 */

# include <mln/core/concept/function.hh>


namespace mln
{

  // FIXME: Doc!

  namespace pw
  {

    template <typename V>
    struct var_ : public Function_v2v< var_<V> >
    {
      typedef V result;

      var_();

      var_(const V& v);

      template <typename P>
      const V& operator()(const P&) const;

    private:
      const V* v_;
    };


    template <typename V>
    var_<V> var(const V& v);


# ifndef MLN_INCLUDE_ONLY

    // pw::var_<V>

    template <typename V>
    inline
    var_<V>::var_()
      : v_(0)
    {
    }

    template <typename V>
    inline
    var_<V>::var_(const V& v)
      : v_(&v)
    {
    }

    template <typename V>
    template <typename P>
    inline
    const V&
    var_<V>::operator()(const P&) const
    {
      mln_precondition(v_ != 0);
      return *v_;
    }

    // pw::var(v)

    template <typename V>
    inline
    var_<V> var(const V& v)
    {
      var_<V> tmp(v);
      return tmp;
    }

# endif // ! MLN_INCLUDE_ONLY

  } // end of namespace mln::pw

} // end of namespace mln


#endif // ! MLN_PW_VAR_HH
