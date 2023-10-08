# linux usbnet 

https://blog.csdn.net/ning_wei/article/details/21156661

## ops

    static const struct net_device_ops usbnet_netdev_ops = {
                .ndo_open               = usbnet_open,
                .ndo_stop               = usbnet_stop,
                .ndo_start_xmit         = usbnet_start_xmit,
                 .ndo_tx_timeout         = usbnet_tx_timeout,
                 .ndo_set_rx_mode        = usbnet_set_rx_mode,
                   .ndo_change_mtu         = usbnet_change_mtu,
                 .ndo_get_stats64        = usbnet_get_stats64,
                  .ndo_set_mac_address    = eth_mac_addr,
                  .ndo_validate_addr      = eth_validate_addr,
    };


主要关注 usnbet_start_xmit 、open和timeout

## usbnet_probe

        dev->msg_enable = netif_msg_init (msg_level, NETIF_MSG_DRV
                                | NETIF_MSG_PROBE | NETIF_MSG_LINK);
        init_waitqueue_head(&dev->wait);
        skb_queue_head_init (&dev->rxq);
        skb_queue_head_init (&dev->txq);
        skb_queue_head_init (&dev->done);
        skb_queue_head_init(&dev->rxq_pause);
        dev->bh.func = (void (*)(unsigned long))usbnet_bh;
        dev->bh.data = (unsigned long)&dev->delay;
        INIT_WORK (&dev->kevent, usbnet_deferred_kevent);
        init_usb_anchor(&dev->deferred);
        timer_setup(&dev->delay, usbnet_bh, 0);
        mutex_init (&dev->phy_mutex);
        mutex_init(&dev->interrupt_mutex);


定义三个队列 分别用于发送、接受和处理

        skb_queue_head_init (&dev->rxq);
        skb_queue_head_init (&dev->txq);
        skb_queue_head_init (&dev->done);



## 关键参数





