#ifndef CORE_CONCEPT_HH_
# define CORE_CONCEPT_HH_

# include <cassert>

/// This file describes all the concepts

namespace mln
{

  /// Object
  template <typename E>
  struct Object
  {
    typedef E exact_t;
  protected:
    Object();
  };

  /// Site concept
  template <typename E>
  struct Site : public Object<E>
  {
    /*
      typedef site
      typedef dsite;
    */
  protected:
    Site();
  };


  template <typename E>
  struct Dsite : public Object<E>
  {
    /*
      typedef dsite;

      const dsite& to_dsite() const;
    */
  protected:
    Dsite();
  };


  /// Psite Concept
  /// Do we need a concept for representing psite?
  template <typename E>
  struct Psite : public Site<E>
  {
    /*
      // Site must be different from psite,
      // if site = psite, then the psite has no sense.
      typedef site;
      typedef psite;

      // Site conversion
      const site& to_site() const;
      operator point() const;


      const psite& ref_();
     */

  protected:
    Psite();
  };

  template <typename E>
  struct Dpsite : public Dsite<E>
  {
    /*
      typedef dpsite;

      const dpsite& to_dpsite() const;
    */
  protected:
    Dpsite();
  };

  /// Piter concept
  /// Psite iterator
  /// use template specializaion for psite == site or psite != psite
  /// to deal with the cast problem
  template <typename E>
  struct Piter : public Object<E>
  {
    /*
      bool is_valid() const;
      void start();
      void next_();

      typedef psite;
      const psite& to_psite() const;
      operator psite() const;

      typedef site;
      const site& to_site() const;
      operator site() const;
     */

    void next();
  protected:
    Piter();
  };

  /// Pset
  /// Psite set
  template <typename E>
  struct Pset : public Object<E>
  {
    /*
      typedef psite;
      typedef fwd_piter;
      typedef bkd_piter;

      bool has(const psite& p) const;
     */
  protected:
    Pset();
  };


  ///FIXME values concepts:....

  /// Image concept:
  template <typename E>
  struct Image : public Object<E>
  {
    /*
      typedef values;
      typedef rvalues;
      typedef lvalues;

      //typedef vset;
      //const vset& destination() const;
      // const vset& values() const;
      bool is_ready();

      typedef site;
      typedef psite;
      rvalue operator() (const psite&) const;
      lvalue operator() (const psite);
      bool owns_(const psite&) const;
      bool has(const psite&) const;

      typedef pset;
      const pset& domain() const;

      typedef fwd_piter;
      typedef bkd_piter;
    */

  protected:
    Image();
  };


# ifndef MLN_INCLUDE_ONLY

  /// Object
  template <typename E>
  inline
  Object<E>::Object()
  {
  }

  /// Site
  template <typename E>
  inline
  Site<E>::Site()
  {
    /// FIXME
  }

  template <typename E>
  inline
  Dsite<E>::Dsite()
  {
    /// FIXME
  }

  template <typename E>
  inline
  Dpsite<E>::Dpsite()
  {
    /// FIXME
  }

  /// Psite
  template <typename E>
  inline
  Psite<E>::Psite()
  {
  }

  /// Piter
  template <typename E>
  inline
  Piter<E>::Piter()
  {
  }

  template <typename E>
  void
  Piter<E>::next()
  {
    exact(this)->next_();
  }

  template <typename E>
  Pset<E>::Pset()
  {
    /// FIXME
  }

  template <typename E>
  Image<E>::Image()
  {
    /// FIXME
  }

# endif // ! MLN_INCLUDE_ONLY

}



#endif // ! CORE_CONCEPT_HH_
