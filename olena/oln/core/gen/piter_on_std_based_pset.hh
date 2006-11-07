// Copyright (C) 2006 EPITA Research and Development Laboratory
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

#ifndef OLN_CORE_GEN_PITER_ON_STD_BASED_PSET_HH
# define OLN_CORE_GEN_PITER_ON_STD_BASED_PSET_HH

# include <oln/core/abstract/point.hh>
# include <oln/core/abstract/iterator_on_points.hh>


namespace oln
{


  // Fwd_piter version.
  // ------------------


  // Forward declaration.
  template <typename C> class fwd_piter_on_std_based_pset;


  // Super.
  template <typename C>
  struct set_super_type< fwd_piter_on_std_based_pset<C> >
  {
    typedef abstract::iterator_on_points< fwd_piter_on_std_based_pset<C> > ret;
  };


  // Vtypes.
  template <typename C>
  struct vtypes< fwd_piter_on_std_based_pset<C> >
  {
    typedef typename C::value_type point_type;
  };


  // Class.
  template <typename C>
  class fwd_piter_on_std_based_pset :

    public abstract::iterator_on_points< fwd_piter_on_std_based_pset<C> >,
    private mlc::assert_< mlc_is_a(typename C::value_type, abstract::point) >
  {
  public:
    fwd_piter_on_std_based_pset(const C& container);

    template <typename point_set>
    fwd_piter_on_std_based_pset(const point_set& ps);

    void impl_start();
    void impl_next();
    void impl_invalidate();
    bool impl_is_valid() const;
    typename C::value_type impl_to_point() const;
    const typename C::value_type* impl_point_adr() const;

  private:

    const C& container_;
    typename C::const_iterator it_;
    
  }; // end of class fwd_piter_on_std_based_pset<C>



  // Bkd_piter version.
  // ------------------


  // Forward declaration.
  template <typename C> class bkd_piter_on_std_based_pset;


  // Super.
  template <typename C>
  struct set_super_type< bkd_piter_on_std_based_pset<C> >
  {
    typedef abstract::iterator_on_points< bkd_piter_on_std_based_pset<C> > ret;
  };


  // Vtypes.
  template <typename C>
  struct vtypes< bkd_piter_on_std_based_pset<C> >
  {
    typedef typename C::value_type point_type;
  };


  // Class.
  template <typename C>
  class bkd_piter_on_std_based_pset :

    public abstract::iterator_on_points< bkd_piter_on_std_based_pset<C> >,
    private mlc::assert_< mlc_is_a(typename C::value_type, abstract::point) >
  {
  public:
    bkd_piter_on_std_based_pset(const C& container);

    template <typename point_set>
    bkd_piter_on_std_based_pset(const point_set& ps);

    void impl_start();
    void impl_next();
    void impl_invalidate();
    bool impl_is_valid() const;
    typename C::value_type impl_to_point() const;
    const typename C::value_type* impl_point_adr() const;

  private:

    const C& container_;
    typename C::const_reverse_iterator it_;
    
  }; // end of class bkd_piter_on_std_based_pset<C>




# ifndef OLN_INCLUDE_ONLY


  // fwd_piter_on_std_based_pset<C>


  template <typename C>
  fwd_piter_on_std_based_pset<C>::fwd_piter_on_std_based_pset(const C& container)
    : container_(container)
  {
    this->invalidate();
  }
  
  template <typename C>
  template <typename point_set>
  fwd_piter_on_std_based_pset<C>::fwd_piter_on_std_based_pset(const point_set& ps)
    : container_(ps.exact().container())
  {
    this->invalidate();
  }

  template <typename C>
  void fwd_piter_on_std_based_pset<C>::impl_start()
  {
    it_ = container_.begin();
  }

  template <typename C>
  void fwd_piter_on_std_based_pset<C>::impl_next()
  {
    ++it_;
  }

  template <typename C>
  void fwd_piter_on_std_based_pset<C>::impl_invalidate()
  {
    it_ = container_.end();
  }

  template <typename C>
  bool fwd_piter_on_std_based_pset<C>::impl_is_valid() const
  {
    return it_ != container_.end();
  }

  template <typename C>
  typename C::value_type
  fwd_piter_on_std_based_pset<C>::impl_to_point() const
  {
    return *it_;
  }

  template <typename C>
  const typename C::value_type*
  fwd_piter_on_std_based_pset<C>::impl_point_adr() const
  {
    return 0;
    // FIXME: Read below.
    // "&(*it_)" is not correct because the std does not ensure that
    // "*it_" is dereferenceable (Cf. std trivial iterator concept).
    // However, "::point_adr()" is only required so that an iterator
    // based on another iterator (e.g., a niter constructed from a
    // piter) can stick to the point location of the latter.  This
    // is not required for iterators on point set so this method should
    // be optional.
  }


  // bkd_piter_on_std_based_pset<C>


  template <typename C>
  bkd_piter_on_std_based_pset<C>::bkd_piter_on_std_based_pset(const C& container)
    : container_(container)
  {
    this->invalidate();
  }
  
  template <typename C>
  template <typename point_set>
  bkd_piter_on_std_based_pset<C>::bkd_piter_on_std_based_pset(const point_set& ps)
    : container_(ps.exact().container())
  {
    this->invalidate();
  }

  template <typename C>
  void bkd_piter_on_std_based_pset<C>::impl_start()
  {
    it_ = container_.rbegin();
  }

  template <typename C>
  void bkd_piter_on_std_based_pset<C>::impl_next()
  {
    ++it_;
  }

  template <typename C>
  void bkd_piter_on_std_based_pset<C>::impl_invalidate()
  {
    it_ = container_.rend();
  }

  template <typename C>
  bool bkd_piter_on_std_based_pset<C>::impl_is_valid() const
  {
    return it_ != container_.rend();
  }

  template <typename C>
  typename C::value_type
  bkd_piter_on_std_based_pset<C>::impl_to_point() const
  {
    return *it_;
  }

  template <typename C>
  const typename C::value_type*
  bkd_piter_on_std_based_pset<C>::impl_point_adr() const
  {
    return 0;
    // FIXME: Read comments in the fwd version of this method.
  }


# endif


} // end of namespace oln


#endif // ! OLN_CORE_GEN_PITER_ON_STD_BASED_PSET_HH
