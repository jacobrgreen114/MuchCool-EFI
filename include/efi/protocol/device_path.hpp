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
// MuchCool-EFI. If not, see <https://www.gnu.org/licenses/>.lude "efi/core.hpp"

#pragma once

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

class DevicePathProtocol {
  DevicePathType type_;
  DevicePathSubType subtype_;
  uint16_t length_;

 public:
  static constexpr auto Guid =
      ::efi::Guid{0x09576e91,
                  0x6d3f,
                  0x11d2,
                  {0x8e, 0x39, 0x00, 0xa0, 0xc9, 0x69, 0x72, 0x3b}};
};

}  // namespace efi