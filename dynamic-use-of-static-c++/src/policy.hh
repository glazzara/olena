#ifndef DYN_POLICY_HH
# define DYN_POLICY_HH

# include "data.hh"

namespace dyn
{
  namespace policy
  {
    
#   define select_dyn_policy(e)                                               \
      (dyn::policy::type)dyn::policy::simplify<                               \
        sizeof((dyn::policy::id_for_pod_2(), e, dyn::policy::id_for_pod_2())) \
      + sizeof((dyn::policy::id_for_ptr_and_ref_2(), e,                       \
                dyn::policy::id_for_ptr_and_ref_2()))>::val

    template <unsigned n>
    struct id
    {
      char d[n];
    };

    template <class T> struct check_is_const { enum { val = none }; };
    template <class T> struct check_is_const <const T> { enum { val = is_const }; };

    template <unsigned N>
    struct id_for_pod : id<N>
    {
      id_for_pod<N>() {}
      template <unsigned M> id_for_pod<N>(id_for_pod<M>&) {}
      id_for_pod<N>& operator() () { return *this; }

      template <typename T>
      id_for_pod<N + is_pod> operator, (T) { assert(0); }
    };
    extern id_for_pod<2> id_for_pod_2;

    template <unsigned N>
    struct id_for_ptr_and_ref : id<N>
    {
      id_for_ptr_and_ref<N>() {}
      template <unsigned M> id_for_ptr_and_ref<N>(id_for_ptr_and_ref<M>&) {}
      id_for_ptr_and_ref<N>& operator() () { return *this; }

      template <typename T>
      id_for_ptr_and_ref<N + is_ref + check_is_const<T>::val> operator, (T&) { assert(0); }

      template <typename T>
      id_for_ptr_and_ref<N + is_ptr + check_is_const<T>::val> operator, (T*) { assert(0); }
    };
    extern id_for_ptr_and_ref<2> id_for_ptr_and_ref_2;

    template <unsigned n>
    struct simplify;

#   define set_simplify(x, y) \
    template <> struct simplify<x + 20> { enum { val = y }; }

    set_simplify(-16, is_void);
    set_simplify(0,   is_pod);
    set_simplify(4,   is_ref);
    set_simplify(5,   is_ref + is_const);
    set_simplify(6,   is_ptr);
    set_simplify(7,   is_ptr + is_const);

    template <type policy>
    struct receiver
    {
      receiver() : proxy_(0) { logger << "receiver() [ policy = " << policy << " ]" << std::endl; }

      receiver& operator() ()
      {
        return *this;
      }

      template <typename T>
      receiver& operator,(T& obj)
      {
        logger << "receiver::operator,(T&) [ T = " << mlc_name<T>::of() << " ]" << std::endl;
        proxy_ = new typename dyn_choose_data_proxy<T, policy>::ret(obj);
        return *this;
      }

      template <typename T>
      receiver& operator,(const T& obj)
      {
        logger << "receiver::operator,(const T&) [ T = " << mlc_name<const T>::of() << " ]" << std::endl;
        proxy_ = new typename dyn_choose_data_proxy<T, policy>::ret(obj);
        return *this;
      }

      abstract_data* proxy()
      {
        if (proxy_)
          return proxy_;
        else
          return new data_nil;
      }

      protected:
      abstract_data* proxy_;
    };

  }
}

#endif // ! DYN_POLICY_HH
