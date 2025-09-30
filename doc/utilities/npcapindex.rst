npcapindex
==========

This utility produces an index file (with the same format of the index produced by n2disk) from a pcap file. 

Available options:

.. code-block:: text

   [-i ] <pcap file>	  		| Pcap file to index
   [-o] <index file>	 		| Index file
   [-f ] <bpf>				| BPF filter to select packets to index
   [-G] <mode>				| Compute index on tunnel content (GTP-U)
   					| 1 - Index both inner and outer headers
   					| 2 - Use inner header as outer header
   [-I] <version>			| Specify the index version:
   					| 1 - Standard index (default)
   					| 2 - Flow-based index
   [-v]					| Verbose
   [-h]					| Help

Example:

.. code-block:: console

   npcapindex -i dummy.pcap -o dummy.index
