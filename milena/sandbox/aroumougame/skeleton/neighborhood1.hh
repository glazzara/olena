#include <mln/core/alias/neighb1d.hh>
#include <mln/core/alias/neighb2d.hh>
#include <mln/core/alias/neighb3d.hh>

using namespace mln;


const neighb1d& max_neighborhood(const Neighborhood<neighb1d>& nbh)
{
  return c2();
}

const neighb2d& max_neighborhood(const Neighborhood<neighb2d>& nbh)
{
  return c8();
}

const neighb3d& max_neighborhood(const Neighborhood<neighb3d>& nbh)
{
  return c26();
}

const neighb2d& complement_neighborhood(const Neighborhood<neighb2d>& nbh)
{
  if(&nbh == &c4())
    return c8();
  return c4();
}

const neighb3d& complement_neighborhood(const Neighborhood<neighb3d>& nbh)
{
  if(&nbh == &c6())
    return c18();
  return c6();
}
