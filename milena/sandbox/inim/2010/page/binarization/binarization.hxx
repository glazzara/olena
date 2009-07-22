#ifndef BINARIZATION_HXX
# define BINARIZATION_HXX

# include <binarization.hh>

namespace binarization
{
  template<typename T1, typename T2>
  void
  sauvola_threshold(const image2d<T1>& src,
                    image2d<T2>& dst,
                    unsigned int w)
  {
    // Control the threshold value in the local window
    // The higher, the lower the threshold form the local
    // mean m(x, y). Badekas et al. said 0.34 was best.
    const double k = 0.34;
    // Maximum value of the standard deviation (128 for
    // grayscale documents).
    const double R = 128;


    // TODO: Merge both calculus into one to improve speed
    // Compute the sum of all intensities of src
    IntegralImage<T1> simple(src, Identity<T1>());
    // Compute the sum of all squared intensities of src
    IntegralImage<T1> squared(src, Square<T1>());

    int w_2 = w >> 1;

    // Savaula Algorithm with I.I.
    for(def::coord row = 0; row < static_cast<def::coord>(dst.nrows()); ++row)
      for(def::coord col = 0; col < static_cast<def::coord>(dst.ncols()); ++col)
      {
        int row_min = std::max(0, row - w_2);
        int col_min = std::max(0, col - w_2);
        int row_max = std::min(static_cast<int>(dst.nrows() - 1),
          row + w_2);
        int col_max = std::min(static_cast<int>(dst.ncols() - 1),
          col + w_2);

        double wh = (row_max - row_min + 1) * (col_max - col_min + 1);

        // Mean
        double m_x_y_tmp = (simple(row_max, col_max)
                            + simple(row_min, col_min)
                            - simple(row_max, col_min)
                            - simple(row_min, col_max));

        double m_x_y = m_x_y_tmp / wh;

        // Standard deviation
        double s_x_y_tmp = (squared(row_max, col_max)
                            + squared(row_min, col_min)
                            - squared(row_max, col_min)
                            - squared(row_min, col_max));

        double s_x_y = sqrt((s_x_y_tmp - (m_x_y_tmp * m_x_y_tmp) / wh)/(wh-1.0f));

        // Thresholding
        double t_x_y = m_x_y * (1.0 + k * ((s_x_y / R) - 1.0));

        if (t_x_y > src(point2d(row, col)))
          dst(point2d(row, col)) = true;
        else
          dst(point2d(row, col)) = false;
      }

  }
}

#endif /* !BINARIZATION_HXX */
