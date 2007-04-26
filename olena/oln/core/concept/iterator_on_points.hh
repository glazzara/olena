// Copyright (C) 2001, 2003, 2004, 2005, 2006, 2007 EPITA Research and
// Development Laboratory
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

#ifndef OLN_CORE_CONCEPT_ITERATOR_ON_POINTS_HH
# define OLN_CORE_CONCEPT_ITERATOR_ON_POINTS_HH

# include <ostream>

# include <oln/core/concept/iterator.hh>
# include <oln/core/concept/point.hh>


namespace oln
{

  /// Concept-class "Iterator_on_Points".

  template <typename Exact>
  struct Iterator_on_Points : public Iterator<Exact>,
			      public Generalized_Point<Exact>
  {
    stc_typename(point);

    const point* point_adr() const;

    // Final.
    operator point() const;

  protected:
    Iterator_on_Points();

  private:
    void check__() const;

  }; // end of class oln::Iterator_on_Points<Exact>


  template <typename It>
  std::ostream& operator<<(std::ostream& ostr, const Iterator_on_Points<It>& it);


  namespace internal
  {
    
    template <typename It>
    const oln_point(It)& to_point(const Iterator_on_Points<It>& it);

  } // end of namespace oln::internal



  // \{
  // Operator - traits.

  template <typename P, typename It>
  struct set_trait_< Point, P, minus_id, Iterator_on_Points, It >
  {
    typedef oln_point(It) Pit;
    typedef oln_minus_trait(P, Pit) ret;
  };

  template <typename It, typename P>
  struct set_trait_< Iterator_on_Points, It, minus_id, Point, P >
  {
    typedef oln_point(It) Pit;
    typedef oln_minus_trait(Pit, P) ret;
  };

  template <typename It1, typename It2>
  struct set_trait_< Iterator_on_Points, It1, minus_id, Iterator_on_Points, It2 >
  {
    typedef oln_point(It1) Pit1;
    typedef oln_point(It2) Pit2;
    typedef oln_minus_trait(Pit1, Pit2) ret;
  };

  // \}




# ifndef OLN_INCLUDE_ONLY

  template <typename Exact>
  const typename Iterator_on_Points<Exact>::point*
  Iterator_on_Points<Exact>::point_adr() const
  {
    return exact(this)->impl_point_adr();
  }

  template <typename Exact>
  Iterator_on_Points<Exact>::operator typename Iterator_on_Points<Exact>::point() const
  {
    precondition(this->is_valid());
    return *(exact(this)->impl_point_adr());
  }

  template <typename Exact>
  Iterator_on_Points<Exact>::Iterator_on_Points()
  {
    mlc::assert_< mlc_is_a(typename Iterator_on_Points<Exact>::point, Point) >::check();
    this->check__();
  }

  template <typename Exact>
  void Iterator_on_Points<Exact>::check__() const
  {
    const point* (Exact::*impl_point_adr_adr)() const = & Exact::impl_point_adr;
    impl_point_adr_adr = 0;
  }

  template <typename It>
  std::ostream& operator<<(std::ostream& ostr, const Iterator_on_Points<It>& it)
  {
    return ostr << (oln_point(It)(it));
  }

  namespace internal
  {
    
    template <typename It>
    const oln_point(It)& to_point(const Iterator_on_Points<It>& it)
    {
      return *(it.point_adr());
    }

  } // end of namespace oln::internal

# endif // ! OLN_INCLUDE_ONLY

} // end of namespace oln


#endif // ! OLN_CORE_CONCEPT_ITERATOR_ON_POINTS_HH
