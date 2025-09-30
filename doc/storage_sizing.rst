Storage Sizing
==============

This section provides some guidelines for sizing the disk according to the 
traffic throughput and desired data retention.

The table below highlights the minimum requirements for a storage able to dump 
traffic according to the sustained traffic throughput. Please note that medium/high
end enterprise disks are considered, with high write throughput designed for endurance.
RAID 0 is considered here, more disks are required with parity (e.g. with RAID 5/50/6).

+--------------------+--------------------+--------------------+--------------------+
| Traffic Throughput | Min # SAS/SATA HDD | Min # SSD          | Min # NVMe         |
+--------------------+--------------------+--------------------+--------------------+
| 1 Gbps             | 1-2                | 1                  | 1                  |
+--------------------+--------------------+--------------------+--------------------+
| 10 Gbps            | 8-10               | 4                  | 1                  |
+--------------------+--------------------+--------------------+--------------------+
| 40 Gbps            | 24-32              | 16                 | 4                  |
+--------------------+--------------------+--------------------+--------------------+
| 100 Gbps           |                    |                    | 8                  |
+--------------------+--------------------+--------------------+--------------------+

Note: a single NVMe disk can handle up to 35 Gbps, numbers reported in the above table
have been produced considering average disks capable of 15-20 Gbps, to fit most hardware
configurations using not the fastest disks on the market. The same applies to spinning 
HDDs and SSDs.

The table below highlights the minimum requirements for a storage able to dump 
traffic according to the desired data retention and considering the worst case
sustained traffic throughput. Please note that the minimum number of disks based
on the traffic throughput of the table above should be used in any case.

+--------------------+--------------------+--------------------+--------------------+
| Traffic Throughput | Data per Hour      | Data per Day       | Data per Week      |
+--------------------+--------------------+--------------------+--------------------+
| 1 Gbps             | 500 GB             | 10 TB              | 70 TB              |
+--------------------+--------------------+--------------------+--------------------+
| 10 Gbps            | 5 TB               | 100 TB             | 700 TB             |
+--------------------+--------------------+--------------------+--------------------+
| 40 Gbps            | 20 TB              | 400 TB             | 2.8 PB             |
+--------------------+--------------------+--------------------+--------------------+
| 100 Gbps           | 50 TB              | 1 PB               | 7 PB               |
+--------------------+--------------------+--------------------+--------------------+


