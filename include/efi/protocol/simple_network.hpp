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

#include "../net_core.hpp"

namespace efi {

enum class SimpleNetworkState : uint32_t {
  Stopped,
  Started,
  Initialized,
};

enum class RecieveFilterSettings : uint32_t {
  Unicast              = 0x01,
  Multicast            = 0x02,
  Broadcast            = 0x04,
  Promiscuous          = 0x08,
  PromiscuousMulticast = 0x10,
};

ENUM_FLAGS(RecieveFilterSettings);

class NetworkStatistics {
  uint64_t RxTotalFrames;
  uint64_t RxGoodFrames;
  uint64_t RxUndersizeFrames;
  uint64_t RxOversizeFrames;
  uint64_t RxDroppedFrames;
  uint64_t RxUnicastFrames;
  uint64_t RxBroadcastFrames;
  uint64_t RxMulticastFrames;
  uint64_t RxCrcErrorFrames;
  uint64_t RxTotalBytes;
  uint64_t TxTotalFrames;
  uint64_t TxGoodFrames;
  uint64_t TxUndersizeFrames;
  uint64_t TxOversizeFrames;
  uint64_t TxDroppedFrames;
  uint64_t TxUnicastFrames;
  uint64_t TxBroadcastFrames;
  uint64_t TxMulticastFrames;
  uint64_t TxCrcErrorFrames;
  uint64_t TxTotalBytes;
  uint64_t Collisions;
  uint64_t UnsupportedProtocol;
  uint64_t RxDuplicatedFrames;
  uint64_t RxDecryptErrorFrames;
  uint64_t TxErrorFrames;
  uint64_t TxRetryFrames;
};

class SimpleNetworkMode final {
  static constexpr auto MCAST_FILTER_COUNT = 16;
  using MCastFilterArray = std::array<MacAddress, MCAST_FILTER_COUNT>;

  SimpleNetworkState State;
  uint32_t HwAddressSize;
  uint32_t MediaHeaderSize;
  uint32_t MaxPacketSize;
  uint32_t NvRamSize;
  uint32_t NvRamAccessSize;
  uint32_t ReceiveFilterMask;
  RecieveFilterSettings ReceiveFilterSetting;
  uint32_t MaxMCastFilterCount;
  uint32_t MCastFilterCount;
  MCastFilterArray MCastFilter;
  MacAddress CurrentAddress;
  MacAddress BroadcastAddress;
  MacAddress PermanentAddress;
  uint8_t IfType;
  bool MacAddressChangeable;
  bool MultipleTxSupported;
  bool MediaPresentSupported;
  bool MediaPresent;
};

class SimpleNetworkProtocol final {
 private:
  using StartFn      = Status(EFI_CALL*)(SimpleNetworkProtocol* self) noexcept;

  using StopFn       = Status(EFI_CALL*)(SimpleNetworkProtocol* self) noexcept;

  using InitializeFn = Status(EFI_CALL*)(SimpleNetworkProtocol* self,
                                         uintn_t extra_rx_buffer_size,
                                         uintn_t extra_tx_buffer_size) noexcept;

  using ResetFn      = Status(EFI_CALL*)(SimpleNetworkProtocol* self,
                                    bool extended_verification) noexcept;

  using ShutdownFn   = Status(EFI_CALL*)(SimpleNetworkProtocol* self) noexcept;

  using ReceiveFiltersFn = Status(EFI_CALL*)(
      SimpleNetworkProtocol* self, uint32_t enable, uint32_t disable,
      bool reset_mcast_filter, uintn_t mcast_filter_count,
      const MacAddress* mcast_filter) noexcept;

  using StationAddressFn =
      Status(EFI_CALL*)(SimpleNetworkProtocol* self, bool reset,
                        const MacAddress* new_address) noexcept;

  using StatisticsFn = Status(EFI_CALL*)(
      SimpleNetworkProtocol* self, bool reset, uintn_t* statistics_size,
      NetworkStatistics* statistics_table) noexcept;

  using MCastIpToMacFn = Status(EFI_CALL*)(SimpleNetworkProtocol* self,
                                           bool ipv6, const IpAddress& ip,
                                           MacAddress* mac) noexcept;

  using NvDataFn       = Status(EFI_CALL*)(SimpleNetworkProtocol* self,
                                     bool read_write, uintn_t offset,
                                     uintn_t buffer_size,
                                     void* buffer) noexcept;

  using GetStatusFn    = Status(EFI_CALL*)(SimpleNetworkProtocol* self,
                                        uint32_t* interrupt_status,
                                        void** tx_buf) noexcept;

  using TransmitFn     = Status(EFI_CALL*)(SimpleNetworkProtocol* self,
                                       uintn_t header_size, uintn_t buffer_size,
                                       const void* buffer,
                                       const MacAddress* src_addr,
                                       const MacAddress* dst_addr,
                                       const InetProtocol* protocol) noexcept;

  using ReceiveFn      = Status(EFI_CALL*)(SimpleNetworkProtocol* self,
                                      uintn_t* header_size,
                                      uintn_t* buffer_size, void* buffer,
                                      MacAddress* src_addr,
                                      MacAddress* dst_addr,
                                      InetProtocol* protocol) noexcept;

  const uint64_t revision_;
  const StartFn start_;
  const StopFn stop_;
  const InitializeFn initialize_;
  const ResetFn reset_;
  const ShutdownFn shutdown_;
  const ReceiveFiltersFn receive_filters_;
  const StationAddressFn station_address_;
  const StatisticsFn statistics_;
  const MCastIpToMacFn mcast_ip_to_mac_;
  const NvDataFn nv_data_;
  const GetStatusFn get_status_;
  const TransmitFn transmit_;
  const ReceiveFn receive_;
  const Event wait_for_packet_;

 public:
  SimpleNetworkProtocol()                                           = delete;
  SimpleNetworkProtocol(SimpleNetworkProtocol&&)                    = delete;
  SimpleNetworkProtocol(const SimpleNetworkProtocol&)               = delete;
  ~SimpleNetworkProtocol()                                          = delete;
  auto operator=(SimpleNetworkProtocol&&) -> SimpleNetworkProtocol& = delete;
  auto operator=(const SimpleNetworkProtocol&)
      -> SimpleNetworkProtocol& = delete;

  FORCE_INLINE auto start() noexcept {
    return start_(this);
  }

  FORCE_INLINE auto stop() noexcept {
    return stop_(this);
  }

  FORCE_INLINE auto initialize(uintn_t extra_rx_buffer_size = 0,
                               uintn_t extra_tx_buffer_size = 0) noexcept {
    return initialize_(this, extra_rx_buffer_size, extra_tx_buffer_size);
  }

  FORCE_INLINE auto reset(bool extended_verification = false) noexcept {
    return reset_(this, extended_verification);
  }

  FORCE_INLINE auto shutdown() noexcept {
    return shutdown_(this);
  }

  FORCE_INLINE auto receive_filters(uint32_t enable, uint32_t disable,
                                    bool reset_mcast_filter,
                                    uintn_t mcast_filter_count,
                                    const MacAddress* mcast_filter) noexcept {
    return receive_filters_(this, enable, disable, reset_mcast_filter,
                            mcast_filter_count, mcast_filter);
  }

  FORCE_INLINE auto station_address(bool reset,
                                    const MacAddress* new_address) noexcept {
    return station_address_(this, reset, new_address);
  }

  FORCE_INLINE auto statistics(bool reset, uintn_t* statistics_size,
                               NetworkStatistics* statistics_table) noexcept {
    return statistics_(this, reset, statistics_size, statistics_table);
  }

  FORCE_INLINE auto mcast_ip_to_mac(const Ipv4Address& ip,
                                    MacAddress* mac) noexcept {
    return mcast_ip_to_mac_(this, false, ip, mac);
  }

  FORCE_INLINE auto mcast_ip_to_mac(const Ipv6Address& ip,
                                    MacAddress* mac) noexcept {
    return mcast_ip_to_mac_(this, true, ip, mac);
  }

  FORCE_INLINE auto nv_data(bool read_write, uintn_t offset,
                            uintn_t buffer_size, void* buffer) noexcept {
    return nv_data_(this, read_write, offset, buffer_size, buffer);
  }

  FORCE_INLINE auto get_status(uint32_t* interrupt_status,
                               void** tx_buf) noexcept {
    return get_status_(this, interrupt_status, tx_buf);
  }

  FORCE_INLINE auto transmit(uintn_t header_size, uintn_t buffer_size,
                             const void* buffer, const MacAddress* src_addr,
                             const MacAddress* dst_addr,
                             const InetProtocol* protocol) noexcept {
    return transmit_(this, header_size, buffer_size, buffer, src_addr, dst_addr,
                     protocol);
  }

  FORCE_INLINE auto receive(uintn_t* header_size, uintn_t* buffer_size,
                            void* buffer, MacAddress* src_addr,
                            MacAddress* dst_addr,
                            InetProtocol* protocol) noexcept {
    return receive_(this, header_size, buffer_size, buffer, src_addr, dst_addr,
                    protocol);
  }

  NODISCARD auto event() const noexcept {
    return wait_for_packet_;
  }

  static constexpr auto Guid =
      ::efi::Guid{0xA19832B9,
                  0xAC25,
                  0x11D3,
                  {0x9A, 0x2D, 0x00, 0x90, 0x27, 0x3f, 0xc1, 0x4d}};
};

}  // namespace efi
