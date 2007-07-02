#ifndef MLN_CORE_INTERNAL_SET_OF_HH
# define MLN_CORE_INTERNAL_SET_OF_HH

# include <vector>
# include <set>
# include <iterator>


namespace mln
{

  namespace internal
  {

    template <typename E>
    class set_of_
    {
    public:

      void insert(const E& elt);
      const E& element(unsigned i) const;
      unsigned nelements() const;
      bool has(const E& elt) const;
      bool is_empty() const;
      void clear();

      const std::vector<E>& vec() const;

    protected:
      std::vector<E> v_;

    private:
      std::set<E> s_;
      void update_();
      
    };


    template <typename E>
    std::ostream& operator<<(std::ostream& ostr, const set_of_<E>& s);


# ifndef MLN_INCLUDE_ONLY

    template <typename E>
    void
    set_of_<E>::insert(const E& elt)
    {
      s_.insert(elt);
      update_();
    }

    template <typename E>
    const E& 
    set_of_<E>::element(unsigned i) const
    {
      assert(i < v_.size());
      return v_[i];
    }

    template <typename E>
    unsigned
    set_of_<E>::nelements() const
    {
      return v_.size();
    }

    template <typename E>
    bool
    set_of_<E>::has(const E& elt) const
    {
      return s_.find(elt) != s_.end();
    }

    template <typename E>
    bool
    set_of_<E>::is_empty() const
    {
      return v_.size() == 0;
    }

    template <typename E>
    void
    set_of_<E>::clear()
    {
      v_.clear();
      s_.clear();
    }

    template <typename E>
    const std::vector<E>&
    set_of_<E>::vec() const
    {
      return v_;
    }

    template <typename E>
    void
    set_of_<E>::update_()
    {
      v_.clear();
      std::copy(s_.begin(), s_.end(), std::back_inserter(v_));
    }

    template <typename E>
    std::ostream& operator<<(std::ostream& ostr,
			     const set_of_<E>& s)
    {
      ostr << '[';
      for (unsigned i = 0; i < s.nelements(); ++i)
	ostr << s.element(i)
	     << (i == s.nelements() - 1 ? ']' : ',');
      return ostr;
    }

# endif // ! MLN_INCLUDE_ONLY

  } // end of namespace mln::internal

} // end of namespace mln


#endif // ! MLN_CORE_INTERNAL_SET_OF_HH
