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

#ifndef OLN_CORE_GEN_BOX_HH
# define OLN_CORE_GEN_BOX_HH

# include <oln/core/concept/point.hh>
# include <oln/core/concept/iterator_on_points.hh>
# include <oln/core/internal/point_set_base.hh>
# include <oln/core/init.hh>


namespace oln
{


  // Forward declarations.
  template <typename P> class box_;
  template <typename P> class box_fwd_piter_;
  template <typename P> class box_bkd_piter_;


  // Super type declaration.
  template <typename P>
  struct super_trait_< box_<P> >
  {
    typedef box_<P> current__;
    typedef internal::point_set_base_<current__> ret;
  };


  /// Virtual types associated to oln::box_<P>.
  template <typename P>
  struct vtypes< box_<P> >
  {
    typedef P                 point;
    typedef box_fwd_piter_<P> fwd_piter;
    typedef box_bkd_piter_<P> bkd_piter;
  };


  /// init__
  namespace internal
  {
    template <typename P>
    void init__(box_<P>& b,
		const initializer_< pair< from_t<P>, to_t<P> > >& data);
  }


  /// Generic box class based on a point class.

  template <typename P>
  class box_ : public internal::point_set_base_< box_<P> >,
	       private mlc::assert_< mlc_is_a(P, Point) >
  {
    typedef box_<P> current;
    typedef internal::point_set_base_<current> super;
  public:

    stc_using(point);
    stc_using(box);

  private:
    typedef stc_type(point, dim) dim__;
    enum { n = mlc_value(dim__) };

  public:

    box_();
    box_(const P& pmin, const P& pmax);
    template <typename D>
    box_(const internal::initializer_<D>& data);

    unsigned       impl_npoints() const;
    bool           impl_has(const P& p) const;
    const box_<P>& impl_bbox() const;

    const P& pmin() const;
          P& pmin();
    const P& pmax() const;
          P& pmax();

  protected:
    point pmin_, pmax_;

  }; // end of class oln::box_<P>


  template <typename P>
  std::ostream& operator<<(std::ostream& ostr, const box_<P>& b)
  {
    return ostr << "{ " << b.pmin() << " .. " << b.pmax() << " }";
  }


  // --------------------   iterators  on  box_<P>




  /// Super types.

  template <typename P>
  struct super_trait_< box_fwd_piter_<P> >
  {
    typedef box_fwd_piter_<P> current__;
    typedef Iterator_on_Points<current__> ret;
  };

  template <typename P>
  struct super_trait_< box_bkd_piter_<P> >
  {
    typedef box_bkd_piter_<P> current__;
    typedef Iterator_on_Points<current__> ret;
  };


  /// Virtual types.

  template <typename P>
  struct vtypes< box_fwd_piter_<P> >
  {
    typedef P point;
  };

  template <typename P>
  struct vtypes< box_bkd_piter_<P> >
  {
    typedef P point;
  };


  /// Class box_fwd_piter_<P>.

  template <typename P>
  class box_fwd_piter_ : public Iterator_on_Points< box_fwd_piter_<P> >,
			 private mlc::assert_< mlc_is_a(P, Point) >
  {
  public:
    box_fwd_piter_();
    box_fwd_piter_(const Point_Set< box_<P> >& b);
    void set_box(const box_<P>& b);

    void impl_start();
    void impl_next();
    void impl_invalidate();
    bool impl_is_valid() const;
    P impl_to_point() const;
    const P* impl_point_adr() const;

  private:
    box_<P> b_;
    P p_, nop_;
  };


  /// Class box_bkd_piter_<P>.

  template <typename P>
  class box_bkd_piter_ : public Iterator_on_Points< box_bkd_piter_<P> >,
			 private mlc::assert_< mlc_is_a(P, Point) >
  {
  public:
    box_bkd_piter_();
    box_bkd_piter_(const Point_Set< box_<P> >& b);
    void set_box(const box_<P>& b);

    void impl_start();
    void impl_next();
    void impl_invalidate();
    bool impl_is_valid() const;
    P impl_to_point() const;
    const P* impl_point_adr() const;

  private:
    box_<P> b_;
    P p_, nop_;
  };




# ifndef OLN_INCLUDE_ONLY


  // --------------------   box_<P>


  template <typename P>
  box_<P>::box_()
  {
  }

  template <typename P>
  box_<P>::box_(const P& pmin, const P& pmax)
  {
    for (unsigned i = 0; i < n; ++i)
      precondition(pmax[i] >= pmin[i]);
    this->pmin_ = pmin;
    this->pmax_ = pmax;
  }

  template <typename P>
  template <typename D>
  box_<P>::box_(const internal::initializer_<D>& data)
  {
    internal::init__(*this, data);
  }
      
  template <typename P>
  unsigned
  box_<P>::impl_npoints() const
  {
    unsigned count = 1;
    for (unsigned i = 0; i < n; ++i)
      count *= (this->pmax_[i] - this->pmin_[i] + 1);
    return count;
  }

  template <typename P>
  bool
  box_<P>::impl_has(const P& p) const
  {
    for (unsigned i = 0; i < n; ++i)
      if (p[i] < this->pmin_[i] or p[i] > this->pmax_[i])
	return false;
    return true;
  }

  template <typename P>
  const box_<P>&
  box_<P>::impl_bbox() const
  {
    return *this;
  }

  template <typename P>
  const P&
  box_<P>::pmin() const
  {
    for (unsigned i = 0; i < n; ++i)
      invariant(this->pmin_[i] <= this->pmax_[i]);
    return this->pmin_;
  }

  template <typename P>
  const P&
  box_<P>::pmax() const
  {
    for (unsigned i = 0; i < n; ++i)
      invariant(this->pmax_[i] >= this->pmin_[i]);
    return this->pmax_;
  }

  template <typename P>
  P&
  box_<P>::pmin()
  {
    return this->pmin_;
  }

  template <typename P>
  P&
  box_<P>::pmax()
  {
    return this->pmax_;
  }


  // --------------------   init__


  namespace internal
  {

    template <typename P>
    void init__(box_<P>& b,
		const initializer_< pair< from_t<P>, to_t<P> > >& data)
    {
      b.pmin() = data->value1.value;
      b.pmax() = data->value2.value;
    }

  }


  // --------------------   box_fwd_piter_<P>


  template <typename P>
  box_fwd_piter_<P>::box_fwd_piter_()
  {
  }

  template <typename P>
  box_fwd_piter_<P>::box_fwd_piter_(const Point_Set< box_<P> >& b)
  {
    this->set_box(exact(b));
  }

  template <typename P>
  void box_fwd_piter_<P>::set_box(const box_<P>& b)
  {
    b_ = b;
    nop_ = b_.pmax();
    ++nop_[0];
    p_ = nop_;
  }

  template <typename P>
  void box_fwd_piter_<P>::impl_start()
  {
    p_ = b_.pmin();
  }

  template <typename P>
  void box_fwd_piter_<P>::impl_next()
  {
    for (int i = P::n - 1; i >= 0; --i)
      if (p_[i] == b_.pmax()[i])
	p_[i] = b_.pmin()[i];
      else
	{
	  ++p_[i];
	  break;
	}
    if (p_ == b_.pmin())
      p_ = nop_;
  }

  template <typename P>
  void box_fwd_piter_<P>::impl_invalidate()
  {
    p_ = nop_;
  }

  template <typename P>
  bool box_fwd_piter_<P>::impl_is_valid() const
  {
    return p_ != nop_;
  }

  template <typename P>
  P box_fwd_piter_<P>::impl_to_point() const
  {
    return p_;
  }

  template <typename P>
  const P* box_fwd_piter_<P>::impl_point_adr() const
  {
    return &p_;
  }



  // --------------------   box_bkd_piter_<P>


  template <typename P>
  box_bkd_piter_<P>::box_bkd_piter_()
  {
  }

  template <typename P>
  box_bkd_piter_<P>::box_bkd_piter_(const Point_Set< box_<P> >& b)
  {
    this->set_box(exact(b));
  }

  template <typename P>
  void box_bkd_piter_<P>::set_box(const box_<P>& b)
  {
    b_ = b;
    nop_ = b_.pmin();
    --nop_[0];
    p_ = nop_;
  }

  template <typename P>
  void box_bkd_piter_<P>::impl_start()
  {
    p_ = b_.pmax();
  }

  template <typename P>
  void box_bkd_piter_<P>::impl_next()
  {
    for (int i = P::n - 1; i >= 0; --i)
      if (p_[i] == b_.pmin()[i])
	p_[i] = b_.pmax()[i];
      else
	{
	  --p_[i];
	  break;
	}
    if (p_ == b_.pmax())
      p_ = nop_;
  }

  template <typename P>
  void box_bkd_piter_<P>::impl_invalidate()
  {
    p_ = nop_;
  }

  template <typename P>
  bool box_bkd_piter_<P>::impl_is_valid() const
  {
    return p_ != nop_;
  }

  template <typename P>
  P box_bkd_piter_<P>::impl_to_point() const
  {
    return p_;
  }

  template <typename P>
  const P* box_bkd_piter_<P>::impl_point_adr() const
  {
    return &p_;
  }

# endif // OLN_INCLUDE_ONLY


} // end of namespace oln


#endif // ! OLN_CORE_GEN_BOX_HH
