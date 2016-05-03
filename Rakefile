require "bundler/gem_tasks"

require 'rake/extensiontask'
spec = Gem::Specification.load('fast_osc.gemspec')
Rake::ExtensionTask.new('fast_osc') do |ext|
  ext.lib_dir = "lib/fast_osc"
end
