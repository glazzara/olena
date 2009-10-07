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

#ifndef MLN_CORE_DINDEX_SITE_HH
# define MLN_CORE_DINDEX_SITE_HH

# include <mln/core/concept/index_site.hh>
# include <mln/core/concept/dindex_site.hh>

namespace mln
{
  /// \{ Forward declarations.
  namespace literal {
    struct zero_t;
    struct one_t;
  }
  /// \}


  template <typename P> struct dindex_site;

  template <typename P>
  struct dindex_site : public Dindex_Site< dindex_site<P> >
  {
    typedef P site;
    typedef P psite;

    typedef int value_t;

    /// Constructors
    /// \{
    dindex_site();
    dindex_site(int i);
    dindex_site(const literal::zero_t&);
    dindex_site(const literal::one_t&);
    /// \}

    /// Value access
    /// \{
    const value_t& value() const;
    void change_value(const value_t& v);
    /// \}

    protected:
      value_t v_;
    };

# ifndef MLN_INCLUDE_ONLY

    template <typename P>
    inline
    dindex_site<P>::dindex_site()
    {
    }

    template <typename P>
    inline
    dindex_site<P>::dindex_site(int i)
      : v_ (i)
    {
    }

    template <typename P>
    inline
    dindex_site<P>::dindex_site(const literal::zero_t&)
      : v_ (0)
    {
    }

    template <typename P>
    inline
    dindex_site<P>::dindex_site(const literal::one_t&)
      : v_ (1)
    {
    }

    template <typename P>
    inline
    const typename dindex_site<P>::value_t&
    dindex_site<P>::value() const
    {
      return v_;
    }

    template <typename P>
    inline
    void
    dindex_site<P>::change_value(const value_t& v)
    {
      v_ = v;
    }

# endif // ! MLN_INCLUDE_ONLY

} // end of namespace mln

#endif // !MLN_CORE_DINDEX_SITE_HH
