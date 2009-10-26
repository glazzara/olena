#ifndef DAMIEN_HH
#define DAMIEN_HH

#include <iostream>

struct up
{
  up(int i) : i_(i) {}
  virtual void print(std::ostream&) const = 0;
  virtual void print_noarg() const = 0;
  virtual up* fake_method(const up&) const = 0;
  virtual ~up() {};
  virtual int get_i() const { return i_; }
  int i_;
};


template <typename T>
struct down : public up
{
  down(const T& t) : up(0), t_(t) {}

  virtual void print(std::ostream& ostr) const
  {
    ostr << "down< " << t_ << " >";
  }

  virtual void print_noarg() const
  {
    std::cout << t_ << std::endl;
  }

  virtual up* fake_method(const up& rhs) const
  {
    return new down<int>(i_ + rhs.get_i());
  }

  const T* get_t() const;
  const down<T>* clone();

  const T& t_;

  virtual ~down() {}
};


std::ostream& operator<<(std::ostream& ostr, const up& obj)
{
  obj.print(ostr);
  return ostr;
}

template <typename T>
void print(const T&);

template <typename T>
T sqr(const T&);

#endif

