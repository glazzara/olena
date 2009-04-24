#ifndef VECTOR_HH
# define VECTOR_HH

# include <utility>
# define MAX_POINTS 10

typedef enum orient
{
  UP = 0,
  UP_R,
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
    Vector();
    Vector(T a);
    void operator=(Vector<T> vect);
    void restart_from(T a, e_orient orient);
    bool add_point(T a, e_orient orient);
    int scalar(Vector<T> vec);
    double angle(Vector<T> vec);
    double norm();
  private:
    std::pair<T, T> vect_;
    int nb_point_;
    double norm_;
    e_orient orient_;
};

# include "vector.hxx"

#endif /* !VECTOR_HH */
