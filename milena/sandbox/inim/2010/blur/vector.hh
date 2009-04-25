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
    Vector(Vector<T>& vect);
    void operator=(Vector<T>& vect);
    void restart_from(T a, int orient);
    bool add_point(T a, int orient);
    double scalar(Vector<T>& vec);
    double angle(Vector<T>& vec);
    double norm();
    void print();
  private:
    std::pair<T, T> vect_;
    int nb_point_;
    double norm_;
    int orient_;
};

# include "vector.hxx"

#endif /* !VECTOR_HH */
