# include <mln/core/io/pbm/load.hh>
int main()
{
  mln::image2d<bool> ima;
  io::pbm::load(ima, "my_image.pbm");
}
