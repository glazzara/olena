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

#ifndef MLN_CORE_CONCEPT_PROXY_HXX
# define MLN_CORE_CONCEPT_PROXY_HXX

# include <mln/core/internal/force_exact.hh>
# include <mln/metal/unqualif.hh>
# include <mln/metal/is_a.hh>
# include <mln/metal/if.hh>
# include <mln/metal/is_const.hh>
# include <mln/metal/const.hh>
# include <mln/metal/unconst.hh>
# include <mln/metal/is_not_ref.hh>
# include <mln/metal/ref.hh>
# include <mln/metal/fix_return.hh>


namespace mln
{

  // Fwd decls.
  template <typename E> struct Proxy;


  namespace internal
  {


    // helper_unprox_binop

    template <bool b, typename P> struct helper_unprox_if;

    template <typename P>
    struct helper_unprox_if< true, P >
    {
      typedef mlc_unqualif(mln_q_subject(P)) ret;
      static mlc_const_return(mln_q_subject(P)) on(const Proxy<P>& p);
    };

    template <typename P>
    struct helper_unprox_if< false, P >
    {
      typedef P ret;
      static const P& on(const Proxy<P>& p);
    };

    template <typename L, typename R>
    struct helper_unprox_binop
    {
      enum { L_level = L::proxy_level,
	     R_level = R::proxy_level }; // To prevent a weird g++ warning.
      typedef internal::helper_unprox_if<(L_level >= R_level), L> L_helper;
      typedef internal::helper_unprox_if<(R_level >= L_level), R> R_helper;
      typedef typename L_helper::ret L_ret;
      typedef typename R_helper::ret R_ret;
    };



    template <typename T>
    struct unproxy_rec_;

    template <typename O, bool is_proxy>
    struct helper_unproxy_rec;


    // P is a proxy type.

    template <typename P>
    struct helper_unproxy_rec< P, true >
    {
      typedef mln_exact(P)::HOT_actual_subject ret;
      static ret on(P& p);
    };

    template <typename P>
    struct helper_unproxy_rec< const P, true >
    {
      typedef mlc_const(mln_exact(P)::HOT_actual_subject) ret;
      static ret on(const P& p);
    };

    // O is *not* a proxy type.

    template <typename O>
    struct helper_unproxy_rec< const O, false >
    {
      typedef const mln_exact(mlc_unqualif(O))& ret;
      static ret on(const O& obj);
    };

    template <typename O>
    struct helper_unproxy_rec< O, false >
    {
      typedef mln_exact(mlc_unqualif(O))& ret;
      static ret on(O& obj);
    };


    template <typename T>
    struct unproxy_rec_
    {
      enum { is_proxy = mlc_is_a(T, Proxy)::value };
      typedef helper_unproxy_rec<T, is_proxy> helper;
      typedef typename helper::ret ret;
      static ret on(T& t);
    };



    
    // External way of getting an address of an object from/through a
    // proxy.  This is a recursive implementation since we can have a
    // proxy of proxy, etc.

    // Case 1: Not found so unproxy_ (once).

    template <typename T, typename P>
    void get_adr(const T *& ptr, const Proxy<P>& p);

    template <typename T, typename P>
    void get_adr(      T *& ptr,       Proxy<P>& p);

    // Case 2: Found.  (Note that T can also be a Proxy.)

    template <typename T>
    void get_adr(const T *& ptr, const Object<T>& obj);

    template <typename T>
    void get_adr(      T *& ptr,       Object<T>& obj);

    template <typename T>
    void get_adr(      T *& ptr,       T& t);

    // Case 3: Fail to found!

    template <typename T, typename O>
    void get_adr(const T *& ptr, const Object<O>&);

    template <typename T, typename O>
    void get_adr(      T *& ptr,       Object<O>&);



    // Every proxied class should derive from subject_impl.  The class
    // below is provided to be specialized so that an effective
    // implementation (with the interface of the proxied class) can
    // equip proxy classes.

    template <typename Subject, typename E>
    struct subject_impl
    {
    };

    template <typename S, typename E>
    struct subject_impl< S&, E >; // Safety.




    template <typename prox>
    struct helper_subject_of
    {
      typedef mlc_unqualif(prox)::q_subject subj;
      // We have the proxy type (prox) and its subject type (subj).
      // Both can be qualified with 'const' and/or '&'.  We have to
      // propagate when possible the proper qualifications to the
      // resulting subject type.

      // The rules are:
      // (1) the subject is constant if either prox or subj is const
      //     ---so we have to propagate the possible constness of
      //        prox;
      typedef mlc_if(mlc_is_const(prox), mlc_const(subj), subj) temp;

      // (2) the subject is a reference if both prox and subj are
      //     references---so we have to remove the possible & if prox
      //     is not a reference.
      typedef mlc_if(mlc_is_not_ref(prox), mlc_unref(temp), temp) ret;

      // +-------------+---------+-------------+-------------+----------------+
      // | prox / subj | const S |      S      |  const S&   |       S&       |
      // +-------------+---------+-------------+-------------+----------------+
      // | const P     | const S | const S (1) | const S (2) | const S  (1,2) |
      // | P           | const S | S           | const S (2) | S          (2) |
      // | const P&    | const S | const S (1) | const S&    | const S&   (1) |
      // | P&          | const S | S           | const S&    | S&         (*) |
      // +-------------+---------+-------------+-------------+----------------+
    };


    template <typename S> struct helper_subject_impl           { typedef const S ret; };
    template <typename S> struct helper_subject_impl<S&>       { typedef       S ret; }; // (*)
    template <typename S> struct helper_subject_impl<const S&> { typedef const S ret; };
    template <typename S> struct helper_subject_impl<const S>  { typedef const S ret; };

    // FIXME: I don't know yet how to deal with these cases:
    template <typename S> struct helper_subject_impl<S*>;
    template <typename S> struct helper_subject_impl<S*const>;


    // A proxy should convert towards its subject.  And, if we have a
    // proxy of proxy, it should also convert towards its subject of
    // subject, and so on.  It leads to a recursive implementation
    // where conversions are automatically obtained through
    // inheritance.
    //
    // E is a Proxy type; Subject is its subject type.

    template <typename Subject, typename E> struct get_proxy_impl;

    template <typename Subject, typename E, bool rec = true>
    struct helper_get_proxy_impl : get_proxy_impl< typename helper_subject_of<Subject>::ret,
						   E > // Rec.
    {
    private:
      typedef mlc_unqualif(Subject) unq_subject_;
    public:
      enum { proxy_level = unq_subject_::proxy_level + 1};
    };

    template <typename Subject, typename E>
    struct helper_get_proxy_impl< Subject, E, false > // Stop rec.
      :
      subject_impl< typename helper_subject_impl<Subject>::ret,
		    E >
    {
      enum { proxy_level = 1 };

      // FIXME: HOT: test code below

      typedef Subject HOT_actual_subject;

      HOT_actual_subject            get_subject();
      mlc_const_return(HOT_actual_subject) get_subject() const;
    };

    template <typename Subject, typename E>
    struct get_proxy_impl : helper_get_proxy_impl< Subject, E,
						   mlc_is_a(Subject, Proxy)::value >
    {
    public:
      operator mlc_fix_return(mlc_const_return(Subject)) () const;
      operator mlc_fix_return(Subject) ();
    };

    template <typename Subject, typename E>
    struct proxy_impl : get_proxy_impl< Subject, E>
    {
      typedef Subject q_subject;

      // HERE: NEW
      q_subject unproxy_()
      {
	return mln::internal::force_exact<E>(*this).subj_();
      }
      mlc_const_return(q_subject) unproxy_() const
      {
	E& self_ = mln::internal::force_exact<E>(*this);
	return self_.subj_();
      }
    };




# ifndef MLN_INCLUDE_ONLY


    // helper_unprox_if

    template <typename P>
    inline
    mlc_const_return(mln_q_subject(P))
    helper_unprox_if< true, P >::on(const Proxy<P>& p)
    {
      return exact(p).unproxy_();
    }

    template <typename P>
    inline
    const P&
    helper_unprox_if< false, P >::on(const Proxy<P>& p)
    {
      return exact(p);
    }

    template <typename P>
    inline
    mln_exact(P)::HOT_actual_subject
    helper_unproxy_rec< P, true >::on(P& p)
    {
      return unproxy_rec(exact(p).unproxy_());
    }

    // helper_unprox_rec

    template <typename P>
    inline
    mlc_const(mln_exact(P)::HOT_actual_subject)
    helper_unproxy_rec< const P, true >::on(const P& p)
    {
      return unproxy_rec(exact(p).unproxy_());
    }

    template <typename O>
    inline
    mln_exact(mlc_unqualif(O))&
    helper_unproxy_rec< O, false >::on(O& obj)
    {
      return exact(obj);
    }

    template <typename O>
    inline
    const mln_exact(mlc_unqualif(O))&
    helper_unproxy_rec< const O, false >::on(const O& obj)
    {
      return exact(obj);
    }

    // unproxy_rec_

    template <typename T>
    inline
    typename unproxy_rec_<T>::ret
    unproxy_rec_<T>::on(T& t)
    {
      typedef typename unproxy_rec_<T>::helper helper;
      return helper::on(t);
    }


    // helper_get_proxy_impl<..>::get_subject()

    template <typename Subject, typename E>
    inline
    typename helper_get_proxy_impl< Subject, E, false >::HOT_actual_subject
    helper_get_proxy_impl< Subject, E, false >::get_subject()
    {
      return unproxy_rec(mln::internal::force_exact<E>(*this));
    }

    template <typename Subject, typename E>
    inline
    typename mln::metal::const_return_< typename helper_get_proxy_impl< Subject, E, false >::HOT_actual_subject >::ret
    helper_get_proxy_impl< Subject, E, false >::get_subject() const
    {
      return unproxy_rec(mln::internal::force_exact<const E>(*this));
    }


    // get_proxy_impl<..>::operator Subject()

    template <typename Subject, typename E>
    inline
    get_proxy_impl<Subject, E>::operator mlc_fix_return(mlc_const_return(Subject)) () const
    {
      return mln::internal::force_exact<const E>(*this).unproxy_();
      
      // Technical note:
      //
      // The code above seems more effective than the one below:
      // 	const Subject* adr;
      // 	get_adr(adr, mln::internal::force_exact<const E>(*this));
      // 	mln_postcondition(adr != 0);
      // 	return *adr;
    }

    template <typename Subject, typename E>
    inline
    get_proxy_impl<Subject, E>::operator mlc_fix_return(Subject) ()
    {
      return mln::internal::force_exact<E>(*this).unproxy_();
    }


    // get_adr

    template <typename T, typename P>
    inline
    void get_adr(const T *& ptr, const Proxy<P>& p)
    {
      get_adr(ptr, exact(p).unproxy_());
    }

    template <typename T, typename P>
    inline
    void get_adr(      T *& ptr,       Proxy<P>& p)
    {
      get_adr(ptr, exact(p).unproxy_());
    }

    template <typename T>
    inline
    void get_adr(const T *& ptr, const Object<T>& obj)
    {
      ptr = & exact(obj);
    }

    template <typename T>
    inline
    void get_adr(      T *& ptr,       Object<T>& obj)
    {
      ptr = & exact(obj);
    }

    template <typename T>
    inline
    void get_adr(      T *& ptr,       T& t)
    {
      ptr = & t;
    }

    template <typename T, typename O>
    inline
    void get_adr(const T *& ptr, const Object<O>&)
    {
      ptr = 0;
    }

    template <typename T, typename O>
    inline
    void get_adr(      T *& ptr,       Object<O>&)
    {
      ptr = 0;
    }

# endif // ! MLN_INCLUDE_ONLY

  } // end of namespace mln::internal

} // end of namespace mln


#endif // ! MLN_CORE_CONCEPT_PROXY_HXX
