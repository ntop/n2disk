ndedup
======

ndedup is a tool that implements packet deduplication. Packet deduplication is an important feature
cleaning up and optimizing the flow of traffic towards monitoring and security tools by removing 
redundant packets. Duplicated packets may be present due to many factors, for instance when using
SPAN ports for monitoring multiple links.
Duplicate packets represents a problem for the monitoring tool because analyzing duplicate packets
produces wrong numbers in your analytics, in addition to requiring higher processing and space 
resources for the monitoring system.

ndedup is a tool that works inline, as a transparent bridge. When a packet comes in the tool uses
one-way encryption to turn the packet into a strong collision-resistant hash value. This hash is 
compared to the hash of every packet that came before it in a pre-configured time window (e.g. last
50 msec), all this at really high speed (up to 100 Gbps). This makes this tool deployable in front
of any appliance running network monitoring software (including n2disk).

This tool can run on top of any adapter (Intel, Mellanox, Napatech) and can scale up the performance
by leveraging on the RSS technology.

Supported options:

.. code-block:: text

   -i <device>     Ingress device name. Multiple interfaces/queues supported:
                   - Multiple -i to run on multiple segments (e.g. -i eth0@0 -i eth0@1 -o eth1@0 -o eth1@1)
                   - RSS short notation (e.g. -i eth0@[0-3] means -i eth0@0 -i eth0@1 -i eth0@2 -i eth0@3)
   -o <device>     Egress device name (see -i for supported notations)
   -c <cluster id> cluster id
   -b              Bridge mode (forward in both directions)
   -B              Enable burst API
   -g <core id>    Bind this app to a core (with -b use <core id>:<core id>)
   -a              Active packet wait
   -f              Flush packets immediately
   -d <msec>       Deduplication window size (msec)
   -s <gbps>       Link Speed (default: 10 Gbps)
   -S <core id>    Enable Time Pulse thread and bind it to a core
   -R <nsec>       Time resolution (nsec) when using Time Pulse thread
                   Note: in non-time-sensitive applications use >= 100usec to reduce cpu load
   -v              Verbose

Example (Intel ZC):

.. code-block:: console

   ndedup -i zc:eth1 -o zc:eth2 -d 10 -s 10 -S 0 -g 1 -B

Example (RSS = 4):

.. code-block:: console

   ndedup -i zc:eth1@[0-3] -o zc:eth2@[0-3] -d 10 -s 10 -S 0 -g 1:2:3:4 -B

