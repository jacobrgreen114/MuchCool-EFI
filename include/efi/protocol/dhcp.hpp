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
// MuchCool-EFI. If not, see <https://www.gnu.org/licenses/>.gma once

#ifndef __cplusplus
#error
#endif

#include "efi/core.hpp"

namespace efi {

enum class Dhcp4State {
  Stopped    = 0x0,
  Init       = 0x1,
  Selecting  = 0x2,
  Requesting = 0x3,
  Bound      = 0x4,
  Renewing   = 0x5,
  Rebinding  = 0x6,
  InitReboot = 0x7,
  Rebooting  = 0x8
};

enum class Dhcp4Event {
  SendDiscover   = 0x01,
  RcvdOffer      = 0x02,
  SelectOffer    = 0x03,
  SendRequest    = 0x04,
  RcvdAck        = 0x05,
  RcvdNak        = 0x06,
  SendDecline    = 0x07,
  BoundCompleted = 0x08,
  EnterRenewing  = 0x09,
  EnterRebinding = 0x0a,
  AddressLost    = 0x0b,
  Fail           = 0x0c
};

class MacAddress final {
 private:
  union {
    std::array<uint8_t, 32> bytes_;
  };

 public:
  consteval explicit MacAddress(const std::array<uint8_t, 32> &bytes)
      : bytes_{bytes} {}

  consteval auto operator==(const MacAddress &mac) const noexcept -> bool {
    return bytes_ == mac.bytes_;
  }
};

class IPv4Address final {
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
  constexpr explicit IPv4Address(const std::array<uint8_t, 4> &bytes)
      : bytes_{bytes} {}

  constexpr IPv4Address(uint8_t data1, uint8_t data2, uint8_t data3,
                        uint8_t data4)
      : s_{.data1 = data1, .data2 = data2, .data3 = data3, .data4 = data4} {}

  constexpr auto operator==(const IPv4Address &address) const noexcept -> bool {
    return bytes_ == address.bytes_;
  }
};

class IPv6Address final {
 private:
  union {
    std::array<uint8_t, 16> bytes_;
  };

 public:
  constexpr explicit IPv6Address(const std::array<uint8_t, 16> &bytes)
      : bytes_{bytes} {}

  constexpr auto operator==(const IPv6Address &address) const noexcept -> bool {
    return bytes_ == address.bytes_;
  }
};

class DHCP4Protocol;

#pragma pack(1)
class Dhcp4PacketOption {
  uint8_t op_code_;
  uint8_t length_;
  uint8_t data_[1];
};
#pragma pack()

#pragma pack(1)
class Dhcp4Header final {
  uint8_t op_code_;
  uint8_t hw_type_;
  uint8_t hw_addr_len_;
  uint8_t hops_;
  uint32_t xid_;
  uint16_t seconds;
  uint16_t reserved_;
  IPv4Address client_addr_;
  IPv4Address your_addr_;
  IPv4Address server_addr_;
  IPv4Address gateway_addr_;
  uint8_t client_hw_addr_[16];
  char8_t server_name[64];
  char8_t boot_file_name[128];
};
#pragma pack()

#pragma pack(1)
class Dhcp4Packet final {
  uint32_t size_;
  uint32_t length_;
  struct {
    Dhcp4Header header_;
    uint32_t magik_;
    uint8_t option_[1];
  } dhcp4;
};
#pragma pack()

class DHCP4Protocol;

using Dhcp4Callback = Status(EFI_CALL *)(DHCP4Protocol *self, void *context,
                                         Dhcp4State current_state,
                                         Dhcp4Event Dhcp4Event,
                                         Dhcp4Packet *Packet,
                                         Dhcp4Packet **NewPacket);

class Dhcp4ListenPoint final {
  IPv4Address listen_address_;
  IPv4Address subnet_mask_;
  uint16_t listen_port_;
};

class Dhcp4TransmitReceiveToken final {
  Status status_;
  Event completion_event_;
  IPv4Address remote_address_;
  uint16_t remote_port_;
  IPv4Address gateway_address_;
  uint32_t listen_point_count_;
  Dhcp4ListenPoint *listen_points_;
  uint32_t timeout_value_;
  Dhcp4Packet *packet_;
  uint32_t response_count_;
  Dhcp4Packet *response_list_;
};

class Dhcp4ConfigData {
  uint32_t discover_try_count_;
  uint32_t *discover_timeout_;
  uint32_t request_try_count_;
  uint32_t *request_timeout_;
  IPv4Address client_address_;
  Dhcp4Callback dhcp4_callback_;
  void *callback_context_;
  uint32_t option_count_;
  Dhcp4PacketOption **option_list_;
};

class Dhcp4ModeData final {
  Dhcp4State state_;
  Dhcp4ConfigData config_data_;
  IPv4Address client_address_;
  MacAddress client_mac_address_;
  IPv4Address server_address_;
  IPv4Address router_address_;
  IPv4Address subnet_mask_;
  uint32_t lease_time_;
  Dhcp4Packet *reply_packet_;
};

class DHCP4Protocol final {
 private:
  using GetModeDataFn = Status(EFI_CALL *)(
      DHCP4Protocol *self, Dhcp4ModeData *dhcp4_mode_data) noexcept;

  using ConfigureFn = Status(EFI_CALL *)(
      DHCP4Protocol *self, const Dhcp4ModeData *dhcp4_mode_data) noexcept;

  using StartFn           = Status(EFI_CALL *)(DHCP4Protocol *self,
                                     Event completion_event) noexcept;

  using RenewRebindFn     = Status(EFI_CALL *)(DHCP4Protocol *self,
                                           bool rebid_request,
                                           Event completion_event) noexcept;

  using ReleaseFn         = Status(EFI_CALL *)(DHCP4Protocol *self) noexcept;

  using StopFn            = Status(EFI_CALL *)(DHCP4Protocol *self) noexcept;

  using BuildFn           = Status(EFI_CALL *)(DHCP4Protocol *self,
                                     const Dhcp4Packet *seed_packet,
                                     uint32_t delete_count,
                                     const uint8_t *delete_list,
                                     uint32_t append_count,
                                     const Dhcp4PacketOption *append_list[],
                                     Dhcp4Packet **new_packet) noexcept;

  using TransmitRecieveFn = Status(EFI_CALL *)(
      DHCP4Protocol *self, const Dhcp4TransmitReceiveToken &token) noexcept;

  using ParseFn = Status(EFI_CALL *)(
      DHCP4Protocol *self, Dhcp4Packet *packet, uint32_t *option_count,
      Dhcp4PacketOption *packet_option_list[]) noexcept;

  const GetModeDataFn get_mode_data_;
  const ConfigureFn configure_;
  const StartFn start_;
  const RenewRebindFn renew_rebind_;
  const ReleaseFn release_;
  const StopFn stop_;
  const BuildFn build_;
  const TransmitRecieveFn transmit_recieve_;
  const ParseFn parse_;

 public:
  DHCP4Protocol()                                          = delete;
  DHCP4Protocol(DHCP4Protocol &&)                          = delete;
  DHCP4Protocol(const DHCP4Protocol &)                     = delete;
  ~DHCP4Protocol()                                         = delete;
  auto operator=(DHCP4Protocol &&) -> DHCP4Protocol      & = delete;
  auto operator=(const DHCP4Protocol &) -> DHCP4Protocol & = delete;

  force_inline auto get_mode_data(Dhcp4ModeData *dhcp4_mode_data) noexcept {
    return get_mode_data_(this, dhcp4_mode_data);
  }

  force_inline auto configure(const Dhcp4ModeData &data) noexcept {
    return configure_(this, &data);
  }

  force_inline auto reset() noexcept {
    return configure_(this, nullptr);
  }

  force_inline auto start(Event completion_event) noexcept {
    return start_(this, completion_event);
  }

  force_inline auto renew_rebind(bool rebid_request,
                                 Event completion_event) noexcept {
    return renew_rebind_(this, rebid_request, completion_event);
  }

  force_inline auto release() noexcept {
    return release_(this);
  }

  force_inline auto stop() noexcept {
    return stop_(this);
  }

  force_inline auto build(const Dhcp4Packet &seed_packet, uint32_t delete_count,
                          const uint8_t *delete_list, uint32_t append_count,
                          const Dhcp4PacketOption *append_list[],
                          Dhcp4Packet **new_packet) noexcept {
    return build_(this, &seed_packet, delete_count, delete_list, append_count,
                  append_list, new_packet);
  }

  force_inline auto transmit_recieve(
      const Dhcp4TransmitReceiveToken &token) noexcept {
    return transmit_recieve_(this, token);
  }

  force_inline auto parse(Dhcp4Packet *packet, uint32_t *option_count,
                          Dhcp4PacketOption *packet_option_list[]) noexcept {
    return parse_(this, packet, option_count, packet_option_list);
  }

  static constexpr auto Guid =
      ::efi::Guid{0x8a219718,
                  0x4ef5,
                  0x4761,
                  {0x91, 0xc8, 0xc0, 0xf0, 0x4b, 0xda, 0x9e, 0x56}};
};

class DHCP6Protocol final {
  // todo : implement
};

}  // namespace efi
