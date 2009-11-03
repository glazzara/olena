/* Copyright (C) 2005, 2009 EPITA Research and Development Laboratory (LRDE)

   This file is part of Olena.

   Olena is free software: you can redistribute it and/or modify it under
   the terms of the GNU General Public License as published by the Free
   Software Foundation, version 2 of the License.

   Olena is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
   General Public License for more details.

   You should have received a copy of the GNU General Public License
   along with Olena.  If not, see <http://www.gnu.org/licenses/>.

   As a special exception, you may use this file as part of a free
   software project without restriction.  Specifically, if other files
   instantiate templates or use macros or inline functions from this
   file, or you compile this file and link it with other files to produce
   an executable, this file does not by itself cause the resulting
   executable to be covered by the GNU General Public License.  This
   exception does not however invalidate any other reasons why the
   executable file might be covered by the GNU General Public License.  */

#ifndef DYN_DATA_HH
# define DYN_DATA_HH

# include <string>
# include <cassert>
# include "function.hh"
# include "name_of.hh"
# include "all_methods.hh"

namespace dyn {

  extern std::ostream& logger;

  namespace language
  {
    struct val;
    struct var;
  }

  namespace policy
  {
    enum type
    {
      none     = 0,
      is_const = 1,
      is_ref   = 2,
      is_ptr   = 4,
      is_pod   = 8,
      is_void  = 16
    };
  }
  struct proxy_tag;

  // data  -->  abstract_data
  //                 ^
  //                 |
  //            data_proxy<T>  -->  T


  // abstract_data

  struct abstract_data
  {
    virtual abstract_data* clone() const = 0;
    virtual abstract_data* const_clone() const { return clone(); }
    virtual std::string proxy_type() const = 0;
    virtual std::string type() const = 0;
    virtual ~abstract_data() {}
  };

  // data_proxy<T>

  template <class T>
  struct data_proxy : public abstract_data
  {
#   define gen_ctor                                      \
    data_proxy()                                         \
    {                                                    \
      logger << "ctor: " << proxy_type() << std::endl;   \
    }

#   define gen_proxy_type                       \
    virtual std::string proxy_type() const      \
    {                                           \
      return mlc_name_of(*this);                \
    }

    gen_proxy_type

    template <typename V>
    operator V() const
    {
      V ret(const_ref());
      return ret;
    }

    virtual std::string type() const
    {
      return mlc_name_of(const_ref());
    }

    virtual const T& const_ref() const = 0;
  };


  // FIXME Perhaps we can use smart pointers here.
  template <class T>
  struct data_proxy_by_ptr : public data_proxy<T>
  {
    data_proxy_by_ptr(T* obj) : p_obj_(obj) { logger << "ctor: " << proxy_type() << std::endl; }

    virtual data_proxy_by_ptr<T>* clone() const
    {
      return new data_proxy_by_ptr<T>(p_obj_);
    }

    virtual data_proxy_by_ptr<const T>* const_clone() const
    {
      return new data_proxy_by_ptr<const T>(p_obj_);
    }

    gen_proxy_type

    T& obj()
    {
      assert(p_obj_);
      return *p_obj_;
    }

    const T& obj() const
    {
      assert(p_obj_);
      return *p_obj_;
    }

    virtual const T& const_ref() const { return obj(); }

    protected:
    T* p_obj_;
  };


  template <class T>
  struct data_proxy_by_ref : public data_proxy<T>
  {
    data_proxy_by_ref(T& obj) : obj_(obj) { logger << "ctor: " << proxy_type() << std::endl; }

    virtual data_proxy_by_ref<T>* clone() const
    {
      return new data_proxy_by_ref<T>(obj_);
    }

    virtual data_proxy_by_ref<const T>* const_clone() const
    {
      return new data_proxy_by_ref<const T>(obj_);
    }

    virtual const T& const_ref() const { return obj_; }
    const T& obj() const { return obj_; }
    T& obj() { return obj_; }

    gen_proxy_type

    protected:
    T& obj_;
  };


  template <class T>
  struct data_proxy_by_cpy : public data_proxy<T>
  {
    data_proxy_by_cpy(const T obj) : obj_(T(obj)) { logger << "ctor: " << proxy_type() << std::endl; }

    virtual data_proxy_by_cpy<T>* clone() const
    {
      return new data_proxy_by_cpy<T>(obj_);
    }

    virtual data_proxy_by_cpy<const T>* const_clone() const
    {
      return new data_proxy_by_cpy<const T>(obj_);
    }

    virtual const T& const_ref() const { return obj_; }
    const T& obj() const { return obj_; }
    T& obj() { return obj_; }

    gen_proxy_type

    protected:
    T obj_;
  };


  struct NilClass
  {
    NilClass(int) {}
  };

  struct data_nil : public data_proxy<NilClass>
  {
    data_nil() : nil_object_(NilClass(0)) { logger << "ctor: " << proxy_type() << std::endl; }
    data_nil(const NilClass& nil_object) : nil_object_(nil_object) {}
    const NilClass& const_ref() const { return nil_object_; }
    const NilClass& obj() const { return nil_object_; }
    virtual data_nil* clone() const { return new data_nil; }
    gen_proxy_type
    const NilClass nil_object_;
  };

  extern const NilClass nil_object;
  extern data_nil* nil_proxy;

  template <typename T1, typename T2>
  T2
  data_cast(const T1& src)
  {
    T2 tmp(src);
    return tmp;
  }

  template <typename T1, typename T2>
  void
  data_assign(T1& lhs, const T2& rhs)
  {
    lhs = rhs;
  }

  // data

  struct data : public all_methods
  {

    virtual const data& self() const
    {
      return *this;
    }

    virtual data& self()
    {
      return *this;
    }

    data& assign(const data& rhs)
    {
      if (&rhs == this) return *this;
      if (rhs.proxy_ == 0)
      {
        logger << "assign(const data& rhs) [ rhs.proxy_ == 0 ]" << std::endl;
        proxy_ = nil_proxy;
      }
      else
      {
        assert(rhs.proxy_ != proxy_);
        // if (proxy_ != 0)
        //   delete proxy_;
        proxy_ = rhs.proxy_->clone();
      }
      return *this;
    }

    data& const_assign(const data& rhs)
    {
      if (&rhs == this) return *this;
      if (rhs.proxy_ == 0)
      {
        logger << "const_assign(const data& rhs) [ rhs.proxy_ == 0 ]" << std::endl;
        proxy_ = nil_proxy;
      }
      else
      {
        assert(rhs.proxy_ != proxy_);
        // if (proxy_ != 0)
        //   delete proxy_;
        proxy_ = rhs.proxy_->const_clone();
      }
      return *this;
    }

    data& operator=(const data& rhs) { return assign(rhs); }

    data& operator=(data& rhs) { return assign(rhs); }


    template <typename T>
    data& operator=(const T& rhs);


    template <typename T>
    T convert_to() const;


    template <typename T>
    T& get_ref_on();

    template <typename T>
    const T& get_ref_on() const;

    std::string& const_stripping(std::string& str) const
    {
      size_t pos;
      while ((pos = str.find("const ")) != std::string::npos)
        str.erase(pos, 6);
      while ((pos = str.find(" const")) != std::string::npos)
        str.erase(pos, 6);
      return str;
    }

    template <typename T>
    operator T() const
    {
      std::string src_type(proxy()->type());
      std::string dest_type(mlc_name<T>::of());
      if (const_stripping(src_type) == const_stripping(dest_type))
        return get_ref_on<T>();
      else
        return convert_to<T>();
    }


    ~data()
    {
      if (proxy_ != nil_proxy)
      {
        logger << "~data [ type() = " << type() << " ]" << std::endl;
        delete proxy_;
        proxy_ = nil_proxy;
      }
    }


    std::string type() const
    {
      return proxy()->proxy_type();
    }


    data operator[](const data&);
    const data& operator[](const data&) const;
    data operator*();
    const data& operator*() const;


    abstract_data* proxy() const
    {
      assert(proxy_);
      return proxy_;
    }

    bool is_const()
    {
      std::string type_(type());
      return type_.find("dyn::data_proxy_by_ref<") == 0
          && type_.rfind("const>") == type_.length() - 6;
    }

    meth method(const std::string& method_name);

    data() : proxy_(nil_proxy) {}

    /* FIXME: What's the purpose of the second argument?  I (Roland)
       presume it serve as a desambiguition mechanism w.r.t to ctors
       `data<T>(T&)' and `data<T>(const T&)'.  If so, document it.  */
    data(abstract_data* proxy, proxy_tag*) : all_methods(), proxy_(proxy) {}

    template <class T>
    data(T& obj) : all_methods()
    {
      logger << "data(T& obj) [ T = " << mlc_name<T>::of() << " ]" << std::endl;
      proxy_ = new data_proxy_by_ref<T>(obj);
    }

    template <class T>
    data(const T& obj) : all_methods()
    {
      logger << "data(const T& obj) [ T = " << mlc_name<T>::of() << " ]" << std::endl;
      proxy_ = new data_proxy_by_ref<const T>(obj);
    }

    data(language::var& rhs);
    data(language::val& rhs);
    data(const language::var& rhs);
    data(const language::val& rhs);

    data(data& rhs) : all_methods(), proxy_(0)
    {
      logger << "data(data& rhs) [ rhs.type() = " << rhs.type() << " ]" << std::endl;
      assign(rhs);
    }

    data(const data& rhs) : all_methods(), proxy_(0)
    {
      logger << "data(const data& rhs) [ rhs.type() = " << rhs.type() << " ]" << std::endl;
      assign(rhs);
    }

    protected:
      abstract_data* proxy_;
  };

  extern const data nil;

  namespace internal {
    extern op operator_push;
    extern op operator_pop;
    extern op operator_incr;
    extern op operator_decr;
    extern op operator_plus;
    extern op operator_star;
    extern op operator_equal;
    extern op operator_not_equal;
    extern meth operator_square_brackets;
  }

}

template <typename T, dyn::policy::type policy>
struct dyn_choose_data_proxy
{
  typedef dyn::data_proxy_by_cpy<T> ret;
};

template <typename T>
struct dyn_choose_data_proxy<T, dyn::policy::is_ref>
{
  typedef dyn::data_proxy_by_ref<T> ret;
};

template <typename T>
struct dyn_choose_data_proxy<T, (dyn::policy::type)(dyn::policy::is_ref + dyn::policy::is_const)>
{
  typedef dyn::data_proxy_by_ref<const T> ret;
};

# ifdef DYN_FULL_IMPLEMENTATION
# include "data.hxx"
# endif

#endif // ! DYN_DATA_HH
