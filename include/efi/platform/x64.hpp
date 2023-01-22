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

#include <cstdint>

namespace efi {

class FXSaveStateX64 final {
  uint16_t fcw_;
  uint16_t fsw_;
  uint16_t ftw_;
  uint16_t opcode_;
  uint64_t rip_;
  uint64_t data_offset_;
  uint8_t reserved1_[8];
  uint8_t st0_mm0_[10], reserved2_[6];
  uint8_t st1_mm1_[10], reserved3_[6];
  uint8_t st2_mm2_[10], reserved4_[6];
  uint8_t st3_mm3_[10], reserved5_[6];
  uint8_t st4_mm4_[10], reserved6_[6];
  uint8_t st5_mm5_[10], reserved7_[6];
  uint8_t st6_mm6_[10], reserved8_[6];
  uint8_t st7_mm7_[10], reserved9_[6];
  uint8_t xmm0_[16];
  uint8_t xmm1_[16];
  uint8_t xmm2_[16];
  uint8_t xmm3_[16];
  uint8_t xmm4_[16];
  uint8_t xmm5_[16];
  uint8_t xmm6_[16];
  uint8_t xmm7_[16];
  uint8_t reserved11_[14 * 16];
};

class SystemContext {
  uint64_t exception_data_;
  FXSaveStateX64 fx_state_;
  uint64_t dr0_, dr1_, dr2_, dr3_, dr6_, dr7_;
  uint64_t cr0, cr1_, cr2_, cr3_, cr4_, cr8_;
  uint64_t rflags_;
  uint64_t ldtr_, tr_;
  uint64_t gdtr_[2], idtr_[2];
  uint64_t rip_;
  uint64_t gs_, fs_, es_, ds_, cs_, ss_;
  uint64_t rdi_, rsi_, rbp_, rsp_, rbx_, rdx_, rcx_, rax_;
  uint64_t r8_, r9_, r10_, r11_, r12_, r13_, r14_, r15_;
};

}  // namespace efi