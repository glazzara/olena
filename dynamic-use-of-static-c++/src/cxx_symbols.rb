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

class SimpleSymbol
  attr_reader :code
  def initialize ( code )
    @code = code
  end
  def to_s
    @code.to_s
  end
end # class SimpleSymbol

class EncodedSymbol
  attr_reader :name, :code, :cxx_char, :patt
  @@codes ||= []
  def initialize ( name, code, cxx_char=nil )
    @name = name
    @code = (code.is_a? Integer)? code.chr.to_sym : code
    if cxx_char.is_a? Regexp
      @patt = cxx_char
      @cxx_char = " #@name "
    else
      @cxx_char = (cxx_char.is_a? Integer)? cxx_char.chr.to_sym : cxx_char
      @patt = to_s.strip
    end
    raise if @@codes.include? @code
    @@codes << self
  end
  def self.[] ( arg )
    @@codes.find { |x| x.code == arg.to_sym }
  end
  def to_s
    (@cxx_char || " #@name ").to_s
  end
  @@operators ||=
  {
    '<<'  => 'push',
    '>>'  => 'pop',
    '<<=' => 'push_assign',
    '>>=' => 'pop_assign',
    '='   => 'assign',
    '=='  => 'equal',
    '!='  => 'not_equal',
    '<='  => 'less_or_equal',
    '<'   => 'less',
    '>='  => 'greater_or_equal',
    '>'   => 'greater',
    '++'  => 'incr',
    '--'  => 'decr',
    '()'  => 'paren',
    '[]'  => 'square_brackets',
    '->'  => 'arrow',
    '!'   => 'bang',
    '~'   => 'tilde',
    '&'   => 'ampersand',
    '->*' => 'arrow_deref',
    '*'   => 'star',
    '/'   => 'slash',
    '%'   => 'percent',
    '+'   => 'plus',
    '-'   => 'minus',
    '^'   => 'xor',
    '|'   => 'bar',
    '&&'  => 'ampersand_ampersand',
    '||'  => 'bar_bar',
    '+='  => 'plus_assign',
    '-='  => 'minus_assign',
    '*='  => 'star_assign',
    '/='  => 'slash_assign',
    '%='  => 'percent_assign',
    '&='  => 'ampersand_assign',
    '^='  => 'xor_assign',
    '|='  => 'bar_assign',
    ','   => 'comma'
  }
  def self.encode ( str )
    result = str.gsub '_', '_U_'
    result.gsub!(/operator\s*/, 'operator')
    @@codes.each do |v|
      next if v.code == :U
      result.gsub!(v.patt, "_#{v.code}_")
    end
    result.gsub!(/\s*/, '')
    result.gsub!(/operator\s*([a-zA-Z]+)/, 'operator_convert_\1')
    result.gsub!(/\s*/, '')
    result.gsub!(/operator\s*([a-zA-Z]+)/, 'OPERATOR_CONVERT_\1')
    result.gsub!(/\s*/, '')
    result.gsub!(/_+/, '_')
    result.gsub!(/^_/, '')
    result.gsub!(/_$/, '')
    result
  end
  def self.enc ( name, code, cxx_char=nil )
    EncodedSymbol.new(name, code, cxx_char)
  end
  @@operators.to_a.sort{|x,y| y.first.size <=> x.first.size}.each do |k, v|
    enc "operator#{v}".to_sym, "operator#{v}".upcase.gsub('_', '').to_sym, "operator#{k}"
  end
  enc :const,      :CONST, /\bconst\b/
  enc :underscore, :U, ?_
  enc :slash,      :S, ?/
  enc :dot,        :D, ?.
  enc :function,   :F
  enc :left,       :L, '< '
  enc :right,      :R, ' >'
  enc :ref,        :REF, ?&
  enc :ptr,        :PTR, ?*
  enc :namespace,  :N, '::'
  enc :path_sep,   :P, ':'
  enc :comma,      :C, ', '
  enc :right_sqr_bra, :RSB, ' ['
  enc :left_sqr_bra,  :LSB, ']'
end # class EncodedSymbol

def mangle ( aString )
  EncodedSymbol.encode(aString)
end
