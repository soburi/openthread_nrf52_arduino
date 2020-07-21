
headers = [ "backbone_router.h", "backbone_router_ftd.h", "border_agent.h", "border_router.h", "channel_manager.h", "channel_monitor.h", "child_supervision.h", "cli.h", "coap.h", "coap_secure.h", "commissioner.h", "config.h", "crypto.h", "dataset.h", "dataset_ftd.h", "diag.h", "dns.h", "entropy.h", "error.h", "heap.h", "icmp6.h", "instance.h", "ip6.h", "jam_detection.h", "joiner.h", "link.h", "link_raw.h", "logging.h", "message.h", "ncp.h", "netdata.h", "netdiag.h", "network_time.h", "ot_api_doc.h", "platform", "random_crypto.h", "random_noncrypto.h", "server.h", "sntp.h", "tasklet.h", "thread.h", "thread_ftd.h", "udp.h"]

filtered = readlines.select do |line|
  x = true
  x = false if not headers.include?( line.split("\t")[0].split(/\//).last )
  x = false if not line =~ /\(/
  x = false if line =~ /::/
  x = false if line =~ /typedef/
  x 
end

filtered.sort.uniq.each do |line|
  print line
end
