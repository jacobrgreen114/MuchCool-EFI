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

class ManagedNetworkConfigData {
  uint32_t ReceivedQueueTimeoutValue;
  uint32_t TransmitQueueTimeoutValue;
  uint16_t ProtocolTypeFilter;
  bool EnableUnicastReceive;
  bool EnableMulticastReceive;
  bool EnableBroadcastReceive;
  bool EnablePromiscuousReceive;
  bool FlushQueuesOnReset;
  bool EnableReceiveTimestamps;
  bool DisableBackgroundPolling;
};

class ManagedNetworkProtocol final {
 private:
  // using GetModeDataFn =
  //     Status(EFI_CALL*)(ManagedNetworkProtocol* self,
  //                       ManagedNetworkConfigData* mng_config_data, )
  //                       noexcept;

 public:
  ManagedNetworkProtocol()                                            = delete;
  ManagedNetworkProtocol(ManagedNetworkProtocol&&)                    = delete;
  ManagedNetworkProtocol(const ManagedNetworkProtocol&)               = delete;
  ~ManagedNetworkProtocol()                                           = delete;
  auto operator=(ManagedNetworkProtocol&&) -> ManagedNetworkProtocol& = delete;
  auto operator=(const ManagedNetworkProtocol&)
      -> ManagedNetworkProtocol& = delete;

  static constexpr auto Guid =
      ::efi::Guid{0x7ab33a91,
                  0xace5,
                  0x4326,
                  {0xb5, 0x72, 0xe7, 0xee, 0x33, 0xd3, 0x9f, 0x16}};
};

}  // namespace efi