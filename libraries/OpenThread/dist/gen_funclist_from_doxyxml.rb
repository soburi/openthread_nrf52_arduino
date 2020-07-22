require 'rexml/document'
require 'rexml/formatters/pretty'
require 'pp'
require 'stringio'

formatter = REXML::Formatters::Pretty.new

ARGV.each do |file|
  xml = REXML::Document.new(File.new(file))


  REXML::XPath.each(xml, "/doxygen/compounddef/sectiondef/memberdef") do |e|
#    print "---- #{file}\n"
    if e.elements['definition']
      print e.elements['location'].attributes['file']
      print "\t"
      print e.elements['definition'].text 
      print e.elements['argsstring'].text if e.elements['argsstring']
      print "\n"
    end
  end
  output = StringIO.new
#  formatter.write(xml, output)
#  print output.string
end
