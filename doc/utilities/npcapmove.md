---
title: "npcapmove"
description: "This utility moves a pcap file along with its index file and timeline links to a new storage location. It is particularly useful for migrating captured..."
outline: [2, 3]
---
# npcapmove {#npcapmove}

This utility moves a pcap file along with its index file and timeline links to a new storage location. It is particularly useful for migrating captured traffic to different storage volumes, archiving data to external drives, or reorganizing storage hierarchies while maintaining the n2disk timeline structure.

npcapmove handles all the complexity of updating symbolic links and maintaining the bidirectional relationship between storage and timeline directories.

## Usage {#usage}

```text
npcapmove [-a] <pcap path> <new storage path> <new timeline path>
```

Options:

```text
[-a]    Produce absolute instead of relative links for the timeline
```

Arguments:

- **pcap path**: Full path to the pcap file to move (e.g., /storage/1.pcap)
- **new storage path**: Destination directory for the pcap and index files
- **new timeline path**: Destination directory for timeline structure

## How It Works {#how-it-works}

npcapmove performs the following operations:

1. **Validation**: Verifies that all required files exist:

   - Source pcap file (e.g., `1.pcap`)
   - Index file (e.g., `1.pcap.idx`)
   - Storage-to-timeline link (e.g., `1.pcap.timeline`)
   - Index-to-timeline link (e.g., `1.pcap.idx.timeline`)

2. **Timeline Structure Analysis**:

   - Reads the existing timeline links to determine the timeline directory structure
   - Extracts the relative timeline path (preserving the YYYY/MM/DD/HH/MM hierarchy)

3. **File Removal**:

   - Removes old symbolic links from both storage and timeline locations
   - Removes the timeline pcap and index symlinks

4. **File Migration**:

   - Creates new storage directory if needed
   - Creates new timeline directory structure if needed
   - Moves pcap and index files to new storage location

5. **Link Recreation**:

   - Creates new symbolic links in timeline pointing to new storage location
   - Creates reverse links in storage pointing back to timeline
   - Uses relative or absolute paths based on `-a` flag

6. **Cleanup**:

   - Removes empty directories from old timeline structure

## Link Modes {#link-modes}

**Relative Links (default)**

By default, npcapmove creates relative symbolic links. This is recommended as it makes the storage location portable - you can move both storage and timeline directories together without breaking links.

```console
npcapmove /storage/1.pcap /new-storage /new-storage/timeline
```

**Absolute Links (-a flag)**

With the `-a` flag, npcapmove creates absolute symbolic links. Use this when storage and timeline are on different filesystems or when portability is not a concern.

```console
npcapmove -a /storage/1.pcap /external-storage /external-storage/timeline
```

## Example Usage {#example-usage}

Move a single pcap file to external storage:

```console
npcapmove /storage/n2disk/1632826800.123456.pcap /external-storage /external-storage/timeline
```

Move multiple files using a shell loop:

```console
for pcap in /storage/n2disk/*.pcap; do
  npcapmove "$pcap" /external-storage /external-storage/timeline
done
```

Move files with absolute links:

```console
npcapmove -a /storage/1.pcap /mnt/archive /mnt/archive/timeline
```

## Important Notes {#important-notes}

- Timeline directory structure (YYYY/MM/DD/HH/MM) is automatically preserved
- Empty directories in the old timeline path are automatically cleaned up
- Both storage and timeline destination directories are created if they don't exist

## Compatibility {#compatibility}

npcapmove is compatible with all n2disk timeline structures created with the `-A` option. It works with both npcap and standard pcap formats, handling the `.npcap` and `.pcap` extensions interchangeably.
