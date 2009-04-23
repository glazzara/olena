#ifndef VECTOR_HH
# define VECTOR_HH

# include <utility>

typedef enum orient
{
  UP,
  UP_L,
  RIGHT,
  DOWN_R,
  DOWN,
  DOWN_L,
  LEFT,
  UP_L
} e_orient;

template<typename T>
class Vector
{
  public:
    Vector(T& a);
    void restart_from(T& a, T& b);

  private:
    std::pair<T, T> vect_;
    int nb_point;
    e_orient orient_;
    const double delta_;
};

# include "vector.hxx"

#endif /* !VECTOR_HH */
