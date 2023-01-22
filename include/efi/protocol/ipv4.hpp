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

#include "managed_network.hpp"

namespace efi {

class Ipv4IcmpType final {
  uint8_t type_;
  uint8_t code_;
};

class Ipv4Route final {
  Ipv4Address subnet_;
  Ipv4Address subnet_mask_;
  Ipv4Address gateway_;
};

class Ipv4ConfigData final {
  uint8_t default_protocol_;
  bool accept_any_protocol_;
  bool accept_icmp_errors_;
  bool accept_broadcast_;
  bool accept_promiscuous_;
  bool use_default_addr_;
  Ipv4Address station_addr_;
  Ipv4Address subnet_mask_;
  uint8_t type_of_service_;
  uint8_t time_to_live_;
  bool do_not_fragment_;
  bool raw_data_;
  uint32_t receive_timeout_;
  uint32_t transmit_timeout_;
};

class Ipv4ModeData final {
  bool started_;
  uint32_t max_packet_size_;
  Ipv4ConfigData config_data_;
  bool configured_;
  uint32_t group_count_;
  Ipv4Address* group_table_;
  uint32_t route_count_;
  Ipv4Route* route_table_;
  uint32_t icmp_type_count_;
  Ipv4IcmpType* icmp_type_list;
};

#pragma pack(push, 1)
class Ipv4Header final {
  uint8_t header_length_ : 4;
  uint8_t version_ : 4;
  uint8_t type_of_service_;
  uint16_t total_length_;
  uint16_t identification_;
  uint16_t fragmentation_;
  uint8_t time_to_live_;
  uint8_t protocol_;
  uint16_t checksum_;
  Ipv4Address source_address_;
  Ipv4Address destination_address_;
};
#pragma pack(pop)

class Ipv4FragmentData final {
  uint32_t length_;
  void* buffer_;
};

class Ipv4OverrideData final {
  Ipv4Address source_address_;
  Ipv4Address gateway_address_;
  uint8_t protocol_;
  uint8_t type_of_service_;
  uint8_t time_to_live_;
  bool do_not_fragment_;
};

class Ipv4ReceiveData final {
  Time timestamp_;
  Event recycle_signal_;
  uint32_t header_length_;
  Ipv4Header* header_;
  uint32_t options_length_;
  void* options_;
  uint32_t data_length_;
  uint32_t fragment_count_;
  Ipv4FragmentData fragment_table_[1];

 public:

  NODISCARD auto fragments() noexcept {
    return std::span<Ipv4FragmentData>{fragment_table_, fragment_count_};
  }

};

class Ipv4TransmitData final {
  Ipv4Address DestinationAddress;
  Ipv4OverrideData* OverrideData;
  uint32_t OptionsLength;
  void* OptionsBuffer;
  uint32_t TotalDataLength;
  uint32_t FragmentCount;
  Ipv4FragmentData FragmentTable[1];
};

class Ipv4CompletionToken {
  Event event_;
  Status status_;
  union {
    Ipv4ReceiveData* rx_data;
    Ipv4TransmitData* tx_data;
  } data_;

 public:
  NODISCARD auto event() const noexcept {
    return event_;
  }

  NODISCARD auto status() const noexcept {
    return status_;
  }

  friend class Ipv4ReceiveCompletionToken;
  friend class Ipv4TransmitCompletionToken;
};

class Ipv4ReceiveCompletionToken final : public Ipv4CompletionToken {
 public:
  NODISCARD auto* data() const noexcept {
    return data_.rx_data;
  }
};

class Ipv4TransmitCompletionToken final : public Ipv4CompletionToken {
 public:
  NODISCARD auto* data() const noexcept {
    return data_.tx_data;
  }
};

class Ipv4Protocol final {
 private:
  using GetModeDataFn =
      Status(EFI_CALL*)(Ipv4Protocol* self, Ipv4ModeData* ipv4_mode_data,
                        ManagedNetworkConfigData* mnp_config_data,
                        SimpleNetworkMode* snp_mode_data) noexcept;

  using ConfigureFn = Status(EFI_CALL*)(
      Ipv4Protocol* self, const Ipv4ConfigData* config_data) noexcept;

  using GroupsFn   = Status(EFI_CALL*)(Ipv4Protocol* self, bool join,
                                     const Ipv4Address* group) noexcept;

  using RoutesFn   = Status(EFI_CALL*)(Ipv4Protocol* self, bool delete_,
                                     const Ipv4Address& subnet,
                                     const Ipv4Address& subnet_mask,
                                     const Ipv4Address& gateway) noexcept;

  using TransmitFn = Status(EFI_CALL*)(
      Ipv4Protocol* self, Ipv4TransmitCompletionToken* token) noexcept;

  using ReceiveFn = Status(EFI_CALL*)(
      Ipv4Protocol* self, Ipv4ReceiveCompletionToken* token) noexcept;

  using CancelFn = Status(EFI_CALL*)(Ipv4Protocol* self,
                                     Ipv4CompletionToken* token) noexcept;

  using PollFn   = Status(EFI_CALL*)(const Ipv4Protocol* self) noexcept;

  const GetModeDataFn get_mode_data_;
  const ConfigureFn configure_;
  const GroupsFn groups_;
  const RoutesFn routes_;
  const TransmitFn transmit_;
  const ReceiveFn receive_;
  const CancelFn cancel_;
  const PollFn poll_;

 public:
  Ipv4Protocol()                                       = delete;
  Ipv4Protocol(Ipv4Protocol&&)                         = delete;
  Ipv4Protocol(const Ipv4Protocol&)                    = delete;
  ~Ipv4Protocol()                                      = delete;
  auto operator=(Ipv4Protocol&&) -> Ipv4Protocol&      = delete;
  auto operator=(const Ipv4Protocol&) -> Ipv4Protocol& = delete;

  auto get_mode_data(Ipv4ModeData* ipv4_mode_data,
                     ManagedNetworkConfigData* mnp_config_data,
                     SimpleNetworkMode* snp_mode_data) noexcept {
    return get_mode_data_(this, ipv4_mode_data, mnp_config_data, snp_mode_data);
  }

  auto configure(const Ipv4ConfigData* config_data) noexcept {
    return configure_(this, config_data);
  }

  auto reset() noexcept {
    return configure_(this, nullptr);
  }

  auto join_group(const Ipv4Address& address) noexcept {
    return groups_(this, true, &address);
  }

  auto leave_group(const Ipv4Address& address) noexcept {
    return groups_(this, false, &address);
  }

  auto leave_all_groups() noexcept {
    return groups_(this, false, nullptr);
  }

  auto add_route(const Ipv4Address& subnet, const Ipv4Address& subnet_mask,
                 const Ipv4Address& gateway) noexcept {
    return routes_(this, false, subnet, subnet_mask, gateway);
  }

  auto delete_route(const Ipv4Address& subnet, const Ipv4Address& subnet_mask,
                    const Ipv4Address& gateway) noexcept {
    return routes_(this, true, subnet, subnet_mask, gateway);
  }

  auto transmit(Ipv4TransmitCompletionToken* token) noexcept {
    return transmit_(this, token);
  }

  auto recieve(Ipv4ReceiveCompletionToken* token) noexcept {
    return receive_(this, token);
  }

  auto cancel(Ipv4CompletionToken* token) noexcept {
    return cancel_(this, token);
  }

  NODISCARD auto poll() const noexcept {
    return poll_(this);
  }

  static constexpr auto guid =
      Guid{0x41d94cd2,
           0x35b6,
           0x455a,
           {0x82, 0x58, 0xd4, 0xe5, 0x13, 0x34, 0xaa, 0xdd}};
};

}  // namespace efi