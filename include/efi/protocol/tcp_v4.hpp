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

#include "ipv4.hpp"

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
  bool        use_default_address_;
  Ipv4Address station_address_;
  Ipv4Address subnet_mask_;
  uint16_t    station_port_;
  Ipv4Address remote_address_;
  uint16_t    remote_port_;
  bool        active_flag_;
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
  bool     enable_nagle_;
  bool     enable_time_stamp_;
  bool     enable_window_scaling_;
  bool     enable_selective_ack_;
  bool     enable_path_mtu_discovery_;
};

class Tcp4ConfigData final {
  uint8_t         type_of_service_;
  uint8_t         time_to_live_;
  Tcp4AccessPoint access_point_;
  Tcp4Option      control_option_;
};

class Tcp4CompletionToken {
  Event  event_;
  Status status_;
};

class Tcp4ConnectionToken : public Tcp4CompletionToken {};

class Tcp4ListenToken : public Tcp4CompletionToken {
  Handle new_child_handle_;
};

class Tcp4CloseToken : public Tcp4CompletionToken {
  bool abort_on_close_;
};

class Tcp4FragmentData final {
  uint32_t length_;
  void*    buffer_;
};

class Tcp4ReceiveData final {
  bool             urgent_;
  uint32_t         data_length_;
  uint32_t         fragment_count_;
  Tcp4FragmentData fragment_table_[1];
};

class Tcp4TransmitData final {
  bool             push_;
  bool             urgent_;
  uint32_t         data_length_;
  uint32_t         fragment_count_;
  Tcp4FragmentData fragment_table_[1];
};

class Tcp4TransmitToken final : public Tcp4CompletionToken {
  Tcp4TransmitData* data_;

 public:
  NODISCARD auto* data() const noexcept {
    return data_;
  }
};

class Tcp4ReceiveToken final : public Tcp4CompletionToken {
  Tcp4ReceiveData* data_;

 public:
  NODISCARD auto* data() const noexcept {
    return data_;
  }
};

class Tcp4Protocol final {
 private:
  using GetModeDataFn = Status(EFI_CALL*)(
      Tcp4Protocol* self, Tcp4ConnectionState* tcp4_state,
      Tcp4ConfigData* tcp4_config_data, Ipv4ModeData* ip4_mode_data,
      ManagedNetworkConfigData* mnp_config_data,
      SimpleNetworkMode*        snp_mode_data) noexcept;

  using ConfigureFn = Status(EFI_CALL*)(
      Tcp4Protocol* self, const Tcp4ConfigData* config_data) noexcept;

  using RoutesFn   = Status(EFI_CALL*)(Tcp4Protocol* self, bool delete_,
                                     const Ipv4Address& subnet,
                                     const Ipv4Address& subnet_mask,
                                     const Ipv4Address& gateway) noexcept;

  using ConnectFn  = Status(EFI_CALL*)(Tcp4Protocol*        self,
                                      Tcp4ConnectionToken* token) noexcept;

  using AcceptFn   = Status(EFI_CALL*)(Tcp4Protocol*    self,
                                     Tcp4ListenToken* token) noexcept;

  using TransmitFn = Status(EFI_CALL*)(Tcp4Protocol*      self,
                                       Tcp4TransmitToken* token) noexcept;

  using ReceiveFn  = Status(EFI_CALL*)(Tcp4Protocol*     self,
                                      Tcp4ReceiveToken* token) noexcept;

  using CloseFn    = Status(EFI_CALL*)(Tcp4Protocol*   self,
                                    Tcp4CloseToken* token) noexcept;

  using CancelFn   = Status(EFI_CALL*)(Tcp4Protocol*        self,
                                     Tcp4CompletionToken* token) noexcept;

  using PollFn     = Status(EFI_CALL*)(const Tcp4Protocol* self) noexcept;

  const GetModeDataFn get_mode_data_;
  const ConfigureFn   configure_;
  const RoutesFn      routes_;
  const ConnectFn     connect_;
  const AcceptFn      accept_;
  const TransmitFn    transmit_;
  const ReceiveFn     receive_;
  const CloseFn       close_;
  const CancelFn      cancel_;
  const PollFn        poll_;

 public:
  Tcp4Protocol()                                      = delete;
  Tcp4Protocol(Tcp4Protocol&&)                        = delete;
  Tcp4Protocol(const Tcp4Protocol&)                   = delete;
  ~Tcp4Protocol()                                     = delete;
  auto operator=(Tcp4Protocol&&) -> Tcp4Protocol      = delete;
  auto operator=(const Tcp4Protocol&) -> Tcp4Protocol = delete;

  auto get_mode_data(Tcp4ConnectionState*      tcp4_state,
                     Tcp4ConfigData*           tcp4_config_data,
                     Ipv4ModeData*             ip4_mode_data,
                     ManagedNetworkConfigData* mnp_config_data,
                     SimpleNetworkMode*        snp_mode_data) noexcept {
    return get_mode_data_(this, tcp4_state, tcp4_config_data, ip4_mode_data,
                          mnp_config_data, snp_mode_data);
  }

  auto configure(const Tcp4ConfigData& config_data) noexcept {
    return configure_(this, &config_data);
  }

  auto reset() noexcept {
    return configure_(this, nullptr);
  }

  auto add_route(const Ipv4Address& subnet, const Ipv4Address& subnet_mask,
                 const Ipv4Address& gateway) noexcept {
    return routes_(this, false, subnet, subnet_mask, gateway);
  }

  auto delete_route(const Ipv4Address& subnet, const Ipv4Address& subnet_mask,
                    const Ipv4Address& gateway) noexcept {
    return routes_(this, true, subnet, subnet_mask, gateway);
  }

  auto connect(Tcp4ConnectionToken* token) noexcept {
    return connect_(this, token);
  }

  auto accept(Tcp4ListenToken* token) noexcept {
    return accept_(this, token);
  }

  auto transmit(Tcp4TransmitToken* token) noexcept {
    return transmit_(this, token);
  }

  auto receive(Tcp4ReceiveToken* token) noexcept {
    return receive_(this, token);
  }

  auto close(Tcp4CloseToken* token) noexcept {
    return close_(this, token);
  }

  auto cancel(Tcp4CompletionToken* token) noexcept {
    return cancel_(this, token);
  }

  NODISCARD auto poll() noexcept {
    return poll_(this);
  }

  static constexpr auto guid =
      Guid{0x65530BC7,
           0xA359,
           0x410f,
           {0xB0, 0x10, 0x5A, 0xAD, 0xC7, 0xEC, 0x2B, 0x62}};
};

}  // namespace efi
