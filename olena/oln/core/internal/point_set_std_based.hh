// Copyright (C) 2006, 2007 EPITA Research and Development Laboratory
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

#ifndef OLN_CORE_INTERNAL_POINT_SET_STD_BASED_HH
# define OLN_CORE_INTERNAL_POINT_SET_STD_BASED_HH

# include <ostream>
# include <algorithm>

# include <oln/core/internal/point_set_base.hh>
# include <oln/core/concept/iterator_on_points.hh>
# include <oln/core/gen/fbbox.hh>


namespace oln
{


  /// Fwd decl.
  namespace internal { template <typename Exact> struct point_set_std_based_; }
  template <typename P> class pset_std_based_fwd_piter_;
  template <typename P> class pset_std_based_bkd_piter_;


  /// Super type.
  template <typename Exact>
  struct super_trait_< internal::point_set_std_based_<Exact> >
  {
    typedef internal::point_set_base_<Exact> ret;
  };


  /// Virtual types.
  template <typename Exact>
  struct vtypes< internal::point_set_std_based_<Exact> >
  {
    typedef stc::abstract std_container;

    typedef stc_deferred(std_container) std_container__;
    typedef stc::final< typename std_container__::value_type > point;
    typedef stc::final< pset_std_based_fwd_piter_<std_container__> > fwd_piter;
    typedef stc::final< pset_std_based_bkd_piter_<std_container__> > bkd_piter;
  };


  namespace internal
  {

    /// Base class for point sets defined over std containers.

    template <typename Exact>
    struct point_set_std_based_ : public point_set_base_<Exact>
    {
      typedef point_set_base_<Exact> super;
    public:

      stc_using(point);
      stc_using(box);

      unsigned   impl_npoints() const;
      bool       impl_has(const point& p) const;
      const box& impl_bbox() const;

      stc_typename(std_container);
      const std_container& con() const;

    protected:

      point_set_std_based_();

      void take_(const point& p);

      std_container con_;
      fbbox_<point> fb_;

    }; // end of class point_set_std_based_<Exact>


    template <typename Exact>
    std::ostream& operator<<(std::ostream& ostr,
			     const point_set_std_based_<Exact>& pts)
    {
      typename Exact::fwd_piter i(pts);
      ostr << "{ ";
      for_all(i)
	ostr << i.to_point() << ' ';
      ostr << "}";
      return ostr;
    }


# ifndef OLN_INCLUDE_ONLY

    template <typename Exact>
    unsigned
    point_set_std_based_<Exact>::impl_npoints() const
    {
      return this->con_.size();
    }

    template <typename Exact>
    bool
    point_set_std_based_<Exact>::impl_has(const typename point_set_std_based_<Exact>::point& p) const
    {
      return std::find(this->con_.begin(), this->con_.end(), p) != this->con_.end();
    }

    template <typename Exact>
    const typename point_set_std_based_<Exact>::box&
    point_set_std_based_<Exact>::impl_bbox() const
    {
      precondition(this->fb_.is_valid());
      return this->fb_.box();
    }

    template <typename Exact>
    point_set_std_based_<Exact>::point_set_std_based_()
    {
    }

    template <typename Exact>
    void
    point_set_std_based_<Exact>::take_(const typename point_set_std_based_<Exact>::point& p)
    {
      this->fb_.take(p);
    }

    template <typename Exact>
    const typename point_set_std_based_<Exact>::std_container&
    point_set_std_based_<Exact>::con() const
    {
      return this->con_;
    }

# endif

  } // end of namespace oln::internal




  // --------------------   iterators  on  classes deriving from internal::point_set_std_based_<Exact>



  // Super types.

  template <typename C>
  struct super_trait_< pset_std_based_fwd_piter_<C> >
  {
    typedef pset_std_based_fwd_piter_<C> current__;
    typedef Iterator_on_Points<current__> ret;
  };

  template <typename C>
  struct super_trait_< pset_std_based_bkd_piter_<C> >
  {
    typedef pset_std_based_bkd_piter_<C> current__;
    typedef Iterator_on_Points<current__> ret;
  };



  /// Virtual types.

  template <typename C>
  struct vtypes< pset_std_based_fwd_piter_<C> >
  {
    typedef typename C::value_type point;
  };

  template <typename C>
  struct vtypes< pset_std_based_bkd_piter_<C> >
  {
    typedef typename C::value_type point;
  };


  // Class pset_std_based_fwd_piter_<C>.

  template <typename C>
  class pset_std_based_fwd_piter_ : public Iterator_on_Points< pset_std_based_fwd_piter_<C> >
  {
    typedef pset_std_based_fwd_piter_<C> current;
    typedef Iterator_on_Points<current>  super;
  public:

    stc_using(point);

    template <typename Ps>
    pset_std_based_fwd_piter_(const internal::point_set_std_based_<Ps>& con);

    void impl_start();
    void impl_next();
    void impl_invalidate();
    bool impl_is_valid() const;
    point impl_to_point() const;
    const point* impl_point_adr() const;

  private:

    const C& con_;
    typename C::const_iterator it_;
    
  }; // end of class pset_std_based_fwd_piter_<C>


  // Class pset_std_based_bkd_piter_<C>.

  template <typename C>
  class pset_std_based_bkd_piter_ : public Iterator_on_Points< pset_std_based_bkd_piter_<C> >
  {
    typedef pset_std_based_bkd_piter_<C> current;
    typedef Iterator_on_Points<current>  super;
  public:

    stc_using(point);

    template <typename Ps>
    pset_std_based_bkd_piter_(const internal::point_set_std_based_<Ps>& pts);

    void impl_start();
    void impl_next();
    void impl_invalidate();
    bool impl_is_valid() const;
    point impl_to_point() const;
    const point* impl_point_adr() const;

  private:

    const C& con_;
    typename C::const_reverse_iterator it_;
    
  }; // end of class pset_std_based_bkd_piter_<C>



# ifndef OLN_INCLUDE_ONLY



  // --------------------   pset_std_based_fwd_piter_<C>


  template <typename C>
  template <typename Ps>
  pset_std_based_fwd_piter_<C>::pset_std_based_fwd_piter_(const internal::point_set_std_based_<Ps>& pts)
    : con_(pts.con())
  {
    this->it_ = this->con_.end();
  }

  template <typename C>
  void pset_std_based_fwd_piter_<C>::impl_start()
  {
    this->it_ = this->con_.begin();
  }

  template <typename C>
  void pset_std_based_fwd_piter_<C>::impl_next()
  {
    ++this->it_;
  }

  template <typename C>
  void pset_std_based_fwd_piter_<C>::impl_invalidate()
  {
    this->it_ = this->con_.end();
  }

  template <typename C>
  bool pset_std_based_fwd_piter_<C>::impl_is_valid() const
  {
    return this->it_ != this->con_.end();
  }

  template <typename C>
  typename pset_std_based_fwd_piter_<C>::point
  pset_std_based_fwd_piter_<C>::impl_to_point() const
  {
    return *this->it_;
  }

  template <typename C>
  const typename pset_std_based_fwd_piter_<C>::point*
  pset_std_based_fwd_piter_<C>::impl_point_adr() const
  {
    return &(*(this->it_));
    // FIXME: "&(*it_)" is not always correct because the std does not
    // ensure that "*it_" is dereferenceable (Cf. std trivial iterator
    // concept).  However, "::point_adr()" is only required so that an
    // iterator based on another iterator (e.g., a niter constructed
    // from a piter) can stick to the point location of the latter.
    // This is not required for iterators on point set so this method
    // should be optional.
  }


  // --------------------   pset_std_based_bkd_piter_<C>


  template <typename C>
  template <typename Ps>
  pset_std_based_bkd_piter_<C>::pset_std_based_bkd_piter_(const internal::point_set_std_based_<Ps>& pts)
    : con_(pts.con())
  {
    this->it_ = this->con_.rend();
  }

  template <typename C>
  void pset_std_based_bkd_piter_<C>::impl_start()
  {
    this->it_ = this->con_.rbegin();
  }

  template <typename C>
  void pset_std_based_bkd_piter_<C>::impl_next()
  {
    ++this->it_;
  }

  template <typename C>
  void pset_std_based_bkd_piter_<C>::impl_invalidate()
  {
    this->it_ = this->con_.rend();
  }

  template <typename C>
  bool pset_std_based_bkd_piter_<C>::impl_is_valid() const
  {
    return this->it_ != this->con_.rend();
  }

  template <typename C>
  typename pset_std_based_bkd_piter_<C>::point
  pset_std_based_bkd_piter_<C>::impl_to_point() const
  {
    return *this->it_;
  }

  template <typename C>
  const typename pset_std_based_bkd_piter_<C>::point*
  pset_std_based_bkd_piter_<C>::impl_point_adr() const
  {
    return &(*(this->it_));
    // FIXME: Read comments in pset_std_based_fwd_piter_<C>.
  }


# endif


} // end of namespace oln


#endif // ! OLN_CORE_INTERNAL_POINT_SET_STD_BASED_HH
