# A C++ implementation of the MD5 algorithm adapted from RSA Data
# Security's implementation.
# 
# http://userpages.umbc.edu/~mabzug1/cs/md5/md5.html
# http://userpages.umbc.edu/~mabzug1/cs/md5/md5-cc-102.tar.gz

# FIXME: Check that types are wide enough.  From README:
# 
#   MD5 assumes that an unsigned char is at least a byte long, that an
#   unsigned short int is at least two bytes long, and that an unsigned
#   int is at least four bytes long.  If your system is different, you 
#   should modify md5.hh accordingly.
#
# We should have `configure' check this.

# FIXME: I (Roland) think that there is no legal problem to include
# this code in Olena. Still, we should check this thoroughly.
#
# We might want to depend on Crypto++ (http://www.cryptopp.com/)
# instead, which is both available from Debian GNU/Linux
# (http://packages.debian.org/lenny/libcrypto++-dev) and the MacPorts
# (http://trac.macports.org/browser/trunk/dports/devel/libcryptopp/Portfile).
# Crypto++ should also work on Cygwin (http://www.cryptopp.com/
# mentions patches to have Crypto++ compile on Cygwin).

noinst_LTLIBRARIES = libmd5.la
libmd5_la_SOURCES = md5.hh md5.cc

check_PROGRAMS = md5
md5_SOURCES = driver.cc
md5_LDADD = libmd5.la

EXTRA_DIST = README test-suite
