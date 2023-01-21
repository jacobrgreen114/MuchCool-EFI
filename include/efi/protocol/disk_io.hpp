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

namespace efi {

class DiskIOProtocol final {
 public:
  enum class Revision : uint64_t {
    Rev1 = 0x00010000
  };

 private:
  using ReadDiskFn  = Status(EFI_CALL*)(DiskIOProtocol* self, uint32_t media_id,
                                       uint64_t offset, uintn_t buffer_size,
                                       void* buffer) noexcept;

  using WriteDiskFn = Status(EFI_CALL*)(DiskIOProtocol* self, uint32_t media_id,
                                        uint64_t offset, uintn_t buffer_size,
                                        const void* buffer) noexcept;

  const Revision revision_;
  const ReadDiskFn read_disk_;
  const WriteDiskFn write_disk_;

 public:
  DiskIOProtocol()                                         = delete;
  DiskIOProtocol(DiskIOProtocol&&)                         = delete;
  DiskIOProtocol(const DiskIOProtocol&)                    = delete;
  ~DiskIOProtocol()                                        = delete;
  auto operator=(DiskIOProtocol&&) -> DiskIOProtocol&      = delete;
  auto operator=(const DiskIOProtocol&) -> DiskIOProtocol& = delete;

  force_inline auto read_disk(uint32_t media_id,
                              uint64_t offset, uintn_t buffer_size,
                              void* buffer) noexcept {
    return read_disk_(this, media_id, offset, buffer_size, buffer);
  }

  force_inline auto write_disk(uint32_t media_id,
                               uint64_t offset, uintn_t buffer_size,
                               const void* buffer) noexcept {
    return write_disk_(this, media_id, offset, buffer_size, buffer);
  }

  static constexpr auto Guid =
      ::efi::Guid{0xCE345171,
                  0xBA0B,
                  0x11d2,
                  {0x8e, 0x4F, 0x00, 0xa0, 0xc9, 0x69, 0x72, 0x3b}};
};

class DiskIO2Protocol final {
  // todo : implement
};

}  // namespace efi