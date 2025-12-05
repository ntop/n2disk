npcapmanage
===========

This utility is used for managing data retention by listing or deleting traffic older than a specified time interval. It operates on the timeline directory structure created by n2disk and can efficiently remove pcap files, their indexes, and associated timeline links within a given time range.

npcapmanage is particularly useful for implementing data retention policies, freeing up disk space, and maintaining compliance with data storage requirements.

Available options are:

.. code-block:: text

   [-d]                  | Delete all PCAPs in the specified time interval
   [-t] <timeline dir>   | Timeline directory created by n2disk
   [-b] <begin time>     | Begin time (format: YYYY-MM-DD HH:MM:SS)
   [-e] <end time>       | End time (format: YYYY-MM-DD HH:MM:SS)
   [-v] <verbosity>      | Verbosity level (0-6, default: 2)
   [-h]                  | Help

How It Works
------------

When run with the ``-d`` flag, npcapmanage performs the following operations for each pcap file in the specified time range:

1. Scans the timeline directory structure in 10-minute intervals
2. Identifies pcap files within the specified time range based on their timestamps
3. Follows symbolic links from timeline to actual storage locations
4. Deletes the pcap file from storage
5. Deletes the corresponding index file (.idx)
6. Removes timeline symbolic links (.timeline files)
7. Cleans up empty timeline subdirectories

The tool is designed to handle the bidirectional symbolic link structure used by n2disk, where:

- Timeline directory contains links pointing to storage files
- Storage files contain reverse links (.timeline) pointing back to timeline entries

Example Usage
-------------

Delete all pcap files captured between specific times:

.. code-block:: console

   npcapmanage -t /storage/n2disk/timeline -b "2024-09-01 00:00:00" -e "2024-09-15 23:59:59" -d

List files that would be deleted (without -d flag, the tool will scan and log but not delete):

.. code-block:: console

   npcapmanage -t /storage/n2disk/timeline -b "2024-09-01 00:00:00" -e "2024-09-15 00:00:00" -v 3

Delete the last 5 minutes of captured traffic:

.. code-block:: console

   npcapmanage -t /timeline -b "2024-09-30 14:55:00" -e "2024-09-30 15:00:00" -d

Important Notes
---------------

- The ``-d`` flag is required to actually delete files. Without it, npcapmanage will only scan and report.
- Time format must be exactly: ``YYYY-MM-DD HH:MM:SS``
- The begin time is inclusive, the end time is exclusive (files with timestamp >= end time are not deleted)
- The tool handles both absolute and relative symbolic links
- Empty timeline directories are automatically removed after file deletion
- Use higher verbosity levels (``-v 6``) to see detailed information about each file operation
- The tool reports the total number of files deleted and processing time upon completion

