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

class Dns4HostToAddrData final {
  uint32_t ip_count_;
  Ipv4Address* ip_list_;
};

class Dns4AddrToHostData final {
  char16_t* hostname_;
};

class Dns4ResourceRecord final {
  char8_t* qname_;
  uint16_t qtype_;
  uint16_t qclass_;
  uint32_t ttl_;
  uint16_t data_length_;
  char8_t* r_data_;
};

class Dns4GeneralLookupData final {
  uintn_t rr_count_;
  Dns4ResourceRecord* rr_list_;
};

class Dns4CompletionToken {
  Event event_{nullptr};
  Status status_{Status::Success};
  uint32_t retry_count_;
  uint32_t retry_interval_;
  union {
    Dns4HostToAddrData* h2a_data;
    Dns4AddrToHostData* a2h_data;
    Dns4GeneralLookupData* glookup_data;
  } data_{nullptr};

 private:
  constexpr explicit Dns4CompletionToken(uint32_t retry_count    = 0,
                                         uint32_t retry_interval = 0)
      : retry_count_{retry_count}, retry_interval_{retry_interval} {}

 public:
  Dns4CompletionToken(Dns4CompletionToken&&)                         = delete;
  Dns4CompletionToken(const Dns4CompletionToken&)                    = delete;
  ~Dns4CompletionToken()                                             = default;
  auto operator=(Dns4CompletionToken&&) -> Dns4CompletionToken&      = delete;
  auto operator=(const Dns4CompletionToken&) -> Dns4CompletionToken& = delete;

  NODISCARD auto event() const noexcept {
    return event_;
  }

  NODISCARD auto status() const noexcept {
    return status_;
  }

  NODISCARD auto retry_count() const noexcept {
    return retry_count_;
  }

  NODISCARD auto retry_interval() const noexcept {
    return retry_interval_;
  }

  friend class Dns4HostToAddrCompletionToken;
  friend class Dns4AddrToHostCompletionToken;
  friend class Dns4GeneralLookupCompletionToken;
};

class Dns4HostToAddrCompletionToken final : public Dns4CompletionToken {
 public:
  constexpr explicit Dns4HostToAddrCompletionToken(uint32_t retry_count    = 0,
                                                   uint32_t retry_interval = 0)
      : Dns4CompletionToken{retry_count, retry_interval} {}

  NODISCARD auto* data() const noexcept {
    return data_.h2a_data;
  }
};

class Dns4AddrToHostCompletionToken final : public Dns4CompletionToken {
 public:
  constexpr explicit Dns4AddrToHostCompletionToken(uint32_t retry_count    = 0,
                                                   uint32_t retry_interval = 0)
      : Dns4CompletionToken{retry_count, retry_interval} {}

  NODISCARD auto* data() const noexcept {
    return data_.a2h_data;
  }
};

class Dns4GeneralLookupCompletionToken final : public Dns4CompletionToken {
 public:
  constexpr explicit Dns4GeneralLookupCompletionToken(
      uint32_t retry_count = 0, uint32_t retry_interval = 0)
      : Dns4CompletionToken{retry_count, retry_interval} {}

  NODISCARD auto* data() const noexcept {
    return data_.glookup_data;
  }
};

class Dns4ConfigData final {
  uintn_t dns_server_list_count_;
  Ipv4Address* dns_server_list_;
  bool use_default_setting_;
  bool enable_dns_cache_;
  uint8_t protocol_;
  Ipv4Address station_ip_;
  Ipv4Address subnet_mask_;
  uint16_t local_port_;
  uint32_t retry_count_;
  uint32_t retry_interval_;
};

class Dns4CacheEntry final {
  const char16_t* hostname_;
  Ipv4Address* ip_address_;
  uint32_t timeout_;

 public:
  constexpr Dns4CacheEntry(const char16_t* hostname, Ipv4Address* address,
                           uint32_t timeout)
      : hostname_{hostname}, ip_address_{address}, timeout_{timeout} {}

  NODISCARD constexpr auto* hostname() const noexcept {
    return hostname_;
  }

  NODISCARD constexpr auto* ip_address() const noexcept {
    return ip_address_;
  }

  NODISCARD constexpr auto timeout() const noexcept {
    return timeout_;
  }
};

class Dns4ModeData final {
  Dns4ConfigData dns_config_data_;
  uint32_t dns_server_count_;
  Ipv4Address* dns_server_list_;
  uint32_t dns_cache_count_;
  Dns4CacheEntry* dns_cache_list_;
};

class DNS4Protocol final {
 private:
  using GetModeDataFn = Status(EFI_CALL*)(
      DNS4Protocol* self, Dns4ConfigData* dns_mode_data) noexcept;

  using ConfigureFn = Status(EFI_CALL*)(
      DNS4Protocol* self, const Dns4ConfigData* dns_mode_data) noexcept;

  using HostnameToIpFn =
      Status(EFI_CALL*)(DNS4Protocol* self, const char16_t* hostname,
                        Dns4HostToAddrCompletionToken* token) noexcept;

  using IpToHostnameFn =
      Status(EFI_CALL*)(DNS4Protocol* self, const Ipv4Address ip_address,
                        Dns4AddrToHostCompletionToken* token) noexcept;

  using GeneralLookupFn = Status(EFI_CALL*)(
      DNS4Protocol* self, const char8_t* qname, uint16_t qtype, uint16_t qclass,
      Dns4GeneralLookupCompletionToken* token) noexcept;

  using UpdateDnsCacheFn =
      Status(EFI_CALL*)(DNS4Protocol* self, bool delete_flag, bool override,
                        const Dns4CacheEntry dns_cache_entry) noexcept;

  using PollFn   = Status(EFI_CALL*)(const DNS4Protocol* self) noexcept;

  using CancelFn = Status(EFI_CALL*)(const DNS4Protocol* self,
                                     Dns4CompletionToken* token) noexcept;

  const GetModeDataFn get_mode_data_;
  const ConfigureFn configure_;
  const HostnameToIpFn hostname_to_ip_;
  const IpToHostnameFn ip_to_hostname_;
  const GeneralLookupFn general_lookup_;
  const UpdateDnsCacheFn update_dns_cache_;
  const PollFn poll_;
  const CancelFn cancel_;

 public:
  DNS4Protocol()                                       = delete;
  DNS4Protocol(DNS4Protocol&&)                         = delete;
  DNS4Protocol(const DNS4Protocol&)                    = delete;
  ~DNS4Protocol()                                      = delete;
  auto operator=(DNS4Protocol&&) -> DNS4Protocol&      = delete;
  auto operator=(const DNS4Protocol&) -> DNS4Protocol& = delete;

  FORCE_INLINE auto get_mode_data(Dns4ConfigData* dns_mode_data) noexcept {
    return get_mode_data_(this, dns_mode_data);
  }

  FORCE_INLINE auto configure(const Dns4ConfigData& dns_mode_data) noexcept {
    return configure_(this, &dns_mode_data);
  }

  FORCE_INLINE auto reset() noexcept {
    return configure_(this, nullptr);
  }

  FORCE_INLINE auto hostname_to_ip(
      const char16_t* hostname, Dns4HostToAddrCompletionToken* token) noexcept {
    return hostname_to_ip_(this, hostname, token);
  }

  FORCE_INLINE auto ip_to_hostname(
      const Ipv4Address ip_address,
      Dns4AddrToHostCompletionToken* token) noexcept {
    return ip_to_hostname_(this, ip_address, token);
  }

  FORCE_INLINE auto general_lookup(
      const char8_t* qname, uint16_t qtype, uint16_t qclass,
      Dns4GeneralLookupCompletionToken* token) noexcept {
    return general_lookup_(this, qname, qtype, qclass, token);
  }

  FORCE_INLINE auto update_dns_cache(
      bool delete_flag, bool override,
      const Dns4CacheEntry dns_cache_entry) noexcept {
    return update_dns_cache_(this, delete_flag, override, dns_cache_entry);
  }

  FORCE_INLINE auto poll() noexcept {
    return poll_(this);
  }

  FORCE_INLINE auto cancel(Dns4CompletionToken* token) noexcept {
    return cancel_(this, token);
  }

  static constexpr auto Guid =
      ::efi::Guid{0xae3d28cc,
                  0xe05b,
                  0x4fa1,
                  {0xa0, 0x11, 0x7e, 0xb5, 0x5a, 0x3f, 0x14, 0x1}};
};

}  // namespace efi