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

#include "core.hpp"

namespace efi {

enum class FileOpenMode : uint64_t {
  Read   = 0x0000000000000001,
  Write  = 0x0000000000000002,
  Create = 0x8000000000000000,
};

enum class FileAttribute : uint64_t {
  ReadOnly  = 0x0000000000000001,
  Hidden    = 0x0000000000000002,
  System    = 0x0000000000000004,
  Reserved  = 0x0000000000000008,
  Directory = 0x0000000000000010,
  Archive   = 0x0000000000000020,
  ValidAttr = 0x0000000000000037,
};

class FileInfo final {
 private:
  uint64_t size_;
  uint64_t file_size_;
  uint64_t physical_size_;
  Time create_time_;
  Time last_access_time_;
  Time modification_time_;
  FileAttribute attribute_;
  char16_t file_name_[];

 public:
  // warn : 0x8e and 0x39 might need to be flipped
  static constexpr auto Guid =
      ::efi::Guid{0x09576e92,
                  0x6d3f,
                  0x11d2,
                  {0x8e, 0x39, 0x00, 0xa0, 0xc9, 0x69, 0x72, 0x3b}};
};

class FileSystemInfo final {
 private:
  uint64_t size_;
  bool read_only_;
  uint64_t volume_size_;
  uint64_t free_space_;
  uint32_t block_size_;
  char16_t volume_label_[];

 public:
  // warn : 0x8e and 0x39 might need to be flipped
  static constexpr auto Guid =
      ::efi::Guid{0x09576e93,
                  0x6d3f,
                  0x11d2,
                  {0x8e, 0x39, 0x00, 0xa0, 0xc9, 0x69, 0x72, 0x3b}};
};

class FileProtocol {
 public:
  enum class Revision : uint64_t {
    Rev1 = 0x00010000,
    Rev2 = 0x00020000
  };

 private:
  using OpenFn   = Status(EFI_CALL*)(FileProtocol* self,
                                   FileProtocol** new_handle,
                                   const char16_t* file_name,
                                   FileOpenMode open_mode,
                                   FileAttribute attributes) noexcept;

  using CloseFn  = Status(EFI_CALL*)(FileProtocol* self) noexcept;

  using DeleteFn = Status(EFI_CALL*)(FileProtocol* self) noexcept;

  using ReadFn   = Status(EFI_CALL*)(FileProtocol* self, uintn_t* buffer_size,
                                   void* buffer) noexcept;

  using WriteFn  = Status(EFI_CALL*)(FileProtocol* self, uintn_t* buffer_size,
                                    const void* buffer) noexcept;

  using FlushFn  = Status(EFI_CALL*)(FileProtocol* self) noexcept;

  using SetPositionFn = Status(EFI_CALL*)(FileProtocol* self,
                                          uint64_t position) noexcept;

  using GetPositionFn = Status(EFI_CALL*)(FileProtocol* self,
                                          uint64_t* position) noexcept;

  using GetInfoFn     = Status(EFI_CALL*)(FileProtocol* self,
                                      const Guid& information_type,
                                      uintn_t* buffer_size,
                                      void* buffer) noexcept;

  using SetInfoFn     = Status(EFI_CALL*)(FileProtocol* self,
                                      const Guid& information_type,
                                      uintn_t buffer_size,
                                      const void* buffer) noexcept;

  const Revision revision_;
  const OpenFn open_;
  const CloseFn close_;
  const DeleteFn delete_;
  const ReadFn read_;
  const WriteFn write_;
  const GetPositionFn get_position_;
  const SetPositionFn set_position_;
  const GetInfoFn get_info_;
  const SetInfoFn set_info_;
  const FlushFn flush_;

 public:
  FileProtocol()                                       = delete;
  FileProtocol(FileProtocol&&)                         = delete;
  FileProtocol(const FileProtocol&)                    = delete;
  ~FileProtocol()                                      = delete;
  auto operator=(FileProtocol&&) -> FileProtocol&      = delete;
  auto operator=(const FileProtocol&) -> FileProtocol& = delete;

  force_inline auto open(FileProtocol** new_handle, const char16_t* file_name,
                         FileOpenMode open_mode,
                         FileAttribute attributes) noexcept {
    return open_(this, new_handle, file_name, open_mode, attributes);
  }

  force_inline auto close() noexcept {
    return close_(this);
  }

  force_inline auto delete_file() noexcept {
    return delete_(this);
  }

  force_inline auto read(uintn_t* buffer_size, void* buffer) noexcept {
    return read_(this, buffer_size, buffer);
  }

  force_inline auto write(uintn_t* buffer_size, const void* buffer) noexcept {
    return write_(this, buffer_size, buffer);
  }

  force_inline auto set_position(uint64_t position) noexcept {
    return set_position_(this, position);
  }

  force_inline auto get_position(uint64_t* position) noexcept {
    return get_position_(this, position);
  }

  force_inline auto get_info(const Guid& information_type, uintn_t* buffer_size,
                             void* buffer) noexcept {
    return get_info_(this, information_type, buffer_size, buffer);
  }

  force_inline auto set_info(const Guid& information_type, uintn_t buffer_size,
                             const void* buffer) noexcept {
    return set_info_(this, information_type, buffer_size, buffer);
  }
};

class FileIOToken final {
 private:
  const Event event_;
  const Status status_;
  const uintn_t buffer_size_;
  void* const buffer_;
};

class FileProtocol2 : public FileProtocol {
 private:
  using OpenExFn  = Status(EFI_CALL*)(FileProtocol* self,
                                     FileProtocol** new_handle,
                                     const char16_t* file_name,
                                     FileOpenMode open_mode,
                                     FileAttribute attributes,
                                     FileIOToken* token) noexcept;

  using ReadExFn  = Status(EFI_CALL*)(FileProtocol* self,
                                     FileIOToken* token) noexcept;

  using WriteExFn = Status(EFI_CALL*)(FileProtocol* self,
                                      FileIOToken* token) noexcept;

  using FlushExFn = Status(EFI_CALL*)(FileProtocol* self,
                                      FileIOToken* token) noexcept;

  const OpenExFn open_ex_;
  const ReadExFn read_ex_;
  const WriteExFn write_ex_;
  const FlushExFn flush_ex_;

 public:
  FileProtocol2()                                        = delete;
  FileProtocol2(FileProtocol2&&)                         = delete;
  FileProtocol2(const FileProtocol2&)                    = delete;
  ~FileProtocol2()                                       = delete;
  auto operator=(FileProtocol2&&) -> FileProtocol2&      = delete;
  auto operator=(const FileProtocol2&) -> FileProtocol2& = delete;
};

}  // namespace efi