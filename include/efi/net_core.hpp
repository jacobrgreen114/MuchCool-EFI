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

enum class InetProtocol : uint16_t {
  Ip                     = 4,
  StDatagramMode         = 5,
  SimpleInternetProtocol = 6,
  TpIx                   = 7,
  PInternetProtocol      = 8,
  TUBA                   = 9
};

class MacAddress final {
 public:
  using Mac = std::array<uint8_t, 6>;

 private:
  union {
    std::array<uint8_t, 32> bytes;
    Mac mac;
  } data_{0};

 public:
  constexpr MacAddress()                                       = default;
  constexpr MacAddress(MacAddress &&)                          = default;
  constexpr MacAddress(const MacAddress &)                     = default;
  constexpr auto operator=(MacAddress &&) -> MacAddress      & = default;
  constexpr auto operator=(const MacAddress &) -> MacAddress & = default;

  constexpr explicit MacAddress(const std::array<uint8_t, 32> &bytes)
      : data_{.bytes = bytes} {}

  constexpr explicit MacAddress(const Mac &mac) : data_{.mac = mac} {}

  constexpr auto operator=(const Mac &mac) -> MacAddress & {
    data_.bytes = {0};
    data_.mac   = mac;
    return *this;
  }

  constexpr auto operator==(const MacAddress &mac) const noexcept -> bool {
    return data_.bytes == mac.data_.bytes;
  }
};

class IpAddress {
 private:
  IpAddress() = default;

  friend class Ipv4Address;
  friend class Ipv6Address;
};

class Ipv4Address final : public IpAddress {
 private:
  union {
    std::array<uint8_t, 4> bytes_;
    struct {
      uint8_t data1;
      uint8_t data2;
      uint8_t data3;
      uint8_t data4;
    } s_;
  };

 public:
  constexpr explicit Ipv4Address(const std::array<uint8_t, 4> &bytes)
      : bytes_{bytes} {}

  constexpr Ipv4Address(uint8_t data1, uint8_t data2, uint8_t data3,
                        uint8_t data4)
      : s_{.data1 = data1, .data2 = data2, .data3 = data3, .data4 = data4} {}

  constexpr auto operator==(const Ipv4Address &address) const noexcept -> bool {
    return bytes_ == address.bytes_;
  }
};

class Ipv6Address final : public IpAddress {
 private:
  union {
    std::array<uint8_t, 16> bytes_;
  };

 public:
  constexpr explicit Ipv6Address(const std::array<uint8_t, 16> &bytes)
      : bytes_{bytes} {}

  constexpr auto operator==(const Ipv6Address &address) const noexcept -> bool {
    return bytes_ == address.bytes_;
  }
};

}  // namespace efi