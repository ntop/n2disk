Examples
========

Dump 10-20 Gbps
~~~~~~~~~~~~~~~

Requirement: 4-core 3Ghz system.

Command line:

.. code-block:: console

   n2disk -i zc:eth1 -o /storage/n2disk/pcap -b 8192 -p 1024 --disk-limit 80% -S 0 -c 1 -w 2

Corresponding configuration file:

.. code-block:: console

   --interface=zc:eth1
   --dump-directory=/storage/n2disk/pcap
   --disk-limit=80%
   --max-file-len=1024
   --buffer-len=8192
   --reader-cpu-affinity=1
   --writer-cpu-affinity=2
   --time-pulse=0

Dump and Index 10-20 Gbps
~~~~~~~~~~~~~~~~~~~~~~~~~

Requirement: 6-core 3Ghz system.

Command line:

.. code-block:: console

   n2disk -i zc:eth1 -o /storage/n2disk/pcap -I -A /storage/n2disk/timeline -b 8192 -p 1024 --disk-limit 80% -S 0 -c 1 -w 2 -k 3,4,5

Configuration file example for dumping and indexing 10-20 Gbps on a 6-core system:

.. code-block:: text

   --interface=zc:eth1
   --dump-directory=/storage/n2disk/pcap
   --timeline-dir=/storage/n2disk/timeline
   --disk-limit=80%
   --max-file-len=1024
   --buffer-len=8192
   --index
   --reader-cpu-affinity=1
   --writer-cpu-affinity=2
   --indexer-cpu-affinity=3,4,5
   --time-pulse=0

Dump and Index at 20-100Gbps
~~~~~~~~~~~~~~~~~~~~~~~~~~~~

Dumping traffic at 100 Gbps requires multiple n2disk instances, or a single instance with multiple streams.

FPGAs
"""""

On specialized FPGA adapters supporting chunk/segment/block PCAP mode like Napatech or Silicom FPGA, it is
possible to capture up to 50 Gbps per instance or stream (100 Gbps requires 2 streams).

PCAP mode is required for best performance. In this mode the adapter moves PCAP data in chunks, optimizing
data transfer on the bus and reducing the CPU load for copying and processing the data.
Please check the 'Napatech Setup' and 'Silicom FPGA Setup' sections for instructions about configuring segment PCAP mode on those adapters.

In order to handle 50 Gbps a single port/stream capture usually delivers enough performance.

Hardware requirement: 6 cores 3Ghz CPU.

n2disk configuration example:

.. code-block:: text

   --interface=nt:0
   --dump-directory=/storage/n2disk
   --timeline-dir=/storage/n2disk/timeline
   --disk-limit=80%
   --max-file-len=1024
   --buffer-len=4096
   --index
   --reader-cpu-affinity=0
   --writer-cpu-affinity=1
   --indexer-cpu-affinity=2,3,4,5

In order to handle 100 Gbps the adapter should be configured with 2 streams.

Hardware requirement: 12 cores 3Ghz CPU.

Tested with no packet loss up to 148 Mpps 100 Gbps (average packet size of 60 bytes).

Streams configuration example:

.. code-block:: text

   /opt/napatech3/bin/ntpl -e "Delete=All"
   /opt/napatech3/bin/ntpl -e "HashMode = Hash5TupleSorted"
   /opt/napatech3/bin/ntpl -e "Setup[NUMANode=0]=Streamid==(0..1)"
   /opt/napatech3/bin/ntpl -e "Assign[streamid=(0..1)] = All" 

n2disk configuration example (stream 0):

.. code-block:: text

   --interface=nt:stream0
   --dump-directory=/storage1/n2disk/pcap
   --dump-directory=/storage2/n2disk/pcap
   --dump-directory=/storage3/n2disk/pcap
   --dump-directory=/storage4/n2disk/pcap
   --timeline-dir=/storage1/n2disk/timeline
   --disk-limit=80%
   --max-file-len=2048
   --buffer-len=16384
   --index
   --reader-cpu-affinity=1
   --indexer-cpu-affinity=2,3,4,5
   --writer-cpu-affinity=0,0,0,0

n2disk configuration example (stream 1):

.. code-block:: text

   --interface=nt:stream1
   --dump-directory=/storage5/n2disk/pcap
   --dump-directory=/storage6/n2disk/pcap
   --dump-directory=/storage7/n2disk/pcap
   --dump-directory=/storage8/n2disk/pcap
   --timeline-dir=/storage5/n2disk/timeline
   --disk-limit=80%
   --max-file-len=2048
   --buffer-len=16384
   --index
   --reader-cpu-affinity=6
   --indexer-cpu-affinity=7,8,9,10
   --writer-cpu-affinity=0,0,0,0

ASICs
"""""

On ASIC adapters, on adequate hardware, it is possible to capture and dump up to 10-20 Gbps per instance or stream.

In order to handle more than 20 Gbps on ASIC adapters (NVIDIA/Mellanox or Intel), n2disk 3.7 and later
includes native support for RSS. RSS should be enabled in the adapter and n2disk can be configured to 
capture from multiple RSS streams. Please note that this requires the configuration
of multiple dump directories and timelines (one per RSS stream), the npcapextract tool can be used at 
extraction time to extract traffic from all the timelines, using the hardware timestamp with nanosecond
resolution (when supported, e.g. on NVIDIA ConnectX and Intel E810) for merging packets in the proper order.

In order to handle 100 Gbps the adapter should be configured with 8 RSS queues/streams.

Hardware requirement: 16-core 3Ghz system (e.g. Xeon Gold 6526Y).

Tested with no packet loss up to 102 Mpps 100 Gbps (average packet size of 100 bytes).

RSS configuration example:

.. code-block:: text

   ethtool -L ens4f1np1 combined 8

n2disk configuration example:

.. code-block:: text

   --interface=mlx:mlx_5@[0-7]
   --dump-directory=/storage1/n2disk/pcap
   --dump-directory=/storage2/n2disk/pcap
   --dump-directory=/storage3/n2disk/pcap
   --dump-directory=/storage4/n2disk/pcap
   --dump-directory=/storage5/n2disk/pcap
   --dump-directory=/storage6/n2disk/pcap
   --dump-directory=/storage7/n2disk/pcap
   --dump-directory=/storage8/n2disk/pcap
   --timeline-dir=/storage1/n2disk/timeline
   --timeline-dir=/storage2/n2disk/timeline
   --timeline-dir=/storage3/n2disk/timeline
   --timeline-dir=/storage4/n2disk/timeline
   --timeline-dir=/storage5/n2disk/timeline
   --timeline-dir=/storage6/n2disk/timeline
   --timeline-dir=/storage7/n2disk/timeline
   --timeline-dir=/storage8/n2disk/timeline
   --disk-limit=80%
   --max-file-len=1024
   --buffer-len=8192
   --index
   --writer-cpu-affinity=0
   --writer-cpu-affinity=0
   --writer-cpu-affinity=0
   --writer-cpu-affinity=0
   --writer-cpu-affinity=0
   --writer-cpu-affinity=0
   --writer-cpu-affinity=0
   --writer-cpu-affinity=0
   --reader-cpu-affinity=1
   --reader-cpu-affinity=2
   --reader-cpu-affinity=3
   --reader-cpu-affinity=4
   --reader-cpu-affinity=5
   --reader-cpu-affinity=6
   --reader-cpu-affinity=7
   --reader-cpu-affinity=8
   --indexer-cpu-affinity=9,10
   --indexer-cpu-affinity=11,12
   --indexer-cpu-affinity=13,14
   --indexer-cpu-affinity=15,0
   --indexer-cpu-affinity=25,26
   --indexer-cpu-affinity=27,28
   --indexer-cpu-affinity=29,30
   --indexer-cpu-affinity=31,16

