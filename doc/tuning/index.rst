Tuning
======

In order to achieve a good n2disk setup able to obtain the maximum performance, it is important to take into account the following aspects.

Besides libpcap, n2disk can take advantage from the PF_RING (standard or ZC) acceleration to capture packets from a live network interface.

For ingress traffic rate above 2-3 Gbit/s, PF_RING ZC is recommended. If you have to merge multiple 10 Gbit/s links into a single dumpset, PF_RING FPGA is recommended, using a card that features aggregation in hardware.

In case standard PF_RING is used it is particularly important to reserve enough ring buffer space inside the kernel. Furthermore, in order to reduce the number of clock-cycles needed to capture the packets and cross the network stack, it is possible to turn on quick_mode in the pf_ring kernel module configuration.

For further info about PF_RING and ZC please read the PF_RING_ documentation.

.. _PF_RING: http://www.ntop.org/guides/pf_ring

Regarding the n2disk start-up parameters particularly important are the following options:

- The maximum file size (-p) should not be very small. A good value has to be more than 64 Mbytes, up to 2GB if traffic rate is above 10Gbit/s.
- The buffer length (-b) has to be big enough. 2GB is sufficient in most cases, better to go up to 8GB when the ingress rate is above 10Gbit/s. In any case this should be a multiple of the maximum file size (-p).
- The storage path (-o)
- The disk limit (--disk-limit) should be set to the maximum disk space to be used for pcap files. For dedicated storage it is recommended to set this to 80% (considering that some additional space is used for index files).
- The write chunk size (-C) has to be greater than or equal to 64 Kbytes, the default value is usually fine.
- The core binding for the reader (-c) and writer (-w) thread. It is highly recommended to bind those threads to different core of the same physical CPU (according to the system topology, you should choose the physical CPU where the network card is closer/directly connected).
- If you are using indexing (-I), which is required by the extraction tool, it is a good practice to enable dedicated indexing threads setting the core affinity (-z <cores> -Z).
- The timeline path (-A) for organizing pcpa and index files and sort them by date (required by the extraction tools).
- If you have a primary/fast storage (used as a cache, to keep up with peak hours) and a secondary/slower storage (used to move data as soon as possible) you can use the -O option to enable archiving on the second storagei (optional).


.. toctree::
    :maxdepth: 2
    :numbered:

    examples
    napatech
    fiberblaze
