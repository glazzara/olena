#ifndef STATS_HH_
# define STATS_HH_

# include <vector>
# include <algorithm>

# include <mln/util/array.hh>

using namespace mln;

//---------------------------------------------------------------------------
// compare_values
//---------------------------------------------------------------------------

template< typename T >
struct compare_values
{
  bool operator() (const T& lhs,
		   const T& rhs)
  {
    return (lhs < rhs);
  }
};


//---------------------------------------------------------------------------
// cluster_stats
//---------------------------------------------------------------------------

template< typename T >
class cluster_stats
{
public:
  cluster_stats()
    : mean_needs_update_(false), median_needs_update_(false),
      variance_needs_update_(false), std_needs_update_(false), size_(0)
  {
  }

  cluster_stats(const unsigned size)
    : mean_needs_update_(false), median_needs_update_(false),
      variance_needs_update_(false), std_needs_update_(false), size_(0)
  {
    data_.reserve(size);
  }

  void reset()
  {
    std::vector< T >& data = data_.hook_std_vector_();
    data.clear();

    size_ = 0;
    needs_update();
  }

  void take(const T& value)
  {
    if (not size_)
    {
      min_ = value;
      max_ = value;
    }
    else
    {
      if (value < min_)
	min_ = value;
      else if (value > max_)
	max_ = value;
    }

    ++size_;
    data_.append(value);
    needs_update();
  }

  T mean()
  {
    if (mean_needs_update_)
    {
      mean_ = 0;

      if (size_ > 0)
      {
	for (unsigned i = 0; i < size_; ++i)
	  mean_ += data_[i];

	mean_ /= size_;
      }
      mean_needs_update_ = false;
    }

    return mean_;
  }

  T median()
  {
    if (median_needs_update_)
    {
      median_ = 0;

      if (size_ > 0)
      {
	std::vector< T >& data = data_.hook_std_vector_();
	std::sort(data.begin(), data.end(), compare_values< T >());

	median_ = data[(size_ - 1) >> 1];
      }

      median_needs_update_ = false;
    }

    return median_;
  }

  T variance()
  {
    if (variance_needs_update_)
    {
      mean();
      variance_ = 0;

      if (size_ > 0)
      {
	for (unsigned i = 0; i < size_; ++i)
	{
	  const T tmp = mean_ - data_[i];

	  variance_ += (tmp * tmp);
	}

	variance_ /= size_;
	std_ = sqrt(variance_);
      }

      variance_needs_update_ = false;
      std_needs_update_ = false;
    }

    return variance_;
  }

  T standard_deviation()
  {
    if (std_needs_update_)
      variance();

    return std_;
  }

  T min() { return (size_ > 0) ? min_ : 0; }
  T max() { return (size_ > 0) ? max_ : 0; }

  void sort()
  {
    std::vector< T >& data = data_.hook_std_vector_();
    std::sort(data.begin(), data.end(), compare_values< T >());
  }

  unsigned nelements() { return size_; }

  T operator[] (const unsigned index)
  {
    return data_[index];
  }

private:
  void needs_update()
  {
    mean_needs_update_ = true;
    median_needs_update_ = true;
    variance_needs_update_ = true;
    std_needs_update_ = true;
  }

private:
  bool mean_needs_update_ : 1;
  bool median_needs_update_ : 1;
  bool variance_needs_update_ : 1;
  bool std_needs_update_ : 1;
  T mean_;
  T median_;
  T min_;
  T max_;
  T variance_;
  T std_;
  unsigned size_;
  util::array< T > data_;
};

//---------------------------------------------------------------------------
// stats
//---------------------------------------------------------------------------

template< typename T >
class stats
{
public:
  stats()
    : clusters_need_update_(true), data_sorted_(false)
  {
  }

  stats(const int size)
    : clusters_need_update_(true), data_sorted_(false), data_(size)
  {
  }

  void reset()
  {
    data_.reset();
    std::vector< cluster_stats< T > >& clusters = clusters_.hook_std_vector_();
    clusters.clear();
    data_sorted_ = false;
    clusters_need_update_ = true;
  }

  void take(const T& value)
  {
    data_.take(value);
    clusters_need_update_ = true;
  }

  T mean()
  {
    return data_.mean();
  }

  T median()
  {
    return data_.median();
  }

  T variance()
  {
    return data_.variance();
  }

  T standard_deviation()
  {
    return data_.standard_deviation();
  }

  T min() { return data_.min(); }
  T max() { return data_.max(); }

  unsigned nelements() { return data_.nelements(); }

  util::array< cluster_stats< T > >& clusters()
  {
    if (clusters_need_update_)
    {
      compute_clusters();
      clusters_need_update_ = false;
    }

    return clusters_;
  }

private:
  void compute_clusters()
  {
    std::vector< unsigned > clusters;
    unsigned cluster_index = 1;

    clusters.reserve(data_.nelements());

    if (not data_sorted_)
    {
      data_.sort();
      data_sorted_ = true;
    }

    unsigned i = 0;
    const unsigned nelements = data_.nelements();

    clusters.push_back(cluster_index);
    const T std = data_.standard_deviation();

    for (i = 1; i < nelements - 1; ++i)
    {
      const T left_distance = data_[i] - data_[i - 1];
      const T right_distance = data_[i + 1] - data_[i];

      if (not ((left_distance <= 1 || left_distance < right_distance)
	       && left_distance <= std))
	++cluster_index;

      clusters.push_back(cluster_index);
    }

    if (nelements > 1)
    {
      if (data_[i] - data_[i - 1] > std)
	++cluster_index;
      clusters.push_back(cluster_index);
    }

    clusters_.clear();
    clusters_.reserve(cluster_index);
    cluster_index = 1;

    i = 0;
    while (i < nelements)
    {
      unsigned tmp = i;

      while (tmp < nelements && clusters[tmp] == clusters[i])
	++tmp;

      cluster_stats< T > cluster(tmp - i);

      tmp = i;
      while (tmp < nelements && clusters[tmp] == clusters[i])
      {
	cluster.take(data_[tmp]);
	++tmp;
      }

      clusters_.append(cluster);

      i = tmp;
      ++cluster_index;
    }
  }

private:
  bool clusters_need_update_ : 1;
  bool data_sorted_ : 1;
  cluster_stats< T > data_;
  util::array< cluster_stats< T > > clusters_;
};

#endif /* STATS_HH_ */
