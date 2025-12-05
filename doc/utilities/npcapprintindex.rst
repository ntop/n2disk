npcapprintindex
===============

This utility reads and displays the contents of an npcap index file (.idx) created by n2disk or npcapindex. It can print packet information in human-readable or JSON format, check index integrity, and display statistics about indexed traffic.

npcapprintindex is useful for debugging index files, verifying capture data, analyzing traffic patterns, and extracting metadata without processing the full pcap file.

Usage
-----

.. code-block:: text

   npcapprintindex -i <index file> [options]

Available options are:

.. code-block:: text

   [-i] <index file>     | Index file to read (.idx file)
   [-j]                  | JSON output format
   [-c]                  | Check index integrity only (no packet printing)
   [-n] <num>            | Print first num packets only
   [-T]                  | Print first and last timestamps only
   [-v]                  | Verbose mode (show index details)
   [-h]                  | Help

Index Information
-----------------

npcapprintindex can read index files in both V1 and V2 formats and supports the following index types:

- **Standard packet index**: Contains packet digests with IP/port information
- **GTP index**: Includes both outer and tunneled (inner) packet information

The tool automatically detects and displays:

- Index version (V1 or V2)
- Timestamp precision (microsecond or nanosecond)
- Optional fields (device ID, port ID)
- Bloom filter bitmaps (MAC, port, IP, L7 protocol, device)
- Compression format used for the index

Output Formats
--------------

**Human-Readable Format (default)**

Displays each packet with:

- Packet number
- Timestamp (HH:MM:SS.microsec or HH:MM:SS.nanosec)
- Packet length
- VLAN IDs (single and QinQ)
- IP version (IPv4 or IPv6)
- Protocol number
- Source and destination IPs
- Source and destination ports
- Layer 7 protocol (when nDPI is available and index V2)
- GTP tunneled packet info (for GTP indexes)

**JSON Format (-j)**

Outputs structured JSON with packet metadata for easy parsing and integration with other tools.

Example Usage
-------------

Display all packets from an index file:

.. code-block:: console

   npcapprintindex -i /storage/dump.pcap.idx

Display packets in JSON format:

.. code-block:: console

   npcapprintindex -i /storage/dump.pcap.idx -j

Check index integrity without printing packets:

.. code-block:: console

   npcapprintindex -i /storage/dump.pcap.idx -c

Display first 100 packets only:

.. code-block:: console

   npcapprintindex -i /storage/dump.pcap.idx -n 100

Show first and last packet timestamps:

.. code-block:: console

   npcapprintindex -i /storage/dump.pcap.idx -T

Display index details with verbose mode:

.. code-block:: console

   npcapprintindex -i /storage/dump.pcap.idx -v

Example Output
--------------

**Human-Readable Format:**

.. code-block:: text

   Found index version 2 [packet index]
   Index includes timestamps
   1) [14:23:45.123456] len: 74, vlan: 0, vlan_qinq: 0, ipv4, proto: 6, 192.168.1.100:49152 -> 10.0.0.1:80, l7proto: HTTP
   2) [14:23:45.123789] len: 60, vlan: 0, vlan_qinq: 0, ipv4, proto: 6, 10.0.0.1:80 -> 192.168.1.100:49152, l7proto: HTTP
   3) [14:23:45.124012] len: 1514, vlan: 0, vlan_qinq: 0, ipv4, proto: 6, 10.0.0.1:80 -> 192.168.1.100:49152, l7proto: HTTP

**JSON Format:**

.. code-block:: json

   { "epoch" : 1234567890.123456, "length" : 74, "tuple" : { "vlan" : 0, "vlan_qinq" : 0, "ip_version" : 4, "protocol" : 6, "src_ip" : "192.168.1.100", "dst_ip" : "10.0.0.1", "src_port" : 49152, "dst_port" : 80, "l7protocol" : "HTTP" } }

**Timestamp-Only Output (-T):**

.. code-block:: text

   First-TS [14:23:45.123456]
   Last-TS  [18:45:12.987654]

Statistics
----------

When compiled with nDPI support and using index V2, npcapprintindex displays Layer 7 protocol statistics at the end:

.. code-block:: text

   --- Protocols stats ---
   HTTP: 1523 pkts (45.32%)
   TLS: 892 pkts (26.54%)
   DNS: 421 pkts (12.53%)
   SSH: 267 pkts (7.95%)
   Unknown: 257 pkts (7.65%)

Index Integrity Check
---------------------

Use the ``-c`` flag to verify index file integrity without printing packets. This is useful for:

- Validating index files after copy/move operations
- Checking for corruption
- Automated health checks in scripts

.. code-block:: console

   npcapprintindex -i /storage/dump.pcap.idx -c
   echo $?  # Returns 0 if index is valid, non-zero otherwise

The check validates:

- Index magic number and version
- Compression integrity
- Expected vs actual digest sizes
- Bitmap structure (if present)

IPv6 Representation
-------------------

Due to index optimization, IPv6 addresses are hashed and displayed in abbreviated form showing only the last 4 bytes:

.. code-block:: text

   *:a1b2:c3d4:45152 -> *:e5f6:7890:443

This is sufficient for most debugging purposes and allows compact index storage.

GTP Tunnel Information
----------------------

For GTP indexes, both outer and tunneled packet information is displayed:

.. code-block:: text

   vlan: 0, ipv4, proto: 17, 10.1.1.1:2152 -> 10.1.1.2:2152, tunneled ipv4, proto: 6, 192.168.1.100:49152 -> 172.16.0.1:80

Examples
--------

**Extract packet count:**

.. code-block:: bash

   npcapprintindex -i dump.pcap.idx | tail -1 | cut -d')' -f1

**Extract time range:**

.. code-block:: bash

   npcapprintindex -i dump.pcap.idx -T

Troubleshooting
---------------

**Error: "Invalid index version"**

The index file may be corrupted or incompatible. Regenerate using:

.. code-block:: console

   npcapindex -i original.pcap -o new.pcap.idx

**Error: "Index too short"**

Index file is truncated or corrupted. Check disk space and file permissions.

**Error: "Decompression error"**

Index compression is corrupted. Regenerate the index file.

**No Layer 7 protocol information**

Either:
- The index is version V1 (only V2 includes L7 info)
- npcapprintindex was compiled without nDPI support
- The index was created without nDPI

Compatibility
-------------

npcapprintindex works with index files created by:
- n2disk (all versions with indexing enabled)
- npcapindex utility

