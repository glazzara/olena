#ifndef CUTTOR_HXX
# define CUTTOR_HXX

# include "cuttor.hh"
# include <mln/linear/gaussian.hh>
# include <mln/arith/revert.hh>
# include <mln/morpho/watershed/flooding.hh>
# include <mln/morpho/watershed/superpose.hh>
# include <mln/core/alias/neighb2d.hh>
# include <mln/morpho/opening/structural.hh>
# include <mln/win/rectangle2d.hh>

template<typename Value>
Cuttor<Value>::Cuttor (std::string filepath)
{
  // Load file
  mln::io::pgm::load (img_, filepath);

  int pos_slash = filepath.find_last_of ("/");
  int pos_dot = filepath.find_last_of (".");

  if (pos_slash == -1)
    filedir_ = "./";
  else
    filedir_ = filepath.substr (0, pos_slash + 1);

  // Extracting filename from filepath
  filename_ = filepath.substr (pos_slash + 1, pos_dot - pos_slash - 1);

  umask(022);
  // Creating result subdir of file treatment
  mkdir (std::string ((filename_ + "_result")).c_str (), 0766);
}

template<typename Value>
void
Cuttor<Value>::start ()
{
  save(img_, filename_, "_orig");
  mln::image2d<Value> blur = mln::linear::gaussian(mln::linear::gaussian (img_, 20, 1),
						   5, 0);
  save (blur, filename_, "_blur");
  mln::image2d<Value> open =
    mln::morpho::opening::structural (blur, mln::win::rectangle2d (5, 21));
  save (open, filename_, "_open");
  Value n;
  mln::image2d<Value> water = mln::morpho::watershed::flooding (open, mln::c4 (), n);
  save (water, filename_, "_water");
  mln::image2d<Value> water_open =
    mln::morpho::opening::structural (water, mln::win::rectangle2d (3, 3));
  save (water_open, filename_, "_water_open");
  mln::image2d<mln::value::rgb8> super =
    mln::morpho::watershed::superpose (img_, water_open);
  save (super, filename_, "_super");
}

template<typename Value>
void
save (mln::image2d<Value>& ima, std::string filename, std::string postfix)
{
  std::string name (filename +  "_result/"
                    + filename + postfix + ".pgm");

  std::cout << ">>>>> Saving " << name << std::endl;
  mln::io::pgm::save (ima, name);
}

template<>
void
save (mln::image2d<mln::value::rgb8>& ima, std::string filename, std::string postfix)
{
  std::string name (filename +  "_result/"
                    + filename + postfix + ".ppm");

  std::cout << ">>>>> Saving " << name << std::endl;
  mln::io::ppm::save (ima, name);
}

#endif /* !CUTTOR_HXX */
