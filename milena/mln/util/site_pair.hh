// Copyright (C) 2008, 2009, 2012 EPITA Research and Development
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

#ifndef MLN_UTIL_SITE_PAIR_HH
# define MLN_UTIL_SITE_PAIR_HH

# include <mln/core/concept/proxy.hh>
# include <mln/util/ord_pair.hh>

/// \file
///
/// Definition of a site pair type.

namespace mln
{

  namespace util
  {

    /* FIXME: The class mln::internal::image_base requires its site
       types to have a `coord' typedef, but util::ord_pair has none.
       Hence this small workaround.  Use ord_pair directly as soon as
       image_base is refurbished.  */
    /// \brief A pair of sites.
    /// It can be used as site.
    ///
    /// \ingroup modutil
    //
    template <typename P>
    class site_pair : public mln::Object< site_pair<P> >
    {
    public:
      /* FIXME: We should not need to define this typedef
	 (see. mln::internal::image_base's site `coord' typedef).  */
      typedef mln_coord(P) coord;

      site_pair();
      site_pair(const P& first, const P& second);

      /// Return the first site.
      const P& first() const;
      /// Return the second site.
      const P& second() const;

      /// Return the underlying pair.
      const util::ord_pair<P>& pair() const;

    private:
      util::ord_pair<P> pair_;
    };


    /* FIXME: The proxy mechanism requires operator== and operator<= for
       line_graph_psite to compile (note: line_graph_psite aggregates a
       site_pair); why?  It seems its static assertions do not check the
       right guy(s).  IMHO (Roland's), it should look for
       line_graph_psite's operators, not site_pair's.  */
    template <typename P>
    bool operator==(const site_pair<P>& lhs, const site_pair<P>& rhs);

    template <typename P>
    bool operator< (const site_pair<P>& lhs, const site_pair<P>& rhs);

    template <typename P>
    bool operator< (const site_pair<P>& lhs, const site_pair<P>& rhs);

    template <typename P>
    std::ostream&
    operator<<(std::ostream& ostr, const site_pair<P>& p);

  } // end of namespace mln::util


  namespace internal
  {

    /// \{
    /*!
      \internal
      \brief subject_impl specialization (Proxy)
    */
    template <typename P, typename E>
    struct subject_impl< const util::site_pair<P>, E >
    {
      const P& first() const;
      const P& second() const;
      const util::ord_pair<P>& pair() const;

    private:
      const E& exact_() const;
    };

    /// \}

  } // end of namespace mln::internal



# ifndef MLN_INCLUDE_ONLY

  namespace util
  {

    /*---------------.
    | Construction.  |
    `---------------*/

    template <typename P>
    site_pair<P>::site_pair()
    {
    }

    template <typename P>
    site_pair<P>::site_pair(const P& first, const P& second)
      : pair_(first, second)
    {
    }

    template <typename P>
    inline
    const P&
    site_pair<P>::first() const
    {
      return pair_.first();
    }

    template <typename P>
    inline
    const P&
    site_pair<P>::second() const
    {
      return pair_.second();
    }

    template <typename P>
    inline
    const util::ord_pair<P>&
    site_pair<P>::pair() const
    {
      return pair_;
    }

    /*-------------.
    | Comparison.  |
    `-------------*/

    template <typename P>
    inline
    bool
    operator==(const site_pair<P>& lhs, const site_pair<P>& rhs)
    {
      return lhs.pair() == rhs.pair();
    }

    template <typename P>
    inline
    bool
    operator< (const site_pair<P>& lhs, const site_pair<P>& rhs)
    {
      return lhs.pair() < rhs.pair();
    }

    template <typename P>
    inline
    bool
    operator<=(const site_pair<P>& lhs, const site_pair<P>& rhs)
    {
      return lhs.pair() <= rhs.pair();
    }

    template <typename P>
    inline
    std::ostream&
    operator<<(std::ostream& ostr, const site_pair<P>& p)
    {
      ostr << "(" << p.first() << ", " << p.second() << ")";
      return ostr;
    }

  } // end of mln::util

  namespace internal
  {

    template <typename P, typename E>
    inline
    const E&
    subject_impl< const util::site_pair<P>, E >::exact_() const
    {
      return internal::force_exact<const E>(*this);
    }

    template <typename P, typename E>
    inline
    const P&
    subject_impl< const util::site_pair<P>, E >::first() const
    {
      return exact_().get_subject().first();
    }

    template <typename P, typename E>
    inline
    const P&
    subject_impl< const util::site_pair<P>, E >::second() const
    {
      return exact_().get_subject().second();
    }

  } // end of namespace mln::internal

# endif // ! MLN_INCLUDE_ONLY

} // end of mln


#endif // ! MLN_UTIL_SITE_PAIR_HH
