The folder 'lbfeat-to-sysfs' should be placed in drivers/misc.  
In file drivers/misc/Makefile following line was added:  
obj-y += lbfeat-to-sysfs/ 

===============================================
Device Tree bindings  
The module requires lbfeat-to-sysfs subnode placed in /dts-v1/ node.  
lbfeat-to-sysfs subnode fields are:  
  * compatible = "lbfeat-to-sysfs"
  * target-fpga = <&fpga0>
  * label = "Smart Loopback management exporting into sysfs"
