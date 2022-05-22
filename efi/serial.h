#ifndef EFI_SERIAL_H
#define EFI_SERIAL_H
#include "efi.h"

/*++

Copyright (c) 1998  Intel Corporation

Module Name:

    efiser.h

Abstract:

    EFI serial protocol

Revision History

--*/

//
// Serial protocol
//

#define EFI_SERIAL_IO_PROTOCOL_GUID \
    { 0xBB25CF6F, 0xF1D4, 0x11D2, {0x9A, 0x0C, 0x00, 0x90, 0x27, 0x3F, 0xC1, 0xFD} }
#define SERIAL_IO_PROTOCOL EFI_SERIAL_IO_PROTOCOL_GUID
#ifdef __x86_64__
#define EFIAPI __attribute__((ms_abi))
#endif
#ifdef __aarch64__
#define EFIAPI
#endif
#define INTERFACE_DECL(x) struct x
INTERFACE_DECL(_EFI_SERIAL_IO_PROTOCOL);

typedef enum {
    DefaultParity,
    NoParity,
    EvenParity,
    OddParity,
    MarkParity,
    SpaceParity
} EFI_PARITY_TYPE;

typedef enum {
    DefaultStopBits,
    OneStopBit,         // 1 stop bit
    OneFiveStopBits,    // 1.5 stop bits
    TwoStopBits         // 2 stop bits
} EFI_STOP_BITS_TYPE;

#define EFI_SERIAL_CLEAR_TO_SEND                   0x0010  // RO
#define EFI_SERIAL_DATA_SET_READY                  0x0020  // RO
#define EFI_SERIAL_RING_INDICATE                   0x0040  // RO
#define EFI_SERIAL_CARRIER_DETECT                  0x0080  // RO
#define EFI_SERIAL_REQUEST_TO_SEND                 0x0002  // WO
#define EFI_SERIAL_DATA_TERMINAL_READY             0x0001  // WO
#define EFI_SERIAL_INPUT_BUFFER_EMPTY              0x0100  // RO
#define EFI_SERIAL_OUTPUT_BUFFER_EMPTY             0x0200  // RO
#define EFI_SERIAL_HARDWARE_LOOPBACK_ENABLE        0x1000  // RW
#define EFI_SERIAL_SOFTWARE_LOOPBACK_ENABLE        0x2000  // RW
#define EFI_SERIAL_HARDWARE_FLOW_CONTROL_ENABLE    0x4000  // RW

typedef
efi_status_t
(EFIAPI *EFI_SERIAL_RESET) (
    struct _EFI_SERIAL_IO_PROTOCOL  *This
    );

typedef
efi_status_t
(EFIAPI *EFI_SERIAL_SET_ATTRIBUTES) (
    struct _EFI_SERIAL_IO_PROTOCOL  *This,
    uint64_t                        BaudRate,
    uint32_t                        ReceiveFifoDepth,
    uint32_t                        Timeout,
    EFI_PARITY_TYPE                 Parity,
    uint8_t                         DataBits,
    EFI_STOP_BITS_TYPE              StopBits
    );

typedef
efi_status_t
(EFIAPI *EFI_SERIAL_SET_CONTROL_BITS) (
    struct _EFI_SERIAL_IO_PROTOCOL  *This,
    uint32_t                          Control
    );

typedef
efi_status_t
(EFIAPI *EFI_SERIAL_GET_CONTROL_BITS) (
    struct _EFI_SERIAL_IO_PROTOCOL  *This,
    uint32_t                         *Control
    );

typedef
efi_status_t
(EFIAPI *EFI_SERIAL_WRITE) (
    struct _EFI_SERIAL_IO_PROTOCOL  *This,
    efi_uint_t                       *BufferSize,
    void                            *Buffer
    );

typedef
efi_status_t
(EFIAPI *EFI_SERIAL_READ) (
    struct _EFI_SERIAL_IO_PROTOCOL  *This,
    efi_uint_t                      *BufferSize,
    void                           *Buffer
    );

typedef struct {
    uint32_t                  ControlMask;

    // current Attributes
    uint32_t                  Timeout;
    uint64_t                  BaudRate;
    uint32_t                  ReceiveFifoDepth;
    uint32_t                  DataBits;
    uint32_t                  Parity;
    uint32_t                  StopBits;
} SERIAL_IO_MODE;

#define SERIAL_IO_INTERFACE_REVISION    0x00010000

typedef struct _EFI_SERIAL_IO_PROTOCOL {
    uint32_t                       Revision;
    EFI_SERIAL_RESET             Reset;
    EFI_SERIAL_SET_ATTRIBUTES    SetAttributes;
    EFI_SERIAL_SET_CONTROL_BITS  SetControl;
    EFI_SERIAL_GET_CONTROL_BITS  GetControl;
    EFI_SERIAL_WRITE             Write;
    EFI_SERIAL_READ              Read;

    SERIAL_IO_MODE               *Mode;
} EFI_SERIAL_IO_PROTOCOL;

typedef struct _EFI_SERIAL_IO_PROTOCOL _SERIAL_IO_INTERFACE;
typedef EFI_SERIAL_IO_PROTOCOL SERIAL_IO_INTERFACE;

#endif
