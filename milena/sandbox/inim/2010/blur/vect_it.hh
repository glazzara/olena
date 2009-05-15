#ifndef VECT_IT_HH
# define VECT_IT_HH

# define MLN_INCLUDE_ONLY
# include <mln/core/alias/point2d.hh>
# include <mln/core/alias/dpoint2d.hh>
# include <mln/core/image/image2d.hh>
# undef MLN_INCLUDE_ONLY
# include <list>
# include "vector.hh"

template <typename Value>
class VectorIterator
{
  public:
    VectorIterator (mln::image2d<Value>& water,
                    mln::point2d p,
                    int orient);
    unsigned count ();
    void start ();
    bool is_valid ();
    bool has_point ();
    void next ();
    void reinit (mln::point2d p,
                 int orient);
    int orient ();
    mln::point2d origin();
    mln::point2d operator*();
    mln::point2d operator->();
  private:
    typedef std::pair<mln::point2d, int> pair_type_t;
    typedef std::vector<pair_type_t> vector_type_t;
    mln::point2d origin_;
    vector_type_t points_;
    vector_type_t::const_iterator current_;
    mln::image2d<Value>& from_;

    static mln::dpoint2d dpoints[8];
};

# include "vect_it_OLD.hxx"

#endif /* !VECT_IT_HH */
