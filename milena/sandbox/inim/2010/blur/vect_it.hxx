#ifndef VECT_IT_HXX
# define VECT_IT_HXX

# include "vect_it.hh"

template <typename Value>
mln::dpoint2d VectorIterator<Value>::dpoints[8] =
{
  mln::dpoint2d (-1, 0), // UP
  mln::dpoint2d (-1, 1), // UP_R
  mln::dpoint2d (0, 1),  // RIGHT
  mln::dpoint2d (1, 1),  // DOWN_R
  mln::dpoint2d (1, 0),  // DOWN
  mln::dpoint2d (1, -1), // DOWN_L
  mln::dpoint2d (0, -1), // LEFT
  mln::dpoint2d (-1, -1) // UP_L
};

template <typename Value>
VectorIterator<Value>::VectorIterator (mln::image2d<Value>& water,
                                       mln::point2d p,
                                       e_orient orient)
  : origin_ (p),
    from_ (water)
{
  points_.reserve (5);
  this->reinit (p, orient);
}

template <typename Value>
unsigned
VectorIterator<Value>::count ()
{
  return points_.size ();
}

template <typename Value>
void
VectorIterator<Value>::reinit (mln::point2d p, e_orient orient)
{
  points_.clear ();
  start();

  if (0u == from_ (p + dpoints[(orient % 8)]))
    points_.push_back (pair_type_t(p + dpoints[orient % 8], orient));

  if (0u == from_ (p + dpoints[(orient + 1) % 8]))
    points_.push_back (pair_type_t(p + dpoints[(orient + 1) % 8],
                                   (e_orient)((orient + 1) % 8)));

  if (0u == from_ (p + dpoints[(orient - 1)% 8]))
    points_.push_back (pair_type_t(p + dpoints[(orient - 1) % 8],
                                   (e_orient)((orient - 1) % 8)));

  if (0u == from_ (p + dpoints[(orient + 2) % 8]))
    points_.push_back (pair_type_t(p + dpoints[(orient + 2) % 8],
                                   (e_orient)((orient + 2) % 8)));

  if (0u == from_ (p + dpoints[(orient - 2) % 8]))
    points_.push_back (pair_type_t(p + dpoints[(orient - 2) % 8],
                                   (e_orient)((orient - 2) % 8)));
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
  return has_point () && from_.has (*current_);
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
  return current_->first;
}

template <typename Value>
mln::point2d
VectorIterator<Value>::operator-> ()
{
  return current_->first;
}

template <typename Value>
e_orient
VectorIterator<Value>::orient ()
{
  return current_->second;
}

#endif /* !VECT_IT_HXX */
