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

#include "system_table.hpp"

#ifdef EFI_INCLUDE_ALL_HEADERS
#include "efi/protocol/pointer.hpp"
#include "efi/protocol/serial.hpp"
#include "efi/protocol/graphics_output.hpp"
#include "efi/protocol/edid.hpp"
#include "efi/protocol/simple_text_ex.hpp"
#include "efi/protocol/load_file.hpp"
#include "efi/protocol/file_system.hpp"
#include "efi/protocol/file.hpp"
#include "efi/protocol/disk_io.hpp"
#include "efi/protocol/block_io.hpp"
#include "efi/protocol/debug_support.hpp"
#include "efi/protocol/debug_port.hpp"
#include "efi/protocol/arp.hpp"
#include "efi/protocol/dhcp.hpp"
#endif
