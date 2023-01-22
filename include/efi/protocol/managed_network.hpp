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

#include "simple_network.hpp"

namespace efi {

class ManagedNetworkReceiveData final {
  Time timestamp_;
  Event recycle_event_;
  uint32_t packet_length_;
  uint32_t header_length_;
  uint32_t address_length_;
  uint32_t data_length_;
  bool roadcast_flag_;
  bool multicast_flag_;
  bool promiscuous_flag_;
  uint16_t protocol_type_;
  void* destination_address_;
  void* source_address_;
  void* media_header_;
  void* packet_data_;
};

class ManagedNetworkFragmentData final {
  uint32_t length_;
  void* buffer_;
};

class ManagedNetworkTransmitData final {
  MacAddress* destination_address_;
  MacAddress* source_address_;
  uint16_t protocol_type_;
  uint32_t data_length_;
  uint16_t header_length_;
  uint16_t fragment_count_;
  ManagedNetworkFragmentData fragment_table_[1];

 public:
  auto fragments() noexcept {
    return std::span<ManagedNetworkFragmentData>{fragment_table_,
                                                 fragment_count_};
  }
};

class ManagedNetworkCompletionToken {
  Event event_;
  Status status_;
  union {
    ManagedNetworkReceiveData* rx_data;
    ManagedNetworkTransmitData* tx_data;
  } data_;

 private:
  ManagedNetworkCompletionToken() = default;

 public:
  NODISCARD auto event() const noexcept {
    return event_;
  }

  NODISCARD auto status() const noexcept {
    return status_;
  }

  friend class ManagedNetworkReceiveCompletionToken;
  friend class ManagedNetworkTransmitCompletionToken;
};

class ManagedNetworkReceiveCompletionToken final
    : public ManagedNetworkCompletionToken {
 public:
  NODISCARD auto* data() const noexcept {
    return data_.rx_data;
  }
};

class ManagedNetworkTransmitCompletionToken final
    : public ManagedNetworkCompletionToken {
 public:
  NODISCARD auto* data() const noexcept {
    return data_.tx_data;
  }
};

class ManagedNetworkConfigData final {
  uint32_t received_queue_timeout_value_;
  uint32_t transmit_queue_timeout_value_;
  uint16_t protocol_type_filter_;
  bool enable_unicast_receive_;
  bool enable_multicast_receive_;
  bool enable_broadcast_receive_;
  bool enable_promiscuous_receive_;
  bool flush_queues_on_reset_;
  bool enable_receive_timestamps_;
  bool disable_background_polling_;
};

class ManagedNetworkProtocol final {
 private:
  using GetModeDataFn = Status(EFI_CALL*)(
      ManagedNetworkProtocol* self, ManagedNetworkConfigData* config_data,
      SimpleNetworkMode* mode_data) noexcept;

  using ConfigureFn =
      Status(EFI_CALL*)(ManagedNetworkProtocol* self,
                        const ManagedNetworkConfigData* config_data) noexcept;

  using MCastIpToMacFn = Status(EFI_CALL*)(ManagedNetworkProtocol* self,
                                           bool ipv6,
                                           const IpAddress& ip_address,
                                           MacAddress* mac_address) noexcept;

  using GroupsFn = Status(EFI_CALL*)(ManagedNetworkProtocol* self, bool join,
                                     const MacAddress* mac_address) noexcept;

  using TransmitFn =
      Status(EFI_CALL*)(ManagedNetworkProtocol* self,
                        ManagedNetworkTransmitCompletionToken* token) noexcept;

  using ReceiveFn =
      Status(EFI_CALL*)(ManagedNetworkProtocol* self,
                        ManagedNetworkReceiveCompletionToken* token) noexcept;

  using CancelFn =
      Status(EFI_CALL*)(ManagedNetworkProtocol* self,
                        ManagedNetworkCompletionToken* token) noexcept;

  using PollFn = Status(EFI_CALL*)(const ManagedNetworkProtocol* self) noexcept;

  const GetModeDataFn get_mode_data_;
  const ConfigureFn configure_;
  const MCastIpToMacFn mcast_ip_to_mac_;
  const GroupsFn groups_;
  const TransmitFn transmit_;
  const ReceiveFn receive_;
  const CancelFn cancel_;
  const PollFn poll_;

 public:
  ManagedNetworkProtocol()                                            = delete;
  ManagedNetworkProtocol(ManagedNetworkProtocol&&)                    = delete;
  ManagedNetworkProtocol(const ManagedNetworkProtocol&)               = delete;
  ~ManagedNetworkProtocol()                                           = delete;
  auto operator=(ManagedNetworkProtocol&&) -> ManagedNetworkProtocol& = delete;
  auto operator=(const ManagedNetworkProtocol&)
      -> ManagedNetworkProtocol& = delete;

  FORCE_INLINE auto get_mode_data(ManagedNetworkConfigData* config_data,
                                  SimpleNetworkMode* mode_data) noexcept {
    return get_mode_data_(this, config_data, mode_data);
  }

  FORCE_INLINE auto configure(
      const ManagedNetworkConfigData* config_data) noexcept {
    return configure_(this, config_data);
  }

  FORCE_INLINE auto mcast_ip_to_mac(const Ipv4Address& ip_address,
                                    MacAddress* mac_address) noexcept {
    return mcast_ip_to_mac_(this, false, ip_address, mac_address);
  }

  FORCE_INLINE auto mcast_ip_to_mac(const Ipv6Address& ip_address,
                                    MacAddress* mac_address) noexcept {
    return mcast_ip_to_mac_(this, true, ip_address, mac_address);
  }

  FORCE_INLINE auto join_group(const MacAddress& mac_address) noexcept {
    return groups_(this, true, &mac_address);
  }

  FORCE_INLINE auto leave_group(const MacAddress& mac_address) noexcept {
    return groups_(this, false, &mac_address);
  }

  FORCE_INLINE auto leave_all_groups() noexcept {
    return groups_(this, false, nullptr);
  }

  FORCE_INLINE auto transmit(
      ManagedNetworkTransmitCompletionToken* token) noexcept {
    return transmit_(this, token);
  }

  FORCE_INLINE auto receive(
      ManagedNetworkReceiveCompletionToken* token) noexcept {
    return receive_(this, token);
  }

  FORCE_INLINE auto cancel(ManagedNetworkCompletionToken* token) noexcept {
    return cancel_(this, token);
  }

  FORCE_INLINE auto poll() noexcept {
    return poll_(this);
  }

  static constexpr auto Guid =
      ::efi::Guid{0x7ab33a91,
                  0xace5,
                  0x4326,
                  {0xb5, 0x72, 0xe7, 0xee, 0x33, 0xd3, 0x9f, 0x16}};
};

}  // namespace efi