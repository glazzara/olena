# include <mln/core/image/image2d.hh>
# include <mln/util/timer.hh>

# include <mln/core/pixter2d.hh>
# include <mln/opt/at.hh>


int size;


float a_la_C()
{
  unsigned len = size * size;
  int* buf = new int[len];

  mln::util::timer t;
  t.start();
  int* p = buf;
  for (unsigned i = 0; i < len; ++i)
    *p++ = 0;
  return t.read();
}


float loops()
{
  typedef mln::image2d<int> I;
  mln::internal::data<I> data(mln::make::box2d(size, size), 1);

  mln::util::timer t;
  t.start();
  int** array = data.array_;
  for (int row = 0; row < size; ++row)
    for (int col = 0; col < size; ++col)
      array[row][col] = 0;
  return t.read();
}


float for_at()
{
  typedef mln::image2d<int> I;
  I ima(size, size);

  mln::util::timer t;
  t.start();
  for (int row = 0; row < size; ++row)
    for (int col = 0; col < size; ++col)
      mln::opt::at(ima, row, col) = 0;
  return t.read();
}


float loops_ref()
{
  typedef mln::image2d<int> I;
  mln::internal::data<I> data(mln::make::box2d(size, size), 1);

  mln::util::timer t;
  t.start();

  int** array = data.array_;
  mln::point2d p;
  mln::def::coord & row = p.row(), & col = p.col();
  for (row = 0; row < size; ++row)
    for (col = 0; col < size; ++col)
      array[row][col] = 0;

  return t.read();
}


float hybrid_1()
{
  typedef mln::image2d<int> I;
  I ima(size, size);

  mln::util::timer t;
  t.start();
  mln::point2d p;
  mln::def::coord & row = p.row(), & col = p.col();
  for (row = 0; row < size; ++row)
    for (col = 0; col < size; ++col)
      mln::opt::at(ima, row, col) = 0;
  return t.read();
}



float milena()
{
  typedef mln::image2d<int> I;
  I ima(size, size);

  mln::util::timer t;
  t.start();
  mln_piter_(I) p(ima.domain());
  for_all(p)
    ima(p) = 0;
  return t.read();
}


float optim()
{
  typedef mln::image2d<int> I;
  I ima(size, size);

  mln::util::timer t;
  t.start();
  mln_piter_(I) p(ima.domain());
  for (p.start(); p.is_valid(); p.next())
    ima.alt(p) = 0;
  return t.read();
}


float fast()
{
  typedef mln::image2d<int> I;
  I ima(size, size);

  mln::util::timer t;
  t.start();
  mln_pixter_(I) p(ima);
  for_all(p)
    p.val() = 0;
  return t.read();
}


void usage(char* argv[])
{
  std::cerr << "usage: " << argv[0] << " size" << std::endl;
  abort();
}


int main(int argc, char* argv[])
{
  if (argc != 2)
    usage(argv);
  size = std::atoi(argv[1]);

  std::cout << "*p++         " << a_la_C()   << std::endl;
  std::cout << "for (r,c)    " << loops()    << std::endl;
  std::cout << "for at(r,c)  " << for_at()   << std::endl;
  std::cout << "for &(r,c)   " << loops_ref()  << std::endl;
  std::cout << "hybrid_1     " << hybrid_1() << std::endl;
  std::cout << "for_all(p)   " << milena()   << std::endl;
  std::cout << "optim        " << optim()    << std::endl;
  std::cout << "for_all(pix) " << fast()   << std::endl;
}
