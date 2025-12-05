npcaprepair
===========

This utility checks and repairs corrupted or incomplete dump sets created by n2disk, including missing indexes and broken timeline links. It scans a storage directory, validates each pcap file, regenerates missing index files, and recreates the timeline symbolic link structure.

npcaprepair is essential for:

- Recovering from interrupted n2disk captures
- Fixing broken timeline structures after storage migration
- Regenerating missing index files
- Repairing corrupted dump sets after system crashes or disk errors

Usage
-----

.. code-block:: text

   npcaprepair <storage path> <timeline path>

Arguments:

- **storage path**: Directory containing pcap/npcap files (searched up to 2 levels deep)
- **timeline path**: Root directory for the timeline structure

Prerequisites
-------------

npcaprepair requires the following tools to be installed:

- **tshark**: For reading packet timestamps from pcap files

Install tshark (Wireshark command-line tool):

.. code-block:: console

   # Debian/Ubuntu
   sudo apt-get install tshark

   # Red Hat/CentOS
   sudo yum install wireshark

How It Works
------------

For each pcap/npcap file found in the storage directory, npcaprepair performs the following checks and repairs:

1. **File Discovery**:

   - Recursively scans storage directory (max depth: 2 levels)
   - Identifies all ``.pcap`` and ``.npcap`` files

2. **Timestamp Extraction**:

   - Uses tshark to read the timestamp of the first packet
   - Extracts date, time, and epoch information
   - Determines the appropriate 10-minute timeline slot

3. **Index Validation/Creation**:

   - Checks if ``.idx`` file exists
   - Validates index integrity using ``npcapprintindex -c``
   - Regenerates index if missing or corrupted using ``npcapindex``

4. **Timeline Link Creation**:

   - Creates timeline directory structure: ``YYYY/MM/DD/HH/MM`` (10-minute slots)
   - Creates symbolic link from timeline to pcap file: ``timeline/YYYY/MM/DD/HH/MM/epoch.pcap``
   - Creates reverse symbolic link from pcap to timeline: ``storage/file.pcap.timeline``
   - Repeats for index files: ``timeline/YYYY/MM/DD/HH/MM/epoch.pcap.idx`` and ``storage/file.pcap.idx.timeline``

5. **Link Type**:

   - Uses relative symbolic links by default (portable and recommended)
   - Fallback to absolute links if relative path calculation fails

Example Usage
-------------

Repair dump set with timeline:

.. code-block:: console

   npcaprepair /storage/n2disk /storage/n2disk/timeline

Repair after storage migration:

.. code-block:: console

   # After moving storage to new location
   npcaprepair /new-storage /new-storage/timeline

Fix broken timeline after manual file operations:

.. code-block:: console

   npcaprepair /data/pcaps /data/timeline

Example Output
--------------

.. code-block:: text

   Checking storage /storage/n2disk and timeline /storage/n2disk/timeline
   Checking /storage/n2disk/1632826800.123456.pcap [date 2021/09/28] [time 14:20:00.123456] [epoch 1632826800.123456] [slot 20]
   Creating index /storage/n2disk/1632826800.123456.pcap.idx
   Creating timeline slot /storage/n2disk/timeline/2021/09/28/14/20
   Creating link /storage/n2disk/timeline/2021/09/28/14/20/1632826800.123456.pcap
   Creating link /storage/n2disk/1632826800.123456.pcap.timeline
   Creating link /storage/n2disk/timeline/2021/09/28/14/20/1632826800.123456.pcap.idx
   Creating link /storage/n2disk/1632826800.123456.pcap.idx.timeline
   Checking /storage/n2disk/1632827400.654321.pcap [date 2021/09/28] [time 14:30:00.654321] [epoch 1632827400.654321] [slot 30]

Timeline Structure
------------------

npcaprepair creates a hierarchical timeline directory structure with 10-minute granularity:

.. code-block:: text

   timeline/
   └── 2021/
       └── 09/
           └── 28/
               └── 14/
                   ├── 20/
                   │   ├── 1632826800.123456.pcap -> ../../../../../../storage/file.pcap
                   │   └── 1632826800.123456.pcap.idx -> ../../../../../../storage/file.pcap.idx
                   └── 30/
                       ├── 1632827400.654321.pcap -> ../../../../../../storage/file2.pcap
                       └── 1632827400.654321.pcap.idx -> ../../../../../../storage/file2.pcap.idx

Reverse links in storage:

.. code-block:: text

   storage/
   ├── file.pcap
   ├── file.pcap.idx
   ├── file.pcap.timeline -> ../timeline/2021/09/28/14/20/1632826800.123456.pcap
   └── file.pcap.idx.timeline -> ../timeline/2021/09/28/14/20/1632826800.123456.pcap.idx

Use Cases
---------

**Scenario 1: Broken Timeline After Migration**

After moving storage to a new location, timeline links break:

.. code-block:: console

   # Remove old broken timeline
   rm -rf /old-location/timeline

   # Create new timeline structure
   npcaprepair /new-location/storage /new-location/timeline

**Scenario 2: Manual File Additions**

After manually copying pcap files into storage:

.. code-block:: console

   # Add new files to timeline
   npcaprepair /storage /storage/timeline

**Scenario 3: Filesystem Corruption Recovery**

After filesystem repair or recovery:

.. code-block:: console

   # Validate and repair all indexes and links
   npcaprepair /recovered-storage /recovered-storage/timeline

Important Notes
---------------

- **File Naming**: Pcap files are renamed in the timeline using epoch timestamps extracted from the first packet
- **10-Minute Slots**: Timeline uses 10-minute granularity (00, 10, 20, 30, 40, 50 minutes)
- **Relative Links**: Uses relative symbolic links by default for portability
- **Depth Limit**: Only scans 2 directory levels deep in storage (use ``-maxdepth`` in the script to adjust)
- **Timestamp Source**: Uses the first packet's timestamp to determine file placement
- **Idempotent**: Safe to run multiple times; skips existing valid indexes and links
- **Non-Destructive**: Does not modify or delete existing valid files

Compatibility
-------------

npcaprepair works with:

- All n2disk dump formats (pcap and npcap)
- Standard pcap files copied into n2disk storage
- Both compressed and uncompressed capture files
- Single and multi-interface captures

