disk2n
======

This utility replays network traffic previously captured with n2disk on live networks observing the original inter-packet time. The industry standard PCAP file format (both regular and nanosecond) is supported.

disk2n has been designed to replay multiple pcap files of any size with limited memory usage. You can specify a playlist of pcap files and instruct n2disk to continue from the first file when the last one is reached. It is possible to determine in advance the amount of memory used by disk2n, in fact it uses a memory buffer of arbitrary size to cache the next packets to replay, achieving good transmit performance with limited memory usage.

disk2n is multithreaded and uses 3 threads: one for reading packets from disk, one for packet transmission and one for precise time generation.

.. code-block:: text

   TX SETTINGS
   
   [--interface|-i] <device>			| Egress interface.
   [--pcap-file|-f] <pcap file path>		| Pcap file to replay.
   [--multiple-pcap-files|-m] <txt path>	| Text file containing a list of pcap files to replay 
						| in sequence.
   [--timeline-dir|-A] <dir>			| Timeline directory created by n2disk (-A).
   [--begin-time|-B] <epoch time>		| Begin time on which packets will be selected.
   [--end-time|-E] <epoch time>			| End time on which packets will be selected.
   [--coherent-ts-across-pcaps|-H]		| Time coherence across pcap files (do not collapse
						| inter-pcap gap).
   [--inter-packet-time|-I] <device>		| Inter-packet time nsec (ignore pcap timestamps).
   [--precise-timing|-p]			| High time accuracy (lower throughput, forces 
						| active wait).
   [--active-wait|-g]				| Active wait (higher cpu load).
   [--one-shot|-O]				| Send pcap(s) once.
   
   BUFFERING SETTINGS
   
   [--chunk-len|-C] <len>			| (KBytes) Size of the chunk read from disk.
   [--buffer-len|-b] <len>			| (MBytes) Buffer length.
   [--hugepages|-U]				| Use hugepages for memory allocation.
   [--disable-direct-io|-r]			| Disable Direct I/O (experts only).
   
   
   CPU AFFINITY
   
   [--reader-cpu-affinity|-c] <id>		| Binds the reader thread to the specified core.
   [--sender-cpu-affinity|-w] <id>		| Binds the sender thread to the specified core.
   [--time-pulse|-S]	[<id>]			| Bind time pulse thread to the specified core.
   
   PACKET REFORGING
   
   [--src-mac] <mac>				| Reforge source MAC address.
   [--dst-mac] <mac>				| Reforge destination MAC address.
   [--src-ip] <ip>				| Reforge source IP address.
   [--dst-ip] <ip>				| Reforge destination IP address.
   [--src-port] <port>				| Reforge source port.
   [--dst-port] <port>				| Reforge destination port.
   						| (comma-separated list in case of multiple egress 
   						| interfaces, one per interface)
   
   OTHER
   
   [--simulation-mode|-e]			| Simulation mode (disk simulation, synthetic traffic is
   						| generated).
   [--unprivileged-user|-u] <username>		| Use <username> instead of nobody when dropping
   						| privileges.
   [--pid|-P] <path>				| Save the pid into the specified path.
   [--version|-V]				| Print application version.
   [--help|-h]					| Help.
   [--verbose|-v]				| Verbose.
   [--show-system-id]				| Print the system identifier.
   [--check-license]				| Checks if the license is present and valid.
   [--syslog|-l]				| Dump trace messages to syslog.
   [--event-log|-Q] <file>			| Save relevant events onto the specified file.

Example:

.. code-block:: console

   disk2n -i zc:eth1 -m playlist.txt -C 4096 -b 2000 -S 0 -c 1 -w 2

