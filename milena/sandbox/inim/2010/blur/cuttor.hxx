#ifndef CUTTOR_HXX
# define CUTTOR_HXX

# include <mln/io/pgm/load.hh>
# include <mln/io/pgm/save.hh>
# include <mln/io/ppm/save.hh>
# include <mln/linear/gaussian.hh>
# include <mln/arith/revert.hh>
# include <mln/morpho/watershed/flooding.hh>
# include <mln/morpho/watershed/superpose.hh>
# include <mln/morpho/opening/structural.hh>

# include <mln/core/alias/neighb2d.hh>
# include <mln/core/alias/point2d.hh>

# include <mln/win/rectangle2d.hh>
# include "cuttor.hh"
# include "advance_iterator.hh"
# include "vect_it.hh"

template<typename Value>
Cuttor<Value>::Cuttor (std::string filepath)
  : last_entry_ (-1)
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
  // Blur the image to loose useless information
  mln::image2d<Value> blur =
    mln::linear::gaussian(mln::linear::gaussian (img_, 20, 1), 5, 0);
  save (blur, filename_, "_blur");

  // Math Morph to fill the holes who still exists in the blured image
  mln::image2d<Value> open =
    mln::morpho::opening::structural (blur, mln::win::rectangle2d (5, 21));
  save (open, filename_, "_open");

  // Apply a watershed to determine the image minimas
  Value n;
  mln::image2d<Value> water =
    mln::morpho::watershed::flooding (open, mln::c4 (), n);
  save (water, filename_, "_water");

  // Used to unify the watershed lines
  mln::image2d<Value> water_open =
    mln::morpho::opening::structural (water, mln::win::rectangle2d (3, 3));
  save (water_open, filename_, "_water_open");

  lined_ = mln::image2d<Value>(img_.domain());
  mln::data::fill (lined_, 1);
  // Find the horizontal line separators
  for (mln::point2d p = find_entry_point(water_open);
       p[1] >= 0; p = find_entry_point(water_open))
  {
    std::cout << "Entry point " << p << std::endl;
    find_line (water_open, p);
    std::cout << "Line found." << std::endl;
  }
  save (lined_, filename_, "_lines");

  // Superpose the watersheded image to the base image.
  mln::image2d<mln::value::rgb8> super =
    mln::morpho::watershed::superpose (img_, lined_);
  save (super, filename_, "_super");
}

template <typename Value>
mln::point2d
find_unique_predecessor (mln::image2d<Value> ima, mln::point2d p)
{
  static mln::dpoint2d dp[] =
  {
    mln::dpoint2d (0, -1),
    mln::dpoint2d (1, -1),
    mln::dpoint2d (-1, -1),
    mln::dpoint2d (1, 0),
    mln::dpoint2d (-1, 0)
  };
  int i;
  for (i = 0; i < 3 && ima(p + dp[i]) == 0u; ++i)
    ;
  if (i == 5)
    return mln::point2d (-1, -1);
  for (; i < 5; ++i)
    if (ima(p + dp[i]) == 0u)
      return mln::point2d (-1, -1);
  return p + dp[i];
}

template <typename Value>
bool
Cuttor<Value>::find_line (mln::image2d<Value>& water, mln::point2d p)
{
  AdvanceIterator<Value> it (water, lined_, p);

  std::cout << "Count: " << it.count() << " at " << p << std::endl;
  // First we advance to the next "intersection"
  while (it.count() == 1 && water.has(*it))
  {
    std::cout << "Advancing " << (*it) << std::endl;
    // Only one pixel at the right: we mark it and we continue
    lined_(it.center()) = 0;
    it.recenter (*it);
  }

  if (!lined_.has(*it))
    // Right edge reached !
    return true;
  else if (it.count() > 1)
  {
    // Several paths are possible
    for_all (it)
    {
      std::cout << "Trying " << *it << std::endl;
      if (find_line (water, *it))
      {
	lined_(it.center()) = 0;
	return true;
      }
    }
    // Absolutely no path to the right.
    // Default behavior is to draw a straight line to the right.
    // (This case only happens when reaching the white block on the right
    // of the document, so it shouldn't bring any problem)
    for (mln::point2d p = it.center (); lined_.has(p); ++p[1])
      lined_(p) = 0;
    return true;
  }
  else // if (it.count() == 0)
  {
//     std::cout << "Backtrack" << std::endl;
//     // Dead end, we must backtrack
//     for (mln::point2d back = find_unique_predecessor (lined_, it.center());
// 	 back[1] >= 0;
// 	 back = find_unique_predecessor (lined_, it.center()))
//     {
//       lined_(it.center()) = 1;
//       it.recenter (mln::point2d(back[1], it.center()[1] - 1));
//     }
//     lined_(it.center()) = 1;
    return false;
  }
}

template<typename Value>
mln::point2d
Cuttor<Value>::find_entry_point(mln::image2d<Value>& ima)
{
  for (unsigned int i = last_entry_ + 1; i < ima.nrows(); ++i)
    if (ima.at_(i, 0) == 0u)
    {
      last_entry_ = i;
      return mln::point2d(last_entry_, 0);
    }

  return mln::point2d(-1,-1);;
}

///////////////// VECTOR STYLE FINDER

template <typename Value>
void
Cuttor<Value>::find_vector_line (mln::image2d<Value>& from,
                                 mln::image2d<Value>& to,
                                 mln::point2d entry)
{
  AdvanceIterator<Value> it (from, entry);
  Vector<mln::point2d> current(entry);
  Vector<mln::point2d> previous(entry);

  while (it.has_value ())
  {
    for_all (it)
    {

    }

    it.reinit (*it);
  }
}

///////////////// END OF VECTOR STYLE FINDER

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
