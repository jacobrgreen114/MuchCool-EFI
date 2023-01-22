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

enum class Tcp4ConnectionState {
  Closed      = 0,
  Listen      = 1,
  SynSent     = 2,
  SynReceived = 3,
  Established = 4,
  FinWait1    = 5,
  FinWait2    = 6,
  Closing     = 7,
  TimeWait    = 8,
  CloseWait   = 9,
  LastAck     = 10
};

class Tcp4AccessPoint final {
  bool use_default_address_;
  Ipv4Address station_address_;
  Ipv4Address subnet_mask_;
  uint16_t station_port_;
  Ipv4Address remote_address_;
  uint16_t remote_port_;
  bool active_flag_;
};

class Tcp4Option final {
  uint32_t receive_buffer_size_;
  uint32_t send_buffer_size_;
  uint32_t max_syn_backlog_;
  uint32_t connection_timeout_;
  uint32_t data_retries_;
  uint32_t fin_timeout_;
  uint32_t time_wait_timeout_;
  uint32_t keep_alive_probes_;
  uint32_t keep_alive_time_;
  uint32_t keep_alive_interval_;
  bool enable_nagle_;
  bool enable_time_stamp_;
  bool enable_window_scaling_;
  bool enable_selective_ack_;
  bool enable_path_mtu_discovery_;
};

class Tcp4ConfigData final {
  uint8_t type_of_service_;
  uint8_t time_to_live_;
  Tcp4AccessPoint access_point_;
  Tcp4Option control_option_;
};

class Tcp4Protocol final {
 private:
  // using GetModeDataFn               = Status(EFI_CALL*)(Tcp4Protocol* self,
  //                                         Tcp4ConnectionState* tcp4_state,
  //                                         Tcp4ConfigData* tcp4_config_data,
  //                                         void* ip4_mode_data,
  //                                         ) noexcept;

  // todo : implement

 public:
  Tcp4Protocol()                                      = delete;
  Tcp4Protocol(Tcp4Protocol&&)                        = delete;
  Tcp4Protocol(const Tcp4Protocol&)                   = delete;
  ~Tcp4Protocol()                                     = delete;
  auto operator=(Tcp4Protocol&&) -> Tcp4Protocol      = delete;
  auto operator=(const Tcp4Protocol&) -> Tcp4Protocol = delete;

  static constexpr auto Guid =
      ::efi::Guid{0x65530BC7,
                  0xA359,
                  0x410f,
                  {0xB0, 0x10, 0x5A, 0xAD, 0xC7, 0xEC, 0x2B, 0x62}};
};

}  // namespace efi
