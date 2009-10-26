

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
