Silicom FPGA Setup
==================

In order to have n2disk working on Silicom FPGA (Fiberblaze) adapters, a PRB group (streams configuration) needs to be configured on the adapter.
For further information please visit the PF_RING documentation. Note that for high performances n2disk automatically configured chunk mode on the adapter. There is no special configuration to apply.

Please just make sure the PF_RING driver is loaded using systemctl:

.. code-block:: console

   systemctl start pf_ring

Then start n2disk:

.. code-block:: console

   n2disk -o /storage -p 1000 -b 4000 -i fbcard:0:a00 -q 1 -C 1024 -c 1 -z 2 -w 3 -n 50 -m 100

