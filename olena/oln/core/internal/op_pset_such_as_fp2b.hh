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

#ifndef OLN_CORE_INTERNAL_OP_PSET_SUCH_AS_FP2B_HH
# define OLN_CORE_INTERNAL_OP_PSET_SUCH_AS_FP2B_HH

# include <oln/core/concept/function.hh>
# include <oln/core/gen/op.hh>
# include <oln/core/internal/piter_adaptor.hh>
# include <oln/core/internal/point_set_base.hh>


namespace oln
{


# define current \
     special_op_< stc::is<Point_Set>, S, such_as, stc::is<Function_p2b>, F >


  /// Fwd decls.
  namespace internal
  {
    template <typename S, typename F> class current;
  }
  template <typename S, typename F> class pset_such_as_fp2b_fwd_piter_;
  template <typename S, typename F> class pset_such_as_fp2b_bkd_piter_;



  /// Super type.
  template <typename S, typename F>
  struct super_trait_< internal::current >
  {
    typedef internal::point_set_base_< op_<S, such_as, F> > ret;
  };


  /// Virtual types.
  template <typename S, typename F>
  struct vtypes< internal::current >
  {
    typedef typename S::point point;
    typedef pset_such_as_fp2b_fwd_piter_<S, F> fwd_piter;
    typedef pset_such_as_fp2b_bkd_piter_<S, F> bkd_piter;
  };


  namespace internal
  {

    /// Implementation class the result of "Point_Set S | Function_p2b F".

    template <typename S, typename F>
    class current
      :
      public internal::point_set_base_< op_<S, such_as, F> >
    {
      typedef internal::point_set_base_< op_<S, such_as, F> > super;
    public:

      stc_using(point);
      stc_using(box);

      unsigned   impl_npoints() const;
      bool       impl_has(const point& p) const;
      const box& impl_bbox() const;

      const S& adapted_() const;
      F fun_() const;

    protected:
      special_op_();
      special_op_(S& pset, F& f);

      S pset_;
      F f_;

      // template <typename D>
      // friend
      // bool init_(internal::current* this_, const D& dat);
    };


# ifndef OLN_INCLUDE_ONLY

    template <typename S, typename F>
    current::special_op_()
    {
    }

    template <typename S, typename F>
    current::special_op_(S& pset, F& f)
      : pset_(pset),
	f_(f)
    {
    }

    template <typename S, typename F>
    unsigned
    current::impl_npoints() const
    {
      return 0; // FIXME: Wrong!!!
    }

    template <typename S, typename F>
    bool
    current::impl_has(const typename current::point& p) const
    {
      return this->pset_.has(p) and this->f_(p);
    }

    template <typename S, typename F>
    const typename current::box&
    current::impl_bbox() const
    {
      return this->pset_.bbox();
    }

    template <typename S, typename F>
    const S&
    current::adapted_() const
    {
      return this->pset_;
    }

    template <typename S, typename F>
    F current::fun_() const
    {
      return this->f_;
    }

# endif

  } // end of namespace oln::internal


//   // Fixme: Activate init

//   template <typename S, typename F, typename D>
//   bool init_(internal::current* this_, const D& dat)
//   {
//     bool pset_ok = init(this_->pset_,  with, dat);
//     bool fun_ok = init(this_->f_, with, dat);
//     postcondition(pset_ok);
//     postcondition(fun_ok);
//     return pset_ok and fun_ok;
//   }


# undef current







# define current pset_such_as_fp2b_fwd_piter_<S, F>
  //             ----------------------------------

# define super   internal::piter_adaptor_< current >


  // Super type.
  template <typename S, typename F>
  struct super_trait_< current >
  {
    typedef super ret;
  };


  // Virtual types.
  template <typename S, typename F>
  struct vtypes< current >
  {
    typedef typename S::point     point;
    typedef typename S::fwd_piter adapted;
  };


  // Class.
  template <typename S, typename F>
  class pset_such_as_fp2b_fwd_piter_ : public super
  {
  public:

    pset_such_as_fp2b_fwd_piter_();
    pset_such_as_fp2b_fwd_piter_(const Point_Set< op_<S, such_as, F> >& pset);

    void impl_start();
    void impl_next();

  private:
    F f_;
    
  };


#  ifndef OLN_INCLUDE_ONLY

  template <typename S, typename F>
  current::pset_such_as_fp2b_fwd_piter_(const Point_Set< op_<S, such_as, F> >& pset)
    : super(exact(pset).adapted_()),
      f_(exact(pset).fun_())
  {
  }

  template <typename S, typename F>
  void
  current::impl_start()
  {
    this->p_.start();
    while (this->p_.is_valid() and not f_(this->p_))
      this->p_.next();
  }

  template <typename S, typename F>
  void
  current::impl_next()
  {
    do
      this->p_.next();
    while (this->p_.is_valid() and not f_(this->p_));
  }

#  endif // OLN_INCLUDE_ONLY

# undef super
# undef current





# define current pset_such_as_fp2b_bkd_piter_<S, F>
  //             ----------------------------------

# define super   internal::piter_adaptor_< current >


  // Super type.
  template <typename S, typename F>
  struct super_trait_< current >
  {
    typedef super ret;
  };


  // Virtual types.
  template <typename S, typename F>
  struct vtypes< current >
  {
    typedef typename S::point     point;
    typedef typename S::fwd_piter adapted;
  };


  // Class.
  template <typename S, typename F>
  class pset_such_as_fp2b_bkd_piter_ : public super
  {
  public:

    pset_such_as_fp2b_bkd_piter_(const Point_Set< op_<S, such_as, F> >& pset);

    void impl_start();
    void impl_next();

  private:
    F f_;
    
  };


# ifndef OLN_INCLUDE_ONLY

  template <typename S, typename F>
  current::pset_such_as_fp2b_bkd_piter_(const Point_Set< op_<S, such_as, F> >& pset)
    : super(exact(pset).adapted_()),
      f_(exact(pset).fun_())
  {
  }

  template <typename S, typename F>
  void
  current::impl_start()
  {
    this->p_.start();
    while (this->p_.is_valid() and not f_(this->p_))
      this->p_.next();
  }

  template <typename S, typename F>
  void
  current::impl_next()
  {
    do
      this->p_.next();
    while (this->p_.is_valid() and not f_(this->p_));
  }

# endif // OLN_INCLUDE_ONLY

# undef super
# undef current


} // end of namespace oln


#endif // ! OLN_CORE_INTERNAL_OP_PSET_SUCH_AS_FP2B_HH
