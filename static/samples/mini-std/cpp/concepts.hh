#ifndef MINI_STD_CONCEPTS_HH
# define MINI_STD_CONCEPTS_HH

# include "equipment.hh"


namespace abc
{

  // Container

  template <typename Exact>
  class Container : public virtual stc::any__simple<Exact>,
		    public automatic::impl<Container, Exact>
  {
  public:
      
    // Typedefs.
    abc_typename(value_type);
    abc_typename(iterator);
    abc_typename(const_iterator);
    abc_typename(reference);
    abc_typename(const_reference);
    abc_typename(size_type);

    // Methods.
    iterator       begin()        { return this->exact().impl_begin(); }
    const_iterator begin() const  { return this->exact().impl_begin(); }
    iterator       end()          { return this->exact().impl_end(); }
    const_iterator end() const    { return this->exact().impl_end(); }
    size_type      size() const   { return this->exact().impl_size(); }
    bool           empty() const  { return this->exact().impl_empty(); }

  protected:
    Container() {}
  };



  // Forward_Container

  template <typename Exact>
  class Forward_Container : public virtual Container<Exact>,
			    public automatic::impl<Forward_Container, Exact>
  {
  public:

    typedef Forward_Container<Exact> self_type;

    // Methods.
    bool operator==(const self_type& rhs) const   { return this->exact().impl_equal(rhs.exact()); }
    bool operator< (const self_type& rhs) const   { return this->exact().impl_less(rhs.exact()); }

    // Concrete. // FIXME: Use the default impl mechanism so that the user can override.
    bool operator!=(const self_type& rhs) const   { return not (*this == rhs); }
    bool operator> (const self_type& rhs) const   { return rhs < *this; }
    bool operator<=(const self_type& rhs) const   { return not (*this > rhs); }
    bool operator>=(const self_type& rhs) const   { return not (*this < rhs); }

  protected:
    Forward_Container() {}
  };



  // Reversible_Container

  template <typename Exact>
  class Reversible_Container : public virtual Forward_Container<Exact>,
			       public automatic::impl<Reversible_Container, Exact>
  {
  public:

    // Typedefs.
    abc_typename(reverse_iterator);
    abc_typename(const_reverse_iterator);

    // Methods.
    reverse_iterator       rbegin()        { return this->exact().impl_rbegin(); }
    const_reverse_iterator rbegin() const  { return this->exact().impl_rbegin(); }
    reverse_iterator       rend()          { return this->exact().impl_rend(); }
    const_reverse_iterator rend() const    { return this->exact().impl_rend(); }

  protected:
    Reversible_Container() {}
  };



  // Random_Access_Container

  template <typename Exact>
  class Random_Access_Container : public virtual Reversible_Container<Exact>,
				  public automatic::impl<Random_Access_Container, Exact>
  {
  public:

    typedef Reversible_Container<Exact> super;
    stc_using(reference);
    stc_using(const_reference);
    stc_using(size_type);

    // Methods.
    reference operator[](size_type n)              { assert(0 <= n and n <= this->size());
                                                     return this->exact().impl_brackets(n); }
    const_reference operator[](size_type n) const  { assert(0 <= n and n <= this->size());
                                                     return this->exact().impl_brackets(n); }
  protected:
    Random_Access_Container() {}
  };



  // Sequence

  template <typename Exact>
  class Sequence : public virtual Forward_Container<Exact>,
		   public automatic::impl<Sequence, Exact>
  {
  public:

    typedef Forward_Container<Exact> super;
    stc_using(iterator);
    stc_using(value_type);
    stc_using(reference);
    stc_using(const_reference);

    // Methods.
    iterator insert(iterator p, value_type t)  { return this->exact().impl_insert(p, t); }
    iterator erase(iterator p)                 { return this->exact().impl_erase(p); }
    iterator erase(iterator p, iterator q)     { return this->exact().impl_erase(p, q); }

    // Concrete.
    reference front()
    {
      assert(not this->empty());
      return *(this->first());
    }
    const_reference front() const
    {
      assert(not this->empty());
      return *(this->first());
    }
    void clear()
    {
      this->erase(this->begin(), this->end());
    }

  protected:
    Sequence() {}
  };




  // Front_Insertion_Sequence

  template <typename Exact>
  class Front_Insertion_Sequence : public virtual Sequence<Exact>,
				   public automatic::impl<Front_Insertion_Sequence, Exact>
  {
  public:

    typedef Sequence<Exact> super;
    stc_using(value_type);

    // Concrete.
    void push_front(value_type t)
    {
      this->insert(this->begin(), t);
    }
    void pop_front()
    {
      assert(not this->empty());
      this->erase(this->begin());
    }

  protected:
    Front_Insertion_Sequence() {}
  };



  // Back_Insertion_Sequence

  template <typename Exact>
  class Back_Insertion_Sequence : public virtual Sequence<Exact>,
				  public automatic::impl<Back_Insertion_Sequence, Exact>
  {
  public:

    typedef Sequence<Exact> super;
    stc_using(reference);
    stc_using(value_type);

    // Concrete.
    reference back()
    {
      assert(not this->empty());
      return *(--this->end());
    }
    void push_back(value_type t)
    {
      this->insert(this->end(), t);
    }
    void pop_back()
    {
      assert(not this->empty());
      this->erase(--this->end());
    }
  protected:
    Back_Insertion_Sequence() {}
  };



  // Iterator

  template <typename Exact>
  class Iterator : public virtual stc::any__simple<Exact>,
		   public automatic::impl<Iterator, Exact>
  {
  public:

    // FIXME

  protected:
    Iterator() {}
  };



  // Trivial_Iterator

  template <typename Exact>
  class Trivial_Iterator : public virtual Iterator<Exact>,
			   public automatic::impl<Trivial_Iterator, Exact>
  {
  public:

    typedef Trivial_Iterator<Exact> self_type;

    // Typedef.
    abc_typename(value_type);

    // Methods.
    value_type operator*() const                 { return this->exact().impl_star(); }
    const value_type& operator->() const         { return this->exact().impl_arrow(); }
    bool operator==(const self_type& rhs) const  { return this->exact().impl_equal(rhs.exact()); }

    // Concrete.
    bool operator!=(const self_type& rhs) const
    {
      return not (*this == rhs);
    }

  protected:
    Trivial_Iterator() {}
  };



  // Input_Iterator

  template <typename Exact>
  class Input_Iterator : public virtual Trivial_Iterator<Exact>,
			 public automatic::impl<Input_Iterator, Exact>
  {
  public:

    // Typedef.
    abc_typename(distance_type);

    // Method.
    Exact& operator++()           { return this->exact().impl_preinc(); }

    // Concrete.
    Exact operator++(int) const
    {
      Exact cpy(*this);
      ++(*this);
      return cpy;
    }

  protected:
    Input_Iterator() {}
  };



  // Output_Iterator

  template <typename Exact>
  class Output_Iterator : public virtual Trivial_Iterator<Exact>,
			  public automatic::impl<Output_Iterator, Exact>
  {
  public:

    typedef Trivial_Iterator<Exact> super;
    stc_using(value_type);

    using super::operator*;
    using super::operator->;

    // Methods.
    value_type& operator*()   { return this->exact().impl_star(); }
    value_type& operator->()  { return this->exact().impl_arrow(); }

  protected:
    Output_Iterator() {}
  };


  
  // Forward_Iterator

  template <typename Exact>
  class Forward_Iterator : public virtual Input_Iterator<Exact>, public virtual Output_Iterator<Exact>,
			   public automatic::impl<Forward_Iterator, Exact>
  {
  public:
    // nothing
  protected:
    Forward_Iterator() {}
  };



  // Bidirectional_Iterator

  template <typename Exact>
  class Bidirectional_Iterator : public virtual Forward_Iterator<Exact>,
				 public automatic::impl<Bidirectional_Iterator, Exact>
  {
  public:

    // Method.
    Exact& operator--()           { return this->exact().impl_predec(); }

    // Concrete.
    Exact operator--(int) const
    {
      Exact cpy(*this);
      --(*this);
      return cpy;
    }

  protected:
    Bidirectional_Iterator() {}
  };


} // end of namespace abc


#endif // ! MINI_STD_CONCEPTS_HH
