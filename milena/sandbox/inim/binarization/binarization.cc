#include <iostream>
#include <string>
#include <mln/io/pgm/all.hh>
#include <mln/win/rectangle2d.hh>
#include <mln/value/all.hh>
#include <mln/core/image/image2d.hh>
#include "binarization.hh"


int main(int argc, char** argv)
{
  if (argc != 5)
  {
    std::cerr << argv[0] << " in out strength size" << std::endl;
    return 42;
  }
  std::string in(argv[1]);
  std::string out(argv[2]);
  std::istringstream is(argv[3]);
  int strength;
  is >> strength;
  std::istringstream iss(argv[4]);
  int size;
  iss >> size;

  mln::image2d<mln::value::int_u8> input;
  mln::image2d<mln::value::int_u8> to_print;
  mln::io::pgm::load(input, in);
  mln::image2d<bool> output = mln::binarization::binarization(input, mln::win::rectangle2d(size,size), strength);
  initialize(to_print, output);
  mln::image2d<mln::value::int_u8>::fwd_piter p(input.domain());
  for_all (p)
    to_print(p) = output(p) * 255;
  mln::io::pgm::save(to_print, out);
  return 0;
}
