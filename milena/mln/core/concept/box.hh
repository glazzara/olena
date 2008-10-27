// Copyright (C) 2007, 2008 EPITA Research and Development Laboratory
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

#ifndef MLN_CORE_CONCEPT_BOX_HH
# define MLN_CORE_CONCEPT_BOX_HH

/*! \file mln/core/concept/box.hh
 *
 * \brief Definition of the concept of mln::Box.
 */

# include <mln/core/concept/site_set.hh>


namespace mln
{

  /*! \brief Base class for implementation classes of boxes.
   *
   * Boxes are particular site sets useful to bound any set of
   * sites defined on a regular grid.
   *
   * \see mln::doc::Box for a complete documentation of this class
   * contents.
   */
  template <typename E>
  struct Box : public Site_Set<E>
  {
    /*
      const site& pmin() const;
      const site& pmax() const;
    */

    /*! \brief Give the length of the \p i-th side of the box.
     *
     * \pre i < site::dim
     *
     * \warning This method is final for all box classes.
     */
    unsigned len(unsigned i) const;

    // Box associated type.
    typedef const E& q_box;

    /*! \brief Give the bounding box of this site set.
     *
     * Return the bounding box of this site set, so that is itself.
     * This method is declared by the mln::Site_Set concept.
     *
     * \warning This method is final for all box classes.
     */
    const E& bbox() const;

    /*! \brief Give the number of sites of this box.
     *
     * Return the number of sites of this box.  This method is
     * declared by the mln::Site_Set concept.
     *
     * \warning This method is final for all box classes.
     */
    std::size_t nsites() const;

  protected:
    Box();
  };



  /*! \brief Equality test between boxes \p lhs and \p rhs.
   *
   * \param[in] lhs A box.
   * \param[in] rhs Another box.
   *
   * \relates mln::Box
   */
  template <typename Bl, typename Br>
  bool operator==(const Box<Bl>& lhs, const Box<Br>& rhs);



  /*! \brief Inclusion test between boxes \p lhs and \p rhs.
   *
   * \param[in] lhs A box (included?).
   * \param[in] rhs Another box (includor?).
   *
   * \relates mln::Box
   */
  template <typename Bl, typename Br>
  bool operator<=(const Box<Bl>& lhs, const Box<Br>& rhs);


  /*! \brief Strict inclusion test between boxes \p lhs and \p rhs.
   *
   * \param[in] lhs A box (strictly included?).
   * \param[in] rhs Another box (includor?).
   *
   * \relates mln::Box
   */
  template <typename Bl, typename Br>
  bool operator<(const Box<Bl>& lhs, const Box<Br>& rhs);




# ifndef MLN_INCLUDE_ONLY

  // Box<E>

  template <typename E>
  inline
  const E& Box<E>::bbox() const
  {
    return exact(*this);
  }

  template <typename E>
  inline
  unsigned Box<E>::len(unsigned i) const
  {
    return 1 + exact(this)->pmax()[i] - exact(this)->pmin()[i];
  }

  template <typename E>
  inline
  Box<E>::Box()
  {
    typedef mln_site(E) site;
    site (E::*m1)() const = & E::pmin;
    m1 = 0;
    site (E::*m2)() const = & E::pmax;
    m2 = 0;
  }

  template <typename E>
  inline
  std::size_t
  Box<E>::nsites() const
  {
    if (! exact(this)->is_valid())
      return 0;
    std::size_t count = 1;
    typedef mln_site(E) P; // Helps g++-3.3.5.
    for (unsigned i = 0; i < P::dim; ++i)
      count *= exact(this)->len(i);
    return count;
  }


  // Operators.

  template <typename Bl, typename Br>
  inline
  bool operator==(const Box<Bl>& lhs_, const Box<Br>& rhs_)
  {
    // FIXME: Same grid!
    const Bl& lhs = exact(lhs_);
    const Br& rhs = exact(rhs_);
    return lhs.pmin() == rhs.pmin() && lhs.pmax() == rhs.pmax();
  }

  template <typename Bl, typename Br>
  inline
  bool operator<=(const Box<Bl>& lhs_, const Box<Br>& rhs_)
  {
    // FIXME: Same grid!
    const Bl& lhs = exact(lhs_);
    const Br& rhs = exact(rhs_);
    typedef mln_site(Bl) P;
    for (unsigned i = 0; i < P::dim; ++i)
      if (lhs.pmin()[i] < rhs.pmin()[i] ||
	  lhs.pmax()[i] > rhs.pmax()[i])
	return false;
    return true;
  }

  template <typename Bl, typename Br>
  inline
  bool operator<(const Box<Bl>& lhs_, const Box<Br>& rhs_)
  {
    // FIXME: Same grid!
    const Bl& lhs = exact(lhs_);
    const Br& rhs = exact(rhs_);
    return lhs <= rhs && ! (lhs == rhs);
  }

# endif // ! MLN_INCLUDE_ONLY

} // end of namespace mln


#endif // ! MLN_CORE_CONCEPT_BOX_HH
