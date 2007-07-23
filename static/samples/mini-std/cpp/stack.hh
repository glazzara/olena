#ifndef MINI_STD_CONTAINERS_HH
# define MINI_STD_CONTAINERS_HH

# include "impl.hh"


namespace abc
{


  template <typename T, typename S> class stack;

  template <typename T, S>
  struct super_trait_< list<T, S> >
  {
    typedef stack<T, S> self__;
    typedef morpher_container<self__> ret;
  };

  template <typename T, typename S>
  struct vtypes< stack<T,S> >
  {
    typedef S delegatee;

    // Properties.
    typedef mlc::false_ has_front_insertion;
    typedef mlc::true_  has_back_insertion;

    typedef tag::identity tag;
  };



  template <typename T, typename S>
  class stack : public morpher_container< stack<T,S> >
  {
  public:

    typedef stack<T,S> self_type;
    typedef morpher_container<self_type> super;

    stc_using(reference);
    stc_using(value_type);
    stc_using(delegatee);
    
    // Renaming.
    reference top()
    {
      assert(not this->empty());
      return this->back();
    }
    void push(value_type t)
    {
      this->push_back(t);
    }
    void pop()
    {
      assert(not this->empty());
      this->pop_back();
    }

    const delegatee& impl_delegatee_() const { return this->seq_; }
    delegatee&       impl_delegatee_()       { return this->seq_; }

  protected:
    S seq_;
  };
  

} // end of namespace abc


#endif // ! MINI_STD_CONTAINERS_HH
