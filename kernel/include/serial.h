#ifndef SERIAL_H_
#define SERIAL_H_
#include <stdint.h>

#ifdef __x86_64__
	#define SERIAL_PORT 0x3f8
	void outb(uint16_t port, uint8_t val);
	uint8_t inb(uint16_t port);
#endif

#ifdef __aarch64__
	#define SERIAL_PORT 0xe0000000
  #define SERIAL_FIFO (uint32_t *)(SERIAL_PORT + 0x30)
	struct UART{
        uint32_t control_reg0;       /* UART Control Register def=0x128 */
        uint32_t mode_reg0;          /* UART Mode Register def=0 */
        uint32_t intrpt_en_reg0;     /* Interrupt Enable Register def=0 */
        uint32_t intrpt_dis_reg0;    /* Interrupt Disable Register def=0 */
        uint32_t intrpt_mask_reg0;   /* Interrupt Mask Register def=0 */
        uint32_t chnl_int_sts_reg0;  /* Channel Interrupt Status Register def=x200 */
        uint32_t baud_rate_gen;      /* Baud Rate Generator Register def=0x28B */
        uint32_t Rcvr_timeout_reg0;          /* Receiver Timeout Register def=0 */
        uint32_t Rcvr_FIFO_trigger_level0;   /* Receiver FIFO Trigger Level Register */
        uint32_t Modem_ctrl_reg0;            /* Modem Control Register def=0 */
        uint32_t Modem_sts_reg0;     /* Modem Status Register */
        uint32_t channel_sts_reg0;   /* Channel Status Register def=0 */
        uint32_t tx_rx_fifo;         /* Transmit and Receive FIFO def=0 */
        uint32_t baud_rate_divider;  /* Baud Rate Divider def=0xf */
        uint32_t Flow_delay_reg0;            /* Flow Control Delay Register  def=0*/
        uint32_t Tx_FIFO_trigger_level;  /* Transmitter FIFO Trigger Level Register */
	};
  static struct UART *UART=(struct UART *)SERIAL_PORT;
	void outb(uint64_t port, uint8_t val);
	uint8_t inb(uint64_t port);
#endif

int init_serial();
int is_transmit_empty();
void write_serial(char a);
void puts_serial(const char *str);

#endif

