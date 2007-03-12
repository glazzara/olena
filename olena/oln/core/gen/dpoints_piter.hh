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

#ifndef OLN_CORE_GEN_DPOINTS_PITER_HH
# define OLN_CORE_GEN_DPOINTS_PITER_HH

# include <oln/core/concept/iterator_on_points.hh>
# include <oln/core/internal/dpoints_impl.hh>


namespace oln
{


  /// Fwd decls.
  template <typename P> class dpoints_fwd_piter_;
  template <typename P> class dpoints_bkd_piter_;


  // Super types.
  template <typename P>
  struct super_trait_< dpoints_fwd_piter_<P> >
  {
    typedef dpoints_fwd_piter_<P> current__;
    typedef Iterator_on_Points<current__> ret;
  };
  template <typename P>
  struct super_trait_< dpoints_bkd_piter_<P> >
  {
    typedef dpoints_bkd_piter_<P> current__;
    typedef Iterator_on_Points<current__> ret;
  };


  /// Virtual types.
  template <typename P>
  struct vtypes< dpoints_fwd_piter_<P> >
  {
    typedef P point;
  };
  template <typename P>
  struct vtypes< dpoints_bkd_piter_<P> >
  {
    typedef P point;
  };



  namespace internal
  {

    /// Class to factor some code.

    template <typename P>
    class dpoints_piter_impl_ : private mlc::assert_< mlc_is_a(P, Point) > 
    {
    public:

      dpoints_piter_impl_(const dpoints_piter_impl_&);
      void operator=(const dpoints_piter_impl_&);

      void impl_invalidate();

      bool impl_is_valid() const;
      
      P impl_to_point() const;
      
      const P* impl_point_adr() const;

    protected:

      const P* p_ref_;
      std::vector<typename P::dpoint> dps_;
      unsigned n_, i_;
      P p_;
      
      // Ctor.
      template <typename Pl>
	dpoints_piter_impl_(const Pl& ref,
			    const internal::dpoints_impl_<typename P::dpoint>& data);

      void update_p_();
    };

  } // end of namespace oln::internal



  /// Forward point iterator class on a set of dpoints.

  template <typename P>
  class dpoints_fwd_piter_ : public Iterator_on_Points< dpoints_fwd_piter_<P> >,
			     public internal::dpoints_piter_impl_<P>
  {
  public:

    template <typename Pl, typename X>
    dpoints_fwd_piter_(const Pl& p, const X& win_or_nbh);

    void impl_start();

    void impl_next();

  }; // end of class oln::dpoints_fwd_piter_<P>



  /// Backward point iterator class on a set of dpoints.

  template <typename P>
  class dpoints_bkd_piter_ : public Iterator_on_Points< dpoints_bkd_piter_<P> >,
			     public internal::dpoints_piter_impl_<P>
  {
  public:

    template <typename Pl, typename X>
    dpoints_bkd_piter_(const Pl& p, const X& win_or_nbh);

    void impl_start();

    void impl_next();

  }; // end of class oln::dpoints_bkd_piter_<P>

  

# ifndef OLN_INCLUDE_ONLY

  namespace internal
  {

    template <typename Pl>
    const Pl* point_adr_(const Point<Pl>& p)
    {
      return exact(&p);
    }

    template <typename Pl>
    const typename Pl::point* point_adr_(const Iterator_on_Points<Pl>& p)
    {
      return p.point_adr();
    }

    template <typename P>
    template <typename Pl>
    dpoints_piter_impl_<P>::dpoints_piter_impl_(const Pl& ref,
						const internal::dpoints_impl_<typename P::dpoint>& data)
    {
      p_ref_ = point_adr_(ref);
      dps_ = data.dpoints();
      n_ = data.size();
      i_ = n_;
      postcondition(n_ != 0);
    }

    template <typename P>
    void
    dpoints_piter_impl_<P>::impl_invalidate()
    {
      i_ = n_;
    }

    template <typename P>
    bool
    dpoints_piter_impl_<P>::impl_is_valid() const
    {
      return i_ != n_;
    }

    template <typename P>
    P
    dpoints_piter_impl_<P>::impl_to_point() const
    {
      return p_;
    }

    template <typename P>
    const P*
    dpoints_piter_impl_<P>::impl_point_adr() const
    {
      return &p_;
    }

    template <typename P>
    void
    dpoints_piter_impl_<P>::update_p_()
    {
      p_ = *p_ref_+ dps_[i_];
    }

  } // end of namespace oln::internal


  // fwd

  template <typename P>
  template <typename Pl, typename X>
  dpoints_fwd_piter_<P>::dpoints_fwd_piter_(const Pl& p, const X& win_or_nbh)
    :
    internal::dpoints_piter_impl_<P>(p, exact(win_or_nbh))
  {
  }

  template <typename P>
  void
  dpoints_fwd_piter_<P>::impl_start()
  {
    this->i_ = 0;
    this->update_p_();
  }

  template <typename P>
  void
  dpoints_fwd_piter_<P>::impl_next()
  {
    if (++this->i_ == this->n_)
      return;
    this->update_p_();
  }


  // bkd

  template <typename P>
  template <typename Pl, typename X>
  dpoints_bkd_piter_<P>::dpoints_bkd_piter_(const Pl& p, const X& win_or_nbh)
    :
    internal::dpoints_piter_impl_<P>(p, exact(win_or_nbh))
  {
  }

  template <typename P>
  void
  dpoints_bkd_piter_<P>::impl_start()
  {
    this->i_ = this->n_ - 1;
    this->update_p_();
  }

  template <typename P>
  void
  dpoints_bkd_piter_<P>::impl_next()
  {
    if (this->i_ == 0)
      {
	this->i_ = this->n_;
	return;
      }
    --this->i_;
    this->update_p_();
  }

# endif

} // end of namespace oln


#endif // ! OLN_CORE_GEN_DPOINTS_PITER_HH
