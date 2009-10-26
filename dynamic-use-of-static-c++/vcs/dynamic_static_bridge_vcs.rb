class Vcs
  # See http://rubyforge.org/projects/vcs
  # and http://vcs.rubyforge.org
  
  protocol_version '0.1'

  def dynamic_static_bridge_commit! ( *args )
    common_commit!("dyn: <%= rev %>: <%= title %>", *args) do |subject|
      mail!(:to => %w[olena-patches@lrde.epita.fr], :subject => subject)
    end
  end
  alias_command  :dystci, :dynamic_static_bridge_commit
  default_commit :dynamic_static_bridge_commit

end # class Vcs
