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

#include "file.hpp"

namespace efi {

class SimpleFileSystemProtocol final {
 private:
  using OpenVolumeFn = Status(EFI_CALL*)(SimpleFileSystemProtocol* self,
                                         FileProtocol** root) noexcept;

  const uint64_t revision_;
  const OpenVolumeFn open_volume_;

 public:
  SimpleFileSystemProtocol()                                = delete;
  SimpleFileSystemProtocol(SimpleFileSystemProtocol&&)      = delete;
  SimpleFileSystemProtocol(const SimpleFileSystemProtocol&) = delete;
  ~SimpleFileSystemProtocol()                               = delete;
  auto operator=(SimpleFileSystemProtocol&&)
      -> SimpleFileSystemProtocol& = delete;
  auto operator=(const SimpleFileSystemProtocol&)
      -> SimpleFileSystemProtocol& = delete;

  force_inline auto open_volume(FileProtocol** root) noexcept {
    return open_volume_(this, root);
  }

  static constexpr auto Guid =
      efi::Guid{0x0964e5b22,
                0x6459,
                0x11d2,
                {0x8e, 0x39, 0x00, 0xa0, 0xc9, 0x69, 0x72, 0x3b}};
};

}  // namespace efi
