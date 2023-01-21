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

class ArpConfigData {
  uint16_t sw_address_type_;
  uint8_t sw_address_length_;
  void* station_address_;
  uint32_t entry_timeout_;
  uint32_t retry_count_;
  uint32_t retry_timeout_;
};

class ArpFindData {
  uint32_t size_;
  bool deny_flag_;
  bool static_flag_;
  uint16_t hw_address_type_;
  uint16_t sw_address_type_;
  uint8_t hw_address_length_;
  uint8_t sw_address_length_;
};

class ArpProtocol final {
 private:
  using ConfigureFn = Status(EFI_CALL*)(ArpProtocol* self,
                                        const ArpConfigData* data) noexcept;

  using AddFn       = Status(EFI_CALL*)(ArpProtocol* self, bool deny_flag,
                                  const void* target_sw_address,
                                  const void* target_hw_address,
                                  uint32_t timeout_value,
                                  bool overwrite) noexcept;

  using FindFn      = Status(EFI_CALL*)(ArpProtocol* self, bool by_sw_address,
                                   const void* address_buffer,
                                   uint32_t* entry_length,
                                   uint32_t* entry_count, ArpFindData** entries,
                                   bool refresh) noexcept;

  using DeleteFn    = Status(EFI_CALL*)(ArpProtocol* self, bool by_sw_address,
                                     const void* address_buffer) noexcept;

  using FlushFn     = Status(EFI_CALL*)(ArpProtocol* self) noexcept;

  using RequestFn   = Status(EFI_CALL*)(ArpProtocol* self,
                                      const void* target_sw_address,
                                      Event resolved_event,
                                      void* target_hw_address) noexcept;

  using CancelFn    = Status(EFI_CALL*)(ArpProtocol* self,
                                     const void* target_sw_address,
                                     Event resolved_event) noexcept;

  const ConfigureFn configure_;
  const AddFn add_;
  const FindFn find_;
  const DeleteFn delete_;
  const FlushFn flush_;
  const RequestFn request_;
  const CancelFn cancel_;

 public:
  ArpProtocol()                                      = delete;
  ArpProtocol(ArpProtocol&&)                         = delete;
  ArpProtocol(const ArpProtocol&)                    = delete;
  ~ArpProtocol()                                     = delete;
  auto operator=(ArpProtocol&&) -> ArpProtocol&      = delete;
  auto operator=(const ArpProtocol&) -> ArpProtocol& = delete;

  force_inline auto configure(const ArpConfigData& data) noexcept {
    return configure_(this, &data);
  }

  force_inline auto reset() noexcept {
    return configure_(this, nullptr);
  }

  force_inline auto add(bool deny_flag, const void* target_sw_address,
                        const void* target_hw_address, uint32_t timeout_value,
                        bool overwrite) noexcept {
    return add_(this, deny_flag, target_sw_address, target_hw_address,
                timeout_value, overwrite);
  }

  force_inline auto find(bool by_sw_address, const void* address_buffer,
                         uint32_t* entry_length, uint32_t* entry_count,
                         ArpFindData** entries, bool refresh) noexcept {
    return find_(this, by_sw_address, address_buffer, entry_length, entry_count,
                 entries, refresh);
  }

  force_inline auto delete_entry(bool by_sw_address,
                                 const void* address_buffer) noexcept {
    return delete_(this, by_sw_address, address_buffer);
  }

  force_inline auto flush() noexcept {
    return flush_(this);
  }

  force_inline auto request(const void* target_sw_address, Event resolved_event,
                            void* target_hw_address) noexcept {
    return request_(this, target_sw_address, resolved_event, target_hw_address);
  }

  force_inline auto cancel(const void* target_sw_address,
                           Event resolved_event) noexcept {
    return cancel_(this, target_sw_address, resolved_event);
  }

  static constexpr auto Guid =
      ::efi::Guid{0xf4b427bb,
                  0xba21,
                  0x4f16,
                  {0xbc, 0x4e, 0x43, 0xe4, 0x16, 0xab, 0x61, 0x9c}};
};

}  // namespace efi