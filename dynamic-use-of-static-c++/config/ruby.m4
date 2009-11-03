# Copyright (C) 2005 EPITA Research and Development Laboratory (LRDE).
#
# This file is part of Olena.
#
# Olena is free software: you can redistribute it and/or modify it under
# the terms of the GNU General Public License as published by the Free
# Software Foundation, version 2 of the License.
#
# Olena is distributed in the hope that it will be useful,
# but WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
# General Public License for more details.
#
# You should have received a copy of the GNU General Public License
# along with Olena.  If not, see <http://www.gnu.org/licenses/>.


AC_DEFUN([RUBY],
[esyscmd([ruby <<\RUBY_EOF $2 $3 $4 $5 # and so on...
require "rbconfig"
block = proc do
$1
end
result = block.call(*ARGV)
# STDERR.puts "RUBY: argv: #{ARGV.inspect}, result: #{result}"
print result
RUBY_EOF
])])



AC_DEFUN([RBCONFIG], [esyscmd([ruby config/rbconfig_gen.rb])])



AC_DEFUN([MK_LOCAL_CONFIG_RB_IN], [
ruby $srcdir/config/mk_local_config_rb_in.rb $1 <<MK_LOCAL_CONFIG_RB_IN_EOF
$ac_subst_vars
MK_LOCAL_CONFIG_RB_IN_EOF
])



# Examples:
# * ERB([Hello, <%= "WORLD".downcase %>! (<%= 6 * 7 %>)])
#   => equivalent to: `Hello, world! (42)'
#
# * But in fact if you want that the code will be really substitute
#   During m4 instantiation you must use esyscmd:
# 
#   esyscmd(ERB([AC_SUBST([<%= 'FOO' + '_' + 'BAR' %>])]))
#   => equivalent to: `AC_SUBST([FOO_BAR])'
AC_DEFUN([ERB], [ruby <<\RUBY_EOF
  require 'erb'
  code = <<RUBY_CODE
$1
RUBY_CODE
  eval('ER' + 'B').new(code, $SAFE, '<-%->', '$erbout_').run
RUBY_EOF
])
