Performance
===========

The n2disk performance is influenced by two factors:

- Packet capture performance
- Disk write performance

Unless you have both fast capture and write performance, n2disk will not operate properly.  Please check the Tuning section and the PF_RING documentation to learn how to use ZC drivers to accelerate packet capture performance.

We have implemented in n2disk a testing facility for allowing you to determine if your system has optimal performance. We suggest you to run this performance test prior to use n2disk. n2disk comes with the -e command line option

.. code-block:: text

   [--simulation-mode|-e] <mode> | Simulation mode (debug only)
                                 | 1 - Test max dump speed (simulate capture)
                                 | 2 - Test max capture speed (simulate storage)

The -e option can have two mode:

- 1: packet capture is simulated. Namely n2disk simulates packet capture so that n2disk does not spend time on capturing and thus the capture speed is virtually unlimited. In this mode we can evaluate the disk write performance as packet capture costs virtually zero and it has virtually and infinite speed.
- 2: write to disk is simulated. Namely n2disk does not save packets on disk but it reports that the pcap file has been saved. This way no time is spent writing on disk, and thus we can measure the packet capture performance. Note that in this setup, you must inject traffic on the ingress (-i) n2disk monitoring interface.

In general, disk-write speed should exceed network speed (or packet capture speed) in order to guarantee that all captured packets can be written to disk. Note that disk performance does not depend just on hardware, but also on filesystem type and configuration. Please check the Disk Partitioning and Formatting section for more details on this subject.

