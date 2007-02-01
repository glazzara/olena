
#include <iostream>
#include "local_scoop.hh"


stc_equip_namespace(abc);



namespace abc
{

  // List of associated types.
  mlc_decl_typedef(value);
  mlc_decl_typedef(forward);
  mlc_decl_typedef(backward);



  // - - - - - - - - - - - - - - - - - - - -   concepts


  // Iterator

  template <typename Exact>
  struct Iterator : public Concept<Exact>,
		    public automatic::impl<Iterator, Exact>
  {
    stc_typename(value);

    bool is_valid() const     { return this->exact().impl_is_valid(); }
    void set(const value& v)  { this->exact().impl_set(v);  }
  };


  // Forward_Iterator

  template <typename Exact>
  struct Forward_Iterator : public Iterator<Exact>,
			    public automatic::impl<Forward_Iterator, Exact>
  {
    void next()  { this->exact().impl_next();  }
  };


  // Bidirectional_Iterator

  template <typename Exact>
  struct Bidirectional_Iterator : public Forward_Iterator<Exact>,
				  public automatic::impl<Bidirectional_Iterator, Exact>
  {
    void prev()  { this->exact().impl_prev();  }
  };




  // - - - - - - - - - - - - - - - - - - - -   FIXME: plug


  namespace internal
  {

    // selector 1 is about Iterator browsing mode

    typedef  selector<Iterator, 1>  Iterator_browsing;
  
    template <typename E>
    struct case_< Iterator_browsing, E,  1 > : where_< mlc::and_< stc_prop(forward), stc_prop(backward) > >
    {
      typedef Bidirectional_Iterator<E> ret;
    };
  
    template <typename E>
    struct case_< Iterator_browsing, E,  2 > : where_< stc_prop(forward) >
    {
      typedef Forward_Iterator<E> ret;
    };
  
  } // end of internal



  // - - - - - - - - - - - - - - - - - - - -   classes



  // iterator_base


# define super      top<E>
  //               -------
  //                  ^
  //                  |
# define current    iterator_base<E>
  //               ------------------

# define templ      template <typename E>
# define classname  iterator_base


  stc_Header;

  typedef stc::is<Iterator> category;
  typedef stc::abstract value;

  stc_End;


  template <typename E>
  class iterator_base : public super
  {
  public:

    stc_lookup(value);

    // a base class can factor some code!
    // for instance:
    const value& get() const { return v_; }

  protected:
    iterator_base() {}
    value v_;
  };


# include "undefs.hh"




  // lightweight_iterator


# define super      iterator_base<E>
  //               ------------------
  //                  ^
  //                  |
# define current    lightweight_iterator<E>
  //               -------------------------

# define templ      template <typename E>
# define classname  lightweight_iterator


  stc_Header;

  typedef stc::abstract delegatee_type;

  stc_End;


  template <typename E>
  class lightweight_iterator : public super
  {
  public:

    stc_lookup(delegatee_type);

    const delegatee_type& delegatee() const  { return delegatee_; }
          delegatee_type& delegatee()        { return delegatee_; }

  protected:
    lightweight_iterator(delegatee_type& d) : delegatee_(d) {}
    delegatee_type& delegatee_;
  };


# include "undefs.hh"
  




  // array_iterator<T>


  //                iterator_base
  //               ---------------
  //                     ^
  //                     |
# define current    array_iterator<T>
  //               ------------------

# define templ      template <typename T>
# define classname  array_iterator

# define super      iterator_base< current >


  stc_Header;

  typedef T value;
  typedef mlc::true_  forward;
  typedef mlc::true_ backward;

  stc_End;


  template <typename T>
  class array_iterator : public super
  {
  public:

    stc_using(value);
    void impl_next()               { i_ = i_ + 1; }
    bool impl_is_valid() const     { return i_ >= 0 and i_ < n_; }
    void impl_set(const value& v)  { this->v_ = v; }
    void impl_prev()               { i_ = i_ - 1; }

    array_iterator(int n)          : i_(0), n_(n) {}
  protected:
    int i_, n_;
  };

# include "undefs.hh"







  // algo

  template <typename I>
  void algo(Iterator<I>& iter, typename I::value val)
  {
    std::cout << "Iterator" << std::endl;
    if (iter.is_valid())
      {
	iter.set(val);
      }
  }

  template <typename I>
  void algo(Forward_Iterator<I>& iter, typename I::value val)
  {
    std::cout << "Forward_Iterator" << std::endl;
    while (iter.is_valid())
      {
	std::cout << '.';
	iter.set(val);
	iter.next();
      }
    std::cout << std::endl;
  }

  template <typename I>
  void algo(Bidirectional_Iterator<I>& iter, typename I::value val)
  {
    std::cout << "Bidirectional_Iterator" << std::endl;
    while (iter.is_valid())
      {
	std::cout << '.';
	iter.set(val);
	iter.next();
      }
    std::cout << std::endl;
  }


} // abc





namespace abc
{

  namespace tag
  {
    struct identity;

  } // abc::tag

} // abc

#include "identity.hh"



namespace abc
{



  // skippy


# define current    skippy<I>
  //               -------------------------

# define templ      template <typename I>
# define classname  skippy

# define super      lightweight_iterator< current >


  stc_Header;

  typedef I delegatee_type;
  typedef tag::identity tag;

  stc_End;


  template <typename I>
  class skippy : public super
  {
  public:

    skippy(Iterator<I>& it, unsigned skip)
      :
      super(it.exact()),
      skip_(skip)
    {
    }

    void impl_next()
    {
      for (unsigned i = 0; i <= skip_; ++i)
	this->delegatee().next();
    }

    void impl_prev()
    {
      for (unsigned i = 0; i <= skip_; ++i)
	this->delegatee().prev();
    }

  protected:
    unsigned skip_;
  };


# include "undefs.hh"


  template <typename I>
  skippy<I> skip(Iterator<I>& it, unsigned s)
  {
    skippy<I> tmp(it, s);
    return tmp;
  }



} // abc




int main()
{
  typedef abc::array_iterator<int> iterator_t;
  int val = 51;

  {
    iterator_t i(8);
    abc::algo(i, val);
  }
  {
    iterator_t i(8);
    abc::skippy<iterator_t> ii = skip(i, 1u);
    abc::algo(ii, val);
  }
}
