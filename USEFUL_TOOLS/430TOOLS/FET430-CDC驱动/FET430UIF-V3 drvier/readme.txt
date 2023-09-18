IAR for 430 5.4以后，UIF使用的是V3版本
如果MSP-FET430UIF驱动安装失败。

winXP:
手工拷贝usbser.sys到C:\windows\system32\drivers

如果不行，查看安装日志：C:\windows\setupapi.log

win7:

如果不行，查看安装日志：C:\windows\inf\setupapi.dev.log
找到要拷贝usbser.sys的路径，手工去创建这个目录，并拷贝usbser.sys到该目录。