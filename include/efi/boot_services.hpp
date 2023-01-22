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
#include "efi/protocol/device_path.hpp"

namespace efi {

using EventNotify = void(EFI_CALL*)(Event event, void* context) noexcept;

enum class AllocateType {
  AnyPages,
  MaxAddress,
  Address,
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
  OpenProtocolAttribute attributes_;
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
                                           const Guid& protocol,
                                           const void* search_key,
                                           uintn_t* buffer_size,
                                           Handle* buffer) noexcept;

  using HandleProtocolFn   = Status(EFI_CALL*)(Handle handle,
                                             const Guid& protocol,
                                             void** interface) noexcept;

  using LocateDevicePathFn = Status(EFI_CALL*)(const Guid& protocol,
                                               DevicePathProtocol** device_path,
                                               Handle* device) noexcept;

  using OpenProtocolFn =
      Status(EFI_CALL*)(Handle handle, const Guid& protocol, void** interface,
                        Handle agent_handle, Handle controller_handle,
                        OpenProtocolAttribute attributes) noexcept;

  using CloseProtocolFn = Status(EFI_CALL*)(Handle handle, const Guid& protocol,
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

  using LocateProtocolFn     = Status(EFI_CALL*)(const Guid& protocol,
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

  using ExitBootServicesFn = Status(EFI_CALL*)(Handle image_handle,
                                               uintn_t map_key) noexcept;

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
  const ExitBootServicesFn exit_boot_services_;

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

#pragma region TPL

  FORCE_INLINE auto raise_tpl(TPL new_tpl) noexcept {
    return raise_tpl_(new_tpl);
  }

  FORCE_INLINE auto restore_tpl(TPL old_tpl) noexcept {
    return restore_tpl_(old_tpl);
  }

#pragma endregion

#pragma region Event

  FORCE_INLINE auto create_event(EventType type, TPL notify_tpl,
                                 EventNotify notify_function, void* context,
                                 Event* event) noexcept {
    return create_event_(type, notify_tpl, notify_function, context, event);
  }

  FORCE_INLINE auto close_event(Event event) noexcept {
    return close_event_(event);
  }

  FORCE_INLINE auto signal_event(Event event) noexcept {
    return signal_event_(event);
  }

  FORCE_INLINE auto wait_for_events(uintn_t num_of_events, const Event* event,
                                    uintn_t* index) noexcept {
    return wait_for_event_(num_of_events, event, index);
  }

  FORCE_INLINE auto check_event(Event event) noexcept {
    return check_event_(event);
  }

  // Trigger Time is 100ns periods
  FORCE_INLINE auto set_timer(Event event, TimerDelay type,
                              uint64_t trigger_time) noexcept {
    return set_timer_(event, type, trigger_time);
  }

#pragma endregion

#pragma region Memory Allocation

  FORCE_INLINE auto allocate_pages(AllocateType type, MemoryType memory_type,
                                   uintn_t pages,
                                   PhysicalAddress* memory) noexcept {
    return allocate_pages_(type, memory_type, pages, memory);
  }

  FORCE_INLINE auto free_pages(PhysicalAddress memory, uintn_t pages) noexcept {
    return free_pages_(memory, pages);
  }

  FORCE_INLINE auto get_memory_map(uintn_t* memory_map_size,
                                   MemoryDescriptor* memory_map,
                                   uintn_t* map_key, uintn_t* descriptor_size,
                                   uint32_t* descriptor_version) noexcept {
    return get_memory_map_(memory_map_size, memory_map, map_key,
                           descriptor_size, descriptor_version);
  }

  FORCE_INLINE auto allocate_pool(MemoryType pool_type, uintn_t size,
                                  void** buffer) noexcept {
    return allocate_pool_(pool_type, size, buffer);
  }

  FORCE_INLINE auto free_pool(void* buffer) noexcept {
    return free_pool_(buffer);
  }

#pragma endregion

#pragma region Image

  FORCE_INLINE auto load_image(bool boot_policy, Handle parent_image_handle,
                               const DevicePathProtocol* device_path,
                               const void* source_buffer, uintn_t source_size,
                               Handle* image_handle) noexcept {
    return load_image_(boot_policy, parent_image_handle, device_path,
                       source_buffer, source_size, image_handle);
  }

  FORCE_INLINE auto start_image(Handle image_handle, uintn_t* exit_data_size,
                                char16_t** exit_data) noexcept {
    return image_start_(image_handle, exit_data_size, exit_data);
  }

  FORCE_INLINE auto exit(Handle image_handle, Status exit_status,
                         uintn_t exit_data_size, const char16_t* exit_data) {
    return exit_(image_handle, exit_status, exit_data_size, exit_data);
  }

  FORCE_INLINE auto exit(Handle image_handle, Status exit_status) {
    return exit(image_handle, exit_status, 0, nullptr);
  }

  FORCE_INLINE auto exit_boot_services(Handle image_handle,
                                       uintn_t map_key) noexcept {
    return exit_boot_services_(image_handle, map_key);
  }

#pragma endregion

#pragma region Protocols

  FORCE_INLINE auto locate_handle(LocateSearchType search_type,
                                  const Guid& protocol, const void* search_key,
                                  uintn_t* buffer_size,
                                  Handle* buffer) noexcept {
    return locate_handle_(search_type, protocol, search_key, buffer_size,
                          buffer);
  }

  FORCE_INLINE auto handle_protocol(Handle handle, const Guid& protocol,
                                    void** interface) noexcept {
    return handle_protocol_(handle, protocol, interface);
  }

  template <IsProtocol Protocol>
  FORCE_INLINE auto handle_protocol(Handle handle,
                                    Protocol** interface) noexcept {
    return handle_protocol_(handle, Protocol::Guid, interface);
  }

  FORCE_INLINE auto locate_device_path(const Guid& protocol,
                                       DevicePathProtocol** device_path,
                                       Handle* device) noexcept {
    return locate_device_path_(protocol, device_path, device);
  }

  template <IsProtocol Protocol>
  FORCE_INLINE auto locate_device_path(DevicePathProtocol** device_path,
                                       Handle* device) noexcept {
    return locate_device_path_(Protocol::Guid, device_path, device);
  }

  FORCE_INLINE auto open_protocol(Handle handle, const Guid& protocol,
                                  void** interface, Handle agent_handle,
                                  Handle controller_handle,
                                  OpenProtocolAttribute attributes) noexcept {
    return open_protocol_(handle, protocol, interface, agent_handle,
                          controller_handle, attributes);
  }

  template <IsProtocol Protocol>
  FORCE_INLINE auto open_protocol(Handle handle, Protocol** interface,
                                  Handle agent_handle, Handle controller_handle,
                                  OpenProtocolAttribute attributes) noexcept {
    return open_protocol_(handle, Protocol::Guid, interface, agent_handle,
                          controller_handle, attributes);
  }

  FORCE_INLINE auto close_protocol(Handle handle, const Guid& protocol,
                                   Handle agent_handle,
                                   Handle controller_handle) noexcept {
    return close_protocol_(handle, protocol, agent_handle, controller_handle);
  }

  template <IsProtocol Protocol>
  FORCE_INLINE auto close_protocol(Handle handle, Handle agent_handle,
                                   Handle controller_handle) noexcept {
    return close_protocol_(handle, Protocol::Guid, agent_handle,
                           controller_handle);
  }

  FORCE_INLINE auto protocols_per_handle(
      Handle controller_handle, Guid*** protocol_buffer,
      uintn_t* protocol_buffer_count) noexcept {
    return protocols_per_handle_(controller_handle, protocol_buffer,
                                 protocol_buffer_count);
  }

  FORCE_INLINE auto locate_handle_buffer(LocateSearchType search_type,
                                         const Guid* protocol,
                                         const void* search_key,
                                         uintn_t* num_handles,
                                         Handle** buffer) noexcept {
    return locate_handle_buffer_(search_type, protocol, search_key, num_handles,
                                 buffer);
  }

  FORCE_INLINE auto locate_handle_buffer(uintn_t* num_handles,
                                         Handle** buffer) noexcept {
    return locate_handle_buffer_(LocateSearchType::AllHandles, nullptr, nullptr,
                                 num_handles, buffer);
  }

  FORCE_INLINE auto locate_handle_buffer(const Guid& protocol,
                                         uintn_t* num_handles,
                                         Handle** buffer) noexcept {
    return locate_handle_buffer_(LocateSearchType::ByProtocol, &protocol,
                                 nullptr, num_handles, buffer);
  }

  template <IsProtocol Protocol>
  FORCE_INLINE auto locate_handle_buffer(uintn_t* num_handles,
                                         Handle** buffer) noexcept {
    return locate_handle_buffer_(LocateSearchType::ByProtocol, &Protocol::Guid,
                                 nullptr, num_handles, buffer);
  }

  FORCE_INLINE auto locate_handle_buffer(const void* search_key,
                                         uintn_t* num_handles,
                                         Handle** buffer) noexcept {
    return locate_handle_buffer_(LocateSearchType::ByRegisterNotify, nullptr,
                                 search_key, num_handles, buffer);
  }

  FORCE_INLINE auto locate_protocol(const Guid& protocol,
                                    const void* registration,
                                    void** interface) noexcept {
    return locate_protocol_(protocol, registration, interface);
  }

  template <IsProtocol Protocol>
  FORCE_INLINE auto locate_protocol(Protocol** interface) noexcept {
    return locate_protocol_(Protocol::Guid, nullptr, interface);
  }

#pragma endregion

#pragma region Misc

  // Watchdog code must be greater than 0xFFFF
  FORCE_INLINE auto set_watchdog_timer(uintn_t timeout, uint64_t watchdog_code,
                                       uintn_t data_size,
                                       const char16_t* watchdog_data) noexcept {
    return set_watchdog_timer_(timeout, watchdog_code, data_size,
                               watchdog_data);
  }

  FORCE_INLINE auto set_watchdog_timer(uintn_t timeout_sec) noexcept {
    return set_watchdog_timer_(timeout_sec, UINT64_MAX, 0, nullptr);
  }

  FORCE_INLINE auto stall(uintn_t microseconds) noexcept {
    return stall_(microseconds);
  }

  FORCE_INLINE auto stall(float seconds) noexcept {
    constexpr auto microseconds_per_second = 1'000'000;
    return stall_(static_cast<uintn_t>(seconds * microseconds_per_second));
  }

  FORCE_INLINE auto copy_mem(void* destination, const void* source,
                             uintn_t length) noexcept {
    return copy_mem_(destination, source, length);
  }

  FORCE_INLINE auto set_mem(void* buffer, uintn_t size,
                            uint8_t value) noexcept {
    return set_mem_(buffer, size, value);
  }

  FORCE_INLINE auto get_next_monotonic_count(uint64_t* count) noexcept {
    return get_next_monotonic_count_(count);
  }

  FORCE_INLINE auto calculate_crc32(const void* data, uintn_t data_size,
                                    uint32_t* crc32) noexcept {
    return calculate_crc32_(data, data_size, crc32);
  }

#pragma endregion

  static constexpr uint64_t Signature = 0x56524553544f4f42;
};

static const Event TimerEvent =
    reinterpret_cast<Event>(static_cast<uintptr_t>(0x80000000));
static const Event RuntimeEvent =
    reinterpret_cast<Event>(static_cast<uintptr_t>(0x40000000));
static const Event RuntimeContextEvent =
    reinterpret_cast<Event>(static_cast<uintptr_t>(0x20000000));

static const Event NotifyWaitEvent =
    reinterpret_cast<Event>(static_cast<uintptr_t>(0x00000100));
static const Event NotifySignalEvent =
    reinterpret_cast<Event>(static_cast<uintptr_t>(0x00000200));

static const Event SignalExitBootServicesEvent =
    reinterpret_cast<Event>(static_cast<uintptr_t>(0x00000201));
static const Event SignalVirtualAddressChangeEvent =
    reinterpret_cast<Event>(static_cast<uintptr_t>(0x60000202));

}  // namespace efi