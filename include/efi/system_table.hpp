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

#include "core.hpp"
#include "boot_services.hpp"
#include "runtime_services.hpp"
#include "efi/protocol/simple_text.hpp"

namespace efi {

class ConfigurationTable final {
 private:
  Guid vendor_guid_;
  void* vendor_table;

 public:
  nodiscard auto& guid() const noexcept {
    return vendor_guid_;
  }

  nodiscard auto* table() const noexcept {
    return vendor_table;
  }
};

class SystemTable final : public Table {
  const char16_t* const firmware_vendor_;
  const uint32_t firmware_revision_;

  const Handle con_in_handle_;
  SimpleTextInputProtocol* const con_in_;
  const Handle con_out_handle_;
  SimpleTextOutputProtocol* const con_out_;
  const Handle std_err_handle_;
  SimpleTextOutputProtocol* const std_err_;

  RuntimeServices* const runtime_services_;
  BootServices* const boot_services_;

  const uintn_t num_table_entries_;
  ConfigurationTable* const configuration_table_;

 public:
  SystemTable()                                      = delete;
  SystemTable(SystemTable&&)                         = delete;
  SystemTable(const SystemTable&)                    = delete;
  ~SystemTable()                                     = delete;
  auto operator=(SystemTable&&) -> SystemTable&      = delete;
  auto operator=(const SystemTable&) -> SystemTable& = delete;

  nodiscard auto* firmware_vendor() const noexcept {
    return firmware_vendor_;
  }

  nodiscard auto* con_out() const noexcept {
    return con_out_;
  }

  nodiscard auto* con_in() const noexcept {
    return con_in_;
  }

  nodiscard auto* std_err() const noexcept {
    return std_err_;
  }

  nodiscard auto* runtime_services() const noexcept {
    return runtime_services_;
  }

  nodiscard auto* boot_services() const noexcept {
    return boot_services_;
  }

  auto get_configuration_table(uintn_t index,
                               ConfigurationTable** entry) const noexcept
      -> Status {
    if (entry == nullptr) return Status::InvalidParameter;
    if (index > num_table_entries_) {
      *entry = nullptr;
      return Status::InvalidParameter;
    }

    auto configuration_tables =
        std::span<ConfigurationTable>{configuration_table_, num_table_entries_};

    *entry = &configuration_tables[index];
    return Status::Success;
  }

  auto find_configuration_table(const Guid& guid,
                                ConfigurationTable** entry) const noexcept
      -> Status {
    auto configuration_tables =
        std::span<ConfigurationTable>{configuration_table_, num_table_entries_};

    for (auto& table : configuration_tables) {
      if (table.guid() == guid) {
        *entry = &table;
        return Status::Success;
      }
    }

    *entry = nullptr;
    return Status::NotFound;
  }

  static constexpr uint64_t Signature = 0x5453595320494249;
};

constexpr auto MPSTableGuid =
    Guid{0xeb9d2d2f,
         0x2d88,
         0x11d3,
         {0x9a, 0x16, 0x0, 0x90, 0x27, 0x3f, 0xc1, 0x4d}};

constexpr auto ACPITableGuid =
    Guid{0xeb9d2d30,
         0x2d88,
         0x11d3,
         {0x9a, 0x16, 0x0, 0x90, 0x27, 0x3f, 0xc1, 0x4d}};

constexpr auto ACPI20TableGuid =
    Guid{0x8868e871,
         0xe4f1,
         0x11d3,
         {0xbc, 0x22, 0x0, 0x80, 0xc7, 0x3c, 0x88, 0x81}};

constexpr auto SMBIOSTableGuid =
    Guid{0xeb9d2d31,
         0x2d88,
         0x11d3,
         {0x9a, 0x16, 0x0, 0x90, 0x27, 0x3f, 0xc1, 0x4d}};

constexpr auto SMBIOS3TableGuid =
    Guid{0xf2fd1544,
         0x9794,
         0x4a2c,
         {0x99, 0x2e, 0xe5, 0xbb, 0xcf, 0x20, 0xe3, 0x94}};

constexpr auto SALSystemTableGuid =
    Guid{0xeb9d2d32,
         0x2d88,
         0x11d3,
         {0x9a, 0x16, 0x0, 0x90, 0x27, 0x3f, 0xc1, 0x4d}};

constexpr auto DTBTableGuid =
    Guid{0xb1b621d5,
         0xf19c,
         0x41a5,
         {0x83, 0x0b, 0xd9, 0x15, 0x2c, 0x69, 0xaa, 0xe0}};

}  // namespace efi