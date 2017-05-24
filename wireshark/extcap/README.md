# Wireshark Extcap

The extcap interface in Wireshark is a plugin-based mechanism to allow external 
executables to be used as traffic source in case the capture interface is not a 
standard network interface directly recognised by Wireshark.

The remotentopdump extcap module can be used to extract traffic from a remote n2disk 
dumpset in Wireshark.

In order to get started with the remotentopdump module, you need to compile the module:

``` 
make
``` 

and copy the module to the extcap path when Wireshark will look for extcap plugins,
in this example under /usr/lib/x86_64-linux-gnu/wireshark/extcap/ (if you install
from sources it will probably be /usr/local/lib/wireshark/extcap/, while on OSX it
should be /Applications/Wireshark.app/Contents/MacOS/extcap/)

``` 
cp remotentopdump /usr/lib/x86_64-linux-gnu/wireshark/extcap/
``` 

You can read the extcap folder from the Wireshark menu:

"Help" -> "About Wireshark" -> "Folders" -> "Extcap path"

At this point you are ready to start Wireshark and start using the remotentopdump module.

Once you start Wireshark, you will see a new interface "Remote n2disk timeline". Before 
running the capture, please configure all the settings to connect to the remote machine
by clicking on the gear icon of the corresponding interface, and set a capture filter as
on a standard interface if needed.

Please note that in order to use the "nDPI inspection" feature, ndpiReader (part of nDPI)
should be installed on the remote machine.
