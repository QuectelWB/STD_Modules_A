#ttyUSB加载






usbserial和option模块加载过程：

	struct bus_type usb_serial_bus_type  虚拟的 usb_serial 总线
	struct usb_driver usb_serial_driver

usbserial模块初始化函数：

	usb_serial_init
		struct tty_driver *usb_serial_tty_driver = alloc_tty_driver(SERIAL_TTY_MINORS);

		bus_register(&usb_serial_bus_type);             注册虚拟总线

		usb_serial_tty_driver       设置tty_driver， 操作函数 serial_ops

		tty_register_driver(usb_serial_tty_driver);     注册 tty_driver

		usb_register(&usb_serial_driver);               注册一个USB接口驱动 usb_serial_driver

	struct usb_driver option_driver
	struct usb_serial_driver * const serial_drivers
	struct usb_serial_driver option_1port_device

option模块初始化函数

	module_usb_serial_driver(option_driver, serial_drivers);
		usb_serial_register_drivers(option_driver, serial_drivers)
			saved_id_table = option_driver->id_table;
			option_driver->id_table = NULL;                   这里把id_table临时保存一下，下面注册的时候，即使有设备，也不会匹配到
			usb_register(option_driver);                      注册一个USB接口驱动 option_driver 这里会将 option_driver->drvwrap.for_devices = 0; 表示这是个接口驱动

			usb_serial_register(option_1port_device);                   注册一个 struct usb_serial_driver
				list_add(&driver->driver_list, &usb_serial_driver_list);        这里会将 option_1port_device 添加到 usb_serial_driver_list 链表

			option_driver->id_table = saved_id_table;           再把id_table取回来
			driver_attach(&option_driver->drvwrap.driver);      在USB虚拟总线匹配USB接口，如果有USB接口匹配到，会调用 option_driver 的probe函数

下面是设备插入后的匹配过程：

如果一个USB设备插入，最终USB接口和接口设备匹配，那么就会匹配到 option_driver 驱动，然后执行它的probe函数：
	
	usb_serial_probe
		struct usb_serial_driver *type = search_serial_device(interface);           这里在链表里找驱动，找到的是 option_1port_device
		struct usb_serial *serial = create_serial(dev, interface, type);            创建一个 usb_serial serial 的type指针就指向了 option_1port_device
		type->probe(serial, id);                                                    执行 option_1port_device 的probe函数 option_probe
			这里注册了一个 struct usb_wwan_intf_private 结构体

		读取接口的信息，设置 usb_serial
		port = kzalloc(sizeof(struct usb_serial_port), GFP_KERNEL);                 分配对应的 usb_serial_port 结构体
		serial->port[i] = port;                                                     usb_serial_port 放入 usb_serial 中
		port->dev.bus = &usb_serial_bus_type;                                       port 设备注册的虚拟总线
		device_add(&port->dev);                                                     向系统中加入中port，这个设备会挂在 usb_serial_bus_type 虚拟总线的设备列表，并执行匹配函数 usb_serial_device_match
			usb_serial_device_match
				driver = to_usb_serial_driver(drv);             这是取得drv嵌入的 struct usb_serial_driver 结构
				if (driver == port->serial->type)               如果这个驱动和serial的type是一个驱动，那么匹配成功，前面看到了驱动的设置，
																所以这里匹配成功的是 option_1port_device 驱动，
																匹配成功后优先执行 usb_serial_bus_type 的probe函数 usb_serial_device_probe

			usb_serial_device_probe
				port = to_usb_serial_port(dev);                 从dev找到 usb_serial_port
				driver = port->serial->type;                    取得serial对应的驱动，也就是 option_1port_device
				driver->port_probe(port);                       如果有 port_probe 函数，就执行，这里没有就不执行
				minor = port->number;
				tty_register_device(usb_serial_tty_driver, minor, dev);     注册tty设备

设备插入后会注册tty设备，那么对应的tty驱动注册过程：

再来看tty驱动的注册和设备注册
	
	usb_serial_init
		struct tty_driver *usb_serial_tty_driver = alloc_tty_driver(SERIAL_TTY_MINORS);
		tty_set_operations(usb_serial_tty_driver, &serial_ops);
		tty_register_driver(usb_serial_tty_driver);
			cdev_init(&driver->cdev, &tty_fops);
			cdev_add(&driver->cdev, dev, driver->num);          注册一个字符型设备
			list_add(&driver->tty_drivers, &tty_drivers);       将 usb_serial_tty_driver 驱动加入到 tty_drivers 链表

设备插入后创建tty设备过程：

再看设备插入后 usb_serial_probe -> device_add -> usb_serial_device_probe -> tty_register_device(usb_serial_tty_driver, minor, dev) 添加设备

	tty_register_device(usb_serial_tty_driver, minor, dev)      这里的参数 dev 代表的是 struct usb_serial_port
		dev = MKDEV(driver->major, driver->minor_start) + index;        取得设备的设备号，这里的主设备号就是 usb_serial_tty_driver 驱动的主设备号
		tty_line_name(driver, index, name);        
		device_create(tty_class, device, dev, NULL, name);      创建设备节点


打开一个设备的过程：

如果 open 一个设备节点，首先会调用到 tty_fops 中的open函数

	tty_open(struct inode *inode, struct file *filp)
		struct tty_driver *driver = tty_lookup_driver(device, filp, &noctty, &index);
			struct tty_driver *driver = get_tty_driver(device, index);

		struct tty_struct *tty = tty_init_dev(driver, index);
			tty = alloc_tty_struct();
			initialize_tty_struct(tty, driver, idx);
				tty->driver = driver;
				tty->ops = driver->ops;                         这里就得到了 usb_serial_tty_driver 驱动中的ops，也就是 tty_fops

			tty_ldisc_setup(tty, tty->link);


		retval = tty->ops->open(tty, filp);                     这里就执行了 tty_fops 中的open函数

