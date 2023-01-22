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
  uint16_t Fcw;
  uint16_t Fsw;
  uint16_t Ftw;
  uint16_t Opcode;
  uint64_t Rip;
  uint64_t DataOffset;
  uint8_t Reserved1[8];
  uint8_t St0Mm0[10], Reserved2[6];
  uint8_t St1Mm1[10], Reserved3[6];
  uint8_t St2Mm2[10], Reserved4[6];
  uint8_t St3Mm3[10], Reserved5[6];
  uint8_t St4Mm4[10], Reserved6[6];
  uint8_t St5Mm5[10], Reserved7[6];
  uint8_t St6Mm6[10], Reserved8[6];
  uint8_t St7Mm7[10], Reserved9[6];
  uint8_t Xmm0[16];
  uint8_t Xmm1[16];
  uint8_t Xmm2[16];
  uint8_t Xmm3[16];
  uint8_t Xmm4[16];
  uint8_t Xmm5[16];
  uint8_t Xmm6[16];
  uint8_t Xmm7[16];
  uint8_t Reserved11[14 * 16];
};

class SystemContext {
  uint64_t ExceptionData;  // ExceptionData is
                           // additional data pushed
                           // on the stack by some
                           // types of x64 64-enum_bit
                           // mode exceptions
  FXSaveStateX64 FxSaveState;
  uint64_t Dr0, Dr1, Dr2, Dr3, Dr6, Dr7;
  uint64_t Cr0, Cr1 /* Reserved */, Cr2, Cr3, Cr4, Cr8;
  uint64_t Rflags;
  uint64_t Ldtr, Tr;
  uint64_t Gdtr[2], Idtr[2];
  uint64_t Rip;
  uint64_t Gs, Fs, Es, Ds, Cs, Ss;
  uint64_t Rdi, Rsi, Rbp, Rsp, Rbx, Rdx, Rcx, Rax;
  uint64_t R8, R9, R10, R11, R12, R13, R14, R15;
};

}  // namespace efi