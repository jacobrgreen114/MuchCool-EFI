// Copyright (c) 2023 Jacob R. Green
//
// This file is part of MuchCool-EFI.
//
// MuchCool-EFI is free software: you can redistribute it and/or modify it under
// the terms of the GNU General Public License as published by the Free Software
// Foundation, either version 3 of the License, or (at your option) any later version.
//
// MuchCool-EFI is distributed in the hope that it will be useful, but WITHOUT
// ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS
// FOR A PARTICULAR PURPOSE. See the GNU General Public License for more details.
//
// You should have received a copy of the GNU General Public License along with
// MuchCool-EFI. If not, see <https://www.gnu.org/licenses/>.

#pragma once

#ifndef __cplusplus
#error
#endif

#include "efi/core.hpp"

namespace efi {

enum class GraphicsPixelFormat {
  RedGreenBlueReserved8BitPerColor,
  BlueGreenRedReserved8BitPerColor,
  BitMask,
  BltOnly,
};

enum class BltOperation {
  VideoFill,
  VideoToBltBuffer,
  BufferToVideo,
  VideoToVideo
};

class PixelBitmask final {
 private:
  uint32_t red_mask_;
  uint32_t green_mask_;
  uint32_t blue_mask_;
  uint32_t reserved_mask_;
};

class BltPixel {
  uint8_t blue_;
  uint8_t green_;
  uint8_t red_;
  uint8_t reserved_;
};

class GraphicsOutputModeInformation final {
  uint32_t            version_;
  uint32_t            horizontal_resolution_;
  uint32_t            vertical_resolution_;
  GraphicsPixelFormat pixel_format_;
  PixelBitmask        pixel_information_;
  uint32_t            pixels_per_scan_line_;
};

class GraphicsOutputMode final {
  uint32_t                       max_mode_;
  uint32_t                       mode_;
  GraphicsOutputModeInformation* info_;
  uintn_t                        size_of_info_;
  PhysicalAddress                framebuffer_base_;
  uintn_t                        framebuffer_size_;
};

class GraphicsOutputProtocol final {
 private:
  using QueryModeFn = Status(EFI_CALL*)(
      GraphicsOutputProtocol* self, uint32_t mode_number, uintn_t* size_of_info,
      GraphicsOutputModeInformation** info) noexcept;

  using SetModeFn = Status(EFI_CALL*)(GraphicsOutputProtocol* self,
                                      uint32_t mode_number) noexcept;

  using BltFn     = Status(EFI_CALL*)(GraphicsOutputProtocol* self,
                                  BltPixel* blt_buffer, BltOperation blt_op,
                                  uintn_t source_x, uintn_t source_y,
                                  uintn_t dest_x, uintn_t dest_y, uintn_t width,
                                  uintn_t height, uintn_t delta) noexcept;

  const QueryModeFn               query_mode_;
  const SetModeFn                 set_mode_;
  const BltFn                     blt_;
  const GraphicsOutputMode* const mode_;

 public:
  GraphicsOutputProtocol()                                            = delete;
  GraphicsOutputProtocol(GraphicsOutputProtocol&&)                    = delete;
  GraphicsOutputProtocol(const GraphicsOutputProtocol&)               = delete;
  ~GraphicsOutputProtocol()                                           = delete;
  auto operator=(GraphicsOutputProtocol&&) -> GraphicsOutputProtocol& = delete;
  auto operator=(const GraphicsOutputProtocol&)
      -> GraphicsOutputProtocol& = delete;

  FORCE_INLINE auto query_mode(uint32_t mode_number, uintn_t* size_of_info,
                               GraphicsOutputModeInformation** info) noexcept {
    return query_mode_(this, mode_number, size_of_info, info);
  }

  FORCE_INLINE auto set_mode(uint32_t mode) noexcept {
    return set_mode_(this, mode);
  }

  FORCE_INLINE auto blt(BltPixel* blt_buffer, BltOperation blt_op,
                        uintn_t source_x, uintn_t source_y, uintn_t dest_x,
                        uintn_t dest_y, uintn_t width, uintn_t height,
                        uintn_t delta) noexcept {
    return blt_(this, blt_buffer, blt_op, source_x, source_y, dest_x, dest_y,
                width, height, delta);
  }

  NODISCARD auto mode() const noexcept -> auto& {
    return *mode_;
  }

  static constexpr auto guid =
      Guid{0x9042a9de,
           0x23dc,
           0x4a38,
           {0x96, 0xfb, 0x7a, 0xde, 0xd0, 0x80, 0x51, 0x6a}};
};

}  // namespace efi
