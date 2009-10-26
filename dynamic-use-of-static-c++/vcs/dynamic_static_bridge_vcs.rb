class Vcs
  # See http://rubyforge.org/projects/vcs
  # and http://vcs.rubyforge.org
  
  protocol_version '0.1'

  def dynamic_static_bridge_commit! ( *args )

    common_commit!("[DynamicStaticBridge]: <%= rev %>: <%= title %>", *args) do |subject|
      
      mail! :to => ["damien.thivolle@gmail.com", "nicolas.pouillard@gmail.com"], :subject => subject
      
    end

  end
  alias_command  :dystci, :dynamic_static_bridge_commit
  default_commit :dynamic_static_bridge_commit

end # class Vcs
