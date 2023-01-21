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

#include "device_path_protocol.hpp"

namespace efi {

class LoadFileProtocol {
 private:
  using LoadFileFn = Status(EFI_CALL*)(LoadFileProtocol* self,
                                       const DevicePathProtocol& file_path,
                                       bool boot_policy, uintn_t* buffer_size,
                                       void* buffer);

  const LoadFileFn load_file_;

 public:
  LoadFileProtocol()                                           = delete;
  LoadFileProtocol(LoadFileProtocol&&)                         = delete;
  LoadFileProtocol(const LoadFileProtocol&)                    = delete;
  ~LoadFileProtocol()                                          = delete;
  auto operator=(LoadFileProtocol&&) -> LoadFileProtocol&      = delete;
  auto operator=(const LoadFileProtocol&) -> LoadFileProtocol& = delete;

  force_inline auto load_file(const DevicePathProtocol& file_path,
                              uintn_t* buffer_size, void* buffer,
                              bool boot_policy = true) noexcept {
    return load_file_(this, file_path, boot_policy, buffer_size, buffer);
  }

  static constexpr auto Guid =
      efi::Guid{0x56EC3091,
                0x954C,
                0x11d2,
                {0x8e, 0x3f, 0x00, 0xa0, 0xc9, 0x69, 0x72, 0x3b}};
};

class LoadFile2Protocol {
 private:
  using LoadFileFn = Status(EFI_CALL*)(LoadFile2Protocol* self,
                                       const DevicePathProtocol& file_path,
                                       bool boot_policy, uintn_t* buffer_size,
                                       void* buffer);

  const LoadFileFn load_file_;

 public:
  LoadFile2Protocol()                                            = delete;
  LoadFile2Protocol(LoadFile2Protocol&&)                         = delete;
  LoadFile2Protocol(const LoadFile2Protocol&)                    = delete;
  ~LoadFile2Protocol()                                           = delete;
  auto operator=(LoadFile2Protocol&&) -> LoadFile2Protocol&      = delete;
  auto operator=(const LoadFile2Protocol&) -> LoadFile2Protocol& = delete;

  force_inline auto load_file(const DevicePathProtocol& file_path,
                              uintn_t* buffer_size, void* buffer) noexcept {
    return load_file_(this, file_path, false, buffer_size, buffer);
  }

  static constexpr auto Guid =
      efi::Guid{0x4006c0c1,
                0xfcb3,
                0x403e,
                {0x99, 0x6d, 0x4a, 0x6c, 0x87, 0x24, 0xe0, 0x6d}};
};

}  // namespace efi