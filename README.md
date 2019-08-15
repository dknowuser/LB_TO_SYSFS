Smart LB to sysfs module  
=============================================
The module allows users to manage LB_BASE FPGA feature via sysfs entries.  
It creates 'smart_lb' directory in /sys/class/ where devices 'lb_X' are placed.  
Instead of X a number is given to each device accordingly to its port.  

Each 'lb_X' directory contains 'enable' and 'level' files.  
'enable' allows users to enable LB_BASE FPGA feature globally.  
To enable it type command 'echo 1 > enable' when 'lb_X' is current directory.  
To disable it type command 'echo 0 > enable'  when 'lb_X' is current directory.  

'level' allows users to set up required loopback level.  
Type command 'echo X > level' where X is

  * 0 -- loopback disabled;
  * 2 -- MAC level loopback;
  * 3 -- IP level loopback;
  * 4 -- TCP/UDP level loopback.

Download  
==============================================
Create folder 'lbfeat-to-sysfs' and git clone there.

The folder 'lbfeat-to-sysfs' should be placed in drivers/misc.  
In file drivers/misc/Makefile following line was added:  
obj-y += lbfeat-to-sysfs/ 

Device Tree bindings  
===============================================
The module requires lbfeat-to-sysfs subnode placed in /dts-v1/ node.  
lbfeat-to-sysfs subnode fields are:  
  * compatible = "stcmtk,lbfeat-to-sysfs"
  * target-fpga = <&fpga0>
  * port = < /*port number */>  
  * label = "Smart Loopback management exporting into sysfs"

Device Tree Source is stcmtk-smartsfp-2.0.dts  
