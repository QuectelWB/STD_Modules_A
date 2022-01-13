USB 零包
=========

ZLP : Zero Length Packet

	#define ZLP             BIT(17) 
	...
	{ USB_DEVICE_AND_INTERFACE_INFO(QUECTEL_VENDOR_ID, QUECTEL_PRODUCT_RM500Q, 0xff, 0xff, 0x10),.driver_info = ZLP },
	...
	 option_attach：
		...
	        if (device_flags & ZLP)
                data->use_zlp = 1;
		...
	
## patch

https://git.kernel.org/pub/scm/linux/kernel/git/next/linux-next.git/commit/drivers/usb/host/xhci-ring.c?id=4758dcd19a7d9ba9610b38fecb93f65f56f86346


https://www.spinics.net/lists/kernel/msg2100618.html
	
	
	
	author	Reyad Attiyat <reyad.attiyat@gmail.com>	2015-08-06 19:23:58 +0300
	committer	Greg Kroah-Hartman <gregkh@linuxfoundation.org>	2015-08-08 15:15:49 -0700
	commit	4758dcd19a7d9ba9610b38fecb93f65f56f86346 (patch)
	tree	54b9c2793c08662ef5a87be26ca07d036771cf2d /drivers/usb/host/xhci-ring.c
	parent	6c05cb145d6716f00d452aeb27916efb6d190cca (diff)
	download	linux-next-4758dcd19a7d9ba9610b38fecb93f65f56f86346.tar.gz
	usb: xhci: Add support for URB_ZERO_PACKET to bulk/sg transfers
	This commit checks for the URB_ZERO_PACKET flag and creates an extra
	zero-length td if the urb transfer length is a multiple of the endpoint's
	max packet length.

	Signed-off-by: Reyad Attiyat <reyad.attiyat@gmail.com>
	Signed-off-by: Mathias Nyman <mathias.nyman@linux.intel.com>
	Signed-off-by: Greg Kroah-Hartman <gregkh@linuxfoundation.org>
	Diffstat (limited to 'drivers/usb/host/xhci-ring.c')
	-rw-r--r--	drivers/usb/host/xhci-ring.c	66	
	1 files changed, 52 insertions, 14 deletions
	diff --git a/drivers/usb/host/xhci-ring.c b/drivers/usb/host/xhci-ring.c
	index 6a8fc52aed586..463562d119297 100644
	--- a/drivers/usb/host/xhci-ring.c
	+++ b/drivers/usb/host/xhci-ring.c
	@@ -3041,9 +3041,11 @@ static int queue_bulk_sg_tx(struct xhci_hcd *xhci, gfp_t mem_flags,
		struct xhci_td *td;
		struct scatterlist *sg;
		int num_sgs;
	-	int trb_buff_len, this_sg_len, running_total;
	+	int trb_buff_len, this_sg_len, running_total, ret;
		unsigned int total_packet_count;
	+	bool zero_length_needed;
		bool first_trb;
	+	int last_trb_num;
		u64 addr;
		bool more_trbs_coming;
	 
	@@ -3059,13 +3061,27 @@ static int queue_bulk_sg_tx(struct xhci_hcd *xhci, gfp_t mem_flags,
		total_packet_count = DIV_ROUND_UP(urb->transfer_buffer_length,
				usb_endpoint_maxp(&urb->ep->desc));
	 
	-	trb_buff_len = prepare_transfer(xhci, xhci->devs[slot_id],
	+	ret = prepare_transfer(xhci, xhci->devs[slot_id],
				ep_index, urb->stream_id,
				num_trbs, urb, 0, mem_flags);
	-	if (trb_buff_len < 0)
	-		return trb_buff_len;
	+	if (ret < 0)
	+		return ret;
	 
		urb_priv = urb->hcpriv;
	+
	+	/* Deal with URB_ZERO_PACKET - need one more td/trb */
	+	zero_length_needed = urb->transfer_flags & URB_ZERO_PACKET &&
	+		urb_priv->length == 2;
	+	if (zero_length_needed) {
	+		num_trbs++;
	+		xhci_dbg(xhci, "Creating zero length td.\n");
	+		ret = prepare_transfer(xhci, xhci->devs[slot_id],
	+				ep_index, urb->stream_id,
	+				1, urb, 1, mem_flags);
	+		if (ret < 0)
	+			return ret;
	+	}
	+
		td = urb_priv->td[0];
	 
		/*
	@@ -3095,6 +3111,7 @@ static int queue_bulk_sg_tx(struct xhci_hcd *xhci, gfp_t mem_flags,
			trb_buff_len = urb->transfer_buffer_length;
	 
		first_trb = true;
	+	last_trb_num = zero_length_needed ? 2 : 1;
		/* Queue the first TRB, even if it's zero-length */
		do {
			u32 field = 0;
	@@ -3112,12 +3129,15 @@ static int queue_bulk_sg_tx(struct xhci_hcd *xhci, gfp_t mem_flags,
			/* Chain all the TRBs together; clear the chain bit in the last
			 * TRB to indicate it's the last TRB in the chain.
			 */
	-		if (num_trbs > 1) {
	+		if (num_trbs > last_trb_num) {
				field |= TRB_CHAIN;
	-		} else {
	-			/* FIXME - add check for ZERO_PACKET flag before this */
	+		} else if (num_trbs == last_trb_num) {
				td->last_trb = ep_ring->enqueue;
				field |= TRB_IOC;
	+		} else if (zero_length_needed && num_trbs == 1) {
	+			trb_buff_len = 0;
	+			urb_priv->td[1]->last_trb = ep_ring->enqueue;
	+			field |= TRB_IOC;
			}
	 
			/* Only set interrupt on short packet for IN endpoints */
	@@ -3179,7 +3199,7 @@ static int queue_bulk_sg_tx(struct xhci_hcd *xhci, gfp_t mem_flags,
			if (running_total + trb_buff_len > urb->transfer_buffer_length)
				trb_buff_len =
					urb->transfer_buffer_length - running_total;
	-	} while (running_total < urb->transfer_buffer_length);
	+	} while (num_trbs > 0);
	 
		check_trb_math(urb, num_trbs, running_total);
		giveback_first_trb(xhci, slot_id, ep_index, urb->stream_id,
	@@ -3197,7 +3217,9 @@ int xhci_queue_bulk_tx(struct xhci_hcd *xhci, gfp_t mem_flags,
		int num_trbs;
		struct xhci_generic_trb *start_trb;
		bool first_trb;
	+	int last_trb_num;
		bool more_trbs_coming;
	+	bool zero_length_needed;
		int start_cycle;
		u32 field, length_field;
	 
	@@ -3228,7 +3250,6 @@ int xhci_queue_bulk_tx(struct xhci_hcd *xhci, gfp_t mem_flags,
			num_trbs++;
			running_total += TRB_MAX_BUFF_SIZE;
		}
	-	/* FIXME: this doesn't deal with URB_ZERO_PACKET - need one more */
	 
		ret = prepare_transfer(xhci, xhci->devs[slot_id],
				ep_index, urb->stream_id,
	@@ -3237,6 +3258,20 @@ int xhci_queue_bulk_tx(struct xhci_hcd *xhci, gfp_t mem_flags,
			return ret;
	 
		urb_priv = urb->hcpriv;
	+
	+	/* Deal with URB_ZERO_PACKET - need one more td/trb */
	+	zero_length_needed = urb->transfer_flags & URB_ZERO_PACKET &&
	+		urb_priv->length == 2;
	+	if (zero_length_needed) {
	+		num_trbs++;
	+		xhci_dbg(xhci, "Creating zero length td.\n");
	+		ret = prepare_transfer(xhci, xhci->devs[slot_id],
	+				ep_index, urb->stream_id,
	+				1, urb, 1, mem_flags);
	+		if (ret < 0)
	+			return ret;
	+	}
	+
		td = urb_priv->td[0];
	 
		/*
	@@ -3258,7 +3293,7 @@ int xhci_queue_bulk_tx(struct xhci_hcd *xhci, gfp_t mem_flags,
			trb_buff_len = urb->transfer_buffer_length;
	 
		first_trb = true;
	-
	+	last_trb_num = zero_length_needed ? 2 : 1;
		/* Queue the first TRB, even if it's zero-length */
		do {
			u32 remainder = 0;
	@@ -3275,12 +3310,15 @@ int xhci_queue_bulk_tx(struct xhci_hcd *xhci, gfp_t mem_flags,
			/* Chain all the TRBs together; clear the chain bit in the last
			 * TRB to indicate it's the last TRB in the chain.
			 */
	-		if (num_trbs > 1) {
	+		if (num_trbs > last_trb_num) {
				field |= TRB_CHAIN;
	-		} else {
	-			/* FIXME - add check for ZERO_PACKET flag before this */
	+		} else if (num_trbs == last_trb_num) {
				td->last_trb = ep_ring->enqueue;
				field |= TRB_IOC;
	+		} else if (zero_length_needed && num_trbs == 1) {
	+			trb_buff_len = 0;
	+			urb_priv->td[1]->last_trb = ep_ring->enqueue;
	+			field |= TRB_IOC;
			}
	 
			/* Only set interrupt on short packet for IN endpoints */
	@@ -3318,7 +3356,7 @@ int xhci_queue_bulk_tx(struct xhci_hcd *xhci, gfp_t mem_flags,
			trb_buff_len = urb->transfer_buffer_length - running_total;
			if (trb_buff_len > TRB_MAX_BUFF_SIZE)
				trb_buff_len = TRB_MAX_BUFF_SIZE;
	-	} while (running_total < urb->transfer_buffer_length);
	+	} while (num_trbs > 0);
	 
		check_trb_math(urb, num_trbs, running_total);
		giveback_first_trb(xhci, slot_id, ep_index, urb->stream_id,
	


