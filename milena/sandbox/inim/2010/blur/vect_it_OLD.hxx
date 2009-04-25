#ifndef VECT_IT_HXX
# define VECT_IT_HXX

# include "vect_it.hh"

int mod(int x, int y);

template <typename Value>
mln::dpoint2d VectorIterator<Value>::dpoints[8] =
{
  mln::dpoint2d (-1, 0), // 0 UP
  mln::dpoint2d (-1, 1), // 1 UP_R
  mln::dpoint2d (0, 1),  // 2 RIGHT
  mln::dpoint2d (1, 1),  // 3 DOWN_R
  mln::dpoint2d (1, 0),  // 4 DOWN
  mln::dpoint2d (1, -1), // 5 DOWN_L
  mln::dpoint2d (0, -1), // 6 LEFT
  mln::dpoint2d (-1, -1) // 7 UP_L
};

template <typename Value>
VectorIterator<Value>::VectorIterator (mln::image2d<Value>& water,
                                       mln::point2d p,
                                       int orient)
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
VectorIterator<Value>::reinit (mln::point2d p, int orient)
{
  points_.clear ();
  origin_ = p;

  if (0u == from_ (p + dpoints[mod(orient, 8)]))
  {
//    std::cout << "**** 0 PUSH " << p + dpoints[mod(orient, 8)] << mod(orient, 8)<< std::endl;
    points_.push_back (pair_type_t(p + dpoints[mod(orient, 8)], mod(orient, 8)));
  }
  if (0u == from_ (p + dpoints[mod(orient + 1, 8)]))
  {
//    std::cout << "**** 1 PUSH " << p + dpoints[mod(orient + 1, 8)] << mod(orient + 1, 8)<<std::endl;
    points_.push_back (pair_type_t(p + dpoints[mod(orient + 1, 8)],
                                   mod(orient + 1, 8)));
  }
  if (0u == from_ (p + dpoints[mod(orient - 1, 8)]))
  {
//    std::cout << "**** 2 PUSH " << p + dpoints[mod(orient - 1, 8)] << mod(orient -1, 8)<< std::endl;
    points_.push_back (pair_type_t(p + dpoints[mod(orient - 1, 8)],
                                   mod(orient - 1, 8)));
  }
//   if (0u == from_ (p + dpoints[mod(orient + 2, 8)]))
//   {
// //    std::cout << "**** 3 PUSH " << p + dpoints[mod(orient + 2, 8)] << mod(orient + 2, 8)<< std::endl;
//     points_.push_back (pair_type_t(p + dpoints[mod(orient + 2, 8)],
//                                    mod(orient + 2, 8)));
//   }
//   if (0u == from_ (p + dpoints[mod(orient - 2, 8)]))
//   {
// //    std::cout << "**** 4 PUSH " << p + dpoints[mod(orient - 2, 8)] << mod(orient - 2, 8)<< std::endl;
//     points_.push_back (pair_type_t(p + dpoints[mod(orient - 2, 8)],
//                                    mod(orient - 2, 8)));
//   }

  start();
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
  return has_point () && from_.has (current_->first);
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
VectorIterator<Value>::origin ()
{
  return origin_;
}

template <typename Value>
mln::point2d
VectorIterator<Value>::operator-> ()
{
  return current_->first;
}

template <typename Value>
int
VectorIterator<Value>::orient ()
{
  return current_->second;
}



int
mod(int x, int y)
{
  int t = x - y * floor(x/y);

  if (t < 0)
    return t + y;
  return t;
}

#endif /* !VECT_IT_HXX */
