#ifndef CUTTOR_HXX
# define CUTTOR_HXX

# include <mln/io/pgm/load.hh>
# include <mln/io/pgm/save.hh>
# include <mln/io/ppm/save.hh>
# include <mln/linear/gaussian_directional_2d.hh>
# include <mln/literal/black.hh>
# include <mln/arith/revert.hh>
# include <mln/morpho/watershed/flooding.hh>
# include <mln/morpho/watershed/superpose.hh>
# include <mln/morpho/opening/structural.hh>
# include <mln/labeling/foreground.hh>

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
Cuttor<Value>::start (bool use_vect)
{
  // Blur the image to loose useless information
  mln::image2d<Value> blur =
//    mln::linear::gaussian(mln::linear::gaussian (img_, 20, 1), 5, 0);
    mln::linear::gaussian_directional_2d(mln::linear::gaussian_directional_2d (img_, 1, 20, 0), 0, 5, 0);
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

  lined_ = mln::image2d<Value>(img_.domain());
  mln::data::fill (lined_, 1);
  // Find the horizontal line separators
  for (mln::point2d p = find_entry_point(water);
       p[1] >= 0; p = find_entry_point(water))
  {
    std::cout << "Entry point " << p << std::endl;
    rightmost_ = mln::point2d(-1, -1);
    if (use_vect)
      find_vector_line(water, p);
    else
      find_line (water, p);
    for (; lined_.domain().has(rightmost_); ++rightmost_[1])
      lined_(rightmost_) = 0;
  }

  // Superpose the line separator image to the base image.
  mln::image2d<mln::value::rgb8> super =
    mln::morpho::watershed::superpose (img_, lined_);
  save (super, filename_, "_super");

  // Label the discovered connected components
  int l;
  mln::image2d<int> line_components =
    mln::labeling::level (lined_, 1, mln::c4(), l);
  save (line_components, filename_, "_lines");

  // Finally, the labeled image.
  mln::image2d<Value> final (img_.domain());
  typename mln::image2d<Value>::fwd_piter p (img_.domain());
  for_all (p)
    if (img_(p))
      final(p) = 0;
    else
      final(p) = line_components(p);

  save (final, filename_, "_final");
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

template <typename Value>
bool
Cuttor<Value>::find_line (mln::image2d<Value>& water, mln::point2d p)
{
  AdvanceIterator<Value> it (water, lined_, p);

  lined_(p) = 0;
  std::cout << "Count: " << it.count() << " at " << p << std::endl;
  // First we advance to the next "intersection"
  while (it.count() == 1 && water.domain().has(*it))
  {
    std::cout << "Advancing " << (*it) << std::endl;
    // Only one pixel at the right: we mark it and we continue
    lined_(*it) = 0;
    it.recenter (*it);
  }

  if (!lined_.domain().has(it.center()))
  {
    // Right edge reached !
    rightmost_ = it.center();
    return true;
  }
  else if (it.count() > 1)
  {
    // Several paths are possible
    for_all (it)
    {
      std::cout << "Trying " << *it << std::endl;
      if (find_line (water, *it))
	return true;
    }
    std::cout << "Several branches, none good at " << it.center() << std::endl;
   }
  std::cout << "Dead end at " << it.center() << std::endl;
  // Dead end :(
  if (it.center()[1] > rightmost_[1])
  {
    std::cout << "Found " << it.center()
              << " better than " << rightmost_ << std::endl;
    rightmost_ = it.center();
  }
  else
    std::cout << "Found " << it.center()
              << " worse than " << rightmost_ << std::endl;
   return false;
}

///////////////// VECTOR STYLE FINDER

template <typename Value>
void
Cuttor<Value>::find_vector_line (mln::image2d<Value>& from,
                                 mln::point2d entry)
{
  VectorIterator<Value> it (from, entry, RIGHT);
  Vector<mln::point2d> current(entry);
  Vector<mln::point2d> previous(entry);

  while (it.has_point())
  {
    // Pass all the pixels until an intersection
    while (it.count() == 1 && it.has_point())
    {
//      std::cout << "### Advancing " << (*it) << std::endl;
      if (!current.add_point(*it, it.orient()))
      {
        previous = current;
//        std::cout << "### 10 size vector " << std::endl;
//        current.print();
        current.restart_from(*it, it.orient());
      }

      lined_(*it) = 0; // Add color
      it.reinit (*it, it.orient());
    }

    current.print();

    if (!it.has_point() || !from.domain().has(*it))
    {
      // Draw finishing line til the end
      rightmost_ = it.origin();
      std::cout << "### END OF LINE" << std::endl;
      return;
    }
    else
    {
      std::cout << "### LOOK FOR ORIENTATION" << std::endl;

      double min_angle = 360;
      mln::point2d new_start (0,0);
      int new_orient = 2; // RIGHT

      while (it.has_point ())
      {
        std::cout << "### Test on " << (*it) << " Orient " << it.orient() << std::endl;

        Vector<mln::point2d>* vect_tmp = retrive_vect_from(from, *it, it.orient());

        vect_tmp->print ();

        double angle_tmp;

        if (current.norm() != 0)
          angle_tmp = current.angle(*vect_tmp);
        else
          angle_tmp = previous.angle(*vect_tmp);

        std::cout << "### Angle " << angle_tmp << std::endl;

        if (angle_tmp < min_angle)
        {
          min_angle = angle_tmp;
          new_start = *it;
          new_orient = it.orient();
        }

        delete vect_tmp;

        it.next();
      }

      std::cout << "### Chosen new start: " << new_start << std::endl;
      lined_(new_start) = 0;
      previous = current;
      current.restart_from(new_start, new_orient);
      it.reinit(new_start, new_orient);
    }
  }
  std::cout << "!!!!!!! A PROBLEM OCCURED, UNEXPECTED BEHAVIOR" << std::endl;
}

template<typename Value>
Vector<mln::point2d>*
Cuttor<Value>::retrive_vect_from(mln::image2d<Value>& from,
                                 mln::point2d start,
                                 int orient)
{
  VectorIterator<Value> it (from, start, orient);
  Vector<mln::point2d>* res =  new Vector<mln::point2d>(start);

  // std::cout << "### Inside retrive from" << start << std::endl;

//   std::cout << "### OUT loop count " << it.count()
//             << " has_point " << it.has_point()
//             << " Cur Point " << (*it) << std::endl;

  while (it.count() == 1 && it.has_point() && res->add_point(*it, it.orient()))
  {
//     std::cout << "### Inside loop count " << it.count()
//               << " has_point " << it.has_point()
//               << " Cur Point " << (*it) << std::endl;
    it.reinit (*it, it.orient());
  }

  return res;
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
