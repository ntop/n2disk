Versions
========

The n2disk software comes in four versions: n2disk1g, n2disk5g, n2disk (10g and above),
depending on the ingress traffic rate that need to be processed (note: there is no 
constraint on the link speed or number of links).

n2disk1g (1 Gbit/s)
-------------------

This is the entry-level version, enough for rates up to 1 Gigabit.

n2disk5g (5 Gbit/s)
-------------------

This is the standard version, with two threads: one for packet capture and one for disk 
writing. This version can handle rates up to 5 Gigabit.

n2disk (10/40/100 Gbit/s)
---------------------

This version includes support for ZC and multithreaded traffic processing, with an internal 
architecture able to load-balance pcap processing and indexing on multiple threads. This 
version can handle 10 Gigabit and above.

Models Comparison Table
-----------------------

.. list-table:: Features by Model
   :widths: 20 20 20 20 20
   :header-rows: 1

   * - Version
     - Max Dump Speed
     - L7 Filtering
     - Linux
     - Unix / OSX
   * - n2disk1g
     - 1 Gigabit
     - Supported (requires FT)
     - Native PF_RING support.
     - Basic libpcap-based packet capture.  
       Available on Request
   * - n2disk5g
     - 5 Gigabit
     - Supported (requires FT)
     - Enhanced PF_RING support  
       (i.e. full packet capture acceleration).
     - Basic libpcap-based packet capture.  
       Available on Request
   * - n2disk
     - 10/40/100 Gigabit
     - Supported (FT included)
     - Multithreaded zero-copy packet capture.
     - Not available.

