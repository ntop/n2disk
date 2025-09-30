How to Start n2disk
###################

Systemd-based Systems
---------------------

The n2disk service is controlled using the systemctl utility on operating systems and distributions that use the systemd service manager.

Configuration Files
===================

Multiple n2disk instances can be simultaneously executed on the same machine. Typically, each instance is bound to an interface. 
To allow different n2disk instances to be run with independent configurations, multiple configuration files co-exist under the directory:

.. code:: bash

   /etc/n2disk/

Each configuration file starts with "n2disk-" and ends with a suffix corresponding to the interface name it is associated to.
For example, assuming two different n2disk daemons have to be run independently for eth0 and eth4, the following configuration files should be present:

.. code:: bash

   # ls -lha /etc/n2disk/
   4.0K -rw-r--r--   1 root root  211 Mar 15 17:54 n2disk-eth0.conf
   4.0K -rw-r--r--   1 root root  195 Jan  8 17:17 n2disk-eth4.conf


Configuration File Format
=========================

A configuration file contains the same options otherwise specified on the command line. The only difference between the command line and the configuration file is that different options need to be specified on different lines. For instance:

.. code:: bash

   n2disk -i eth0 -o /storage

is the same as:

.. code:: bash
   
   n2disk /etc/n2disk/n2disk-eth0.conf

where :code`/etc/n2disk/n2disk-eth0.conf` contains the following lines:

.. code:: bash

   # cat /etc/n2disk/n2disk-eth0.conf
  -i=eth0
  -o=/storage

Automatic Startup
=================

To start, stop and restart a n2disk service type:

.. code:: bash

   systemctl start n2disk@eth0
   systemctl stop n2disk@eth0
   systemctl restart n2disk@eth0

To prevent n2disk from starting on boot type:

.. code:: bash

   systemctl disable n2disk@eth0

Or to start n2disk on boot, assuming it has previously been disabled, type:

.. code:: bash

   systemctl enable n2disk@eth0

To check the status of the service, including its output and PID, type:

.. code:: bash

   systemctl status n2disk@eth0

init-based Systems
------------------

The n2disk service can be controlled on old init-based systems by means of the script script located at:

.. code:: bash

   /etc/init.d/n2disk

In order to launch n2disk daemons automatically on system startup, empty files ending with “.start” must be created in the same directory of the configuration files. 

.. code:: bash

   # ls -lha /etc/n2disk
   -rw-r--r--   1 root root  211 mar 15 17:54 n2disk-eth0.conf
   -rw-r--r--   1 root root    0 mar 17 15:44 n2disk-eth0.start

Those configurations can be controlled with the script /etc/init.d/n2disk. The script accept different options and one or more interface names as input. Calling the script without options yields the following brief help

.. code:: bash

   sudo /etc/init.d/n2disk
   
   Usage: /etc/init.d/n2disk {start|force-start|stop|restart|status} [interface(s)]

The options and the usage of the daemon control script is discusse below.

*start*

This option is used to start daemon n2disks for interfaces that have a “.start” file. Calling start on interfaces with missing “.start” files yield and error. For example

.. code:: bash

   # ls -lha /etc/n2disk
   -rw-r--r--   1 root root  211 mar 15 17:54 n2disk-eth0.conf
   -rw-r--r--   1 root root    0 mar 17 15:44 n2disk-eth0.start
   -rw-r--r--   1 root root  195 Jan  8 17:17 n2disk-eth4.conf
   # /etc/init.d/n2disk start eth0
   Starting n2disk eth0
   # /etc/init.d/n2disk start eth4
   n2disk eth4 not started: missing /etc/n2disk/n2disk-eth4.start

*force-start*

This option is used to start daemon n2disks for instances that do not have a “.start” file. Calling force-start on interface eth4 shown in the example above doesn’t raise any error and the daemon is properly started

.. code:: bash

   # /etc/init.d/n2disk force-start eth4
   Starting n2disk eth4

*stop*

This option is used to stop an n2disk daemon instance. For example 

.. code:: bash

   # /etc/init.d/n2disk stop eth4
   Stopping n2disk eth4

*restart*

This option causes the restart of a daemon associated to a given interface. Example:

.. code:: bash

   # /etc/init.d/n2disk restart eth0
   Stopping n2disk eth0
   Starting n2disk eth0

*status*

This options prints the status of a daemon associated to a given interface. Example: 

.. code:: bash

   # /etc/init.d/n2disk status eth0
   
   n2disk running

