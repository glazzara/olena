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

#ifndef OLN_CORE_INTERNAL_BOX_HH
# define OLN_CORE_INTERNAL_BOX_HH

//# include <oln/core/concept/point.hh>
# include <oln/core/concept/point_set.hh>
# include <oln/core/concept/iterator_on_points.hh>
# include <oln/core/internal/point_set_base.hh>


namespace oln
{


  // Forward declarations.
  namespace internal { template <typename Exact> class box_; }
  template <typename Exact> class box_fwd_piter_;
  template <typename Exact> class box_bkd_piter_;

  // Super type declaration.
  template <typename Exact>
  struct super_trait_< internal::box_<Exact> >
  {
    typedef internal::point_set_base_<Exact> ret;
  };


  /// Virtual types associated to oln::box_<P>.
  template <typename Exact>
  struct vtypes< internal::box_<Exact> >
  {
    typedef stc::final< box_fwd_piter_<Exact> > fwd_piter;
    typedef stc::final< box_bkd_piter_<Exact> > bkd_piter;
  };



  namespace internal
  {
    /// Generic box class based on a point class.
    template <typename Exact>
    class box_ : public point_set_base_< Exact >
    {
      typedef box_<Exact> current;
      typedef point_set_base_<Exact> super;
    public:
      stc_using(point);
      stc_using(box);


      typedef internal::initializer_<
	internal::pair< internal::from_t<point>, internal::to_t<point> >
      > from_to_t;

    private:
      typedef stc_type(point, dim) dim__;

    public:
      enum { n = mlc_value(dim__) };


      unsigned       impl_npoints() const;
      bool           impl_has(const point& p) const;
      const Exact& impl_bbox() const;

      const point& pmin() const;
      point& pmin();
      const point& pmax() const;
      point& pmax();

    protected:
      box_();
      box_(const point& pmin, const point& pmax);
      box_(const from_to_t& data);
      template <typename D>
      box_(const internal::initializer_<D>& data);

      point pmin_, pmax_;

    }; // end of class oln::box_<P>

    template <typename Exact>
    std::ostream& operator<<(std::ostream& ostr, const box_<Exact>& b)
    {
      return ostr << "{ " << b.pmin() << " .. " << b.pmax() << " }";
    }

  } // end of namespace internal


  // --------------------   iterators  on  box_<Exact>



  /// Super types.

  template <typename B>
  struct super_trait_< box_fwd_piter_<B> >
  {
    typedef box_fwd_piter_<B> current__;
    typedef Iterator_on_Points<current__> ret;
  };

  template <typename B>
  struct super_trait_<box_bkd_piter_<B> >
  {
    typedef box_bkd_piter_<B> current__;
    typedef Iterator_on_Points<current__> ret;
  };


  /// Virtual types.

  template <typename B>
  struct vtypes< box_fwd_piter_<B> >
  {
    typedef typename B::point point;
  };

  template <typename B>
  struct vtypes< box_bkd_piter_<B> >
  {
    typedef typename B::point point;
  };


  /// Class box_fwd_piter_<P>.

  template <typename B>
  class box_fwd_piter_ : public Iterator_on_Points< box_fwd_piter_<B> >,
			 private mlc::assert_< mlc_is_a(B, Point_Set) >
  {
    typedef box_fwd_piter_<B> current;
    typedef Iterator_on_Points<current> super;
  public:
    stc_using(point);

    box_fwd_piter_();
    box_fwd_piter_(const B& b);
    void set_box(const B& b);

    void impl_start();
    void impl_next();
    void impl_invalidate();
    bool impl_is_valid() const;
    point impl_to_point() const;
    const point* impl_point_adr() const;

  private:
    B b_;
    point p_, nop_;
  };


  /// Class box_bkd_piter_<B>.

  template <typename B>
  class box_bkd_piter_ : public Iterator_on_Points< box_bkd_piter_<B> >,
			 private mlc::assert_< mlc_is_a(B, Point_Set) >
  {
    typedef box_bkd_piter_<B> current;
    typedef Iterator_on_Points<current> super;
  public:
    stc_using(point);

    box_bkd_piter_();
    box_bkd_piter_(const B& b);
    void set_box(const B& b);

    void impl_start();
    void impl_next();
    void impl_invalidate();
    bool impl_is_valid() const;
    point impl_to_point() const;
    const point* impl_point_adr() const;

  private:
    B b_;
    point p_, nop_;
  };




# ifndef OLN_INCLUDE_ONLY

  namespace internal
  {

    // --------------------   box_<Exact>


    template <typename Exact>
    box_<Exact>::box_()
    {
    }

    template <typename Exact>
    box_<Exact>::box_(const box_<Exact>::point& pmin,
		      const box_<Exact>::point& pmax)
    {
      for (unsigned i = 0; i < n; ++i)
	precondition(pmax[i] >= pmin[i]);
      this->pmin_ = pmin;
      this->pmax_ = pmax;
    }

    template <typename P>
    box_<P>::box_(const typename box_<P>::from_to_t& dat)
    {
      this->pmin_ = dat->first.value;
      this->pmax_ = dat->second.value;
    }

    template <typename Exact>
    template <typename D>
    box_<Exact>::box_(const internal::initializer_<D>& data)
    {
      bool box_ok = internal::init__(internal::tag::box_t(), *this, data.value());
      postcondition(box_ok);
    }

    template <typename Exact>
    unsigned
    box_<Exact>::impl_npoints() const
    {
      unsigned count = 1;
      for (unsigned i = 0; i < n; ++i)
	count *= (this->pmax_[i] - this->pmin_[i] + 1);
      return count;
    }

    template <typename Exact>
    bool
    box_<Exact>::impl_has(const box_<Exact>::point& p) const
    {
      for (unsigned i = 0; i < n; ++i)
	if (p[i] < this->pmin_[i] or p[i] > this->pmax_[i])
	  return false;
      return true;
    }

    template <typename Exact>
    const Exact&
    box_<Exact>::impl_bbox() const
    {
      return *exact(this);
    }

    template <typename Exact>
    const typename box_<Exact>::point&
    box_<Exact>::pmin() const
    {
      for (unsigned i = 0; i < n; ++i)
	invariant(this->pmin_[i] <= this->pmax_[i]);
      return this->pmin_;
    }

    template <typename Exact>
    const typename box_<Exact>::point&
    box_<Exact>::pmax() const
    {
      for (unsigned i = 0; i < n; ++i)
	invariant(this->pmax_[i] >= this->pmin_[i]);
      return this->pmax_;
    }

    template <typename Exact>
    typename box_<Exact>::point&
    box_<Exact>::pmin()
    {
      return this->pmin_;
    }

    template <typename Exact>
    typename box_<Exact>::point&
    box_<Exact>::pmax()
    {
      return this->pmax_;
    }
  }

    // --------------------   box_fwd_piter_<B>


    template <typename B>
    box_fwd_piter_<B>::box_fwd_piter_()
    {
    }

    template <typename B>
    box_fwd_piter_<B>::box_fwd_piter_(const B& b)
    {
      this->set_box(b);
    }

    template <typename B>
    void
    box_fwd_piter_<B>::set_box(const B& b)
    {
      b_ = b;
      nop_ = b_.pmax();
      ++nop_[0];
      p_ = nop_;
    }

    template <typename B>
    void
    box_fwd_piter_<B>::impl_start()
    {
      p_ = b_.pmin();
    }

    template <typename B>
    void
    box_fwd_piter_<B>::impl_next()
    {
      for (int i = B::n - 1; i >= 0; --i)
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

    template <typename B>
    void
    box_fwd_piter_<B>::impl_invalidate()
    {
      p_ = nop_;
    }

    template <typename B>
    bool
    box_fwd_piter_<B>::impl_is_valid() const
    {
      return p_ != nop_;
    }

    template <typename B>
    typename box_fwd_piter_<B>::point
    box_fwd_piter_<B>::impl_to_point() const
    {
      return p_;
    }

    template <typename B>
    const typename box_fwd_piter_<B>::point*
    box_fwd_piter_<B>::impl_point_adr() const
    {
      return &p_;
    }



    // --------------------   box_bkd_piter_<P>


    template <typename B>
    box_bkd_piter_<B>::box_bkd_piter_()
    {
    }

    template <typename B>
    box_bkd_piter_<B>::box_bkd_piter_(const B& b)
    {
      this->set_box(b);
    }

    template <typename B>
    void
    box_bkd_piter_<B>::set_box(const B& b)
    {
      b_ = b;
      nop_ = b_.pmin();
      --nop_[0];
      p_ = nop_;
    }

    template <typename B>
    void
    box_bkd_piter_<B>::impl_start()
    {
      p_ = b_.pmax();
    }

    template <typename B>
    void
    box_bkd_piter_<B>::impl_next()
    {
      for (int i = B::n - 1; i >= 0; --i)
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

    template <typename B>
    void
    box_bkd_piter_<B>::impl_invalidate()
    {
      p_ = nop_;
    }

    template <typename B>
    bool
    box_bkd_piter_<B>::impl_is_valid() const
    {
      return p_ != nop_;
    }

    template <typename B>
    typename box_bkd_piter_<B>::point
    box_bkd_piter_<B>::impl_to_point() const
    {
      return p_;
    }

    template <typename B>
    const typename box_bkd_piter_<B>::point*
    box_bkd_piter_<B>::impl_point_adr() const
    {
      return &p_;
    }

# endif // OLN_INCLUDE_ONLY
} // end of namespace oln


#endif // ! OLN_CORE_INTERNAL_BOX_HH
