n2disk Documentation
====================

n2disk is a network traffic recorder application. With n2disk you can capture full-sized network packets at multi-Gigabit rate (above 10 Gigabit/s on adequate hardware) from a live network interface, and write them into files without any packet loss. n2disk has been designed to write files into disks for very long periods, you have to specify a maximum number of distinct file that may be written during the execution, and if n2disk reaches the maximum number of files, it will start recycling the files from the oldest one. This way you can have a complete view of the traffic for a fixed temporal window, knowing in advance the amount of disk space needed.

n2disk uses the industry standard PCAP file format (regular and nanosecond) to dump packets into files so the resulting output can be easily integrated with existing third party or even open/source analysis tools (like Wireshark). Alternatively n2disk can produce compressed PCAP files (.npcap) to reduce disk space and optimise disk throughput.

n2disk has been designed and developed mainly because most network security systems rely on capturing all packets (both header and payload), since any packets may have been responsible for the attack or could contain the problems that we are trying to find. Netflow information is more manageable and requires less disk space to be stored, but in some cases, like deep-packet-inspection analysis or controlled traffic regeneration, it is not useful. When we need to collect the entire packet, because we need all the information, n2disk has to be used. 

n2disk can be effectively used to perform numerous activities, among these:

- Off-line network packets analysis by feeding a specialised tools like Snort.
- Reconstruct particular communication flows or network activities.
- Reproduce the previous captured traffic to a different network interface.

.. toctree::
   :maxdepth: 2
   :caption: User's Guide

   features
   performance
   versions
   installation
   usage
   how_to_start
   tuning/index
   storage_sizing
   utilities/index
   using_with_ntopng
   netflow
   filters
   filesystem
   license

.. toctree::
   :caption: Other Products

   ntopng <https://www.ntop.org/guides/ntopng/>
   nProbe <https://www.ntop.org/guides/nprobe/>
   nProbe Cento <https://www.ntop.org/guides/cento/>
   nDPI <https://www.ntop.org/guides/nDPI/>
   PF_RING <https://www.ntop.org/guides/pf_ring/>
   nEdge <https://www.ntop.org/guides/nedge/>
   nScrub <https://www.ntop.org/guides/nscrub/>
   nBox <https://www.ntop.org/guides/nbox/>
   nTap <https://www.ntop.org/guides/ntap/>
   
.. Indices and tables
.. ==================
.. 
.. * :ref:`genindex`
.. * :ref:`modindex`
.. * :ref:`search`

