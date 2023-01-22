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

class EdidDiscoveredProtocol final {
 private:
  const uint32_t size_of_edid_;
  const uint8_t* const edid_;

 public:
  EdidDiscoveredProtocol()                                            = delete;
  EdidDiscoveredProtocol(EdidDiscoveredProtocol&&)                    = delete;
  EdidDiscoveredProtocol(const EdidDiscoveredProtocol&)               = delete;
  ~EdidDiscoveredProtocol()                                           = delete;
  auto operator=(EdidDiscoveredProtocol&&) -> EdidDiscoveredProtocol& = delete;
  auto operator=(const EdidDiscoveredProtocol&)
      -> EdidDiscoveredProtocol& = delete;

  NODISCARD auto size() const {
    return size_of_edid_;
  }
  NODISCARD auto edid() const {
    return edid_;
  }

  static constexpr auto Guid =
      ::efi::Guid{0x1c0c34f6,
                  0xd380,
                  0x41fa,
                  {0xa0, 0x49, 0x8a, 0xd0, 0x6c, 0x1a, 0x66, 0xaa}};
};

class EdidActiveProtocol final {
 private:
  const uint32_t size_of_edid_;
  const uint8_t* const edid_;

 public:
  EdidActiveProtocol()                                             = delete;
  EdidActiveProtocol(EdidActiveProtocol&&)                         = delete;
  EdidActiveProtocol(const EdidActiveProtocol&)                    = delete;
  ~EdidActiveProtocol()                                            = delete;
  auto operator=(EdidActiveProtocol&&) -> EdidActiveProtocol&      = delete;
  auto operator=(const EdidActiveProtocol&) -> EdidActiveProtocol& = delete;

  NODISCARD auto size() const {
    return size_of_edid_;
  }
  NODISCARD auto edid() const {
    return edid_;
  }

  static constexpr auto Guid =
      ::efi::Guid{0xbd8c1056,
                  0x9f36,
                  0x44ec,
                  {0x92, 0xa8, 0xa6, 0x33, 0x7f, 0x81, 0x79, 0x86}};
};

class EdidOverrideProtocol final {
 private:
  using GetEDIDFn = Status(EFI_CALL*)(EdidOverrideProtocol* self,
                                      const Handle* child_handle,
                                      uint32_t* attributes, uintn_t* edid_size,
                                      uint8_t** edid) noexcept;

  const GetEDIDFn get_edid_;

 public:
  EdidOverrideProtocol()                                               = delete;
  EdidOverrideProtocol(EdidOverrideProtocol&&)                         = delete;
  EdidOverrideProtocol(const EdidOverrideProtocol&)                    = delete;
  ~EdidOverrideProtocol()                                              = delete;
  auto operator=(EdidOverrideProtocol&&) -> EdidOverrideProtocol&      = delete;
  auto operator=(const EdidOverrideProtocol&) -> EdidOverrideProtocol& = delete;

  FORCE_INLINE auto get_edid(const Handle* child_handle, uint32_t* attributes,
                             uintn_t* edid_size, uint8_t** edid) noexcept {
    return get_edid_(this, child_handle, attributes, edid_size, edid);
  }

  static constexpr auto Guid =
      ::efi::Guid{0x48ecb431,
                  0xfb72,
                  0x45c0,
                  {0xa9, 0x22, 0xf4, 0x58, 0xfe, 0x04, 0x0b, 0xd5}};
};

}  // namespace efi