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
#include "pointer_protocol.hpp"
#include "serial_protocol.hpp"
#include "graphics_output_protocol.hpp"
#include "edid_protocol.hpp"
#include "simple_text_ex_protocol.hpp"
#include "load_file_protocol.hpp"
#include "file_system_protocol.hpp"
#include "file_protocol.hpp"
#include "disk_io_protocol.hpp"
#include "block_io_protocol.hpp"
#include "debug_support_protocol.hpp"
#include "debug_port_protocol.hpp"
#include "arp_protocol.hpp"
#include "dhcp_protocol.hpp"
#endif
