#ifndef CUTTOR_HH
# define CUTTOR_HH

# include <sys/types.h>
# include <sys/stat.h>

# include <mln/core/image/image2d.hh>
# include <mln/value/all.hh>
# include "vector.hh"

template<typename Value>
class Cuttor
{
  public:
    // Eat the filepath to the image to be treated.
    Cuttor (std::string filepath);

    // Lauch all the treatments needed at once on the image
    void start (bool use_vect);

  private:
    // TODO Private methods called by start.

    // Draw the separation line which starts at the given position
    bool find_line (mln::image2d<Value>& water, mln::point2d p);

    mln::point2d find_entry_point(mln::image2d<Value>& ima);


    ///////////////// VECTOR STYLE FINDER

    void find_vector_line (mln::image2d<Value>& from,
                           mln::point2d entry);

    Vector<mln::point2d> retrive_vect_from(mln::image2d<Value>& from,
                                           mln::point2d start,
                                           e_orient orient);

    ///////////////// END OF VECTOR STYLE FINDER

    unsigned int last_entry_;
    // directory of the file
    std::string filedir_;
    // filename (no extension which should be .pgm)
    std::string filename_;

    // The original image
    mln::image2d<Value> img_;

    // The line separation image
    mln::image2d<Value> lined_;
    // The furthest point reached in the current line being drawn
    mln::point2d rightmost_;
};

template <typename Value>
void save (mln::image2d<Value>& ima, std::string filename, std::string postfix);

# include "cuttor.hxx"

#endif /* !CUTTOR_HH */
