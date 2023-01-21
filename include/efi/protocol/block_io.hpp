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
// MuchCool-EFI. If not, see <https://www.gnu.org/licenses/>.gma once

#ifndef __cplusplus
#error
#endif

#include "efi/core.hpp"

namespace efi {

class BlockToMedia final {
  // todo : implement
};

class BlockIOProtocol final {
 public:
  enum class Revision : uint64_t {
    Rev2 = 0x00020001,
    Rev3 = ((2 << 16) | (31))
  };

 private:
  using ResetFn       = Status(EFI_CALL*)(BlockIOProtocol* self,
                                    bool extended_verification) noexcept;

  using ReadBlocksFn  = Status(EFI_CALL*)(BlockIOProtocol* self,
                                         uint32_t media_id, LBA lba,
                                         uintn_t buffer_size,
                                         void* buffer) noexcept;

  using WriteBlocksFn = Status(EFI_CALL*)(BlockIOProtocol* self,
                                          uint32_t media_id, LBA lba,
                                          uintn_t buffer_size,
                                          const void* buffer) noexcept;

  using FlushBlocksFn = Status(EFI_CALL*)(BlockIOProtocol* self) noexcept;

  const Revision revision_;
  const BlockToMedia* const media_;
  const ResetFn reset_;
  const ReadBlocksFn read_blocks_;
  const WriteBlocksFn write_blocks_;
  const FlushBlocksFn flush_blocks_;

 public:
  BlockIOProtocol()                                          = delete;
  BlockIOProtocol(BlockIOProtocol&&)                         = delete;
  BlockIOProtocol(const BlockIOProtocol&)                    = delete;
  ~BlockIOProtocol()                                         = delete;
  auto operator=(BlockIOProtocol&&) -> BlockIOProtocol&      = delete;
  auto operator=(const BlockIOProtocol&) -> BlockIOProtocol& = delete;

  nodiscard force_inline auto revision() const noexcept {
    return revision_;
  }

  nodiscard force_inline auto& media() const noexcept {
    return *media_;
  }

  force_inline auto reset(bool extended_verification) noexcept {
    return reset_(this, extended_verification);
  }

  force_inline auto read_blocks(uint32_t media_id, LBA lba, uintn_t buffer_size,
                                void* buffer) noexcept {
    return read_blocks_(this, media_id, lba, buffer_size, buffer);
  }

  force_inline auto write_blocks(uint32_t media_id, LBA lba,
                                 uintn_t buffer_size,
                                 const void* buffer) noexcept {
    return write_blocks_(this, media_id, lba, buffer_size, buffer);
  }

  force_inline auto flush_blocks() noexcept {
    return flush_blocks_(this);
  }

  static constexpr auto Guid =
      ::efi::Guid{0x964e5b21,
                  0x6459,
                  0x11d2,
                  {0x8e, 0x39, 0x00, 0xa0, 0xc9, 0x69, 0x72, 0x3b}};
};

class BlockIO2Protocol final {
  // todo : implement
};

class EraseBlockToken {
  Event event_;
  Status transaction_status_;
};

class EraseBlockProtocol final {
 public:
  enum class Revision : uint64_t {
    Rev  = 0x00010000,
    Rev2 = 0x00020001,
    Rev3 = ((2 << 16) | (31))
  };

 private:
  using EraseBlockFn = Status(EFI_CALL*)(EraseBlockProtocol* self,
                                         uint32_t media_id, LBA lba,
                                         EraseBlockToken* token,
                                         uintn_t size) noexcept;

  const Revision revision_;
  const uint32_t erase_length_granularity;
  const EraseBlockFn erase_blocks_;

 public:
  EraseBlockProtocol()                                             = delete;
  EraseBlockProtocol(EraseBlockProtocol&&)                         = delete;
  EraseBlockProtocol(const EraseBlockProtocol&)                    = delete;
  ~EraseBlockProtocol()                                            = delete;
  auto operator=(EraseBlockProtocol&&) -> EraseBlockProtocol&      = delete;
  auto operator=(const EraseBlockProtocol&) -> EraseBlockProtocol& = delete;

  force_inline auto erase_blocks(uint32_t media_id, LBA lba,
                                 EraseBlockToken* token,
                                 uintn_t size) noexcept {
    return erase_blocks_(this, media_id, lba, token, size);
  }

  static constexpr auto Guid =
      ::efi::Guid{0x95A9A93E,
                  0xA86E,
                  0x4926,
                  {0xaa, 0xef, 0x99, 0x18, 0xe7, 0x72, 0xd9, 0x87}};
};

}  // namespace efi