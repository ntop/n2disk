Packet Filtering
================

L2/L3/L4 Filtering
------------------

BPF-like filters can be specified using a subset of the BPF syntax. 
As the filter expression complexity affects:

- packet capture speed when used for filtering incoming traffic
- index complexity and speed when used for filtering dumped traffic

Below we define a set of constraints and allowed expressions.

An expression consists of one or more primitives. 
Complex filter expressions are built by using **AND**, **OR** and **NOT** operators.

Allowed qualifiers for primitive expressions:

Protocol: **ether**, **ip**, **ip6**, **tcp**, **udp**, **sctp**
Direction: **src**, **dst**, **src or dst**, **src and dst**
Type: **host**, **net**, **port**

Other allowed primitives: **vlan**

Primitive filter examples:

.. code-block:: text

   ether host 00:11:22:33:44:55
   ether src host 00:11:22:33:44:55
   
   ip host 192.168.0.1
   ip dst host 192.168.0.1
   
   ip6 host 2001:0db8:85a3:0042:0000:8a2e:0370:7334
   ip6 src host 2001:0db8:85a3:0042:0000:8a2e:0370:7334
   
   ip net 192.168.1.0/24
   ip src net 192.168.1
   
   port 80
   udp port 9000
   tcp src port 80
   vlan 32

Complex capture filter examples:

.. code-block:: text

   ip host 192.168.1.1 and 192.168.1.2
   ip src 192.168.1.1 and dst 192.168.1.2
   
   ip host 192.168.1.1 and tcp port (80 or 443)
   (ip host 192.168.1.1 or 192.168.1.2) and (port 80 or 443)

Capture Filters
~~~~~~~~~~~~~~~

Please note that there are additional constraints when using BPF-like 
filters for packet capture: 

- it is not possible to use the **NOT** operator.
- it is possible to use up to two level of nesting with parenthesis
- on the same nesting level, and inside the same parenthesis, it is not possible to mix different operators

Extraction Filters
~~~~~~~~~~~~~~~~~~

Please find below a few tricks to speed up packet extraction when 
using BPF-like filters for retrieving traffic:

- Use /32 and /24 IPv4
- Use /128 only IPv6
- Use exact ports (no port range)
- Do not use protocol identifiers

L7 Filtering
------------

Filtering traffic based on the application protocol is also 
possible both at capture and extraction time, by leveraging
on the PF_RING FT support, which uses in turn the nDPI library.

.. note::

   n2disk 10/40/100G already includes a PF_RING FT license. n2disk 1/5G instead
   require a PF_RING FT license for enabling Layer 7 indexing/filtering.

Capture Filters
~~~~~~~~~~~~~~~

In order to enable L7 filtering at capture time, all you need to do 
is to provide a configuration file with PF_RING FT filtering rules to
n2disk using the *--l7-filter-conf <file>* option. This file contains 
the protocols that you want to discard ('discard' action) or dump ('forward' action). 
Since protocol detection is provided by `nDPI <https://www.ntop.org/products/deep-packet-inspection/ndpi/>`_ 
you can specify all application protocol names supported by the nDPI
library.
Adaptive (per application) packet slicing is also supported, by configuring
the protocols for which packets should be sliced removing the payload ('slice' action).

Example:

.. code-block:: text

   [global]
   default = forward
   
   [filter]
   YouTube = discard
   Netflix = discard
   SSH = slice

Please note that the dumped PCAP may still contain some packet for
communications matching discarded protocols. This is expected and
depends on the way Deep Packet Inspection works: it has to inspect
a few packets at the beginning of each communication, before being
able to identify with a reasonable degree of confidence the L7 protocol.
For this reason, the initial packets of any communication are dumped
to disk, until the L7 protocol is detected. This is meant to be used
to save disk space for unwanted traffic, thus having a small percentage
of unwanted traffic to disk is usually acceptable.

Example with 'default = discard' and 'MDNS = forward':

Packets on the Network:

.. code-block:: text

   	Unknown              packets: 567           bytes: 64030         flows: 4            
   	DNS                  packets: 8             bytes: 864           flows: 2            
   	MDNS                 packets: 6             bytes: 480           flows: 2            
   	SSDP                 packets: 8             bytes: 1728          flows: 2            
   	SSH                  packets: 1755          bytes: 702890        flows: 4            
   	Dropbox              packets: 3             bytes: 639           flows: 1  

Dumped packets:

.. code-block:: text

   	Unknown              packets: 30            bytes: 4549          flows: 4            
   	DNS                  packets: 8             bytes: 864           flows: 2            
   	MDNS                 packets: 6             bytes: 480           flows: 2            
   	SSH                  packets: 32            bytes: 7490          flows: 4   

Please check the `PF_RING FT <http://www.ntop.org/guides/pf_ring/ft.html>`_ 
documentation for further information about PF_RING FT and enhanced configurations.

Extraction Filters
~~~~~~~~~~~~~~~~~~

The BPF syntax for extraction filters contains additional primitives
that can leverage on index extentions to match traffic based on extra attributes

- Layer 7 application protocol: **l7proto <protocol>** is available when using the *--extended-index|-E 2* option (please check the help or the *Usage* section for more options).
- Device and Interface ID: **device <ID>**, **interface <ID>** are available when device and port information are added to the packet as metadata by external packet brokers. Arista Metawatch devices export Metamako trailers encoding the device ID and port ID, they can be used by n2disk by adding *--extended-index 4* and *--hw-timestamp metawatch* to the configuration.
- Tunneling: **inner**, **outer** are available when enabling —index-tunnel-content|-G

Example using Layer 7 indexing and extraction:

.. code-block:: console

   n2disk -i eth1 -o /storage -b 1024 -p 512 --disk-limit 50% -I -E 2 -A /storage -Z -w 0 -r 1 -z 1

At this point you are able to filter traffic based on the application
protocol using BPF-like filters in the *npcapextract* tool as described
in the *Utilities* / *npcapextract* section. The *l7proto* primitive 
should be used for this, as in the filter example below:

.. code-block:: text

   ip host 192.168.1.1 and l7proto SSH

Since protocol detection is provided by `nDPI <https://www.ntop.org/products/deep-packet-inspection/ndpi/>`_
also for the index, you can specify all application protocol names 
supported by the nDPI library.
Please find below a full example using *npcapextract* to retrieve *SSH*
traffic generated by *192.168.1.1*.

.. code-block:: console

   npcapextract -t /storage -b "2020-09-16 12:05:32" -e "2020-09-16 12:10:32" -o output.pcap -f "ip host 192.168.1.1 and l7proto SSH"


