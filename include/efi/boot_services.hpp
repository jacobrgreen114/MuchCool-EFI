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
//

#pragma once
#ifndef __cplusplus
#error
#endif

#include "core.hpp"
#include "device_path_protocol.hpp"

namespace efi {

using EventNotify = void(EFI_CALL*)(Event event, void* context) noexcept;

enum class AllocateType {
  AnyPages,
  MaxAddress,
  Address,
};

enum class MemoryType : uint32_t {
  ReservedMemoryType,
  LoaderCode,
  LoaderData,
  SimpleTextInputProtocolCode,
  SimpleTextInputProtocolData,
  RuntimeServicesCode,
  RuntimeServicesData,
  ConventionalMemory,
  UnusableMemory,
  ACPIReclaimMemory,
  ACPIMemoryNVS,
  MemoryMappedIO,
  MemoryMappedIOPortSpace,
  PalCode,
  PersistentMemory,
  UnacceptedMemoryType
};

enum class EventType : uint32_t {
  Timer                             = 0x80000000,
  Runtime                           = 0x40000000,

  NotifyWait                        = 0x00000100,
  NotifySignal                      = 0x00000200,

  SignalExitSimpleTextInputProtocol = 0x00000201,
  SignalVirtualAddressChange        = 0x60000202
};

enum class LocateSearchType {
  AllHandles,
  ByRegisterNotify,
  ByProtocol
};

enum class OpenProtocolAttribute : uint32_t {
  ByHandleProtocol  = 0x00000001,
  GetProtocol       = 0x00000002,
  TestProtocol      = 0x00000004,
  ByChildController = 0x00000008,
  ByDriver          = 0x00000010,
  Exclusive         = 0x00000020,
};

class MemoryDescriptor {
 private:
  MemoryType type_;
  PhysicalAddress physical_start_;
  VirtualAddress virtual_start_;
  uint64_t number_of_pages_;
  uint64_t attribute_;
};

class OpenProtocolInformationEntry {
 private:
  Handle agent_handle_;
  Handle controller_handle_;
  // todo : check if this is OpenProtocolAttribute
  uint32_t attributes_;
  uint32_t open_count_;
};

class BootServices final : public Table {
 private:
  using RaiseTPLFn      = TPL(EFI_CALL*)(TPL new_tpl) noexcept;

  using RestoreTPLFn    = void(EFI_CALL*)(TPL old_tpl) noexcept;

  using CreateEventFn   = Status(EFI_CALL*)(EventType type, TPL notify_tpl,
                                          EventNotify notify_function,
                                          void* context, Event* event) noexcept;

  using CreateEventExFn = Status(EFI_CALL*)(EventType type, TPL notify_tpl,
                                            EventNotify notify_function,
                                            void* context,
                                            const Guid* event_group,
                                            Event* event) noexcept;

  using CloseEventFn    = Status(EFI_CALL*)(Event event) noexcept;

  using SignalEventFn   = Status(EFI_CALL*)(Event event) noexcept;

  using WaitForEventFn  = Status(EFI_CALL*)(uintn_t num_of_events,
                                           const Event* event,
                                           uintn_t* index) noexcept;

  using CheckEventFn    = Status(EFI_CALL*)(Event event) noexcept;

  using SetTimerFn      = Status(EFI_CALL*)(Event event, TimerDelay type,
                                       uint64_t trigger_time) noexcept;

  using AllocatePagesFn = Status(EFI_CALL*)(AllocateType type,
                                            MemoryType memory_type,
                                            uintn_t pages,
                                            PhysicalAddress* memory) noexcept;

  using FreePagesFn     = Status(EFI_CALL*)(PhysicalAddress memory,
                                        uintn_t pages) noexcept;

  using GetMemoryMapFn  = Status(EFI_CALL*)(
      uintn_t* memory_map_size, MemoryDescriptor* memory_map, uintn_t* map_key,
      uintn_t* descriptor_size, uint32_t* descriptor_version) noexcept;

  using AllocatePoolFn = Status(EFI_CALL*)(MemoryType pool_type, uintn_t size,
                                           void** buffer) noexcept;

  using FreePoolFn     = Status(EFI_CALL*)(void* buffer) noexcept;

  using InstallProtocolInterfaceFn = Status(EFI_CALL*)(
      Handle* handle, const Guid* protocol, InterfaceType interface_type,
      const void* interface) noexcept;

  using UninstallProtocolInterfaceFn = Status(EFI_CALL*)(
      Handle handle, const Guid* protocol, const void* interface) noexcept;

  using ReinstallProtocolInterfaceFn = Status(EFI_CALL*)(
      Handle handle, const Guid* protocol, const void* old_interface,
      const void* new_interface) noexcept;

  using RegisterProtocolNotifyFn = Status(EFI_CALL*)(
      const Guid* protocol, Event event, void** registration) noexcept;

  using LocateHandleFn     = Status(EFI_CALL*)(LocateSearchType search_type,
                                           const Guid* protocol,
                                           const void* search_key,
                                           uintn_t* buffer_size,
                                           Handle* buffer) noexcept;

  using HandleProtocolFn   = Status(EFI_CALL*)(Handle handle,
                                             const Guid* protocol,
                                             void** interface) noexcept;

  using LocateDevicePathFn = Status(EFI_CALL*)(const Guid* protocol,
                                               DevicePathProtocol** device_path,
                                               Handle* device) noexcept;

  using OpenProtocolFn =
      Status(EFI_CALL*)(Handle handle, const Guid* protocol, void** interface,
                        Handle agent_handle, Handle controller_handle,
                        OpenProtocolAttribute attributes) noexcept;

  using CloseProtocolFn = Status(EFI_CALL*)(Handle handle, const Guid* protocol,
                                            Handle agent_handle,
                                            Handle controller_handle) noexcept;

  using OpenProtocolInformationFn =
      Status(EFI_CALL*)(Handle handle, const Guid* protocol,
                        OpenProtocolInformationEntry** entry_buffer,
                        uintn_t* entry_count) noexcept;

  using ConnectControllerFn = Status(EFI_CALL*)(
      Handle controller_handle, Handle* driver_image_handle,
      DevicePathProtocol* remaining_device_path, bool recursive) noexcept;

  using DisconnectControllerFn =
      Status(EFI_CALL*)(Handle controller_handle, Handle driver_image_handle,
                        Handle child_handle) noexcept;

  using ProtocolsPerHandleFn =
      Status(EFI_CALL*)(Handle controller_handle, Guid*** protocol_buffer,
                        uintn_t* protocol_buffer_count) noexcept;

  using LocateHandleBufferFn = Status(EFI_CALL*)(LocateSearchType search_type,
                                                 const Guid* protocol,
                                                 const void* search_key,
                                                 uintn_t* num_handles,
                                                 Handle** buffer) noexcept;

  using LocateProtocolFn     = Status(EFI_CALL*)(const Guid* protocol,
                                             const void* registration,
                                             void** interface) noexcept;

  using InstallMultipleProtocolInterfacesFn = Status(EFI_CALL*)(Handle* handle,
                                                                ...) noexcept;

  using UninstallMultipleProtocolInterfacesFn = Status(EFI_CALL*)(Handle handle,
                                                                  ...) noexcept;

  using LoadImageFn   = Status(EFI_CALL*)(bool boot_policy,
                                        Handle parent_image_handle,
                                        const DevicePathProtocol* device_path,
                                        const void* source_buffer,
                                        uintn_t source_size,
                                        Handle* image_handle) noexcept;

  using StartImageFn  = Status(EFI_CALL*)(Handle image_handle,
                                         uintn_t* exit_data_size,
                                         char16_t** exit_data) noexcept;

  using UnloadImageFn = Status(EFI_CALL*)(Handle image_handle) noexcept;

  using ExitFn = Status(EFI_CALL*)(Handle image_handle, Status exit_status,
                                   uintn_t exit_data_size,
                                   const char16_t* exit_data) noexcept;

  using ExitSimpleTextInputProtocolFn =
      Status(EFI_CALL*)(Handle image_handle, uintn_t map_key) noexcept;

  using SetWatchdogTimerFn = Status(EFI_CALL*)(
      uintn_t timeout, uint64_t watchdog_code, uintn_t data_size,
      const char16_t* watchdog_data) noexcept;

  using StallFn   = Status(EFI_CALL*)(uintn_t microseconds);

  using CopyMemFn = void(EFI_CALL*)(void* destination, const void* source,
                                    uintn_t length) noexcept;

  using SetMemFn  = void(EFI_CALL*)(void* buffer, uintn_t size,
                                   uint8_t value) noexcept;

  using GetNextMonotonicCountFn = Status(EFI_CALL*)(uint64_t* count) noexcept;

  using InstallConfigurationTableFn = Status(EFI_CALL*)(const Guid* guid,
                                                        void* table) noexcept;

  using CalculateCrc32Fn            = Status(EFI_CALL*)(const void* data,
                                             uintn_t data_size,
                                             uint32_t* crc32) noexcept;

  const RaiseTPLFn raise_tpl_;
  const RestoreTPLFn restore_tpl_;

  const AllocatePagesFn allocate_pages_;
  const FreePagesFn free_pages_;
  const GetMemoryMapFn get_memory_map_;
  const AllocatePoolFn allocate_pool_;
  const FreePoolFn free_pool_;

  const CreateEventFn create_event_;
  const SetTimerFn set_timer_;
  const WaitForEventFn wait_for_event_;
  const SignalEventFn signal_event_;
  const CloseEventFn close_event_;
  const CheckEventFn check_event_;

  const InstallProtocolInterfaceFn install_protocol_interface_;
  const ReinstallProtocolInterfaceFn reinstall_protocol_interface_;
  const UninstallProtocolInterfaceFn uninstall_protocol_interface_;
  const HandleProtocolFn handle_protocol_;

  const void* const reserved_;

  const RegisterProtocolNotifyFn register_protocol_notify_;
  const LocateHandleFn locate_handle_;
  const LocateDevicePathFn locate_device_path_;
  const InstallConfigurationTableFn install_configuration_table_;

  const LoadImageFn load_image_;
  const StartImageFn image_start_;
  const ExitFn exit_;
  const UnloadImageFn unload_image_;
  const ExitSimpleTextInputProtocolFn exit_boot_services_;

  const GetNextMonotonicCountFn get_next_monotonic_count_;
  const StallFn stall_;
  const SetWatchdogTimerFn set_watchdog_timer_;

  const ConnectControllerFn connect_controller_;
  const DisconnectControllerFn disconnect_controller_;

  const OpenProtocolFn open_protocol_;
  const CloseProtocolFn close_protocol_;
  const OpenProtocolInformationFn open_protocol_information_;

  const ProtocolsPerHandleFn protocols_per_handle_;
  const LocateHandleBufferFn locate_handle_buffer_;
  const LocateProtocolFn locate_protocol_;
  const InstallMultipleProtocolInterfacesFn
      install_multiple_protocol_interfaces_;
  const UninstallMultipleProtocolInterfacesFn
      uninstall_multiple_protocol_interfaces_;

  const CalculateCrc32Fn calculate_crc32_;

  const CopyMemFn copy_mem_;
  const SetMemFn set_mem_;

  const CreateEventExFn create_event_ex_;

 public:
  BootServices()                                       = delete;
  BootServices(BootServices&&)                         = delete;
  BootServices(const BootServices&)                    = delete;
  ~BootServices()                                      = delete;
  auto operator=(BootServices&&) -> BootServices&      = delete;
  auto operator=(const BootServices&) -> BootServices& = delete;

  static constexpr uint64_t Signature                  = 0x56524553544f4f42;
};

static constexpr Event TimerEvent                      = 0x80000000;
static constexpr Event RuntimeEvent                    = 0x40000000;
static constexpr Event RuntimeContextEvent             = 0x20000000;

static constexpr Event NotifyWaitEvent                 = 0x00000100;
static constexpr Event NotifySignalEvent               = 0x00000200;

static constexpr Event SignalExitBootServicesEvent     = 0x00000201;
static constexpr Event SignalVirtualAddressChangeEvent = 0x60000202;

}  // namespace efi