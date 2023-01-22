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

enum class TextAttribute : uint32_t {
  Black               = 0x00,
  Blue                = 0x01,
  Green               = 0x02,
  Cyan                = 0x03,
  Red                 = 0x04,
  Magenta             = 0x05,
  Brown               = 0x06,
  LightGray           = 0x07,
  DarkGray            = 0x08,
  LightBlue           = 0x09,
  LightGreen          = 0x0A,
  LightCyan           = 0x0B,
  LightRed            = 0x0C,
  LightMagenta        = 0x0D,
  Yellow              = 0x0E,
  White               = 0x0F,

  BackgroundBlack     = 0x00,
  BackgroundBlue      = 0x10,
  BackgroundGreen     = 0x20,
  BackgroundCyan      = 0x30,
  BackgroundRed       = 0x40,
  BackgroundMagenta   = 0x50,
  BackgroundBrown     = 0x60,
  BackgroundLightGray = 0x70,
};

class InputKey {
 private:
  uint16_t scan_code_;
  char16_t unicode_char_;
};

class TextOutputMode {
 private:
  uint32_t      max_mode_;

  int32_t       mode_;
  TextAttribute attribute_;
  int32_t       cursor_column_;
  int32_t       cursor_row_;
  bool          visible_;
};

class SimpleTextInputProtocol final {
 private:
  using InputResetFn   = Status(EFI_CALL*)(SimpleTextInputProtocol* self,
                                         bool extended_verification) noexcept;

  using InputReadKeyFn = Status(EFI_CALL*)(SimpleTextInputProtocol* self,
                                           InputKey* out_key) noexcept;

  const InputResetFn   reset_;
  const InputReadKeyFn read_key_stroke_;
  const Event          wait_for_key_;

 public:
  SimpleTextInputProtocol()                               = delete;
  SimpleTextInputProtocol(SimpleTextInputProtocol&&)      = delete;
  SimpleTextInputProtocol(const SimpleTextInputProtocol&) = delete;
  ~SimpleTextInputProtocol()                              = delete;
  auto operator=(SimpleTextInputProtocol&&)
      -> SimpleTextInputProtocol& = delete;
  auto operator=(const SimpleTextInputProtocol&)
      -> SimpleTextInputProtocol& = delete;

  FORCE_INLINE auto reset(bool extended_verification = false) noexcept {
    return reset_(this, extended_verification);
  }

  FORCE_INLINE auto read_key_stroke(InputKey* out_key) noexcept {
    return read_key_stroke_(this, out_key);
  }

  FORCE_INLINE auto wait_event() noexcept {
    return wait_for_key_;
  }

  static constexpr auto guid =
      Guid{0x387477c1,
           0x69c7,
           0x11d2,
           {0x8e, 0x39, 0x00, 0xa0, 0xc9, 0x69, 0x72, 0x3b}};
};

class SimpleTextOutputProtocol final {
 private:
  using TextResetFn        = Status(EFI_CALL*)(SimpleTextOutputProtocol* self,
                                        bool extended_verification) noexcept;

  using TextStringFn       = Status(EFI_CALL*)(SimpleTextOutputProtocol* self,
                                         const char16_t* str) noexcept;

  using TextTestStringFn   = Status(EFI_CALL*)(SimpleTextOutputProtocol* self,
                                             const char16_t* str) noexcept;

  using TextQueryModeFn    = Status(EFI_CALL*)(SimpleTextOutputProtocol* self,
                                            uintn_t  mode_number,
                                            uintn_t* out_columns,
                                            uintn_t* out_rows) noexcept;

  using TextSetModeFn      = Status(EFI_CALL*)(SimpleTextOutputProtocol* self,
                                          uintn_t mode_number) noexcept;

  using TextSetAttributeFn = Status(EFI_CALL*)(
      SimpleTextOutputProtocol* self, TextAttribute attribute) noexcept;

  using TextClearScreenFn =
      Status(EFI_CALL*)(SimpleTextOutputProtocol* self) noexcept;

  using TextSetCursorPositionFn = Status(EFI_CALL*)(
      SimpleTextOutputProtocol* self, uintn_t column, uintn_t row) noexcept;

  using TextEnableCursorFn = Status(EFI_CALL*)(SimpleTextOutputProtocol* self,
                                               bool visible) noexcept;

  const TextResetFn             reset_;
  const TextStringFn            output_string_;
  const TextTestStringFn        test_string_;
  const TextQueryModeFn         query_mode_;
  const TextSetModeFn           set_mode_;
  const TextSetAttributeFn      set_attribute_;
  const TextClearScreenFn       clear_screen_;
  const TextSetCursorPositionFn set_cursor_position_;
  const TextEnableCursorFn      enable_cursor_;
  const TextOutputMode*         mode_;

 public:
  SimpleTextOutputProtocol()                                = delete;
  SimpleTextOutputProtocol(SimpleTextOutputProtocol&&)      = delete;
  SimpleTextOutputProtocol(const SimpleTextOutputProtocol&) = delete;
  ~SimpleTextOutputProtocol()                               = delete;
  auto operator=(SimpleTextOutputProtocol&&)
      -> SimpleTextOutputProtocol& = delete;
  auto operator=(const SimpleTextOutputProtocol&)
      -> SimpleTextOutputProtocol& = delete;

  FORCE_INLINE auto reset(bool extended_verification = false) noexcept {
    return reset_(this, extended_verification);
  }

  FORCE_INLINE auto output_string(const char16_t* str) noexcept {
    return output_string_(this, str);
  }

  FORCE_INLINE auto test_string(const char16_t* str) noexcept {
    return test_string_(this, str);
  }

  FORCE_INLINE auto query_mode(uintn_t mode, uintn_t* out_columns,
                               uintn_t* out_rows) noexcept {
    return query_mode_(this, mode, out_columns, out_rows);
  }

  FORCE_INLINE auto set_mode(uintn_t mode) noexcept {
    return set_mode_(this, mode);
  }

  FORCE_INLINE auto set_attribute(TextAttribute attribute) noexcept {
    return set_attribute_(this, attribute);
  }

  FORCE_INLINE auto clear_screen() noexcept {
    return clear_screen_(this);
  }

  FORCE_INLINE auto set_cursor_position(uintn_t column, uintn_t row) noexcept {
    return set_cursor_position_(this, column, row);
  }

  FORCE_INLINE auto enable_cursor(bool visible) noexcept {
    return enable_cursor_(this, visible);
  }

  static constexpr auto guid =
      Guid{0x387477c2,
           0x69c7,
           0x11d2,
           {0x8e, 0x39, 0x00, 0xa0, 0xc9, 0x69, 0x72, 0x3b}};
};

}  // namespace efi