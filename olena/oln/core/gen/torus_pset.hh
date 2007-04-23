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

#ifndef OLN_CORE_GEN_TORUS_PSET_HH
# define OLN_CORE_GEN_TORUS_PSET_HH

# include <ostream>

# include <oln/core/internal/pset_adaptor.hh>
# include <oln/core/concept/dpoint.hh>



namespace oln
{

# define current torus_pset<Ps>
# define super   internal::pset_adaptor_< current >

  // Fwd decl.
  template <typename Ps> class torus_pset;


  // Super type.
  template <typename Ps>
  struct super_trait_< current >
  {
    typedef super ret;
  };


  // Virtual types.
  template <typename Ps>
  struct vtypes< current >
  {
    typedef Ps adapted;
  };


  /// Generic box class based on a point class.

  template <typename Ps>
  class torus_pset : public super
  {
  public:

    stc_using(point);
    stc_using(box);
    typedef oln_dpoint(point) dpoint;

    torus_pset();
    torus_pset(const Ps& pset);
    void init_pset(const Ps& pset);

    operator Ps() const;

    point relocate(const point& p) const;
    bool impl_has(const point& p) const;

    const box& impl_theoretical_bbox() const;

  private:
    point pmin_;
    dpoint size_;

  }; // end of class oln::torus_pset<Ps>


  template <typename Ps>
  std::ostream& operator<<(std::ostream& ostr, const torus_pset<Ps>& ps);


# ifndef OLN_INCLUDE_ONLY

  template <typename Ps>
  current::torus_pset()
  {
    mlc::assert_< mlc_is_a(Ps, Point_Set) >::check();
  }

  template <typename Ps>
  current::torus_pset(const Ps& pset)
    : super(pset)
  {
    mlc::assert_< mlc_is_a(Ps, Point_Set) >::check();
    this->pmin_ = pset.bbox().pmin();
    dpoint unit; unit.set_all(1);
    this->size_ = pset.bbox().pmax() - pset.bbox().pmin() + unit;
  }

  template <typename Ps>
  void
  current::init_pset(const Ps& pset)
  {
    this->ps_ = pset;
    this->pmin_ = pset.bbox().pmin();
    dpoint unit; unit.set_all(1);
    this->size_ = pset.bbox().pmax() - pset.bbox().pmin() + unit;
  }

  template <typename Ps>
  current::operator Ps() const
  {
    return this->ps_;
  }

  template <typename Ps>
  typename current::point
  current::relocate(const typename current::point& p) const
  {
    return pmin_ + (p - pmin_) % size_;
  }

  template <typename Ps>
  bool 
  current::impl_has(const typename current::point& p) const
  {
    return this->ps_.has(relocate(p));
  }

  template <typename Ps>
  const typename current::box&
  current::impl_theoretical_bbox() const
  {
    return box::infty();
  }

  template <typename Ps>
  std::ostream& operator<<(std::ostream& ostr, const torus_pset<Ps>& ps)
  {
    return ostr << "torus " << ps.adapted_();
  }

# endif // ! OLN_INCLUDE_ONLY

# undef super
# undef current

} // end of namespace oln


#endif // ! OLN_CORE_GEN_TORUS_PSET_HH
