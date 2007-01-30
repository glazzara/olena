#ifndef MINI_STD_IDENTITY_HH
# define MINI_STD_IDENTITY_HH

# include "equipment.hh"
# include "fwddecls.hh"



namespace abc
{

  // tag::identity.

  namespace tag { struct identity; }


  namespace automatic
  {

    // FIXME: Hack! (to be removed)

    template < template <class> class abstraction, typename E >
    struct impl< abstraction, E, tag::identity >
    {
      // ...
    };



    // Container

    template <typename E>
    class impl< Container, E, tag::identity >
      :
      public virtual stc::any__simple<E>
    {
    protected:
      abc_typename(value_type);
      abc_typename(iterator);
      abc_typename(const_iterator);
      abc_typename(size_type);
    public:
      iterator       impl_begin()        { return this->exact().delegatee().begin(); }
      const_iterator impl_begin() const  { return this->exact().delegatee().begin(); }
      iterator       impl_end()          { return this->exact().delegatee().end(); }
      const_iterator impl_end() const    { return this->exact().delegatee().end(); }
      size_type      impl_size() const   { return this->exact().delegatee().size(); }
      bool           impl_empty() const  { return this->exact().delegatee().empty(); }
    };



    // Forward_Container

    template <typename E>
    class impl< Forward_Container, E, tag::identity >
      :
      public virtual stc::any__simple<E>
    {
    protected:
      typedef Forward_Container<E> self_type;
    public:
      bool impl_equal(const self_type& rhs) const   { return this->exact().delegatee().operator==(rhs); }
      bool impl_less (const self_type& rhs) const   { return this->exact().delegatee().operator< (rhs); }
    };



    // Reversible_Container

    template <typename E>
    class impl< Reversible_Container, E, tag::identity >
      :
      public virtual stc::any__simple<E>
    {
    protected:
      abc_typename(reverse_iterator);
      abc_typename(const_reverse_iterator);
    public:
      reverse_iterator       impl_rbegin()        { return this->exact().delegatee().rbegin(); }
      const_reverse_iterator impl_rbegin() const  { return this->exact().delegatee().rbegin(); }
      reverse_iterator       impl_rend()          { return this->exact().delegatee().rend(); }
      const_reverse_iterator impl_rend() const    { return this->exact().delegatee().rend(); }
    };



    // Random_Access_Container

    template <typename E>
    class impl< Random_Access_Container, E, tag::identity >
      :
      public virtual stc::any__simple<E>
    {
    protected:
      abc_typename(reference);
      abc_typename(const_reference);
      abc_typename(size_type);
    public:
      reference       impl_brackets(size_type n)        { return this->exact().delegatee()[n]; }
      const_reference impl_brackets(size_type n) const  { return this->exact().delegatee()[n]; }
    };



    // Sequence

    template <typename E>
    class impl< Sequence, E, tag::identity >
      :
      public virtual stc::any__simple<E>
    {
    protected:
      abc_typename(value_type);
      abc_typename(iterator);
    public:
      iterator impl_insert(iterator p, value_type t)  { return this->exact().delegatee().insert(p, t); }
      iterator impl_erase(iterator p)                 { return this->exact().delegatee().erase(p); }
      iterator impl_erase(iterator p, iterator q)     { return this->exact().delegatee().erase(p, q); }
    };



    // Front_Insertion_Sequence

    template <typename E>
    class impl< Front_Insertion_Sequence, E, tag::identity >
      :
      public virtual stc::any__simple<E>
    {
    protected:
      abc_typename(value_type);
    public:
      void impl_push_front(value_type t)  { this->exact().delegatee().push_front(t); }
      void impl_pop_front()               { this->exact().delegatee().pop_front(); }
    };



    // Back_Insertion_Sequence

    template <typename E>
    class impl< Back_Insertion_Sequence, E, tag::identity >
      :
      public virtual stc::any__simple<E>
    {
    protected:
      abc_typename(value_type);
      abc_typename(reference);
    public:
      reference impl_back()              { this->exact().delegatee().back(); }
      void impl_push_back(value_type t)  { this->exact().delegatee().push_back(t); }
      void impl_pop_back()               { this->exact().delegatee().pop_back(); }
    };



  } // end of namespace abc::automatic


} // end of namespace abc


#endif // ! MINI_STD_IDENTITY_HH
