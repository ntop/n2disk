Usage
=====

In order to save all the traffic into disks, the n2disk application has to be activated on an interface from which it is possible to capture all the traffic you are interested in. Once activated, n2disk will save the traffic data into the specified directory recycling the files already written, starting from the oldest one, this in case the maximum number of created files is reached.

In the following sections, we discuss all the n2disk command line options and how to efficiently configure n2disk to capture all the traffic flowing in your network. 

Command Line Options
--------------------

Below the available options and a detailed explanation of each option are listed:

CAPTURE SETTINGS
~~~~~~~~~~~~~~~~

.. code-block:: text

   [--interface|-i] <device>                     | Ingress packet device. This option supports:
                                                 | - Comma-separated list to aggregate multiple interfaces (e.g. -i eth0,eth1)
                                                 | - Multiple -i to span multiple concurrent captures (e.g. -i eth0 -i eth1)
                                                 | - RSS short notation (e.g. -i eth0@[0-3] means -i eth0@0 -i eth0@1 -i eth0@2 -i eth0@3)
   [--active-wait|-g]                            | Active packet polling.
   [--poll-duration|-q] <poll>                   | Poll time (msec): default 1. Lower more CPU
                                                 | is used, better response time.
   [--snaplen|-s] <len>                          | Max packet capture length. Default: 1536.
   [--packet-slicing|-0] <header level>          | Slice packet after the specified header.
                                                 | 2 - Dump up to L2
                                                 | 3 - Dump up to L3
                                                 | 4 - Dump up to L4
   [--strip-header-bytes|-Y] <len>               | Strip <len> bytes from packet header.
   [--sample-rate|-y] <rate>                     | Packet sample rate (e.g. 100 means 1:100 sampling).
   [--capture-direction|-2] <direction>          | Capture direction: 0=RX+TX, 1=RX only (default), 2=TX only
   [--not-promisc|-3]                            | Do not set the interface in promiscuous mode.
   [--dump-fcs]                                  | Dump the Frame Check Sequence (FCS/CRC)
   [--kernel-cluster-id|-K] <id>                 | Cluster ID for PF_RING Kernel clustering
   [--disable-rss]                               | Send all traffic to queue 0, even if RSS is enabled

FILTERING
~~~~~~~~~

.. code-block:: text

   [--filter|-f] <filter>                        | BPF ingress packet filter (tcpdump-like).
   [--fast-filter|-F] <filter>                   | Faster replacement for BPF ingress packet filter.
                                                 | NOTE: If <filter> is a file path, the filter is read from it.
   [--l7-filter-conf] <conf file>                | FT configuration file for L7 filtering.

TIMESTAMPING
~~~~~~~~~~~~

.. code-block:: text

   [--hw-timestamp|-J] <mode>                    | Hardware timestamp packet strip. Supported modes:
                                                 | arista - Timestamped packets by Arista 7150 series devices.
                                                 | ixia - Timestamped packets by ixiacom.com hardware devices.
                                                 | metawatch - Timestamped packets by Arista Metawatch devices.
                                                 | NOTE: Silicom Hw timestamping NICs are supported automatically.
   [--time-pulse|-S] <core id>                   | Enable time pulse thread and bind it to the specified core.
   [--time-pulse-precision] <usec>               | Specify the required timestamp precision to lower the cpu load for -S.
   [--nanoseconds|-d]                            | Dump timestamps in nsec format.
   [--no-timestamp|-T]                           | Do not compute timestamps in n2disk.

DUMP SETTINGS
~~~~~~~~~~~~~

.. code-block:: text

   [--chunk-len|-C] <len>                        | (KB) Size of the chunk written to disk (must be
                                                 | multiple of 4096). Default: 64 KB.
   [--journaling|-j]                             | Enable journaling.
   [--buffer-len|-b] <len>                       | Buffer length (MBytes).
   [--dump-directory|-o] <directory>             | Directory where dump files will be saved. Multiple -o can be specified
                                                 | to write to multiple volumes in round-robin, or in case of multiple -i.
                                                 | Multiple -o for each -i can be also specified, they are assigned in round-robin.
   [--archive-directory|-O] <directory>          | Directory where dump files will be archived (slower disks).
   [--max-file-len|-p] <len>                     | Max pcap file length (MBytes).
   [--max-file-duration|-t] <seconds>            | Max pcap file duration (sec).
   [--max-file-packets|-N] <number>              | Max packets per pcap file.
                                                 | Default is 0 that means no max duration.
   [--disk-limit|-6] <size>                      | Max amount of disk space to use (MBytes or %). Default: 80%.
   [--max-num-files|-m] <number>                 | Max number of files per directory (0 for infinite). Default: 100.
   [--max-nested-dirs|-n] <number>               | Max number of directories. Default: 0. (Deprecated: use --disk-limit)
   [--dynamic-disk-limit-check]                  | Constantly check the disk for the actual disk utilization
                                                 | Note: this adds more stress on the filesystem and should be
                                                 | used only when there are other processes using the same volume.
   [--no-overwriting|-4]                         | Do not overwrite old files to make room for new ones.
   [--file-prefix|-x] <prefix>                   | Dump file prefix (ignored with --disk-limit).
   [--no-tmp-file|-5]                            | Do not write temporary .pcap.tmp files.
   [--hugepages|-U]                              | Use hugepages for memory allocation.
   [--disable-direct-io|-r]                      | Disable Direct I/O (experts only).
   [--stop-on-limit|-L][<mode>]                  | Stop capture when the specified limit is reached depending on the mode:
                                                 | 1 - Use the single-file max len/duration/packets as limit (default)
                                                 | 2 - Use the max number of files on disk as limit.
   [--remove-ahead|-H]                           | Delete in advance dumped files that will soon be overwritten.
                                                 | Use this option to reduce file deletion cost.
   [--pcap-compression|-M]                       | Compress pcap files (experimental).
   [--file-mode <mode>]                          | Permissions for created files (default: 640)
   [--dir-mode <mode>]                           | Permissions for created folders (default: 750)

INDEX SETTINGS
~~~~~~~~~~~~~~

.. code-block:: text

   [--index|-I][<directory>]                     | Create pcap-index on-the-fly. Optionally it is possible to specify the directory
                                                 | if different from -o. Multiple -I can be specifies in case of multiple -i/-o.
   [--timeline-dir|-A] <directory>               | Time-arranged directory for produced pcaps and indexes.
                                                 | Multiple -A can be specifies in case of multiple -i.
   [--extended-index|-E] <type>                  | Extended index type (combinations are allowed using a mask):
                                                 | 1 - add per-packet timestamp
                                                 | 2 - add L7 protocol (also enabled with --zmq)
                                                 | 4 - add device and port ID (when using --hw-timestamp metawatch)
   [--index-tunnel-content|-G] <mode>            | Compute index on tunnel content (GTP-U, GRE):
                                                 | 1 - Index both inner and outer headers.
                                                 | 2 - Use inner header as outer header.
   [--exec-cmd] <cmd>                            | Execute the specified command when a pcap has been dumped.

CPU AFFINITY
~~~~~~~~~~~~

.. code-block:: text

   [--reader-cpu-affinity|-c] <core>             | Bind the reader thread to the specified core.
                                                 | Multiple -c can be specified in case of multiple -i.
   [--writer-cpu-affinity|-w] <cores>            | Bind the writer thread(s) to the specified core ids.
                                                 | A comma-separated list of cores (e.g. 0,1,2,3) should be
                                                 | specified in case of multiple dump directories (-o).
                                                 | Multiple -w can be specified in case of multiple -i.
   [--indexer-cpu-affinity|-k] <cores>           | Enable multithread indexing and bind thread(s).
                                                 | to the specified core ids (e.g. 0,1,2,3).
                                                 | Multiple -k can be specified in case of multiple -i.
                                                 | Note: the first available CPU corresponds to 0.
   [--compressor-cpu-affinity|-z] <cores>        | Enable multithread compression (and indexing) and bind thread(s)
                                                 | to the specified core ids (e.g. 0,1,2,3).
                                                 | Multiple -z can be specified in case of multiple -i.
   [--index-on-compressor-threads|-Z]            | Compute index on the same threads used for compression (-z).
   [--other-cpu-affinity] <core>                 | Bind other/service threads to the specified core.
   [--numa-affinity] <node>                      | Numa node (CPU) affinity. This is automatically computed by default.

THREADS PRIORITY
~~~~~~~~~~~~~~~~

.. code-block:: text

   [--reader-priority] <priority>                | Set the priority for the reader thread (Default: -15)
   [--writer-priority] <priority>                | Set the priority for the writer thread(s) (Default: 0)
   [--indexer-priority] <priority>               | Set the priority for the indexer thread(s) (Default: 0)
                                                 | Note: priority is a value in the range -20 to 19, where -20 is the highest.
                                                 | A priority outside this range translates into no priority set.

MICRO-BURST DETECTION
~~~~~~~~~~~~~~~~~~~~~

.. code-block:: text

   [--uburst-detection]                          | Enable microburst detection.
   [--uburst-log] <path>                         | Microbursts log file.
   [--uburst-win-size] <usec>                    | Window size for microburst check (usec).
   [--uburst-link-speed] <mbit/s>                | Link speed (Mbit/s).
   [--uburst-threshold] <percent>                | Traffic threshold wrt link speed (percentage).

FLOWS EXPORT
~~~~~~~~~~~~

.. code-block:: text

   [--zmq <socket>]                              | Deliver flows/stats to subscribers connected to the specified endpoint.
                                                 | Example tcp://*:5556 or ipc://flows.ipc
                                                 | Note: probe mode is used, configure ntopng as collector.
   [--zmq-format <format>]                       | Flows export format ('t' for TLV - default, 'j' for JSON)
   [--zmq-encryption-key] <pub key>              | Encrypt ZMQ data using the specified public key.
   [--probe-ip <address>]                        | Probe IP address exported via ZMQ (default: management IP)
   [--lifetime-timeout <sec>]                    | Set maximum flow duration (default: 120 sec)
   [--idle-timeout <sec>]                        | Set maximum flow idle_time (default: 10 sec)

OTHER
~~~~~

.. code-block:: text

   [--daemon]                                    | Daemonize this app at startup.
   [--disable-chunk-mode]                        | Do not enable chunk mode on supported adapters
   [--simulation-mode|-e] <mode>                 | Simulation mode (debug only)
                                                 | 1 - Test max dump speed (simulate capture).
                                                 | 2 - Test max capture speed (simulate storage).
                                                 | 3 - Test max capture speed (simulate storage, produce real index).
                                                 | 4 - Test both capture and storage (test the core engine).
   [--hole-detection] <msec>                     | Enable detection of traffic holes (<msec> is the min hole size).
   [--ts-lsb-interf-encoding]                    | Encode ingress interface index in LSB of the timestamp.
   [--unprivileged-user|-u] <username>           | Use <username> instead of nobody when dropping privileges.
   [--pid|-P] <path>                             | Save the pid into the specified path.
   [--version|-V]                                | Print application version.
   [--help|-h]                                   | Help.
   [--verbose|-v]                                | Verbose.
   [--show-system-id]                            | Print the system identifier
   [--check-license]                             | Checks if the license is present and valid
   [--check-maintenance]                         | Checks the maintenance status for the specified license
   [--syslog|-l]                                 | Dump trace messages to syslog
   [--trace-log] <file>                          | Dump trace messages to the specified file
   [--event-log|-Q] <file>                       | Save relevant events (i.e. drops) onto the specified file


Basic example:

.. code-block:: console

   n2disk -i eth1 -o /storage -b 1024 -p 512 --disk-limit 50% -I -A /storage

For performance tuning please read the Tuning section.

Dump Set Format
---------------

n2disk uses the industry standard PCAP file format to dump packets into files, 
so the resulting output can be easily integrated with existing third party and 
Open Source analysis tools like *ntopng* and *Wireshark*.

PCAP files are indexed and organized in a timeline to enable on-demand retrieval, 
specifying time interval and BPF criteria.

n2disk by default changes the user to n2disk. This can be changed using the 
[--unprivileged-user|-u] USER option. As a consequence of this:

- PCAP files are created as 0640 *drwxr-x--- n2disk n2disk*
- Folders are created as 0750 *-rw-r----- n2disk n2disk*
- In order to read pcap files you need to use n2disk or the n2disk group (npcapextract also supports PAM, please take a look at the *Utilities* -> *npcapextract* section)
