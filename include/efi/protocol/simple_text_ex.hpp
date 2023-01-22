

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

enum class KeyShiftState : uint32_t {
  RIGHT_SHIFT_PRESSED   = 0x00000001,
  LEFT_SHIFT_PRESSED    = 0x00000002,
  RIGHT_CONTROL_PRESSED = 0x00000004,
  LEFT_CONTROL_PRESSED  = 0x00000008,
  RIGHT_ALT_PRESSED     = 0x00000010,
  LEFT_ALT_PRESSED      = 0x00000020,
  RIGHT_LOGO_PRESSED    = 0x00000040,
  LEFT_LOGO_PRESSED     = 0x00000080,
  MENU_KEY_PRESSED      = 0x00000100,
  SYS_REQ_PRESSED       = 0x00000200,

  SHIFT_STATE_VALID     = 0x80000000,
};

enum class KeyToggleState : uint8_t {
  SCROLL_LOCK_ACTIVE = 0x01,
  NUM_LOCK_ACTIVE    = 0x02,
  CAPS_LOCK_ACTIVE   = 0x04,
  KEY_STATE_EXPOSED  = 0x40,
  TOGGLE_STATE_VALID = 0x80,
};

class KeyState final {
  KeyShiftState  key_shift_state_;
  KeyToggleState key_toggle_state_;
};

class KeyData final {
  InputKey key_;
  KeyState key_state_;
};

using KeyNotifyFunction = Status(EFI_CALL*)(const KeyData& key_data) noexcept;

using NotifyHandle      = Handle;

class SimpleTextInputExProtocol final {
 private:
  using ResetExFn   = Status(EFI_CALL*)(SimpleTextInputExProtocol* self,
                                      bool extended_verification);

  using ReadKeyExFn = Status(EFI_CALL*)(SimpleTextInputExProtocol* self,
                                        KeyData*                   key_data);

  using SetStateFn  = Status(EFI_CALL*)(SimpleTextInputExProtocol* self,
                                       const KeyToggleState& key_toggle_state);

  using RegisterKeystrokeNotifyFn = Status(EFI_CALL*)(
      SimpleTextInputExProtocol* self, const KeyData& key_data,
      KeyNotifyFunction key_notify_function, NotifyHandle* notify_handle);

  using UnregisterKeystrokeNotifyFn = Status(EFI_CALL*)(
      SimpleTextInputExProtocol* self, NotifyHandle notification_handle);

  const ResetExFn                   reset_;
  const ReadKeyExFn                 read_keystroke_;
  const Event                       wait_for_key_;
  const SetStateFn                  set_state_;
  const RegisterKeystrokeNotifyFn   register_key_notify_;
  const UnregisterKeystrokeNotifyFn unregister_key_notify_;

 public:
  SimpleTextInputExProtocol()                                 = delete;
  SimpleTextInputExProtocol(SimpleTextInputExProtocol&&)      = delete;
  SimpleTextInputExProtocol(const SimpleTextInputExProtocol&) = delete;
  ~SimpleTextInputExProtocol()                                = delete;
  auto operator=(SimpleTextInputExProtocol&&)
      -> SimpleTextInputExProtocol& = delete;
  auto operator=(const SimpleTextInputExProtocol&)
      -> SimpleTextInputExProtocol& = delete;

  FORCE_INLINE auto reset(bool extended_verificatin = false) noexcept {
    return reset_(this, extended_verificatin);
  }

  FORCE_INLINE auto read_keystroke(KeyData* data) noexcept {
    return read_keystroke_(this, data);
  }

  NODISCARD auto event() const noexcept {
    return wait_for_key_;
  }

  FORCE_INLINE auto set_state(const KeyToggleState& key_toggle_state) noexcept {
    return set_state_(this, key_toggle_state);
  }

  FORCE_INLINE auto register_key_notify(const KeyData&    key_data,
                                        KeyNotifyFunction key_notify_function,
                                        NotifyHandle* notify_handle) noexcept {
    return register_key_notify_(this, key_data, key_notify_function,
                                notify_handle);
  }

  FORCE_INLINE auto unregister_key_notify(
      NotifyHandle notification_handle) noexcept {
    return unregister_key_notify_(this, notification_handle);
  }

  static constexpr auto guid =
      Guid{0xdd9e7534,
           0x7762,
           0x4698,
           {0x8c, 0x14, 0xf5, 0x85, 0x17, 0xa6, 0x25, 0xaa}};
};

}  // namespace efi