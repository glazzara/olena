class Vcs

  # See http://rubyforge.org/projects/vcs
  # and http://vcs.rubyforge.org

  protocol_version '0.1'

  def milena_commit! ( *args )
    common_commit!("milena <%= rev %>: <%= title %>", *args) do |subject|
      mail!(:to => %w[olena-patches@lrde.epita.fr], :subject => subject)
    end
  end
  alias_command  :mlnci, :milena_commit
  default_commit :milena_commit

end # class Vcs
