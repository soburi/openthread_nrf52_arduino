
lines = readlines

clzz = []

lines.each do |line|
  line =~ /(\w*)::/
  clzz.append($1)
end


clzz.sort.uniq.each do |clz|

  print "  class OT#{clz} {\n"
  print "  friend class OpenThreadClass;\n"
  print "  public:\n"
  lines.each do |line|
    if line =~ /#{clz}::/
      if line =~ /\(otInstance\* aInstance/
        line.gsub!(/#{clz}::/,"")
        line.gsub!(/\(otInstance\* aInstance/,"(")
        line.gsub!(/\(, /,"(")
        print "    #{line.strip.sub(/\t/, " ")}".gsub(/const_/, "const ")
        print ";\n"
      else
        print "    //#{line.strip.sub(/\t/, " ")}".gsub(/const_/, "const ")
        print ";\n"
      end
    end
  end
  print "  };\n"
  print "\n"
end

