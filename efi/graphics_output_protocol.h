#ifndef __GRAPHICS_OUTPUT_H__
#define __GRAPHICS_OUTPUT_H__
#include <stdint.h>
#define EFI_GRAPHICS_OUTPUT_PROTOCOL_GUID \
  { \
    0x9042a9de, 0x23dc, 0x4a38, {0x96, 0xfb, 0x7a, 0xde, 0xd0, 0x80, 0x51, 0x6a } \
  }

typedef struct _EFI_GRAPHICS_OUTPUT_PROTOCOL efi_graphics_output_protocol;
typedef struct {
  uint32_t    RedMask;
  uint32_t    GreenMask;
  uint32_t    BlueMask;
  uint32_t    ReservedMask;
} EFI_PIXEL_BITMASK;

typedef enum {
  ///
  /// A pixel is 32-bits and byte zero represents red, byte one represents green,
  /// byte two represents blue, and byte three is reserved. This is the definition
  /// for the physical frame buffer. The byte values for the red, green, and blue
  /// components represent the color intensity. This color intensity value range
  /// from a minimum intensity of 0 to maximum intensity of 255.
  ///
  PixelRedGreenBlueReserved8BitPerColor,
  ///
  /// A pixel is 32-bits and byte zero represents blue, byte one represents green,
  /// byte two represents red, and byte three is reserved. This is the definition
  /// for the physical frame buffer. The byte values for the red, green, and blue
  /// components represent the color intensity. This color intensity value range
  /// from a minimum intensity of 0 to maximum intensity of 255.
  ///
  PixelBlueGreenRedReserved8BitPerColor,
  ///
  /// The Pixel definition of the physical frame buffer.
  ///
  PixelBitMask,
  ///
  /// This mode does not support a physical frame buffer.
  ///
  PixelBltOnly,
  ///
  /// Valid EFI_GRAPHICS_PIXEL_FORMAT enum values are less than this value.
  ///
  PixelFormatMax
} EFI_GRAPHICS_PIXEL_FORMAT;

typedef struct {
  ///
  /// The version of this data structure. A value of zero represents the
  /// EFI_GRAPHICS_OUTPUT_MODE_INFORMATION structure as defined in this specification.
  ///
  uint32_t                       Version;
  ///
  /// The size of video screen in pixels in the X dimension.
  ///
  uint32_t                       HorizontalResolution;
  ///
  /// The size of video screen in pixels in the Y dimension.
  ///
  uint32_t                       VerticalResolution;
  ///
  /// Enumeration that defines the physical format of the pixel. A value of PixelBltOnly
  /// implies that a linear frame buffer is not available for this mode.
  ///
  EFI_GRAPHICS_PIXEL_FORMAT    PixelFormat;
  ///
  /// This bit-mask is only valid if PixelFormat is set to PixelPixelBitMask.
  /// A bit being set defines what bits are used for what purpose such as Red, Green, Blue, or Reserved.
  ///
  EFI_PIXEL_BITMASK            PixelInformation;
  ///
  /// Defines the number of pixel elements per video memory line.
  ///
  uint32_t                       PixelsPerScanLine;
} EFI_GRAPHICS_OUTPUT_MODE_INFORMATION;



typedef struct {
  uint8_t    Blue;
  uint8_t    Green;
  uint8_t    Red;
  uint8_t    Reserved;
} EFI_GRAPHICS_OUTPUT_BLT_PIXEL;

typedef struct {
  ///
  /// The number of modes supported by QueryMode() and SetMode().
  ///
  uint32_t                                  MaxMode;
  ///
  /// Current Mode of the graphics device. Valid mode numbers are 0 to MaxMode -1.
  ///
  uint32_t                                  Mode;
  ///
  /// Pointer to read-only EFI_GRAPHICS_OUTPUT_MODE_INFORMATION data.
  ///
  EFI_GRAPHICS_OUTPUT_MODE_INFORMATION      *Info;
  ///
  /// Size of Info structure in bytes.
  ///
  uintptr_t                                   SizeOfInfo;
  ///
  /// Base address of graphics linear frame buffer.
  /// Offset zero in FrameBufferBase represents the upper left pixel of the display.
  ///
  uintptr_t                    FrameBufferBase;
  ///
  /// Amount of frame buffer needed to support the active mode as defined by
  /// PixelsPerScanLine xVerticalResolution x PixelElementSize.
  ///
  uintptr_t                                   FrameBufferSize;
} EFI_GRAPHICS_OUTPUT_PROTOCOL_MODE;

///
/// Provides a basic abstraction to set video modes and copy pixels to and from
/// the graphics controller's frame buffer. The linear address of the hardware
/// frame buffer is also exposed so software can write directly to the video hardware.
///
struct _EFI_GRAPHICS_OUTPUT_PROTOCOL {
  void *QueryMode;
  void (*SetMode)(uint32_t, void *);
  void *Blt;
  ///
  /// Pointer to EFI_GRAPHICS_OUTPUT_PROTOCOL_MODE data.
  ///
  EFI_GRAPHICS_OUTPUT_PROTOCOL_MODE          *Mode;
};

#endif
