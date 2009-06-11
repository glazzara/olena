class Vcs

  # See http://rubyforge.org/projects/vcs
  # and http://vcs.rubyforge.org

  protocol_version '0.1'

  def scribo_commit! ( *args )
    common_commit!("scribo <%= rev %>: <%= title %>", *args) do |subject|
      mail!(:to => %w[olena-patches@lrde.epita.fr], :subject => subject)
    end
  end
  alias_command  :mlnci, :scribo_commit
  default_commit :scribo_commit

end # class Vcs
