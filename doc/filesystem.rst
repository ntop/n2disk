Disk Partitioning and Formatting
================================

n2disk writes files sequentially. In our experience the XFS filesystem is the best option you can select when
writing to HDDs, although other filesystems such as EXT4 can also be used. Supposing that you have created the 
/dev/sda1 disk partition, you can format it as follows:

.. code-block:: console

   mkfs.xfs -f -d sunit=128,swidth=1024,agcount=6000 -l size=64m /dev/sda1

Once you have formatted the disk you can mount it as follows:

.. code-block:: console

   mount -o noatime,nodiratime,attr2,nobarrier,logbufs=8,logbsize=256k,osyncisdsync /dev/sda1 /storage/ 

Note that for small partition sizes, you might need to adapt some of the above parameters.

If you are using SSDs, EXT4 usually performs better, you can format your partition as follows (assuming you have a RAID 0 array with 16 SSDs):

.. code-block:: console

   mkfs.ext4 -F -b 4096 -E discard,stride=16,stripe-width=256 /dev/sda1

Where:

- -b is the block size (max 4K)
- stride = chunk size (64K) / block size (4K) = 16
- stripe-width = number of disks x stride = 256

And mount it with:

.. code-block:: console

   mount -o rw,user,auto,discard /dev/sda1 /storage/

It is also recommended to set the "noop" scheduler with SSDs:

.. code-block:: console

   echo noop > /sys/block/sda/queue/scheduler

You can test the disk write performance as explained in chapter 4. Please note that depending on the disk types 
and controller, your write performance can be influenced by the chunk size (-C option) that specifies. the minimal 
unit of data written to disk. Values in the range between 64 and 512 KBytes should guaranteed adequate performance, 
although some combination of disks/controllers operate better with 1024 KBytes or more. You can check that with “-e 1”.

