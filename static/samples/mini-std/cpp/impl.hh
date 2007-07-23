#ifndef MINI_STD_IMPL_HH
# define MINI_STD_IMPL_HH

# include "concepts.hh"


namespace abc
{


  // container SWITCH-CASE

  namespace internal
  {

    // front insertion
    template <typename b, typename Exact> struct front_insertion_sequence;
    template <typename Exact> struct front_insertion_sequence <mlc::true_,  Exact> : virtual Front_Insertion_Sequence<Exact> {};
    template <typename Exact> struct front_insertion_sequence <mlc::false_, Exact> : virtual Sequence<Exact> {};

    // back insertion
    template <typename b, typename Exact> struct back_insertion_sequence;
    template <typename Exact> struct back_insertion_sequence <mlc::true_,  Exact> : virtual Back_Insertion_Sequence<Exact> {};
    template <typename Exact> struct back_insertion_sequence <mlc::false_, Exact> : virtual Sequence<Exact> {};

    // random access
    template <typename b, typename Exact> struct random_access_container;
    template <typename Exact> struct random_access_container <mlc::true_,  Exact> : virtual Random_Access_Container<Exact> {};
    template <typename Exact> struct random_access_container <mlc::false_, Exact> : virtual Container<Exact> {};

    template <typename Exact>
    struct container_switch
      : front_insertion_sequence< abc_vtype(Exact, has_front_insertion), Exact >,
	back_insertion_sequence < abc_vtype(Exact, has_back_insertion),  Exact >,
	random_access_container < abc_vtype(Exact, has_random_access),   Exact >
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




  // Class container_base<Exact>.

  template <typename Exact> class container_base;

  template <typename Exact>
  struct super_trait_< container_base<Exact> >
  {
    typedef mlc::none ret;
  };

  template <typename Exact>
  struct vtypes< container_base<Exact> >
  {
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

  template <typename Exact>
  class container_base : public internal::container_switch<Exact>
  {
  public:
    // FIXME: remove below.
    typedef typename Back_Insertion_Sequence<Exact>::reference reference;
    typedef typename Back_Insertion_Sequence<Exact>::const_reference const_reference;
  protected:
    container_base() {}
  };



  // Class morpher_container<Exact>.

  template <typename Exact> class morpher_container;


  template <typename Exact>
  struct super_trait_< morpher_container<Exact> >
  {
    typedef container_base<Exact> ret;
  };

  template <typename Exact>
  struct vtypes< morpher_container<Exact> >
  {
    typedef stc::abstract delegatee;
  };

  template <typename Exact>
  class morpher_container : public container_base<Exact>
  {
  public:
    stc_using_from(Container, value_type);
    abc_typename(delegatee);

    // FIXME: `delegatee_' and `impl_delegatee_' are not elegant
    // names, but we cannot use `delegatee' (since it is a typedef in
    // this class).  Find better names.
    const delegatee& delegatee_() const { return this->exact().impl_delegatee_(); }
    delegatee&       delegatee_()       { return this->exact().impl_delegatee_(); }

  protected:
    morpher_container() {}
  };



  // Class primary_container<Exact>.

  template <typename Exact> class primary_container;

  template <typename Exact>
  struct super_trait_< primary_container<Exact> >
  {
    typedef container_base<Exact> ret;
  };

  template <typename Exact>
  struct vtypes< primary_container<Exact> >
  {
    typedef mlc::none tag;
  };

  template <typename Exact>
  class primary_container : public container_base<Exact>
  {
  public:
    stc_using_from(Container, value_type);
  protected:
    primary_container() {}
  };



  // Class primary_std_container<Exact>.

  template <typename Exact> class primary_std_container;

  template <typename Exact>
  struct super_trait_< primary_std_container<Exact> >
  {
    typedef primary_container<Exact> ret;
  };

  template <typename Exact>
  struct vtypes< primary_std_container<Exact> >
  {
    typedef stc::abstract std_type;

    typedef stc_defer(std_type) container_t;
    typedef stc::final<typename container_t::value_type> value_type;
    typedef stc::final<typename container_t::iterator> iterator;
    typedef stc::final<typename container_t::const_iterator> const_iterator;
    typedef stc::final<typename container_t::reference> reference;
    typedef stc::final<typename container_t::const_reference> const_reference;
    typedef stc::final<typename container_t::size_type> size_type;
  };
  
  template <typename Exact>
  class primary_std_container : public primary_container<Exact>
  {
  public:
    typedef primary_container<Exact> super;

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
    bool impl_equal(const Exact& rhs) const  { return data_ == rhs.data_; }
    bool impl_less (const Exact& rhs) const  { return data_ <  rhs.data_; }

  protected:

    primary_std_container()                                                     {}
    primary_std_container(size_type n)                       : data_(n)         {}
    primary_std_container(size_type n, const value_type& t)  : data_(n, t)      {}
    primary_std_container(const primary_std_container& rhs)  : data_(rhs.data_) {}
    
    abc_vtype(Exact, std_type) data_;    
  };
  

} // end of namespace abc


#endif // ! MINI_STD_IMPL_HH
