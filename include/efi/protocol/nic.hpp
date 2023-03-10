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

#include "../core.hpp"

namespace efi {

enum class NetworkInterfaceType : uint8_t {
  Undi = 1
};

class NetworkInterfaceIdentifierProtocol final {
 private:
  using StringIdArray = std::array<char8_t, 4>;

  uint64_t             revision_;
  uint64_t             id_;
  uint64_t             image_addr_;
  uint32_t             image_size_;
  StringIdArray        string_id_;
  NetworkInterfaceType type_;
  uint8_t              major_ver_;
  uint8_t              minor_ver_;
  bool                 ipv6_supported_;
  uint16_t             if_num_;

 public:
  NetworkInterfaceIdentifierProtocol() = delete;
  NetworkInterfaceIdentifierProtocol(NetworkInterfaceIdentifierProtocol&&) =
      delete;
  NetworkInterfaceIdentifierProtocol(
      const NetworkInterfaceIdentifierProtocol&) = delete;
  ~NetworkInterfaceIdentifierProtocol()          = delete;
  auto operator=(NetworkInterfaceIdentifierProtocol&&)
      -> NetworkInterfaceIdentifierProtocol& = delete;
  auto operator=(const NetworkInterfaceIdentifierProtocol&)
      -> NetworkInterfaceIdentifierProtocol& = delete;

  NODISCARD auto id() const noexcept {
    return id_;
  }

  NODISCARD auto image_addr() const noexcept {
    return image_addr_;
  }

  NODISCARD auto image_size() const noexcept {
    return image_size_;
  }

  NODISCARD auto string_id() const noexcept {
    return string_id_;
  }

  NODISCARD auto type() const noexcept {
    return type_;
  }

  NODISCARD auto major_ver() const noexcept {
    return major_ver_;
  }

  NODISCARD auto minor_ver() const noexcept {
    return minor_ver_;
  }

  NODISCARD auto supports_ipv6() const noexcept {
    return ipv6_supported_;
  }

  NODISCARD auto interface_num() const noexcept {
    return if_num_;
  }

  static constexpr auto guid =
      Guid{0x1ACED566,
           0x76ED,
           0x4218,
           {0xBC, 0x81, 0x76, 0x7F, 0x1F, 0x97, 0x7A, 0x89}};
};

}  // namespace efi