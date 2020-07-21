print "#include \"OpenThread.h\"\n"
print "#include <openthread/openthread-freertos.h>\n"
print "\n"

readlines.each do |line|
  line.gsub!(/\s[\w\*]*,/, ",")
  line.gsub!(/\s[\w\*]*\)/, ")")
  line.gsub!(/\s+\w+\[], /, "*, ")
  line.gsub!(/::/,"\t")
  line.gsub!(/\(/,"\t")
  line.gsub!(/,/,"\t")
  line.gsub!(/\)/,"")
  line.gsub!(/static /, "")
  data = line.chomp.split(/\t/).collect {|d| d.strip}

  if data[3] == "otInstance*"
    cols = [(data.count-4).to_s, data[1], data[0], data[2] ].concat( data[4..-1].reject{|i|i=='void'} )
    if data[0] == "void"
      print "OT_DECL_VFUNC(#{cols.join(', ')})\n".gsub(/const_/,"const ")
    else
      print "OT_DECL__FUNC(#{cols.join(', ')})\n".gsub(/const_/,"const ")
    end
  else
    cols = [(data.count-3).to_s, data[1], data[2], data[0] ] + data[3..-1]
    print "/*OT_DECL__FUNC(#{cols.join(', ')})*/\n".gsub(/const_/,"const ")
  end


end
