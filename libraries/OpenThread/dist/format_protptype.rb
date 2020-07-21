def classese_extract(line, classes)
  classes.each do |clz|
    if line =~ / ot#{clz}([^\(]*\()/
      line.sub!(/ ot#{clz}/, "\t#{clz}::")
      return true
    end
  end

  return false
end

def compact_space(line)
  line.gsub!(/\ \ */," ")
  line.gsub!(/ \*/,"* ")
  line.gsub!(/const /,"const_")
  line.gsub!(/static_/,"static_")
  line.gsub!(/OT_TOOL_WEAK /,"OT_TOOL_WEAK_")
end

readlines.each do |line|
  compact_space(line)
  ret = classese_extract(line,
    ['Udp', 'Thread', 'Tasklets', 'SntpClient', 'Server', 'RandomNonCrypto', 'RandomCrypto',
     'Plat', 'NetworkTime', 'NetData', 'Ncp', 'Message', 'Logging', 'Link', 'Joiner', 'JamDetection',
     'Ip6', 'Instance', 'Icmp6', 'Heap', 'Entropy', 'DnsClient', 'Diag', 'Dataset',
     'Crypto', 'Cli', 'Commissioner', 'CoapSecure', 'Coap', 'ChildSupervision', 'ChannelMonitor',
     'ChannelManager', 'BorderRouter', 'BorderAgent', 'BackboneRouter'])
  
  if ret
    STDOUT.print line
  else
    STDERR.print line
  end

end
