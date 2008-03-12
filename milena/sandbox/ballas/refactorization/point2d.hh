#ifndef POINT2D_HH_
# define POINT2D_HH_

# include <concept.hh>
# include <vector>
# include <point2d_impl.hh>
# include <iostream>

namespace mln
{
  /// fwd declaration
  template <typename C> struct point2d;

  /// Dpoint2d
  template <typename C>
  struct dpoint2d : public Dsite< dpoint2d<C> >
  {
    enum { dim = 2 };
    typedef point2d<C> point;
    typedef C coord;

    C  operator[](unsigned i) const;
    C& operator[](unsigned i);

    dpoint2d();
    dpoint2d(C row, C col);

    void set_all(C c);

  protected:
    std::vector<C> coord_;
  };

  /// Point2d
  template <typename C>
  struct point2d : public Site< point2d<C> >
  {

    enum { dim = 2 };
    typedef dpoint2d<C> dpoint;
    typedef C coord;


    C  operator[](unsigned i) const;
    C& operator[](unsigned i);

    point2d();
    point2d(C row, C col);

    void set_all(C c);

    point2d<C>& operator+=(const dpoint& dp);
    point2d<C>& operator-=(const dpoint& dp);

  protected:
    std::vector<C> coord_;
  };

  template <typename C>
  inline
  bool operator==(const point2d<C>& lhs_, const point2d<C>& rhs_);

  template <typename C>
  inline
  bool operator!=(const point2d<C>& lhs, const point2d<C>& rhs);


# ifndef MLN_INCLUDE_ONLY

  /// Dpoint2d
  template <typename C>
  C
  dpoint2d<C>::operator[](unsigned i) const
  {
    assert(i < 2);
    return coord_[i];
  }

  template <typename C>
  C&
  dpoint2d<C>::operator[](unsigned i)
  {
    assert(i < 2);
    return coord_[i];
  }


  template <typename C>
  dpoint2d<C>::dpoint2d() :
    coord_(2, 0)
  {
  }

  template <typename C>
  dpoint2d<C>::dpoint2d(C row, C col) :
    coord_(0)
  {
    coord_[0] = row;
    coord_[1] = col;
  }

  template <typename C>
  void
  dpoint2d<C>::set_all(C c)
  {
    coord_[0] = c;
    coord_[1] = c;
  }


  /// Point2d
  template <typename C>
  inline
  C point2d<C>::operator[](unsigned i) const
  {
    assert(i < dim);
    return this->coord_[i];
  }

  template <typename C>
  inline
  C& point2d<C>::operator[](unsigned i)
  {
    assert(i < dim);
    return this->coord_[i];
  }

  // Constructors.
  template <typename C>
  inline
  point2d<C>::point2d() :
    coord_(2, 0)
  {
  }

  template <typename C>
  inline
  point2d<C>::point2d(C row, C col) :
    coord_(2)
  {
    coord_[0] = row;
    coord_[1] = col;
  }

  template <typename C>
  inline
  void point2d<C>::set_all(C c)
  {
    coord_[0] = c;
    coord_[1] = c;
  }

  template <typename C>
  inline
  point2d<C>&
  point2d<C>::operator+=(const dpoint& dp)
  {
    for (unsigned i = 0; i < dim; ++i)
      coord_[i] += dp[i];
    return *this;
  }

  template <typename C>
  inline
  point2d<C>&
  point2d<C>::operator-=(const dpoint& dp)
  {
    for (unsigned i = 0; i < dim; ++i)
      coord_[i] -= dp[i];
    return *this;
  }


  template <typename C>
  inline
  bool operator==(const point2d<C>& lhs, const point2d<C>& rhs)
  {
    for (unsigned i = 0; i < 2; ++i)
      if (lhs[i] != rhs[i])
        return false;
    return true;
  }

  template <typename C>
  inline
  bool operator!=(const point2d<C>& lhs, const point2d<C>& rhs)
  {
    return !(lhs == rhs);
  }

  template <typename C>
  std::ostream& operator<<(std::ostream& ostr, const point2d<C>& p)
  {
    return ostr << "(" << p[0] << "," << p[1] << ")";
  }

# endif // ! MLN_INCLUDE_ONLY

}

#endif // !POINT2D_HH_
