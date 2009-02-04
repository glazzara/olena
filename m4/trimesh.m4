#                                                       -*- Autoconf -*-

# OLN_WITH_TRIMESH(WITH, WITHOUT)
# -------------------------------
# Should we build Trimesh or not.
AC_DEFUN([OLN_WITH_TRIMESH],
[AC_ARG_WITH([trimesh],
             [AS_HELP_STRING([--with-trimesh],
                             [build the Trimesh library])],
                             [],
                             [with_trimesh=no])

case $with_trimesh in
  yes) $1;;
  no ) $2;;
  *)   AC_MSG_ERROR([incorrect with_trimesh value: $with_trimesh]);;
esac
])
