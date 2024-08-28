import sst

# Define SST core options
#sst.setProgramOption("timebase", "1ps")

tiles = 96

for x in range (0, tiles):

  # Define the simulation components
  comp_cpu = sst.Component("cpu_"+str(x), "miranda.BaseCPU")
  comp_cpu.addParams({
	  "verbose" : 0,
	  "clock" : "2GHz",
	  "printStats" : 1,
  })

  gen = comp_cpu.setSubComponent("generator", "miranda.Stencil3DBenchGenerator")
  gen.addParams({
	  "verbose" : 0,
	  "nx" : 10,
	  "ny" : 10,
	  "nz" : 10,
  })

  comp_l1cache = sst.Component("l1cache_"+str(x), "memHierarchy.Cache")
  comp_l1cache.addParams({
        "access_latency_cycles" : "2",
        "cache_frequency" : "2 GHz",
        "replacement_policy" : "lru",
        "coherence_protocol" : "MESI",
        "associativity" : "4",
        "cache_line_size" : "64",
        "prefetcher" : "cassini.StridePrefetcher",
        "debug" : "0",
        "L1" : "1",
        "cache_size" : "32KB"
  })

  comp_l2cache = sst.Component("l2cache_"+str(x), "memHierarchy.Cache")
  comp_l2cache.addParams({
        "access_latency_cycles" : "2",
        "cache_frequency" : "2 GHz",
        "replacement_policy" : "lru",
        "coherence_protocol" : "MESI",
        "associativity" : "4",
        "cache_line_size" : "64",
        "prefetcher" : "cassini.StridePrefetcher",
        "debug" : "0",
        "L1" : "0",
        "cache_size" : "32KB"
  })

  comp_l3cache = sst.Component("l3cache_"+str(x), "memHierarchy.Cache")
  comp_l3cache.addParams({
        "access_latency_cycles" : "2",
        "cache_frequency" : "2 GHz",
        "replacement_policy" : "lru",
        "coherence_protocol" : "MESI",
        "associativity" : "4",
        "cache_line_size" : "64",
        "prefetcher" : "cassini.StridePrefetcher",
        "debug" : "0",
        "L1" : "0",
        "cache_size" : "32KB"
  })

  comp_l4cache = sst.Component("l4cache_"+str(x), "memHierarchy.Cache")
  comp_l4cache.addParams({
        "access_latency_cycles" : "2",
        "cache_frequency" : "2 GHz",
        "replacement_policy" : "lru",
        "coherence_protocol" : "MESI",
        "associativity" : "4",
        "cache_line_size" : "64",
        "prefetcher" : "cassini.StridePrefetcher",
        "debug" : "0",
        "L1" : "0",
        "cache_size" : "32KB"
  })

  comp_l5cache = sst.Component("l5cache_"+str(x), "memHierarchy.Cache")
  comp_l5cache.addParams({
        "access_latency_cycles" : "2",
        "cache_frequency" : "2 GHz",
        "replacement_policy" : "lru",
        "coherence_protocol" : "MESI",
        "associativity" : "4",
        "cache_line_size" : "64",
        "prefetcher" : "cassini.StridePrefetcher",
        "debug" : "0",
        "L1" : "0",
        "cache_size" : "32KB"
  })


  # Enable statistics outputs
  #comp_l1cache.enableAllStatistics({"type":"sst.AccumulatorStatistic"})

  comp_memctrl = sst.Component("memory_"+str(x), "memHierarchy.MemController")
  comp_memctrl.addParams({
      "clock" : "1GHz",
      "addr_range_end" : 4096 * 1024 * 1024 - 1
  })
  memory = comp_memctrl.setSubComponent("backend", "memHierarchy.simpleMem")
  memory.addParams({
        "access_time" : "100 ns",
        "mem_size" : "4096MiB",
  })

  # Define the simulation links
  link_cpu_cache_link = sst.Link("link_cpu_cache_link_"+str(x))
  link_cpu_cache_link.connect( (comp_cpu, "cache_link", "1000ps"), (comp_l1cache, "high_network_0", "1000ps") )
  link_cpu_cache_link.setNoCut()

  link_mem_bus_link1 = sst.Link("link_mem_bus_link_1_"+str(x))
  link_mem_bus_link1.connect( (comp_l1cache, "low_network_0", "50ps"), (comp_l2cache, "high_network_0", "50ps") )
  link_mem_bus_link2 = sst.Link("link_mem_bus_link_2_"+str(x))
  link_mem_bus_link2.connect( (comp_l2cache, "low_network_0", "50ps"), (comp_l3cache, "high_network_0", "50ps") )
  link_mem_bus_link3 = sst.Link("link_mem_bus_link_3_"+str(x))
  link_mem_bus_link3.connect( (comp_l3cache, "low_network_0", "50ps"), (comp_l4cache, "high_network_0", "50ps") )
  link_mem_bus_link4 = sst.Link("link_mem_bus_link_4_"+str(x))
  link_mem_bus_link4.connect( (comp_l4cache, "low_network_0", "50ps"), (comp_l5cache, "high_network_0", "50ps") )


  # -- final level
  link_mem_bus_link5 = sst.Link("link_mem_bus_link_5_"+str(x))
  link_mem_bus_link5.connect( (comp_l5cache, "low_network_0", "50ps"), (comp_memctrl, "direct_link", "50ps") )

sst.setStatisticLoadLevel(16)
sst.enableAllStatisticsForAllComponents({"type":"sst.AccumulatorStatistic"})
sst.setStatisticOutput("sst.statOutputCSV")
sst.setStatisticOutputOptions( {
        "filepath"  : "bench3.csv",
        "separator" : ", "
} )
