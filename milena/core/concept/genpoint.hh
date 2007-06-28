#ifndef MLN_CORE_CONCEPT_GENPOINT_HH
# define MLN_CORE_CONCEPT_GENPOINT_HH

# include <core/concept/object.hh>
# include <core/macros.hh>
# include <mlc/equal.hh>
# include <mlc/same_point.hh>


namespace mln
{

  template <typename E>
  struct GenPoint // stand-alone class!
  {
    /*
      typedef point;
      typedef dpoint;
      typedef coord;
      typedef topo; // FIXME

      either Point
      or operator point() const;

      const point* pointer() const;
      coord operator[](unsigned i) const;
    */

  protected:
    GenPoint();
  };


  template <typename Pl, typename Pr>
  bool operator==(const GenPoint<Pl>& lhs, const GenPoint<Pr>& rhs);

  template <typename Pl, typename Pr>
  bool operator<(const GenPoint<Pl>& lhs, const GenPoint<Pr>& rhs);

  template <typename Pl, typename Pr>
  mln_dpoint(Pl)
  operator-(const GenPoint<Pl>& lhs, const GenPoint<Pr>& rhs);

  template <typename P>
  mln_point(P)
  operator+(const GenPoint<P>& lhs, const mln_dpoint(P)& rhs);

  template <typename P>
  std::ostream& operator<<(std::ostream& ostr, const GenPoint<P>& p);


# ifndef MLN_INCLUDE_ONLY

  template <typename E>
  GenPoint<E>::GenPoint()
  {
    typedef  mln_point(E)  point;
    typedef mln_dpoint(E) dpoint;
    typedef mln_coord(E)  coord;
    const point* (E::*m1)() const = & E::pointer;
    m1 = 0;
    coord (E::*m2)(unsigned i) const = & E::operator[];
    m2 = 0;
  }

  template <typename Pl, typename Pr>
  bool operator==(const GenPoint<Pl>& lhs, const GenPoint<Pr>& rhs)
  {
    const Pl& lhs_ = force_exact<Pl>(lhs);
    const Pr& rhs_ = force_exact<Pr>(rhs);
    mlc::same_point<Pl, Pr>::check();
    for (unsigned i = 0; i < Pl::dim; ++i)
      if (lhs_[i] != rhs_[i])
	return false;
    return true;
  }

  template <typename Pl, typename Pr>
  bool operator<(const GenPoint<Pl>& lhs, const GenPoint<Pr>& rhs)
  {
    mlc::same_point<Pl, Pr>::check();
    const Pl& lhs_ = force_exact<Pl>(lhs);
    const Pr& rhs_ = force_exact<Pr>(rhs);
    for (unsigned i = 0; i < Pl::dim; ++i)
      {
	if (lhs_[i] == rhs_[i])
	  continue;
	return lhs_[i] < rhs_[i];
      }
    return false;
  }


  template <typename Pl, typename Pr>
  mln_dpoint(Pl)
  operator-(const GenPoint<Pl>& lhs, const GenPoint<Pr>& rhs)
  {
    const Pl& lhs_ = force_exact<Pl>(lhs);
    const Pr& rhs_ = force_exact<Pr>(rhs);
    mln_dpoint(Pl) tmp;
    for (unsigned i = 0; i < Pl::dim; ++i)
      tmp[i] = lhs_[i] - rhs_[i];
    return tmp;
  }


  template <typename P>
  mln_point(P)
  operator+(const GenPoint<P>& lhs, const mln_dpoint(P)& rhs)
  {
    const P& lhs_ = force_exact<P>(lhs);
    mln_point(P) tmp;
    for (unsigned i = 0; i < P::dim; ++i)
      tmp[i] = lhs_[i] + rhs[i];
    return tmp;
  }

  template <typename P>
  std::ostream& operator<<(std::ostream& ostr, const GenPoint<P>& p)
  {
    const P& p_ = force_exact<P>(p);
    ostr << '(';
    for (unsigned i = 0; i < P::dim; ++i)
      {
	ostr << p_[i]
	     << (i == P::dim - 1 ? ')' : ',');
      }
    return ostr;
  }

# endif // ! MLN_INCLUDE_ONLY

} // end of namespace mln


#endif // ! MLN_CORE_CONCEPT_GENPOINT_HH
