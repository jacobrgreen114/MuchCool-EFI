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

enum class VariableAttribute : uint32_t {
  NonVolatile                       = 0x00000001,
  BootServiceAccess                 = 0x00000002,
  RuntimeAccess                     = 0x00000004,
  HardwareErrorRecord               = 0x00000008,
  AuthenticatedWriteAccess          = 0x00000010,
  TimeBasedAuthenticatedWriteAccess = 0x00000020,
  AppendWrite                       = 0x00000040,
};

enum class CapsuleFlag : uint32_t {
  PersistAcrossReset  = 0x00010000,
  PopulateSystemTable = 0x00020000,
  InitiateReset       = 0x00040000,
};

class CapsuleHeader {
 private:
  Guid capsule_guid_;
  uint32_t header_size_;
  CapsuleFlag flags_;
  uint32_t capsule_image_size_;
};

class RuntimeServices final : public Table {
 private:
  using GetTimeFn       = Status(EFI_CALL*)(Time* time,
                                      TimeCapabilities* capabilities) noexcept;

  using SetTimeFn       = Status(EFI_CALL*)(const Time& time) noexcept;

  using GetWakeupTimeFn = Status(EFI_CALL*)(bool* enabled, bool* pending,
                                            Time* time) noexcept;

  using SetWakeupTimeFn = Status(EFI_CALL*)(bool enabled,
                                            const Time* time) noexcept;

  using GetNextHighMonotonicCountFn =
      Status(EFI_CALL*)(uint32_t* high_count) noexcept;

  using SetVirtualAddressMapFn = Status(EFI_CALL*)(
      uintn_t memory_map_size, uintn_t descriptor_size,
      uintn_t descriptor_version, const MemoryDescriptor* virtual_map) noexcept;

  using ConvertPointerFn      = Status(EFI_CALL*)(uintn_t debug_disposition,
                                             void** address) noexcept;

  using GetVariableFn         = Status(EFI_CALL*)(const char16_t* variable_name,
                                          const Guid& vendor_guid,
                                          VariableAttribute* attributes,
                                          uintn_t* data_size,
                                          void* data) noexcept;

  using GetNextVariableNameFn = Status(EFI_CALL*)(uintn_t* variable_name_size,
                                                  char16_t* variable_name,
                                                  Guid* vendor_guid) noexcept;

  using SetVariableFn         = Status(EFI_CALL*)(const char16_t* variable_name,
                                          const Guid& vendor_guid,
                                          VariableAttribute attributes,
                                          uintn_t data_size,
                                          const void* data) noexcept;

  using QueryVariableInfoFn   = Status(EFI_CALL*)(
      uint32_t attributes, uint64_t* maximum_variable_storage_size,
      uint64_t* remaining_variable_storage_size,
      uint64_t* maximum_variable_size) noexcept;

  using ResetSystemFn   = void(EFI_CALL*)(ResetType reset_type,
                                        Status reset_status, uintn_t data_size,
                                        const void* reset_data) noexcept;

  using UpdateCapsuleFn = Status(EFI_CALL*)(
      CapsuleHeader** capsule_header_array, uintn_t capsule_count,
      PhysicalAddress scatter_gather_list) noexcept;

  using QueryCapsuleCapabilitiesFn = Status(EFI_CALL*)(
      CapsuleHeader** capsule_header_array, uintn_t capsule_count,
      uint64_t* maximum_capsule_size, ResetType* reset_type) noexcept;

  const GetTimeFn get_time_;
  const SetTimeFn set_time_;
  const GetWakeupTimeFn get_wakeup_time_;
  const SetWakeupTimeFn set_wakeup_time_;

  const SetVirtualAddressMapFn set_virtual_address_map_;
  const ConvertPointerFn convert_pointer_;

  const GetVariableFn get_variable_;
  const GetNextVariableNameFn get_next_variable_name_;
  const SetVariableFn set_variable_;

  const GetNextHighMonotonicCountFn get_next_high_monotonic_count_;
  const ResetSystemFn reset_system_;

  const UpdateCapsuleFn update_capsule_;
  const QueryCapsuleCapabilitiesFn query_capsule_capabilities_;

  const QueryVariableInfoFn query_variable_info_;

 public:
  RuntimeServices()                                          = delete;
  RuntimeServices(RuntimeServices&&)                         = delete;
  RuntimeServices(const RuntimeServices&)                    = delete;
  ~RuntimeServices()                                         = delete;
  auto operator=(RuntimeServices&&) -> RuntimeServices&      = delete;
  auto operator=(const RuntimeServices&) -> RuntimeServices& = delete;

  FORCE_INLINE auto get_time(Time* time,
                             TimeCapabilities* capabilities) noexcept {
    return get_time_(time, capabilities);
  }

  FORCE_INLINE auto set_time(const Time& time) noexcept {
    return set_time_(time);
  }

  FORCE_INLINE auto get_wakeup_time(bool* enabled, bool* pending,
                                    Time* time) noexcept {
    return get_wakeup_time_(enabled, pending, time);
  }

  FORCE_INLINE auto set_wakeup_time() noexcept {
    return set_wakeup_time_(false, nullptr);
  }

  FORCE_INLINE auto set_wakeup_time(const Time& time) noexcept {
    return set_wakeup_time_(true, &time);
  }

  FORCE_INLINE auto set_virtual_address_map(
      uintn_t memory_map_size, uintn_t descriptor_size,
      uintn_t descriptor_version,
      const MemoryDescriptor* virtual_map) noexcept {
    return set_virtual_address_map_(memory_map_size, descriptor_size,
                                    descriptor_version, virtual_map);
  }

  FORCE_INLINE auto convert_pointer(uintn_t debug_disposition,
                                    void** address) noexcept {
    return convert_pointer_(debug_disposition, address);
  }

  FORCE_INLINE auto get_variable(const char16_t* variable_name,
                                 const Guid& vendor_guid,
                                 VariableAttribute* attributes,
                                 uintn_t* data_size, void* data) noexcept {
    return get_variable_(variable_name, vendor_guid, attributes, data_size,
                         data);
  }

  template <typename T>
  FORCE_INLINE auto get_variable(const char16_t* variable_name,
                                 const Guid& vendor_guid, uint32_t* attributes,
                                 T* data) noexcept {
    return get_variable(variable_name, vendor_guid, attributes, sizeof(T),
                        data);
  }

  FORCE_INLINE auto set_variable(const char16_t* variable_name,
                                 const Guid& vendor_guid,
                                 VariableAttribute attributes,
                                 uintn_t data_size, const void* data) noexcept {
    return set_variable_(variable_name, vendor_guid, attributes, data_size,
                         data);
  }

  template <typename T>
  FORCE_INLINE auto set_variable(const char16_t* variable_name,
                                 const Guid* vendor_guid, uint32_t attributes,
                                 const T& data) {
    return set_variable(variable_name, vendor_guid, attributes, sizeof(T),
                        &data);
  }

  FORCE_INLINE auto get_next_variable_name(uintn_t* variable_name_size,
                                           char16_t* variable_name,
                                           Guid* vendor_guid) noexcept {
    return get_next_variable_name_(variable_name_size, variable_name,
                                   vendor_guid);
  }

  FORCE_INLINE auto get_next_high_monotonic_count(
      uint32_t* high_count) noexcept {
    return get_next_high_monotonic_count_(high_count);
  }

  NORETURN FORCE_INLINE auto reset_system(
      ResetType reset_type, Status reset_status, uintn_t data_size = 0,
      const void* reset_data = nullptr) noexcept {
    reset_system_(reset_type, reset_status, data_size, reset_data);
  }

  template <typename T>
  NORETURN FORCE_INLINE auto reset_system(ResetType reset_type,
                                          Status reset_status,
                                          const T& data) noexcept {
    reset_system(reset_type, reset_status, sizeof(T), &data);
  }

  FORCE_INLINE auto update_capsule(
      CapsuleHeader** capsule_header_array, uintn_t capsule_count,
      PhysicalAddress scatter_gather_list) noexcept {
    return update_capsule_(capsule_header_array, capsule_count,
                           scatter_gather_list);
  }

  FORCE_INLINE auto query_capsule_capabilities(
      CapsuleHeader** capsule_header_array, uintn_t capsule_count,
      uint64_t* maximum_capsule_size, ResetType* reset_type) noexcept {
    return query_capsule_capabilities_(capsule_header_array, capsule_count,
                                       maximum_capsule_size, reset_type);
  }

  static constexpr uint64_t Signature = 0x56524553544e5552;
};

}  // namespace efi