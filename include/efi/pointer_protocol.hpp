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

class SimplePointerState final {
 private:
  int32_t relative_movement_x_;
  int32_t relative_movement_y_;
  int32_t relative_movement_z_;
  bool left_button_;
  bool right_button_;
};

class SimpleInputMode final {
 private:
  uint64_t resolution_x_;
  uint64_t resolution_y_;
  uint64_t resolution_z_;
  bool left_button_;
  bool right_button_;
};

class SimplePointerProtocol final {
 private:
  using ResetFn    = Status(EFI_CALL*)(SimplePointerProtocol* self,
                                    bool extended_verification) noexcept;

  using GetStateFn = Status(EFI_CALL*)(SimplePointerProtocol* self,
                                       SimplePointerState* out_state) noexcept;

  const ResetFn reset_;
  const GetStateFn get_state_;
  const Event wait_for_input_;
  const SimpleInputMode* const mode_;

 public:
  SimplePointerProtocol()                                           = delete;
  SimplePointerProtocol(SimplePointerProtocol&&)                    = delete;
  SimplePointerProtocol(const SimplePointerProtocol&)               = delete;
  ~SimplePointerProtocol()                                          = delete;
  auto operator=(SimplePointerProtocol&&) -> SimplePointerProtocol& = delete;
  auto operator=(const SimplePointerProtocol&)
      -> SimplePointerProtocol& = delete;

  force_inline auto reset(bool extended_verification = false) noexcept {
    return reset_(this, extended_verification);
  }

  force_inline auto get_state(SimplePointerState* out_state) noexcept {
    return get_state_(this, out_state);
  }

  nodiscard auto event() const noexcept {
    return wait_for_input_;
  }

  nodiscard auto mode() const -> auto& {
    return *mode_;
  }

  static constexpr auto Guid =
      ::efi::Guid{0x31878c87,
                  0xb75,
                  0x11d5,
                  {0x9a, 0x4f, 0x00, 0x90, 0x27, 0x3f, 0xc1, 0x4d}};
};

class AbsolutePointerState final {
 private:
  uint64_t current_x_;
  uint64_t current_y_;
  uint64_t current_z_;
  uint32_t active_buttons_;
};

class AbsolutePointerMode final {
 private:
  uint64_t absolute_min_x_;
  uint64_t absolute_min_y_;
  uint64_t absolute_min_z_;
  uint64_t absolute_max_x_;
  uint64_t absolute_max_y_;
  uint64_t absolute_max_z_;
  uint32_t attributes;
};

class AbsolutePointerProtocol final {
  using ResetFn    = Status(EFI_CALL*)(AbsolutePointerProtocol* self,
                                    bool extended_verification) noexcept;

  using GetStateFn = Status(EFI_CALL*)(
      AbsolutePointerProtocol* self, AbsolutePointerState* out_state) noexcept;

  const ResetFn reset_;
  const GetStateFn get_state_;
  const Event wait_for_input_;
  const AbsolutePointerMode* const mode_;

 public:
  AbsolutePointerProtocol()                               = delete;
  AbsolutePointerProtocol(AbsolutePointerProtocol&&)      = delete;
  AbsolutePointerProtocol(const AbsolutePointerProtocol&) = delete;
  ~AbsolutePointerProtocol()                              = delete;
  auto operator=(AbsolutePointerProtocol&&)
      -> AbsolutePointerProtocol& = default;
  auto operator=(const AbsolutePointerProtocol&)
      -> AbsolutePointerProtocol& = default;

  auto reset(bool extended_verification = false) noexcept {
    return reset_(this, extended_verification);
  }

  auto get_state(AbsolutePointerState* out_state) noexcept {
    return get_state_(this, out_state);
  }

  nodiscard auto event() const noexcept {
    return wait_for_input_;
  }

  nodiscard auto mode() const noexcept -> auto& {
    return *mode_;
  }

  static constexpr auto Guid =
      ::efi::Guid{0x8D59D32B,
                  0xC655,
                  0x4AE9,
                  {0x9B, 0x15, 0xF2, 0x59, 0x04, 0x99, 0x2A, 0x43}};
};

}  // namespace efi
