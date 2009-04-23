#ifndef VECT_IT_HXX
# define VECT_IT_HXX

# include "vect_it.hh"

template <typename Value>
mln::dpoint2d* VectorIterator<Value>::dpoints =
{
  mln::dpoint2d (0, -1),
  mln::dpoint2d (1, -1),
  mln::dpoint2d (1, 0),
  mln::dpoint2d (1, 1),
  mln::dpoint2d (0, 1),
  mln::dpoint2d (-1, 1),
  mln::dpoint2d (-1, 0),
  mln::dpoint2d (-1, -1)
};

template <typename Value>
VectorIterator<Value>::VectorIterator (mln::image2d<Value>& water,
                                       mln::point2d p,
                                       e_orient orient)
  : origin_ (p),
    water_ (water)
{
  this->reinit (p, orient);
}

template <typename Value>
void
VectorIterator<Value>::reinit (mln::point2d p, e_orient orient)
{
  points_.clear ();
  origin_ = current_ = p;

  if (0 == water_ (p + dpoints[(orient % 8)]))
    points_.push_front ( p + dpoints[orient % 8]);

  if (0 == water_ (p + dpoints[(orient + 1) % 8]))
    points_.push_front ( p + dpoints[(orient + 1) % 8]);

  if (0 == water_ (p + dpoints[(orient - 1)% 8]))
    points_.push_front ( p + dpoints[(orient - 1) % 8]);

  if (0 == water_ (p + dpoints[(orient + 2) % 8]))
    points_.push_front ( p + dpoints[(orient + 2) % 8]);

  if (0 == water_ (p + dpoints[(orient - 2) % 8]))
    points_.push_front ( p + dpoints[(orient - 2) % 8]);
}

template <typename Value>
bool
VectorIterator<Value>::has_point ()
{
  return current_ != points_.end ();
}

template <typename Value>
void
VectorIterator<Value>::start ()
{
  current_ = points_.begin ();
}

template <typename Value>
bool
VectorIterator<Value>::is_valid ()
{
  return has_point () && water_.has (*current_);
}

template <typename Value>
void
VectorIterator<Value>::next ()
{
  ++current_;
}

template <typename Value>
mln::point2d
VectorIterator<Value>::operator* ()
{
  return *current_;
}

template <typename Value>
mln::point2d
VectorIterator<Value>::operator-> ()
{
  return *current_;
}

#endif /* !VECT_IT_HXX */
