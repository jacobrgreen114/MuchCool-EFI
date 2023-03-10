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

class DebugPortProtocol final {
 private:
  using ResetFn = Status(EFI_CALL*)(DebugPortProtocol* self) noexcept;

  using WriteFn = Status(EFI_CALL*)(DebugPortProtocol* self, uint32_t timeout,
                                    uintn_t*    buffer_size,
                                    const void* buffer) noexcept;

  using ReadFn  = Status(EFI_CALL*)(DebugPortProtocol* self, uint32_t timeout,
                                   uintn_t* buffer_size, void* buffer) noexcept;

  using PollFn  = Status(EFI_CALL*)(const DebugPortProtocol* self) noexcept;

  const ResetFn reset_;
  const WriteFn write_;
  const ReadFn  read_;
  const PollFn  poll_;

 public:
  DebugPortProtocol()                                            = delete;
  DebugPortProtocol(DebugPortProtocol&&)                         = delete;
  DebugPortProtocol(const DebugPortProtocol&)                    = delete;
  ~DebugPortProtocol()                                           = delete;
  auto operator=(DebugPortProtocol&&) -> DebugPortProtocol&      = delete;
  auto operator=(const DebugPortProtocol&) -> DebugPortProtocol& = delete;

  FORCE_INLINE auto reset() noexcept {
    return reset_(this);
  }

  FORCE_INLINE auto write(uintn_t* buffer_size, const void* buffer,
                          uint32_t timeout = 0) noexcept {
    return write_(this, timeout, buffer_size, buffer);
  }

  FORCE_INLINE auto read(uintn_t* buffer_size, void* buffer,
                         uint32_t timeout = 0) noexcept {
    return read_(this, timeout, buffer_size, buffer);
  }

  NODISCARD FORCE_INLINE auto poll() const noexcept {
    return poll_(this);
  }

  static constexpr auto guid =
      Guid{0xEBA4E8D2,
           0x3858,
           0x41EC,
           {0xA2, 0x81, 0x26, 0x47, 0xBA, 0x96, 0x60, 0xD0}};
};

}  // namespace efi