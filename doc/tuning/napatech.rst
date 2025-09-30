Napatech Setup
==============

In order to have n2disk working with the highest performances on Napatech adapters, the system need to be properly configured.
Once the Napatech software has been installed, you should customise the /opt/napatech3/config/ntservice.ini configuration file modifying the parameters as below.

Standard PCAP with microsecond timestamps:

.. code-block:: text

   TimestampFormat = PCAP

PCAP with nanosecond timestamps:

.. code-block:: text

   TimestampFormat = PCAP_NS

Packet descriptor:

.. code-block:: text

   PacketDescriptor = PCAP

Segment size (maximum 4MB):

.. code-block:: text

   HostBufferSegmentSizeRx = 4

Optionally, increase RX host buffer to 128 MB:

.. code-block:: text

   HostBuffersRx = [4,128,0]

Once customisation has been completed, the Napatech driver can be started with:

.. code-block:: console

   /opt/napatech3/bin/ntstart.sh

The PF_RING driver can be loaded using systemctl:

.. code-block:: console

   systemctl start pf_ring

Now you are ready to start n2disk:

.. code-block:: console

   n2disk -o /storage -p 1000 -b 4000 -i nt:0 -q 1 -C 1024 -c 1 -z 2 -w 3 -n 50 -m 100

where nt:0 means Napatech port 0.

The Napatech card can be instructed to create a stream, for instance aggregating two ports. It is possible to open a stream using nt:stream<ID> as interface name. Example:

.. code-block:: console

   ntpl -e "Assign[streamid=0] = port == 0,1"
   
   n2disk -o /storage -p 1000 -b 4000 -i nt:stream0 -q 1 -C 1024 -w 0 -c 1 --disk-limit 80%
