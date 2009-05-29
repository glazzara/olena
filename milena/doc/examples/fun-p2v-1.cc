#include <mln/core/alias/point2d.hh>
// \{
unsigned my_values(const mln::point2d& p)
{
  if (p.row() == 0)
    return 8;
  return 9;
}
// \}

int main()
{
}
