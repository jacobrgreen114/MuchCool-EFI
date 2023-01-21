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

enum class ParityType : uint32_t {
  DefaultParity,
  NoParity,
  EvenParity,
  OddParity,
  MarkParity,
  SpaceParity
};

enum class StopBits : uint32_t {
  Default,
  One,      // 1 stop bit
  OneFive,  // 1.5 stop bits
  Two       // 2 stop bits
};

enum class ControlBits : uint32_t {
  DataTerminalReady         = 0x0001,
  RequestToSend             = 0x0002,

  ClearToSend               = 0x0010,
  DataSetReady              = 0x0020,
  RingIndicate              = 0x0040,
  CarrierDetect             = 0x0080,

  InputBufferEmpty          = 0x0100,
  OutputBufferEmpty         = 0x0200,

  HardwareLoopbackEnable    = 0x1000,
  SoftwareLoopbackEnable    = 0x2000,
  HardwareFlowControlEnable = 0x4000,
};

class SerialIOMode final {
  uint32_t control_mask_;

  uint32_t timeout_;
  uint64_t baudrate_;
  uint32_t receive_fifo_depth_;
  uint32_t databits_;
  ParityType parity_;
  StopBits stopbits_;
};

class SerialIOProtocol final {
  using ResetFn        = Status(EFI_CALL*)(SerialIOProtocol* self) noexcept;

  using SetAttributeFn = Status(EFI_CALL*)(
      SerialIOProtocol* self, uint64_t baud_rate, uint32_t recieve_fifo_depth,
      uint32_t timeout, ParityType parityType, uint8_t databits,
      StopBits stopbits) noexcept;

  using SetControlBitsFn = Status(EFI_CALL*)(SerialIOProtocol* self,
                                             ControlBits control) noexcept;

  using GetControlBitsFn = Status(EFI_CALL*)(SerialIOProtocol* self,
                                             ControlBits* out_control) noexcept;

  using WriteFn          = Status(EFI_CALL*)(SerialIOProtocol* self,
                                    uintn_t* buffer_size, const void* buffer);

  using ReadFn = Status(EFI_CALL*)(SerialIOProtocol* self, uintn_t* buffer_size,
                                   void* buffer);

  const uint32_t revision_;
  const ResetFn reset_;
  const SetAttributeFn set_attribute_;
  const SetControlBitsFn set_control_;
  const GetControlBitsFn get_control_;
  const WriteFn write_;
  const ReadFn read_;
  const SerialIOMode* const mode_;
  const Guid* const device_type_guid_;

 public:
  SerialIOProtocol()                                           = delete;
  SerialIOProtocol(SerialIOProtocol&&)                         = delete;
  SerialIOProtocol(const SerialIOProtocol&)                    = delete;
  ~SerialIOProtocol()                                          = delete;
  auto operator=(SerialIOProtocol&&) -> SerialIOProtocol&      = delete;
  auto operator=(const SerialIOProtocol&) -> SerialIOProtocol& = delete;

  force_inline auto reset() noexcept {
    return reset_(this);
  }

  force_inline auto set_attribute(uint64_t baud_rate,
                                  uint32_t recieve_fifo_depth, uint32_t timeout,
                                  ParityType parity_type, uint8_t databits,
                                  StopBits stopbits) noexcept {
    return set_attribute_(this, baud_rate, recieve_fifo_depth, timeout,
                          parity_type, databits, stopbits);
  }

  force_inline auto set_control(ControlBits control) noexcept {
    return set_control_(this, control);
  }

  force_inline auto get_control(ControlBits* out_control) noexcept {
    return get_control_(this, out_control);
  }

  force_inline auto write(uintn_t* buffer_size, const void* buffer) noexcept {
    return write_(this, buffer_size, buffer);
  }

  force_inline auto read(uintn_t* buffer_size, void* buffer) noexcept {
    return read_(this, buffer_size, buffer);
  }

  nodiscard auto mode() const noexcept -> auto& {
    return *mode_;
  }

  nodiscard auto device_type() const noexcept -> auto& {
    return *device_type_guid_;
  }

  static constexpr auto Guid =
      efi::Guid{0xBB25CF6F,
                0xF1D4,
                0x11D2,
                {0x9a, 0x0c, 0x00, 0x90, 0x27, 0x3f, 0xc1, 0xfd}};

  static constexpr uint32_t Revision1   = 0x00010000;
  static constexpr uint32_t Revision1p1 = 0x00010001;

  static constexpr auto TerminalDeviceTypeGuid =
      efi::Guid{0x6ad9a60f,
                0x5815,
                0x4c7c,
                {0x8a, 0x10, 0x50, 0x53, 0xd2, 0xbf, 0x7a, 0x1b}};
};

}  // namespace efi
