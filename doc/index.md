---
title: "n2disk Documentation"
description: "n2disk is a network traffic recorder application. With n2disk you can capture full-sized network packets at multi-Gigabit rate (above 10 Gigabit/s on..."
outline: [2, 3]
---
# n2disk Documentation {#n2disk-documentation}

n2disk is a network traffic recorder application. With n2disk you can capture full-sized network packets at multi-Gigabit rate (above 10 Gigabit/s on adequate hardware) from a live network interface, and write them into files without any packet loss. n2disk has been designed to write files into disks for very long periods, you have to specify a maximum number of distinct file that may be written during the execution, and if n2disk reaches the maximum number of files, it will start recycling the files from the oldest one. This way you can have a complete view of the traffic for a fixed temporal window, knowing in advance the amount of disk space needed.

n2disk uses the industry standard PCAP file format (regular and nanosecond) to dump packets into files so the resulting output can be easily integrated with existing third party or even open/source analysis tools (like Wireshark). Alternatively n2disk can produce compressed PCAP files (.npcap) to reduce disk space and optimise disk throughput.

n2disk has been designed and developed mainly because most network security systems rely on capturing all packets (both header and payload), since any packets may have been responsible for the attack or could contain the problems that we are trying to find. Netflow information is more manageable and requires less disk space to be stored, but in some cases, like deep-packet-inspection analysis or controlled traffic regeneration, it is not useful. When we need to collect the entire packet, because we need all the information, n2disk has to be used.

n2disk can be effectively used to perform numerous activities, among these:

- Off-line network packets analysis by feeding a specialised tools like Snort.
- Reconstruct particular communication flows or network activities.
- Reproduce the previous captured traffic to a different network interface.

**User's Guide**

- [Main Features](/features.md)
  - [Dump Format](/features.md#dump-format)
  - [Capture Filters](/features.md#capture-filters)
  - [Index and Post-Capture Filters](/features.md#index-and-post-capture-filters)
  - [Timeline](/features.md#timeline)
- [Performance](/performance.md)
- [Versions](/versions.md)
  - [n2disk1g (1 Gbit/s)](/versions.md#n2disk1g-1-gbit-s)
  - [n2disk5g (5 Gbit/s)](/versions.md#n2disk5g-5-gbit-s)
  - [n2disk (10/40/100 Gbit/s)](/versions.md#n2disk-10-40-100-gbit-s)
  - [Models Comparison Table](/versions.md#models-comparison-table)
- [Installation](/installation.md)
  - [Licensing](/installation.md#licensing)
  - [Applying the License](/installation.md#applying-the-license)
- [Usage](/usage.md)
  - [Command Line Options](/usage.md#command-line-options)
  - [Dump Set Format](/usage.md#dump-set-format)
- [How to Start n2disk](/how_to_start.md)
  - [Systemd-based Systems](/how_to_start.md#systemd-based-systems)
  - [init-based Systems](/how_to_start.md#init-based-systems)
- [Tuning](/tuning/index.md)
  - [Examples](/tuning/examples.md)
  - [Napatech Setup](/tuning/napatech.md)
  - [Silicom FPGA Setup](/tuning/fiberblaze.md)
- [Storage Sizing](/storage_sizing.md)
- [Utilities](/utilities/index.md)
  - [disk2n](/utilities/disk2n.md)
  - [npcapindex](/utilities/npcapindex.md)
  - [npcapextract](/utilities/npcapextract.md)
  - [npcapdecompress](/utilities/npcapdecompress.md)
  - [npcapprintindex](/utilities/npcapprintindex.md)
  - [npcapmanage](/utilities/npcapmanage.md)
  - [npcapmove](/utilities/npcapmove.md)
  - [npcaprepair](/utilities/npcaprepair.md)
  - [ndedup](/utilities/ndedup.md)
- [Flows Export](/using_with_ntopng.md)
- [Netflow and Raw Recording](/netflow.md)
  - [nProbe Cento to n2disk](/netflow.md#nprobe-cento-to-n2disk)
  - [n2disk to nProbe Cento with Flow-Offload](/netflow.md#n2disk-to-nprobe-cento-with-flow-offload)
- [Packet Filtering](/filters.md)
  - [L2/L3/L4 Filtering](/filters.md#l2-l3-l4-filtering)
  - [L7 Filtering](/filters.md#l7-filtering)
- [Disk Partitioning and Formatting](/filesystem.md)
  - [HDDs](/filesystem.md#hdds)
  - [SSDs](/filesystem.md#ssds)
  - [Additional Notes](/filesystem.md#additional-notes)
- [License](/license.md)


**Other Products**

- [ntopng](https://www.ntop.org/guides/ntopng/)
- [nProbe](https://www.ntop.org/guides/nprobe/)
- [nProbe Cento](https://www.ntop.org/guides/cento/)
- [nDPI](https://www.ntop.org/guides/nDPI/)
- [PF_RING](https://www.ntop.org/guides/pf_ring/)
- [nEdge](https://www.ntop.org/guides/nedge/)
- [nScrub](https://www.ntop.org/guides/nscrub/)
- [nBox](https://www.ntop.org/guides/nbox/)
- [nTap](https://www.ntop.org/guides/ntap/)







