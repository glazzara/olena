#include <oln/basics2d.hh>
#include <oln/debug/print.hh>

#include <oln/level/apply.hh>
#include <oln/level/assign.hh>
#include <oln/level/clone.hh>
#include <oln/level/fill.hh>



struct chessboard_2d : public oln::abstract::fun_p2v< chessboard_2d >
{
  bool operator()(const oln::point2d& p) const
  {
    return (p.row() + p.col()) % 2;
  }
};


struct negate : public oln::abstract::fun_v2v< negate >
{
  bool operator()(bool b) const
  {
    return not b;
  }
};



int main()
{
  using namespace oln;
  image2d<bool> ima(3,3);

  oln::level::fill(ima, chessboard_2d());
  debug::print(ima);

  oln::level::apply_inplace(ima, negate());
  debug::print(ima);
}
