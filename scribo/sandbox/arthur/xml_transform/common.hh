#ifndef COMMON_HH
# define COMMON_HH

# include <QtGui>

class Common : public QObject
{
  Q_OBJECT
public:
  Common() {}
  ~Common() {}
  static QString get_file_name(const QString& image);
  static QString get_file_ext(const QString& image);
};

#endif // ! COMMON_HH
