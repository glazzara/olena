# include <mln/essential/2d.hh>
int main()
{
  using namespace mln;

  // \{
  bool vals[5][5] = {
    {1, 0, 1, 0, 0},
    {0, 1, 0, 1, 0},
    {1, 0, 1, 0, 0},
    {0, 1, 0, 1, 0},
    {0, 1, 0, 1, 0}
  };
  image2d<bool> ima = make::image(vals);

  // ...

  io::pbm::save(ima, "../figures/ima_save.pbm");
  // \}
}
