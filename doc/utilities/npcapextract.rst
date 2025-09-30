npcapextract
============

This utility, given a pcap file and its index, or alternatively a “timeline” tree created by n2disk, produces a new pcap file (or several pcap files according to the specified file limit) with the packets matching the provided filter in BPF-like syntax. Available options are:

.. code-block:: text

   FIND FROM TIMELINE
   
   [-t] <timeline dir>		| Timeline directory created by n2disk with -A (multiple -t are allowed)
   [-b] <begin time>			| Begin time on which packets will be selected
   [-e] <end time>			| Endtime on which packets will be selected
   
   FIND FROM SINGLE FILE/INDEX
   
   [-a] <pcap file>			| Pcap file from which extract packets
   [-i] <index file>			| Index corresponding to the pcap file
   
   FILTERING
   
   [-f] <filter>			| BPF ingress packet filter (tcpdump-like)
   					| NOTE: -b/-e time must specified as YYYY-MM-DD hh:mm:ss
   
   DUMP SETTINGS
   
   [-o] <pcap file>			| Pcap file or directory where matched packets will be saved
   [-s] <len>				| Cut packets to len (Bytes)
   [-P] <len>				| Max pcap file length (MBytes)
   [-N] <number>			| Max packets per pcap file. (Default: 0 - no max duration)
   [-x] <prefix>			| Dump file prefix (to be used with -P or -N)
   
   OTHER
   
   [-g] <core_id>			| CPU core affinity
   [-u] <username>			| Use <username> instead of nobody when dropping privileges
   [-O]				| Write output to stdout (npcapextract -O .. | tshark -i -)
   [-l]				| List matching pcap files only (no packet extraction)
   [-T]				| Test index scan performance (no packet extraction)
   [-0]				| Write empty pcap on no match
   [-v] <level>			| Verbosity level: 0..6 (default = 2)
   [-h]				| Help

Example of extraction from single PCAP/Index file:

.. code-block:: console

   npcapextract -a dummy.pcap -i dummy.pcap.idx -o output.pcap -f "host 192.168.1.1 or port 80"

Example of extraction from a timeline/index produced by n2disk:

.. code-block:: console

   npcapextract -t /tmp/n2disk/timeline -b "2012-10-02 12:00:00" -e "2012-10-03 00:00:00" -o output.pcap -f "host 192.168.1.1 and port 80"

Since version 3.0 npcapextract features PAM support. With PAM it is possible to integrate multiple authentication schemes (including LDAP) for granting traffic retrieval capabilities to selected users/groups. By default the user running npcapextract (or the one specified with -u) should have read permissions on the dump set in order to extract/retrieve traffic. In order to enable PAM support, you should set the SUID to the npcapextract executable, and configure /etc/pam.d/npcapextract with your preferred authentication scheme (by default npcapextract uses the unix authentication!).

