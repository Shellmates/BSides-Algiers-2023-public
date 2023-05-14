#!/usr/bin/env ruby

# frozen_string_literal: true

require 'colorize'
require 'socket'
require 'date'

# Disable stdout and stderr buffering
$stdout.sync = true
$stderr.sync = true

class ApplianceTUI
  PROMPT = ' >'
  RUBY_LOGO = <<~RUBY
     .     '     ,
       #{'_________'.light_red}
    _ #{'/_|_____|_\\'.light_red} _
      #{"'. \\   / .'".light_red}
        #{"'.\\ /.'".light_red}
          #{"'.'".light_red}
  RUBY

  HELP_MESSAGES = {
    help: 'Display help on a particular command',
    echo: 'Display a line of text',
    colors: "[on|off]\tEnable or disable colors in the output",
    date: 'Display the current date',
    ip: "Display the machine's IP addresses",
    get_flag: "Obviously, it won't be that easy",
    exit: 'Quit'
  }

  def initialize
    puts 'CLI interface menu'
    puts RUBY_LOGO
    @colors = true
  end

  def run(cmd)
    tokens = cmd.chomp.split
    begin
      send(*tokens) unless tokens.empty?
    rescue NoMethodError => e
      puts "No command #{@colors ? tokens[0].light_red : tokens[0]}"
      help
    rescue ArgumentError => e
      expected_args = method(tokens[0].intern).arity.to_s
      puts "Expected #{@colors ? expected_args.light_red : expected_args} parameters for command #{@colors ? tokens[0].light_red : tokens[0]}"
      help(tokens[0])
    end
  end

  # Displays the help
  def help(method = nil)
    if method && HELP_MESSAGES.has_key?(method&.intern)
      puts "#{@colors ? method.light_red : method}:\t#{HELP_MESSAGES[method&.intern]}"
    else
      puts 'Valid commands:'

      # same as: puts HELP_MESSAGES.map{|cmd, msg| ... }.join(?\n)
      cmds = self.class.instance_methods(false) - %i[run repl]
      puts cmds.map { |cmd| "\t- #{@colors ? cmd.to_s.light_red : cmd}: #{HELP_MESSAGES[cmd]}" }.join("\n")
    end
  end

  # Display the current date
  def date
    today = ::Date.today.to_s
    puts @colors ? today.gsub(/\d+/) { _1.light_red } : today
  end

  # Display the IP addresses of all the interfaces
  def ip
    ifaces = ::Socket.getifaddrs.select { |i| i.addr&.ipv4? }
    puts 'Network interfaces:'
    ifaces.each do |i|
      puts "\t- #{@colors ? i.addr.ip_address.light_red : i.addr.ip_address}\t#{@colors ? i.name : i.name}"
    end
  end

  # Display the given string
  def echo(str)
    puts str
  end

  def exit
    ::Kernel.exit
  end

  # Enable or disable output coloring
  def colors(state)
    raise ArgumentError, 'State should be "on" or "off"' unless %w[on off].include?(state)

    @colors = state == 'on'
  end

  def get_flag
    # This is a red herring, you have been warned

    return; # Obviously, I don't want you to get past this line, muahahaha

    puts File.open('/flag.txt', 'rb', &:read)
  end

  def repl
    loop do
      print PROMPT
      line = gets.chomp
      run(line)
    end
  end
end

begin
  ApplianceTUI.new.repl
rescue StandardError => e
  puts e.message
  puts e.backtrace
  exit 1;
end
