#ifndef MINI_STD_IMPL_HH
# define MINI_STD_IMPL_HH

# include "concepts.hh"


namespace abc
{


  // container SWITCH-CASE

  namespace internal
  {

    // front insertion
    template <typename b, typename E> struct front_insertion_sequence;
    template <typename E> struct front_insertion_sequence <mlc::true_,  E> : virtual Front_Insertion_Sequence<E> {};
    template <typename E> struct front_insertion_sequence <mlc::false_, E> : virtual Sequence<E> {};

    // back insertion
    template <typename b, typename E> struct back_insertion_sequence;
    template <typename E> struct back_insertion_sequence <mlc::true_,  E> : virtual Back_Insertion_Sequence<E> {};
    template <typename E> struct back_insertion_sequence <mlc::false_, E> : virtual Sequence<E> {};

    // random access
    template <typename b, typename E> struct random_access_container;
    template <typename E> struct random_access_container <mlc::true_,  E> : virtual Random_Access_Container<E> {};
    template <typename E> struct random_access_container <mlc::false_, E> : virtual Container<E> {};

    template <typename E>
    struct container_switch
      : front_insertion_sequence< abc_vtype(E, has_front_insertion), E >,
	back_insertion_sequence < abc_vtype(E, has_back_insertion),  E >,
	random_access_container < abc_vtype(E, has_random_access),   E >
    {
    protected:
      container_switch() {}
    };


  } // end of abc::internal




//   container_base
//    |
//    +-- morpher_container
//    |
//    +-- primary_container
//         |
//         +-- primary_std_container




  // Class container_base<E>.

  template <typename E> class container_base;

  template <typename E>
  struct vtypes< container_base<E> >
  {
    typedef mlc::none super_type;

    typedef stc::abstract value_type;
    typedef stc::abstract iterator;
    typedef stc::abstract const_iterator;
    typedef stc::abstract reference;        // FIXME: or stc::final<value_type&>?
    typedef stc::abstract const_reference;  // FIXME: or stc::final<const value_type&>?
    typedef stc::abstract size_type;

    typedef stc::abstract has_random_access;
    typedef stc::abstract has_front_insertion;
    typedef stc::abstract has_back_insertion;

    typedef stc::abstract tag;
  };

  template <typename E>
  class container_base : public internal::container_switch<E>
  {
  public:
    // FIXME: remove below.
    typedef typename Back_Insertion_Sequence<E>::reference reference;
    typedef typename Back_Insertion_Sequence<E>::const_reference const_reference;
  protected:
    container_base() {}
  };



  // Class morpher_container<E>.

  template <typename E> class morpher_container;

  template <typename E>
  struct vtypes< morpher_container<E> >
  {
    typedef container_base<E> super_type;
    typedef stc::abstract delegatee_type;
  };

  template <typename E>
  class morpher_container : public container_base<E>
  {
  public:
    stc_using_from(Container<E>, value_type);
    abc_typename(delegatee_type);

    const delegatee_type& delegatee() const { return this->exact().impl_delegatee(); }
    delegatee_type&       delegatee()       { return this->exact().impl_delegatee(); }

  protected:
    morpher_container() {}
  };



  // Class primary_container<E>.

  template <typename E> class primary_container;

  template <typename E>
  struct vtypes< primary_container<E> >
  {
    typedef container_base<E> super_type;
    typedef mlc::none tag;
  };

  template <typename E>
  class primary_container : public container_base<E>
  {
  public:
    stc_using_from(Container<E>, value_type);
  protected:
    primary_container() {}
  };



  // Class primary_std_container<E>.

  template <typename E> class primary_std_container;

  template <typename E>
  struct vtypes< primary_std_container<E> >
  {
    typedef primary_container<E> super_type;
    typedef stc::abstract std_type;

    typedef stc_defer(std_type) container_t;
    typedef stc::final<typename container_t::value_type> value_type;
    typedef stc::final<typename container_t::iterator> iterator;
    typedef stc::final<typename container_t::const_iterator> const_iterator;
    typedef stc::final<typename container_t::reference> reference;
    typedef stc::final<typename container_t::const_reference> const_reference;
    typedef stc::final<typename container_t::size_type> size_type;
  };
  
  template <typename E>
  class primary_std_container : public primary_container<E>
  {
  public:
    typedef primary_container<E> super;

    stc_using(iterator);
    stc_using(const_iterator);
    stc_using(size_type);
    stc_using(value_type);

    // Container.
    iterator       impl_begin()        { return data_.begin(); }
    const_iterator impl_begin() const  { return data_.begin(); }
    iterator       impl_end()          { return data_.end(); }
    const_iterator impl_end() const    { return data_.end(); }
    size_type      impl_size() const   { return data_.size(); }
    bool           impl_empty() const  { return data_.empty(); }

    // Forward_Container.
    bool impl_equal(const E& rhs) const  { return data_ == rhs.data_; }
    bool impl_less (const E& rhs) const  { return data_ <  rhs.data_; }

  protected:

    primary_std_container()                                                     {}
    primary_std_container(size_type n)                       : data_(n)         {}
    primary_std_container(size_type n, const value_type& t)  : data_(n, t)      {}
    primary_std_container(const primary_std_container& rhs)  : data_(rhs.data_) {}
    
    abc_vtype(E, std_type) data_;    
  };
  

} // end of namespace abc


#endif // ! MINI_STD_IMPL_HH
