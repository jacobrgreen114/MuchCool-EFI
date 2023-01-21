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

#if defined(_M_X64) || defined(__x86_64__) || defined(__amd64__)
#include "platform/x64.hpp"
#elif
#error Unsupported platform
#endif

namespace efi {

using InstructionSetArchitecture = ImageMachine;

// enum class InstructionSetArchitecture : uint32_t {
//   I386        = static_cast<uint32_t>(ImageMachine::I386),  // 0x014C
//   X64         = IMAGE_FILE_MACHINE_X64,                     // 0x8664
//   Ipf         = IMAGE_FILE_MACHINE_IA64,                    // 0x0200
//   Ebc         = IMAGE_FILE_MACHINE_EBC,                     // 0x0EBC
//   Arm         = IMAGE_FILE_MACHINE_ARMTHUMB_MIXED,          // 0x1C2
//   AArch64     = IMAGE_FILE_MACHINE_AARCH64,                 // 0xAA64
//   RISCV32     = IMAGE_FILE_MACHINE_RISCV32,                 // 0x5032
//   RISCV64     = IMAGE_FILE_MACHINE_RISCV64,                 // 0x5064
//   RISCV128    = IMAGE_FILE_MACHINE_RISCV128,                // 0x5128
//   LoongArch32 = IMAGE_FILE_MACHINE_LOONGARCH32,             // 0x6232
//   LoongArch64 = IMAGE_FILE_MACHINE_LOONGARCH64,             // 0x6264
// };

enum class ExceptionType : uintn_t {
};

using PeriodicCallback  = void(EFI_CALL *)(SystemContext *systemContext);

using ExceptionCallback = void(EFI_CALL *)(ExceptionType exception_type,
                                           SystemContext *systemContext);

class DebugSupportProtocol final {
 private:
  using GetMaximumProcessorIndexFn = Status(EFI_CALL *)(
      DebugSupportProtocol *self, uintn_t *max_processor_index);

  using RegisterPeriodicCallbackFn =
      Status(EFI_CALL *)(DebugSupportProtocol *self, uintn_t processor_index,
                         PeriodicCallback periodic_callback) noexcept;

  using RegisterExceptionCallbackFn =
      Status(EFI_CALL *)(DebugSupportProtocol *self, uintn_t processor_index,
                         ExceptionCallback exception_callback,
                         ExceptionType exception_type) noexcept;

  using InvalidateInstructionCacheFn =
      Status(EFI_CALL *)(DebugSupportProtocol *self, uintn_t processor_index,
                         void *start, uint64_t length);

  const InstructionSetArchitecture isa_;
  const GetMaximumProcessorIndexFn get_maximum_processor_index_;
  const RegisterPeriodicCallbackFn register_periodic_callback_;
  const RegisterExceptionCallbackFn register_exception_callback_;
  const InvalidateInstructionCacheFn invalidate_instruction_cache_;

 public:
  DebugSupportProtocol()                                            = delete;
  DebugSupportProtocol(DebugSupportProtocol &&)                     = delete;
  DebugSupportProtocol(const DebugSupportProtocol &)                = delete;
  ~DebugSupportProtocol()                                           = delete;
  auto operator=(DebugSupportProtocol &&) -> DebugSupportProtocol & = delete;
  auto operator=(const DebugSupportProtocol &)
      -> DebugSupportProtocol & = delete;

  nodiscard auto architecture() const {
    return isa_;
  }

  force_inline auto get_max_processor_index(
      uintn_t *max_processor_index) noexcept {
    return get_maximum_processor_index_(this, max_processor_index);
  }

  force_inline auto register_periodic_callback(
      uintn_t processor_index, PeriodicCallback periodic_callback) noexcept {
    return register_periodic_callback_(this, processor_index,
                                       periodic_callback);
  }

  force_inline auto register_exception_callback(
      uintn_t processor_index, ExceptionCallback exception_callback,
      ExceptionType exception_type) noexcept {
    return register_exception_callback_(this, processor_index,
                                        exception_callback, exception_type);
  }

  force_inline auto invalidate_instruction_cache(uintn_t processor_index,
                                                 void *start,
                                                 uint64_t length) noexcept {
    return invalidate_instruction_cache_(this, processor_index, start, length);
  }

  static constexpr auto Guid =
      ::efi::Guid{0x2755590C,
                  0x6F3C,
                  0x42FA,
                  {0x9E, 0xA4, 0xA3, 0xBA, 0x54, 0x3C, 0xDA, 0x25}};
};

}  // namespace efi