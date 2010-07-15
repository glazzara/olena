# include "common.hh"

QString Common::get_file_name(QString image)
{
  QString tmp = image.mid(image.lastIndexOf("/") + 1);
  tmp.chop(tmp.size() - tmp.lastIndexOf("."));
  return tmp;
}

QString Common::get_file_ext(QString image)
{
  return image.right(image.size() - image.lastIndexOf("."));
}
