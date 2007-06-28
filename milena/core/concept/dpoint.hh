#ifndef MLN_CORE_CONCEPT_DPOINT_HH
# define MLN_CORE_CONCEPT_DPOINT_HH

# include <core/concept/object.hh>


namespace mln
{


  template <typename E>
  struct Dpoint : public Object<E>
  {
    /*
      typedef point;
      typedef coord;
      enum { dim };
      coord operator[](unsigned i) const;
    */

  protected:
    Dpoint();
  };


  template <typename D>
  D operator-(const Dpoint<D>& dp);

  template <typename Dl, typename Dr>
  bool operator==(const Dpoint<Dl>& lhs, const Dpoint<Dr>& rhs);

  template <typename Dl, typename Dr>
  bool operator<(const Dpoint<Dl>& lhs, const Dpoint<Dr>& rhs);

  template <typename D>
  std::ostream& operator<<(std::ostream& ostr, const Dpoint<D>& dp);



# ifndef MLN_INCLUDE_ONLY

  template <typename E>
  Dpoint<E>::Dpoint()
  {
    typedef mln_point(E) point;
    typedef mln_coord(E) coord;
    enum { dim = E::dim };
    coord (E::*m)(unsigned i) const = & E::operator[];
    m = 0;
  }

  template <typename D>
  D operator-(const Dpoint<D>& dp)
  {
    D tmp; 
    for (unsigned i = 0; i < D::dim; ++i)
      tmp[i] = - exact(dp)[i];
    return tmp;
  }

  template <typename Dl, typename Dr>
  bool operator==(const Dpoint<Dl>& lhs, const Dpoint<Dr>& rhs)
  {
    for (unsigned i = 0; i < Dl::dim; ++i)
      if (exact(lhs)[i] != exact(rhs)[i])
	return false;
    return true;
  }

  template <typename Dl, typename Dr>
  bool operator<(const Dpoint<Dl>& lhs, const Dpoint<Dr>& rhs)
  {
    for (unsigned i = 0; i < Dl::dim; ++i)
      {
	if (exact(lhs)[i] == exact(rhs)[i])
	  continue;
	return exact(lhs)[i] < exact(rhs)[i];
      }
    return false;
  }

  template <typename D>
  std::ostream& operator<<(std::ostream& ostr, const Dpoint<D>& dp)
  {
    ostr << '(';
    for (unsigned i = 0; i < D::dim; ++i)
      {
	ostr << exact(dp)[i]
	     << (i == D::dim - 1 ? ')' : ',');
      }
    return ostr;
  }

# endif // ! MLN_INCLUDE_ONLY

} // end of namespace mln


#endif // ! MLN_CORE_CONCEPT_DPOINT_HH
