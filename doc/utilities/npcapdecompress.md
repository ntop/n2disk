---
title: "npcapdecompress"
description: "This utility decompress compressed .npcap files to standard pcap files. Available options:"
outline: [2, 3]
---
# npcapdecompress {#npcapdecompress}

This utility decompress compressed .npcap files to standard pcap files.

Available options:

```text
[-v]                         | Verbose
[-h]                         | Help
[-i ] <.npcap file>                  | Compressed npcap file to decompress
[-o] <.pcap file>                    | Decompressed pcap file (output)
```

Example:

```console
npcapdecompress -i dummy.npcap -o dummy.pcap
```
