
template <typename E>
struct base
{
  typedef const E& box;
  typedef const E& box_;
  box bbox() const { return *(E*)(void*)(this); }
};


struct box : base<box>
{
  box() {}
};


template <typename S>
void foo(const S&)
{
  typename S::box b;
//   void* v = b;
  typename S::box_ b_;
}

int main()
{
  box b;
  foo(b);
}
