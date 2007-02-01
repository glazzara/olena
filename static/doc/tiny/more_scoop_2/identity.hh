
namespace abc
{

  namespace automatic
  {


    // Iterator

    template <typename Exact>
    struct impl< Iterator, Exact, tag::identity > : public virtual stc::any<Exact>
    {
      stc_typename(value);

      bool impl_is_valid() const     { return this->exact().delegatee().is_valid(); }
      void impl_set(const value& v)  { this->exact().delegatee().set(v);  }
    };


    // Forward_Iterator

    template <typename Exact>
    struct impl< Forward_Iterator, Exact, tag::identity > : public virtual stc::any<Exact>
    {
      void impl_next()  { this->exact().delegatee().next();  }
    };


    // Bidirectional_Iterator

    template <typename Exact>
    struct impl< Bidirectional_Iterator, Exact, tag::identity > : public virtual stc::any<Exact>
    {
      void impl_prev()  { this->exact().delegatee().impl_prev();  }
    };


  } // end of namespace abc::automatic


} // end of namespace abc

