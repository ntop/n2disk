npcapdecompress
===============

This utility decompress compressed .npcap files to standard pcap files.

Available options:

.. code-block:: text 

   [-v]                        	| Verbose
   [-h]                        	| Help
   [-i ] <.npcap file>     	 	| Compressed npcap file to decompress
   [-o] <.pcap file>     		| Decompressed pcap file (output)

Example:

.. code-block:: console

   npcapdecompress -i dummy.npcap -o dummy.pcap

