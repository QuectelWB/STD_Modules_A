
Kernel 里的解释

/ * @status: This is read in non-iso completion functions to get the
 *	status of the particular request.  ISO requests only use it
 *	to tell whether the URB was unlinked; detailed status for
 *	each frame is in the fields of the iso_frame-desc.
*/

关于Urb

Data Transfer Buffers:
 *
 * Normally drivers provide I/O buffers allocated with kmalloc() or otherwise
 * taken from the general page pool.  That is provided by transfer_buffer
 * (control requests also use setup_packet), and host controller drivers
 * perform a dma mapping (and unmapping) for each buffer transferred.  Those
 * mapping operations can be expensive on some platforms (perhaps using a dma
 * bounce buffer or talking to an IOMMU),
 * although they're cheap on commodity x86 and ppc hardware.
 *
 * Alternatively, drivers may pass the URB_NO_TRANSFER_DMA_MAP transfer flag,
 * which tells the host controller driver that no such mapping is needed for
 * the transfer_buffer since
 * the device driver is DMA-aware.  For example, a device driver might
 * allocate a DMA buffer with usb_alloc_coherent() or call usb_buffer_map().
 * When this transfer flag is provided, host controller drivers will
 * attempt to use the dma address found in the transfer_dma
 * field rather than determining a dma address themselves.
 *
 * Note that transfer_buffer must still be set if the controller
 * does not support DMA (as indicated by bus.uses_dma) and when talking
 * to root hub. If you have to trasfer between highmem zone and the device
 * on such controller, create a bounce buffer or bail out with an error.
 * If transfer_buffer cannot be set (is in highmem) and the controller is DMA
 * capable, assign NULL to it, so that usbmon knows not to use the value.
 * The setup_packet must always be set, so it cannot be located in highmem.
 *
 * Initialization:
 *
 * All URBs submitted must initialize the dev, pipe, transfer_flags (may be
 * zero), and complete fields.  All URBs must also initialize
 * transfer_buffer and transfer_buffer_length.  They may provide the
 * URB_SHORT_NOT_OK transfer flag, indicating that short reads are
 * to be treated as errors; that flag is invalid for write requests.
 *
 * Bulk URBs may
 * use the URB_ZERO_PACKET transfer flag, indicating that bulk OUT transfers
 * should always terminate with a short packet, even if it means adding an
 * extra zero length packet.
 *
 * Control URBs must provide a valid pointer in the setup_packet field.
 * Unlike the transfer_buffer, the setup_packet may not be mapped for DMA
 * beforehand.
 *
 * Interrupt URBs must provide an interval, saying how often (in milliseconds
 * or, for highspeed devices, 125 microsecond units)
 * to poll for transfers.  After the URB has been submitted, the interval
 * field reflects how the transfer was actually scheduled.
 * The polling interval may be more frequent than requested.
 * For example, some controllers have a maximum interval of 32 milliseconds,
 * while others support intervals of up to 1024 milliseconds.
 * Isochronous URBs also have transfer intervals.  (Note that for isochronous
 * endpoints, as well as high speed interrupt endpoints, the encoding of
 * the transfer interval in the endpoint descriptor is logarithmic.
 * Device drivers must convert that value to linear units themselves.)
 *
 * If an isochronous endpoint queue isn't already running, the host
 * controller will schedule a new URB to start as soon as bandwidth
 * utilization allows.  If the queue is running then a new URB will be
 * scheduled to start in the first transfer slot following the end of the
 * preceding URB, if that slot has not already expired.  If the slot has
 * expired (which can happen when IRQ delivery is delayed for a long time),
 * the scheduling behavior depends on the URB_ISO_ASAP flag.  If the flag
 * is clear then the URB will be scheduled to start in the expired slot,
 * implying that some of its packets will not be transferred; if the flag
 * is set then the URB will be scheduled in the first unexpired slot,
 * breaking the queue's synchronization.  Upon URB completion, the
 * start_frame field will be set to the (micro)frame number in which the
 * transfer was scheduled.  Ranges for frame counter values are HC-specific
 * and can go from as low as 256 to as high as 65536 frames.
 *
 * Isochronous URBs have a different data transfer model, in part because
 * the quality of service is only "best effort".  Callers provide specially
 * allocated URBs, with number_of_packets worth of iso_frame_desc structures
 * at the end.  Each such packet is an individual ISO transfer.  Isochronous
 * URBs are normally queued, submitted by drivers to arrange that
 * transfers are at least double buffered, and then explicitly resubmitted
 * in completion handlers, so
 * that data (such as audio or video) streams at as constant a rate as the
 * host controller scheduler can support.
 *
 * Completion Callbacks:
 *
 * The completion callback is made in_interrupt(), and one of the first
 * things that a completion handler should do is check the status field.
 * The status field is provided for all URBs.  It is used to report
 * unlinked URBs, and status for all non-ISO transfers.  It should not
 * be examined before the URB is returned to the completion handler.
 *
 * The context field is normally used to link URBs back to the relevant
 * driver or request state.
 *
 * When the completion callback is invoked for non-isochronous URBs, the
 * actual_length field tells how many bytes were transferred.  This field
 * is updated even when the URB terminated with an error or was unlinked.
 *
 * ISO transfer status is reported in the status and actual_length fields
 * of the iso_frame_desc array, and the number of errors is reported in
 * error_count.  Completion callbacks for ISO transfers will normally
 * (re)submit URBs to ensure a constant transfer rate.
 *
 * Note that even fields marked "public" should not be touched by the driver
 * when the urb is owned by the hcd, that is, since the call to
 * usb_submit_urb() till the entry into the completion routine.
 */


