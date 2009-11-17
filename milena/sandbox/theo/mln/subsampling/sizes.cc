#include <iostream>
#include <cstdlib>


void usage(const char* argv[])
{
  std::cerr << argv[0] << " n_scales s q  nr nc" << std::endl;
  std::abort();
}


unsigned sub(unsigned nbr, unsigned down_scaling)
{
  return (nbr + down_scaling - 1) / down_scaling;
}


int main(int argc, const char* argv[])
{
  unsigned n_scales = std::atoi(argv[1]);
  unsigned s = std::atoi(argv[2]);
  unsigned q = std::atoi(argv[3]);

  unsigned nr_1 = std::atoi(argv[4]);
  unsigned nc_1 = std::atoi(argv[5]);
  
  unsigned* nc;
  nc = new unsigned[n_scales + 1];

  nc[1] = nc_1;
  nc[2] = sub(nc[1], s);
  for (unsigned i = 3; i <= n_scales; ++i)
    nc[i] = sub(nc[i - 1], q);

  for (unsigned i = 1; i <= n_scales; ++i)
    std::cout << nc[i] << ' ';
  std::cout << std::endl;

  for (unsigned i = n_scales; i > 2; --i)
    nc[i - 1] = q * nc[i];

  for (unsigned i = 1; i <= n_scales; ++i)
    std::cout << nc[i] << ' ';
  std::cout << std::endl;
}
