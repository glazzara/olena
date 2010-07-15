#ifndef COMMON_HH
# define COMMON_HH

# include <QtGui>

class Common : public QObject
{
  Q_OBJECT
public:
  Common() {}
  ~Common() {}
  static QString get_file_name(QString image);
  static QString get_file_ext(QString image);
};

#endif // ! COMMON_HH
