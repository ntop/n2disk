Installation
============

At http://packages.ntop.org we build binary n2disk packages ready to use, all you need to do is to select your platform and follow the installation instructions.

n2disk needs a license in order to operate permanently (i.e. not in demo mode). In order to  obtain a license, you need to go to http://shop.ntop.org and order it. Once you get the order, you need to generate the license based on your System ID, you can read it running n2disk:

.. code-block:: console

   n2disk -V
   n2disk 3.0 [SystemID: 1234567890ABCDEF]

Generating the license you also get instructions for installing it.

You are now ready to start your licensed n2disk copy. You can either do that using the scripts provided under /etc/init.d or systemctl, according to your OS, or alternatively you can run n2disk from command line.

Remember that for good results, you need to use n2disk over ZC. In order to do that please refer to the PF_RING_ documentation.

.. _PF_RING: http://www.ntop.org/guides/pf_ring


Licensing
=========

Binary n2disk instances require a per-server license that is released according to the EULA (End User License Agreement). Each license is perpetual (i.e. it does not expire) and it allows to install updates for one year since purchase/license issue. This means that a license generated on 1/1/2018 will be able to activate new versions of the software until 31/12/2018. If you want to install new versions of the software release after that date, you need to renew the license maintenance or avoid further updating the software.

n2disk licenses are generated using the orderId and email you provided when the license has been purchased on https://shop.ntop.org/.

Applying the License
--------------------
Once the license has been generated, it can be applied in order for
n2disk to become fully-functional. The license must
be placed in a one-line file :code:`/etc/n2disk.license`.

.. note::

   An n2disk restart is recommended once the license has been applied
   to make sure all the new functionalities will be unlocked.
