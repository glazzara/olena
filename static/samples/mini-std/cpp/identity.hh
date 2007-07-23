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

    template < template <class> class abstraction, typename Exact >
    struct impl< abstraction, Exact, tag::identity >
    {
      // ...
    };



    // Container

    template <typename Exact>
    class impl< Container, Exact, tag::identity >
      :
      public virtual stc::any__simple<Exact>
    {
    protected:
      abc_typename(value_type);
      abc_typename(iterator);
      abc_typename(const_iterator);
      abc_typename(size_type);
    public:
      iterator       impl_begin()        { return this->exact().delegatee_().begin(); }
      const_iterator impl_begin() const  { return this->exact().delegatee_().begin(); }
      iterator       impl_end()          { return this->exact().delegatee_().end(); }
      const_iterator impl_end() const    { return this->exact().delegatee_().end(); }
      size_type      impl_size() const   { return this->exact().delegatee_().size(); }
      bool           impl_empty() const  { return this->exact().delegatee_().empty(); }
    };



    // Forward_Container

    template <typename Exact>
    class impl< Forward_Container, Exact, tag::identity >
      :
      public virtual stc::any__simple<Exact>
    {
    protected:
      typedef Forward_Container<Exact> self_type;
    public:
      bool impl_equal(const self_type& rhs) const   { return this->exact().delegatee_().operator==(rhs); }
      bool impl_less (const self_type& rhs) const   { return this->exact().delegatee_().operator< (rhs); }
    };



    // Reversible_Container

    template <typename Exact>
    class impl< Reversible_Container, Exact, tag::identity >
      :
      public virtual stc::any__simple<Exact>
    {
    protected:
      abc_typename(reverse_iterator);
      abc_typename(const_reverse_iterator);
    public:
      reverse_iterator       impl_rbegin()        { return this->exact().delegatee_().rbegin(); }
      const_reverse_iterator impl_rbegin() const  { return this->exact().delegatee_().rbegin(); }
      reverse_iterator       impl_rend()          { return this->exact().delegatee_().rend(); }
      const_reverse_iterator impl_rend() const    { return this->exact().delegatee_().rend(); }
    };



    // Random_Access_Container

    template <typename Exact>
    class impl< Random_Access_Container, Exact, tag::identity >
      :
      public virtual stc::any__simple<Exact>
    {
    protected:
      abc_typename(reference);
      abc_typename(const_reference);
      abc_typename(size_type);
    public:
      reference       impl_brackets(size_type n)        { return this->exact().delegatee_()[n]; }
      const_reference impl_brackets(size_type n) const  { return this->exact().delegatee_()[n]; }
    };



    // Sequence

    template <typename Exact>
    class impl< Sequence, Exact, tag::identity >
      :
      public virtual stc::any__simple<Exact>
    {
    protected:
      abc_typename(value_type);
      abc_typename(iterator);
    public:
      iterator impl_insert(iterator p, value_type t)  { return this->exact().delegatee_().insert(p, t); }
      iterator impl_erase(iterator p)                 { return this->exact().delegatee_().erase(p); }
      iterator impl_erase(iterator p, iterator q)     { return this->exact().delegatee_().erase(p, q); }
    };



    // Front_Insertion_Sequence

    template <typename Exact>
    class impl< Front_Insertion_Sequence, Exact, tag::identity >
      :
      public virtual stc::any__simple<Exact>
    {
    protected:
      abc_typename(value_type);
    public:
      void impl_push_front(value_type t)  { this->exact().delegatee_().push_front(t); }
      void impl_pop_front()               { this->exact().delegatee_().pop_front(); }
    };



    // Back_Insertion_Sequence

    template <typename Exact>
    class impl< Back_Insertion_Sequence, Exact, tag::identity >
      :
      public virtual stc::any__simple<Exact>
    {
    protected:
      abc_typename(value_type);
      abc_typename(reference);
    public:
      reference impl_back()              { this->exact().delegatee_().back(); }
      void impl_push_back(value_type t)  { this->exact().delegatee_().push_back(t); }
      void impl_pop_back()               { this->exact().delegatee_().pop_back(); }
    };



  } // end of namespace abc::automatic


} // end of namespace abc


#endif // ! MINI_STD_IDENTITY_HH
