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

enum class DevicePathType : uint8_t {
  Hardware              = 0x01,
  ACPI                  = 0x02,
  Messaging             = 0x03,
  Media                 = 0x04,
  BIOSBootSpecification = 0x05,

  EndOfHardware         = 0x7F,
};

enum class DevicePathSubType : uint8_t {
};

enum class HardwareDevicePathSubType : uint8_t {
  PCI          = 1,
  PCCARD       = 2,
  MemoryMapped = 3,
  Vendor       = 4,
  Contoller    = 5,
  BMC          = 5,
};

enum class ACPIDevicePathSubType : uint8_t {
  ACPI         = 1,
  ExpandedACPI = 2,
  _ADR         = 3,
  NVDIMM       = 4,
};

enum class MessagingDevicePathSubType : uint8_t {
  ATAPI             = 1,
  SCSI              = 2,
  FibreChannel      = 3,
  Firewire          = 4,
  USB               = 5,
  I2O               = 6,
  InfiniBand        = 9,
  Vendor            = 10,
  MacAddress        = 11,
  IPV4              = 12,
  IPV6              = 13,
  UART              = 14,
  USBClass          = 15,
  USB_WWID          = 16,
  DeviceLogicalUnit = 17,
  SATA              = 18,
  iSCSI             = 19,
  VLAN              = 20,
  FibreChannelEx    = 21,
  SASEx             = 22,
  NVMExpress        = 23,
  URI               = 24,
  UFS               = 25,
  SD                = 26,
  Bluetooth         = 27,
  WiFi              = 28,
  eMMC              = 29,
  BluetoothLE       = 30,
  DNS               = 31,
  NVDIMM            = 32,
  RESTService       = 33,
  NVMEoF            = 34,
};

enum class MediaDevicePathSubType : uint8_t {
  HardDrive           = 1,
  CdRom               = 2,
  Vendor              = 3,
  FilePath            = 4,
  MediaProtocol       = 5,
  PIWGFirmwareFile    = 6,
  RelativeOffsetRange = 8,
  RAMDisk             = 9,
};

enum class BIOSBootSpecDevicePathSubType : uint8_t {
  V1_01 = 1
};

#pragma pack(push, 1)

class DevicePathProtocol {
  DevicePathType    type_;
  DevicePathSubType subtype_;
  uint16_t          length_;

 public:
  NODISCARD auto type() const noexcept {
    return type_;
  }

  NODISCARD auto subtype() const noexcept {
    return subtype_;
  }

  NODISCARD auto length() const noexcept {
    return length_;
  }

  static constexpr auto guid =
      Guid{0x09576e91,
           0x6d3f,
           0x11d2,
           {0x8e, 0x39, 0x00, 0xa0, 0xc9, 0x69, 0x72, 0x3b}};
};

class PciDevicePath final : public DevicePathProtocol {
  uint8_t function_;
  uint8_t device_;

 public:
  NODISCARD auto function() const noexcept {
    return function_;
  }

  NODISCARD auto device() const noexcept {
    return device_;
  }
};

class PcCardDevicePath final : public DevicePathProtocol {
  uint8_t function_;

 public:
  NODISCARD auto function() const noexcept {
    return function_;
  }
};

class MemoryMappedDevicePath final : public DevicePathProtocol {
  MemoryType memory_type_;
  void*      start_addr_;
  void*      end_addr_;

 public:
  NODISCARD auto memory_type() const noexcept {
    return memory_type_;
  }

  NODISCARD auto start_addr() const noexcept {
    return start_addr_;
  }

  NODISCARD auto end_addr() const noexcept {
    return end_addr_;
  }
};

class VendorDevicePath : public DevicePathProtocol {
  Guid vendor_guid_;

 public:
  NODISCARD auto& vendor_guid() const noexcept {
    return vendor_guid_;
  }

  NODISCARD auto* data() noexcept {
    return static_cast<void*>(&this[1]);
  }

  NODISCARD auto* data() const noexcept {
    return static_cast<const void*>(&this[1]);
  }

  NODISCARD auto data_length() const noexcept {
    return length() - sizeof(VendorDevicePath);
  }
};

class ControllerDevicePath final : public DevicePathProtocol {
  uint32_t controller_number_;

 public:
  NODISCARD auto controller_number() const noexcept {
    return controller_number_;
  }
};

enum class BaseboardManagementControllerInterfaceType : uint8_t {
  Unknown = 0x00,
  // KeyboardControllerStyle
  KCS     = 0x01,
  // ServerManagementInterfaceChip
  SMIC    = 0x02,
  // BlockTransfer
  BT      = 0x03
};

class BaseboardManagementControllerDevicePath final
    : public DevicePathProtocol {
  BaseboardManagementControllerInterfaceType interface_type_;
  void*                                      base_addr_;

 public:
  NODISCARD auto interface_type() const noexcept {
    return interface_type_;
  }

  NODISCARD auto base_addr() const noexcept {
    return base_addr_;
  }
};

class AcpiDevicePath : public DevicePathProtocol {
  uint32_t hid_;
  uint32_t uid_;

 public:
  NODISCARD auto hid() const noexcept {
    return hid_;
  }

  NODISCARD auto uid() const noexcept {
    return uid_;
  }
};

class ExpandedAcpiDevicePath : public AcpiDevicePath {
  uint32_t cid_;

  // todo : figure out best way to implement string variable. possible template
  // char8_t hid_uid_cid_str[];
 public:
  NODISCARD auto cid() const noexcept {
    return cid_;
  }
};

// todo : possibly convert to template
class AcpiAdrDevicePath : public DevicePathProtocol {
  uint32_t adr_;
  uint32_t additional_adr_[];

 public:
  NODISCARD auto adr() const noexcept {
    return adr_;
  }
};

class NVDIMMDevicePath final : public DevicePathProtocol {
  uint32_t device_handle_;

 public:
  NODISCARD auto handle() const noexcept {
    return device_handle_;
  }
};

class AtapiDevicePath final : public DevicePathProtocol {
  bool     secondary_;
  bool     slave_;
  uint16_t logical_unit_number_;

 public:
  NODISCARD auto primary() const noexcept {
    return !secondary_;
  }

  NODISCARD auto master() const noexcept {
    return !slave_;
  }

  NODISCARD auto logical_unit_number() const noexcept {
    return logical_unit_number_;
  }
};

class ScsiDevicePath final : public DevicePathProtocol {
  uint16_t target_id_;
  uint16_t logical_unit_number_;

 public:
  NODISCARD auto target_id() const noexcept {
    return target_id_;
  }

  NODISCARD auto logical_unit_number() const noexcept {
    return logical_unit_number_;
  }
};

class FibreChannelDevicePath final : public DevicePathProtocol {
  uint32_t reserved_;
  uint64_t world_wide_name_;
  uint64_t logical_unit_number_;

 public:
  NODISCARD auto world_wide_name() const noexcept {
    return world_wide_name_;
  }

  NODISCARD auto logical_unit_number() const noexcept {
    return logical_unit_number_;
  }
};

class FibreChannelExDevicePath final : public DevicePathProtocol {
 public:
  using WorldWideName     = std::array<uint8_t, 8>;
  using LogicalUnitNumber = std::array<uint8_t, 8>;

 private:
  uint32_t          reserved_;
  WorldWideName     world_wide_name_;
  LogicalUnitNumber logical_unit_number_;

 public:
  NODISCARD auto world_wide_name() const noexcept {
    return world_wide_name_;
  }

  NODISCARD auto logical_unit_number() const noexcept {
    return logical_unit_number_;
  }
};

class FirewireDevicePath final : public DevicePathProtocol {
  uint32_t reserved_;
  uint64_t guid_;

 public:
  NODISCARD auto guid() const noexcept {
    return guid_;
  }
};

class UsbDevicePath final : public DevicePathProtocol {
  uint8_t parent_port_;
  uint8_t interface_;

 public:
  NODISCARD auto port() const noexcept {
    return parent_port_;
  }

  NODISCARD auto interface() const noexcept {
    return interface_;
  }
};

class SataDevicePath final : public DevicePathProtocol {
  uint16_t hba_port_;
  uint16_t port_multiplier_port_;
  uint16_t logical_unit_number_;

 public:
  NODISCARD auto hba_port() const noexcept {
    return hba_port_;
  }

  NODISCARD auto port_multiplier_port() const noexcept {
    return port_multiplier_port_;
  }

  NODISCARD auto logical_unit_number() const noexcept {
    return logical_unit_number_;
  }
};

class UsbWWIDDevicePath final : public DevicePathProtocol {
  uint16_t interface_;
  uint16_t vendor_id_;
  uint16_t product_id_;
  char16_t serial_[];

  // todo : implement getters and make a template
};

class DeviceLogicalUnitDevicePath final : public DevicePathProtocol {
  uint8_t lun_;
};

class UsbClassDevicePath final : public DevicePathProtocol {
  uint16_t vendor_id_;
  uint16_t product_id_;
  uint8_t  device_class_;
  uint8_t  device_subclass_;
  uint8_t  device_protocol_;
};

class I2ODevicePath final : public DevicePathProtocol {
  uint32_t target_id_;
};

class MacAddressDevicePath final : public DevicePathProtocol {
  MacAddress mac_;
  // todo : convert iftype to enum
  uint8_t    if_type_;
};

class Ipv4DevicePath final : public DevicePathProtocol {
  Ipv4Address local_addr_;
  Ipv4Address remote_addr_;
  uint16_t    local_port_;
  uint16_t    remote_port_;
  // todo : convert to enum (UDP, TCP, etc.)
  uint16_t    protocol_;
  bool        static_;
  Ipv4Address gateway_addr_;
  // todo : convert to class
  uint32_t    subnet_mask_;
};

enum class IpAddressOrigin : uint8_t {
  Manual            = 0x00,
  Ipv6StatelessAuto = 0x01,
  Ipv6Stateful      = 0x02
};

class Ipv6DevicePath final : public DevicePathProtocol {
  Ipv6Address     local_addr_;
  Ipv6Address     remote_addr_;
  uint16_t        local_port_;
  uint16_t        remote_port_;
  // todo : convert to enum (UDP, TCP, etc.)
  uint16_t        protocol_;
  IpAddressOrigin origin_;
  uint8_t         prefix_length_;
  Ipv6Address     gateway_addr_;
};

class VlanDevicePath final : public DevicePathProtocol {
  uint16_t vlan_id_;
};

enum class InfiniBandResourceFlags : uint32_t {
  Service                 = enum_bit(0),
  ExtendedBootEnvironment = enum_bit(1),
  ConsoleProtocol         = enum_bit(2),
  StorageProtocol         = enum_bit(3),
  NetworkProtocol         = enum_bit(4)
};
ENUM_FLAGS(InfiniBandResourceFlags);

class InfiniBandDevicePath final : public DevicePathProtocol {
  InfiniBandResourceFlags resource_flags_;
  ::efi::Guid             port_gid_;
  uint64_t                iocguid_serviceid_;
  uint64_t                target_port_id_;
  uint64_t                device_id_;
};

enum class ParityTypeU8 : uint8_t {
  DefaultParity,
  NoParity,
  EvenParity,
  OddParity,
  MarkParity,
  SpaceParity
};

enum class StopBitsU8 : uint8_t {
  Default,
  One,      // 1 stop enum_bit
  OneFive,  // 1.5 stop bits
  Two       // 2 stop bits
};

class UartDevicePath final : public DevicePathProtocol {
  uint32_t     reserved_;
  uint64_t     baud_rate_;
  uint8_t      data_bits_;
  ParityTypeU8 parity_;
  StopBitsU8   stop_bits_;
};

class VendorMessagingDevicePath final : public DevicePathProtocol {
  ::efi::Guid vendor_guid_;

 public:
  static constexpr auto PCAnsiGuid =
      ::efi::Guid{0xe0c14753,
                  0xf9be,
                  0x11d2,
                  {0x9a, 0x0c, 0x00, 0x90, 0x27, 0x3f, 0xc1, 0x4d}};

  static constexpr auto VT100Guid =
      ::efi::Guid{0xdfa66065,
                  0xb419,
                  0x11d3,
                  {0x9a, 0x2d, 0x00, 0x90, 0x27, 0x3f, 0xc1, 0x4d}};

  static constexpr auto VT100PlusGuid =
      ::efi::Guid{0x7baec70b,
                  0x57e0,
                  0x4c76,
                  {0x8e, 0x87, 0x2f, 0x9e, 0x28, 0x08, 0x83, 0x43}};

  static constexpr auto VTUtf8Guid =
      ::efi::Guid{0xad15a0d6,
                  0x8bec,
                  0x4acf,
                  {0xa0, 0x73, 0xd0, 0x1d, 0xe7, 0x7e, 0x2d, 0x88}};
};

enum class FlowControlFlags : uint32_t {
  Hardware = enum_bit(0),
  XOnXOff  = enum_bit(1)
};
ENUM_FLAGS(FlowControlFlags);

class UartFlowControlMessagingDevicePath final : public DevicePathProtocol {
  ::efi::Guid      vendor_guid_;
  FlowControlFlags flow_control_;

 public:
  static constexpr auto UartFlowControlGuid =
      ::efi::Guid{0x37499a9d,
                  0x542f,
                  0x4c89,
                  {0xa0, 0x26, 0x35, 0xda, 0x14, 0x20, 0x94, 0xe4}};
};

// todo : implement bits
class SasSataDeviceTopologyInfo final {
  uint16_t data_;
};

class SasDevicePath final : public DevicePathProtocol {
  ::efi::Guid               vendor_guid_;
  uint32_t                  reserved_;
  uint64_t                  sas_addr_;
  uint64_t                  lun_;
  SasSataDeviceTopologyInfo dev_top_info_;
  uint16_t                  relative_target_port_;
};

using LogicalUnitNumberArray = std::array<uint8_t, 8>;

class SasExDevicePath final : public DevicePathProtocol {
 public:
  using SasAddress = std::array<uint8_t, 8>;

 private:
  SasAddress                sas_addr_;
  uint64_t                  reserved_;
  LogicalUnitNumberArray    lun_;
  SasSataDeviceTopologyInfo dev_top_info_;
  uint16_t                  relative_target_port_;
};

enum class IScsiProtocol : uint16_t {
  Tcp = 0
};

class IScsiLoginOptions final {
  uint16_t data_;
};

class IScsiDevicePath final : public DevicePathProtocol {
  IScsiProtocol          protocol_;
  IScsiLoginOptions      options_;
  LogicalUnitNumberArray lun_;
  uint16_t               target_portal_group_tag_;
  // todo : implement as template
  char8_t                target_name[];
};

class NvmExpressDevicePath final : public DevicePathProtocol {
  uint32_t namespace_identifier_;
  uint64_t extended_unique_identifier_;
};

class UriDevicePath final : public DevicePathProtocol {
  // todo : implement as template and double check char type
  char8_t uri_[];
};

class UfsDevicePath final : public DevicePathProtocol {
  uint8_t target_id_;
  uint8_t lun_;
};

class SecureDigitalDevicePath final : public DevicePathProtocol {
  uint8_t slot_;
};

using BluetoothAddress = std::array<uint8_t, 6>;

class BluetoothDevicePath final : public DevicePathProtocol {
 private:
  BluetoothAddress address_;
};

class WirelessDevicePath final : public DevicePathProtocol {
 public:
  using SSID = std::array<char8_t, 32>;

 private:
  SSID ssid_;
};

class EmmcDevicePath final : public DevicePathProtocol {
  uint8_t slot_;
};

class BluetoothLEDevicePath final : public DevicePathProtocol {
  BluetoothAddress address_;
  uint8_t          address_type_;
};

class DnsDevicePath final : public DevicePathProtocol {
  bool      ipv6_;
  // todo : implement as template
  IpAddress addresses_[];
};

class NVDIMMNamespacePath final : public DevicePathProtocol {
  ::efi::Guid uuid_;
};

enum class RestService : uint8_t {
  Redfish = 0x01,
  OData   = 0x02,
};

enum class RestServiceAccessMode : uint8_t {
  InBand    = 0x01,
  OutOfBand = 0x02
};

class RestServiceDevicePath : public DevicePathProtocol {
  RestService           service_;
  RestServiceAccessMode access_mode_;
};

class VendorRestServiceDevicePath : public RestServiceDevicePath {
  ::efi::Guid service_guid_;
  // todo : implement as template
  uint8_t     data_[];
};

class NvmeOverFabricDevicePath final : public DevicePathProtocol {
  uint8_t     nidt_;
  ::efi::Guid nid_;
  // todo : implement as template
  char8_t     subsystem_nqn_[];
};

enum class PartitionFormat : uint8_t {
  MBR = 0x01,
  GPT = 0x02
};

enum class DiskSignatureType : uint8_t {
  SigNone  = 0x00,
  Sig32Bit = 0x01,
};

class HardDriveDevicePath final : public DevicePathProtocol {
  uint32_t          partition_;
  uint64_t          partition_start_;
  uint64_t          partition_size_;
  ::efi::Guid       partition_signature_;
  PartitionFormat   partition_format_;
  DiskSignatureType signature_type_;
};

class CdRomDevicePath final : public DevicePathProtocol {
  uint32_t boot_entry_;
  uint64_t partition_start_;
  uint64_t partition_size_;
};

class VendorMediaDevicePath final : public DevicePathProtocol {
  ::efi::Guid vendor_guid_;
  uint8_t     data_[];
};

class FilePathDevicePath final : public DevicePathProtocol {
  char16_t path_name_[];
};

class MediaProtocolDevicePath final : public DevicePathProtocol {
  ::efi::Guid protocol_guid_;
};

class PIWGFirmwareFileDevicePath final : public DevicePathProtocol {
  // todo : implement
};

class PIWGFirmwareVolumeDevicePath final : public DevicePathProtocol {
  // todo : implement
};

class RamDiskDevicePath final : public DevicePathProtocol {
  void*       start_add_;
  void*       end_addr_;
  ::efi::Guid disk_type_;
  uint16_t    instance_;

 public:
  constexpr static const auto VirtualDiskGuid =
      ::efi::Guid{0x77AB535A,
                  0x45FC,
                  0x624B,
                  {0x55, 0x60, 0xF7, 0xB2, 0x81, 0xD1, 0xF9, 0x6E}};

  constexpr static const auto VirtualCdGuid =
      ::efi::Guid{0x3D5ABD30,
                  0x4175,
                  0x87CE,
                  {0x6D, 0x64, 0xD2, 0xAD, 0xE5, 0x23, 0xC4, 0xBB}};

  constexpr static const auto PersistentVirtualDiskGuid =
      ::efi::Guid{0x5CEA02C9,
                  0x4D07,
                  0x69D3,
                  {0x26, 0x9F, 0x44, 0x96, 0xFB, 0xE0, 0x96, 0xF9}};

  constexpr static const auto PersistentVirtualCdGuid =
      ::efi::Guid{0x08018188,
                  0x42CD,
                  0xBB48,
                  {0x10, 0x0F, 0x53, 0x87, 0xD5, 0x3D, 0xED, 0x3D}};
};

enum class BiosBootSpecificationDeviceType : uint16_t {
  Floppy          = 0x01,
  HardDisk        = 0x02,
  CdRom           = 0x03,
  Pcmcia          = 0x04,
  Usb             = 0x05,
  EmbeddedNetwork = 0x06,
  BevDevice       = 0x80,
  Unknown         = 0xFF,
};

class BiosBootSpecificationDevicePath final : public DevicePathProtocol {
  BiosBootSpecificationDeviceType device_type_;
  uint16_t                        status_flag_;
  char8_t                         description_[];
};

#pragma pack(pop)

}  // namespace efi