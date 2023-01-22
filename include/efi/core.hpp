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

#include <span>
#include <array>
#include <cstdint>
#include <type_traits>

#ifdef _WINDOWS
#define EFI_CALL
#define FORCE_INLINE __forceinline
#else
#define EFI_CALL __attribute__((ms_abi))
#define force_inline __attribute__((always_inline))
#endif

#define NODISCARD [[nodiscard]]
#define NORETURN [[noreturn]]

#define GETTER NODISCARD FORCE_INLINE

#define DEFINE_ENUM_OPERATOR(enum_name, op)                             \
  constexpr auto operator op(enum_name lhs, enum_name rhs)->enum_name { \
    return static_cast<enum_name>(                                      \
        static_cast<std::underlying_type_t<enum_name>>(lhs)             \
            op static_cast<std::underlying_type_t<enum_name>>(rhs));    \
  }

#define ENUM_FLAGS(enum_name)        \
  DEFINE_ENUM_OPERATOR(enum_name, |) \
  DEFINE_ENUM_OPERATOR(enum_name, &)

consteval auto enum_bit(auto index) {
  return 1 << index;
}

namespace efi {

using uintn_t                    = uintptr_t;
using intn_t                     = intptr_t;

using Handle                     = void*;
using Event                      = void*;
using LBA                        = uint64_t;
using TPL                        = uintn_t;

using PhysicalAddress            = uint64_t;
using VirtualAddress             = uint64_t;

constexpr auto status_error_flag = uintn_t{1} << 63;

consteval auto status_error_code_(uintn_t code) -> uintn_t {
  return code & status_error_flag;
}

consteval auto status_warning_code_(uintn_t code) -> uintn_t {
  return code;
}

enum class Status : uintn_t {
  Success             = 0,

  WarnUnknownGlyph    = status_warning_code_(1),
  WarnDeleteFailure   = status_warning_code_(2),
  WarnWriteFailure    = status_warning_code_(3),
  WarnBufferTooSmall  = status_warning_code_(4),
  WarnStaleData       = status_warning_code_(5),
  WarnFileSystem      = status_warning_code_(6),
  WarnResetRequired   = status_warning_code_(7),

  LoadError           = status_error_code_(1),
  InvalidParameter    = status_error_code_(2),
  Unsupported         = status_error_code_(3),
  BadBufferSize       = status_error_code_(4),
  BufferTooSmall      = status_error_code_(5),
  NotReady            = status_error_code_(6),
  DeviceError         = status_error_code_(7),
  WriteProtected      = status_error_code_(8),
  OutOfResources      = status_error_code_(9),
  VolumeCorrupted     = status_error_code_(10),
  VolumeFull          = status_error_code_(11),
  NoMedia             = status_error_code_(12),
  MediaChanged        = status_error_code_(13),
  NotFound            = status_error_code_(14),
  AccessDenied        = status_error_code_(15),
  NoResponse          = status_error_code_(16),
  NoMapping           = status_error_code_(17),
  Timeout             = status_error_code_(18),
  NotStarted          = status_error_code_(19),
  AlreadyStarted      = status_error_code_(20),
  Aborted             = status_error_code_(21),
  ICMPError           = status_error_code_(22),
  TFTPError           = status_error_code_(23),
  ProtocolError       = status_error_code_(24),
  IncompatibleVersion = status_error_code_(25),
  SecurityViolation   = status_error_code_(26),
  CrcError            = status_error_code_(27),
  EndOfMedia          = status_error_code_(28),

  EndOfFile           = status_error_code_(31),
  InvalidLanguage     = status_error_code_(32),
  CompromisedData     = status_error_code_(33),
  IPAddressConflict   = status_error_code_(34),
  HTTPError           = status_error_code_(35),
};

auto status_is_error(Status status) -> bool {
  return (static_cast<std::underlying_type_t<Status>>(status) &
          status_error_flag) > 0;
}

enum class ImageSubsystem {
  Application       = 10,
  BootServiceDriver = 11,
  RuntimeDriver     = 12
};

enum class ImageMachine : uint32_t {
  I386     = 0x014c,
  X64      = 0x8664,
  IA64     = 0x0200,
  EBC      = 0x0EBC,
  ARM      = 0x01c0,
  ARMT     = 0x01C2,
  ARM64    = 0xAA64,
  RISCV32  = 0x5032,
  RISCV64  = 0x5064,
  RISCV128 = 0x5128,
};

enum class ResetType {
  Cold,
  Warn,
  Shutdown,
  PlatformSpecific
};

enum class TimerDelay {
  Cancel,
  Periodic,
  Relative
};

enum class InterfaceType {
  Native
};

enum class MemoryType : uint32_t {
  ReservedMemoryType,
  LoaderCode,
  LoaderData,
  SimpleTextInputProtocolCode,
  SimpleTextInputProtocolData,
  RuntimeServicesCode,
  RuntimeServicesData,
  ConventionalMemory,
  UnusableMemory,
  ACPIReclaimMemory,
  ACPIMemoryNVS,
  MemoryMappedIO,
  MemoryMappedIOPortSpace,
  PalCode,
  PersistentMemory,
  UnacceptedMemoryType
};

class Guid {
 public:
  using Bytes      = std::array<uint8_t, 16>;
  using Data4Bytes = std::array<uint8_t, 8>;

 private:
  union {
    Bytes bytes_;
    struct {
      uint32_t data1;
      uint16_t data2;
      uint16_t data3;
      union {
        Data4Bytes bytes;
        uint64_t ull;
      } data4;
    } struct_;
  };

 public:
  constexpr explicit Guid(const Bytes& bytes) : bytes_{bytes} {}

  constexpr Guid(uint32_t data1, uint16_t data2, uint16_t data3,
                 const Data4Bytes& data4)
      : struct_{.data1 = data1,
                .data2 = data2,
                .data3 = data3,
                .data4{.bytes = data4}} {}

  NODISCARD constexpr auto& bytes() const noexcept {
    return bytes_;
  }

  NODISCARD constexpr auto operator==(const Guid& guid) const -> bool {
    return bytes_ == guid.bytes_;
  }
};

struct Time {
  uint16_t year;   // 1900 - 9999
  uint8_t month;   // 1 - 12
  uint8_t day;     // 1 - 31
  uint8_t hour;    // 0 - 23
  uint8_t minute;  // 0 - 59
  uint8_t second;  // 0 - 59
 private:
  uint8_t pad1_;

 public:
  uint32_t nanosecond;  // 0 - 999,999,999
  int16_t timezone;     // --1440 to 1440 or 2047
  uint8_t daylight;

 private:
  uint8_t pad2_;
};

struct TimeCapabilities {
  uint32_t resolution;
  uint32_t accuracy;
  bool sets_to_zero;
};

class Table {
 private:
  const uint64_t signature_;
  const uint32_t revision_;
  const uint32_t header_size_;
  const uint32_t crc32_;
  const uint32_t reserved_;

 public:
  Table()                                = delete;
  Table(Table&&)                         = delete;
  Table(const Table&)                    = delete;
  ~Table()                               = delete;
  auto operator=(Table&&) -> Table&      = delete;
  auto operator=(const Table&) -> Table& = delete;

  NODISCARD auto signature() const noexcept {
    return signature_;
  }

  NODISCARD auto revision() const noexcept {
    return revision_;
  }

  NODISCARD auto crc32() const noexcept {
    return crc32_;
  }
};

consteval auto revision(uint16_t major, uint16_t point) noexcept -> uint32_t {
  return (static_cast<uint32_t>(major) << 16) | point;
}

template <typename T>
concept IsProtocol = requires(T) { T::Guid->Guid; };

}  // namespace efi