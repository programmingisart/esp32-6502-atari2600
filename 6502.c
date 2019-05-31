
#include <Arduino.h>

uint16_t PC; //Program Counter, 16bit
uint8_t SP; //Stack Pointer
uint8_t A; //Accumulator
uint8_t X; //X Index Register
uint8_t Y; //Y Index Register
uint8_t P; //Processor Status (NVssDIZC)

uint8_t MEMORY[8192]; //RAM

uint8_t VSYNC;
uint8_t VBLANK;
uint8_t WSYNC;
uint8_t COLUBK;

uint8_t PF0;
uint8_t PF1;
uint8_t PF2;
uint8_t COLUPF;



uint8_t execute(uint8_t opc, uint8_t arg1, uint8_t arg2);


uint8_t execute(uint8_t opc, uint8_t arg1, uint8_t arg2) { //oP takes two arguments, returns execution time (cycles needed to execute a operation)
  int temp;
  uint16_t astemp;
  uint8_t toadd;
  uint8_t tosub;
  switch (opc) {

    case 0x69: //ADC Immediate
      toadd = arg1;
      astemp = A + toadd + (P & 0b1);
      PC += 2;
      P = P & 0b00111100; //clear previous nvzc
      if (A > 0 && toadd > 0 && astemp < 0) P |= 0b01000000; //set V
      if (astemp > 255)P |= 0b00000001; //set C
      A = astemp & 0b0000000011111111;
      if (A & 0b10000000)P |= 0b10000000; //set N
      if (A == 0) P |= 0b00000010; //set Z
      return 2;

    case 0x65: //ADC Zero Page
      toadd = MEMORY[arg1];
      astemp = A + toadd + (P & 0b1);
      PC += 2;
      P = P & 0b00111100; //clear previous nvzc
      if (A > 0 && toadd > 0 && astemp < 0) P |= 0b01000000; //set V
      if (astemp > 255)P |= 0b00000001; //set C
      A = astemp & 0b0000000011111111;
      if (A & 0b10000000)P |= 0b10000000; //set N
      if (A == 0) P |= 0b00000010; //set Z
      return 3;

    case 0x75: //ADC Zero Page,X
      toadd = MEMORY[arg1 + X];
      astemp = A + toadd + (P & 0b1);
      PC += 2;
      P = P & 0b00111100; //clear previous nvzc
      if (A > 0 && toadd > 0 && astemp < 0) P |= 0b01000000; //set V
      if (astemp > 255)P |= 0b00000001; //set C
      A = astemp & 0b0000000011111111;
      if (A & 0b10000000)P |= 0b10000000; //set N
      if (A == 0) P |= 0b00000010; //set Z
      return 4;

    case 0x6D: //ADC Absolute
      toadd = MEMORY[(((uint16_t)arg2) << 8) + arg1];
      astemp = A + toadd + (P & 0b1);
      PC += 3;
      P = P & 0b00111100; //clear previous nvzc
      if (A > 0 && toadd > 0 && astemp < 0) P |= 0b01000000; //set V
      if (astemp > 255)P |= 0b00000001; //set C
      A = astemp & 0b0000000011111111;
      if (A & 0b10000000)P |= 0b10000000; //set N
      if (A == 0) P |= 0b00000010; //set Z
      return 4;

    case 0x7D: //ADC Absolute,X
      toadd = MEMORY[(((uint16_t)arg2) << 8) + arg1 + X];
      astemp = A + toadd + (P & 0b1);
      PC += 3;
      P = P & 0b00111100; //clear previous nvzc
      if (A > 0 && toadd > 0 && astemp < 0) P |= 0b01000000; //set V
      if (astemp > 255)P |= 0b00000001; //set C
      A = astemp & 0b0000000011111111;
      if (A & 0b10000000)P |= 0b10000000; //set N
      if (A == 0) P |= 0b00000010; //set Z
      return 4; //TODO Page Boundary

    case 0x79: //ADC Absolute,Y
      toadd = MEMORY[(((uint16_t)arg2) << 8) + arg1 + Y];
      astemp = A + toadd + (P & 0b1);
      PC += 3;
      P = P & 0b00111100; //clear previous nvzc
      if (A > 0 && toadd > 0 && astemp < 0) P |= 0b01000000; //set V
      if (astemp > 255)P |= 0b00000001; //set C
      A = astemp & 0b0000000011111111;
      if (A & 0b10000000)P |= 0b10000000; //set N
      if (A == 0) P |= 0b00000010; //set Z
      return 4; //TODO Page Boundary

    case 0x61: //ADC (Indirect,X)
      toadd = MEMORY[MEMORY[(((uint16_t)arg2) << 8) + arg1 + X]];
      astemp = A + toadd + (P & 0b1);
      PC += 2;
      P = P & 0b00111100; //clear previous nvzc
      if (A > 0 && toadd > 0 && astemp < 0) P |= 0b01000000; //set V
      if (astemp > 255)P |= 0b00000001; //set C
      A = astemp & 0b0000000011111111;
      if (A & 0b10000000)P |= 0b10000000; //set N
      if (A == 0) P |= 0b00000010; //set Z
      return 6;

    case 0x71: //ADC (Indirect),Y
      toadd = MEMORY[MEMORY[(((uint16_t)arg2) << 8) + arg1] + Y];
      astemp = A + toadd + (P & 0b1);
      PC += 2;
      P = P & 0b00111100; //clear previous nvzc
      if (A > 0 && toadd > 0 && astemp < 0) P |= 0b01000000; //set V
      if (astemp > 255)P |= 0b00000001; //set C
      A = astemp & 0b0000000011111111;
      if (A & 0b10000000)P |= 0b10000000; //set N
      if (A == 0) P |= 0b00000010; //set Z
      return 5; //TODO Page Boundary

    //============================================================

    case 0x29: //AND Immediate
      A &= arg1;
      PC += 2;
      P = P & 0b01111101; //clear previous nz
      if (A & 0b10000000)P |= 0b10000000; //set N
      if (A == 0) P |= 0b00000010; //set Z
      return 2;

    case 0x25: //AND Zero Page
      A &= MEMORY[arg1];
      PC += 2;
      P = P & 0b01111101; //clear previous nz
      if (A & 0b10000000)P |= 0b10000000; //set N
      if (A == 0) P |= 0b00000010; //set Z
      return 3;

    case 0x35: //AND Zero Page,X
      A &= MEMORY[arg1 + X];
      PC += 2;
      P = P & 0b01111101; //clear previous nz
      if (A & 0b10000000)P |= 0b10000000; //set N
      if (A == 0) P |= 0b00000010; //set Z
      return 4;

    case 0x2D: //AND Absolute
      A &= MEMORY[(((uint16_t)arg2) << 8) + arg1];
      PC += 3;
      P = P & 0b01111101; //clear previous nz
      if (A & 0b10000000)P |= 0b10000000; //set N
      if (A == 0) P |= 0b00000010; //set Z
      return 4;

    case 0x3D: //AND Absolute,X
      A &= MEMORY[(((uint16_t)arg2) << 8) + arg1 + X];
      PC += 3;
      P = P & 0b01111101; //clear previous nz
      if (A & 0b10000000)P |= 0b10000000; //set N
      if (A == 0) P |= 0b00000010; //set Z
      return 4; //TODO Page Boundary

    case 0x39: //AND Absolute,Y
      A &= MEMORY[(((uint16_t)arg2) << 8) + arg1 + Y];
      PC += 3;
      P = P & 0b01111101; //clear previous nz
      if (A & 0b10000000)P |= 0b10000000; //set N
      if (A == 0) P |= 0b00000010; //set Z
      return 4; //TODO Page Boundary

    case 0x21: //AND (Indirect,X)
      A &= MEMORY[MEMORY[(((uint16_t)arg2) << 8) + arg1 + X]];
      PC += 2;
      P = P & 0b01111101; //clear previous nz
      if (A & 0b10000000)P |= 0b10000000; //set N
      if (A == 0) P |= 0b00000010; //set Z
      return 6;

    case 0x31: //AND (Indirect),Y
      A &= MEMORY[MEMORY[(((uint16_t)arg2) << 8) + arg1] + Y];
      PC += 2;
      P = P & 0b01111101; //clear previous nz
      if (A & 0b10000000)P |= 0b10000000; //set N
      if (A == 0) P |= 0b00000010; //set Z
      return 5; //TODO Page Boundary

    //============================================================

    case 0x0A: //ASL Accumulator
      P |= (A & 0b10000000); //set C
      A << 1;
      PC += 1;
      P = P & 0b01111100; //clear previous nzc
      if (A & 0b10000000)P |= 0b10000000; //set N
      if (A == 0) P |= 0b00000010; //set Z
      return 2;

    case 0x06: //ASL Zero Page
      P |= (MEMORY[arg1] & 0b10000000); //set C
      MEMORY[arg1] << 1;
      PC += 2;
      P = P & 0b01111100; //clear previous nzc
      if (MEMORY[arg1] & 0b10000000)P |= 0b10000000; //set N
      if (MEMORY[arg1] == 0) P |= 0b00000010; //set Z
      return 5;

    case 0x16: //ASL Zero Page,X
      P |= (MEMORY[arg1 + X] & 0b10000000); //set C
      MEMORY[arg1 + X] << 1;
      PC += 2;
      P = P & 0b01111100; //clear previous nzc
      if (MEMORY[arg1 + X] & 0b10000000)P |= 0b10000000; //set N
      if (MEMORY[arg1 + X] == 0) P |= 0b00000010; //set Z
      return 6;

    case 0x0E: //ASL Absolute
      P |= (MEMORY[(((uint16_t)arg2) << 8) + arg1] & 0b10000000); //set C
      MEMORY[arg1] << 1;
      PC += 3;
      P = P & 0b01111100; //clear previous nzc
      if (MEMORY[arg1] & 0b10000000)P |= 0b10000000; //set N
      if (MEMORY[arg1] == 0) P |= 0b00000010; //set Z
      return 6;

    case 0x1E: //ASL Absolute,X
      P |= (MEMORY[(((uint16_t)arg2) << 8) + arg1 + X] & 0b10000000); //set C
      MEMORY[arg1 + X] << 1;
      PC += 3;
      P = P & 0b01111100; //clear previous nzc
      if (MEMORY[arg1 + X] & 0b10000000)P |= 0b10000000; //set N
      if (MEMORY[arg1 + X] == 0) P |= 0b00000010; //set Z
      return 7;

    //============================================================

    case 0x24: //BIT Zero Page
      temp = A & MEMORY[arg1];
      PC += 2;
      P = P & 0b00111101; //clear previous nvz
      P |= MEMORY[arg1] & 0b10000000; //set N
      P |= MEMORY[arg1] & 0b01000000; //set V
      if (temp == 0)P |= 0b00000010; //set Z
      return 3;

    case 0x2C: //BIT Absolute
      temp = A & MEMORY[(((uint16_t)arg2) << 8) + arg1];
      PC += 3;
      P = P & 0b00111101; //clear previous nvz
      P |= MEMORY[(((uint16_t)arg2) << 8) + arg1] & 0b10000000; //set N
      P |= MEMORY[(((uint16_t)arg2) << 8) + arg1] & 0b01000000; //set V
      if (temp == 0)P |= 0b00000010; //set Z
      return 4;

    //============================================================

    case 0x10: //BPL
      if (!(P & 0b10000000))
        if (!(arg1 & 0b1000000)) PC += arg1 + 1 ;
        else PC -= (254 - arg1);
      else {
        PC += 2;
        return 2;
      }
      return 3; //TODO Page Boundary

    case 0x30: //BMI
      if (P & 0b10000000)
        if (!(arg1 & 0b1000000)) PC += arg1 + 1;
        else PC -= (254 - arg1);
      else {
        PC += 2;
        return 2;
      }
      return 3; //TODO Page Boundary

    case 0x50: //BVC
      if (!(P & 0b01000000))
        if (!(arg1 & 0b1000000)) PC += arg1 + 1;
        else PC -= (254 - arg1);
      else {
        PC += 2;
        return 2;
      }
      return 3; //TODO Page Boundary

    case 0x70: //BVS
      if (P & 0b01000000)
        if (!(arg1 & 0b1000000)) PC += arg1 + 1;
        else PC -= (254 - arg1);
      else {
        PC += 2;
        return 2;
      }
      return 3; //TODO Page Boundary

    case 0x90: //BCC
      if (!(P & 0b00000001))
        if (!(arg1 & 0b1000000)) PC += arg1 + 1;
        else PC -= (254 - arg1);
      else {
        PC += 2;
        return 2;
      }
      return 3; //TODO Page Boundary

    case 0xB0: //BCS
      if (P & 0b00000001)
        if (!(arg1 & 0b1000000)) PC += arg1 + 1;
        else PC -= (254 - arg1);
      else {
        PC += 2;
        return 2;
      }
      return 3; //TODO Page Boundary

    case 0xD0: //BNE
      if (!(P & 0b00000010))
        if (!(arg1 & 0b1000000)) PC += arg1 + 1;
        else PC -= (254 - arg1);
      else {
        PC += 2;
        return 2;
      }
      return 3; //TODO Page Boundary

    case 0xF0: //BEQ
      if (P & 0b00000010)
        if (!(arg1 & 0b1000000)) PC += arg1 + 1;
        else PC -= (254 - arg1);
      else {
        PC += 2;
        return 2;
      }
      return 3; //TODO Page Boundary

    //============================================================

    case 0x00: //BRK
      PC = 8192;
      return 0;
    //TODO

    //============================================================

    case 0xC9: //CMP Immediate
      temp = A - arg1 + (P & 0b1);
      PC += 2;
      P = P & 0b01111100; //clear previous nzc
      if (A & 0b10000000)P |= 0b10000000; //set N
      if (temp == 0) P |= 0b00000010; //set Z
      if (temp >= 0)P |= 0b00000001; //set C
      return 2;

    case 0xC5: //CMP Zero Page
      temp = A - MEMORY[arg1] + (P & 0b1);
      PC += 2;
      P = P & 0b01111100; //clear previous nzc
      if (temp & 0b10000000)P |= 0b10000000; //set N
      if (temp == 0) P |= 0b00000010; //set Z
      if (temp >= 0)P |= 0b00000001; //set C
      return 3;

    case 0xD5: //CMP Zero Page,X
      temp = A - MEMORY[arg1 + X] + (P & 0b1);
      PC += 2;
      P = P & 0b01111100; //clear previous nzc
      if (temp & 0b10000000)P |= 0b10000000; //set N
      if (temp == 0) P |= 0b00000010; //set Z
      if (temp >= 0)P |= 0b00000001; //set C
      return 4;

    case 0xCD: //CMP Absolute
      temp = A - MEMORY[(((uint16_t)arg2) << 8) + arg1] + (P & 0b1);
      PC += 3;
      P = P & 0b01111100; //clear previous nzc
      if (temp & 0b10000000)P |= 0b10000000; //set N
      if (temp == 0) P |= 0b00000010; //set Z
      if (temp >= 0)P |= 0b00000001; //set C
      return 4;

    case 0xDD: //CMP Absolute,X
      temp = A - MEMORY[(((uint16_t)arg2) << 8) + arg1 + X] + (P & 0b1);
      PC += 3;
      P = P & 0b01111100; //clear previous nzc
      if (temp & 0b10000000)P |= 0b10000000; //set N
      if (temp == 0) P |= 0b00000010; //set Z
      if (temp >= 0)P |= 0b00000001; //set C
      return 4; //TODO Page Boundary

    case 0xD9: //CMP Absolute,Y
      temp = A - MEMORY[(((uint16_t)arg2) << 8) + arg1 + Y] + (P & 0b1);
      PC += 3;
      P = P & 0b01111100; //clear previous nzc
      if (temp & 0b10000000)P |= 0b10000000; //set N
      if (temp == 0) P |= 0b00000010; //set Z
      if (temp >= 0)P |= 0b00000001; //set C
      return 4; //TODO Page Boundary

    case 0xC1: //CMP (Indirect,X)
      temp = A - MEMORY[MEMORY[(((uint16_t)arg2) << 8) + arg1 + X]] + (P & 0b1);
      PC += 2;
      P = P & 0b01111100; //clear previous nzc
      if (temp & 0b10000000)P |= 0b10000000; //set N
      if (temp == 0) P |= 0b00000010; //set Z
      if (temp >= 0)P |= 0b00000001; //set C
      return 6;

    case 0xD1: //CMP (Indirect),Y
      temp = A - MEMORY[MEMORY[(((uint16_t)arg2) << 8) + arg1] + Y] + (P & 0b1);
      PC += 2;
      P = P & 0b01111100; //clear previous nzc
      if (temp & 0b10000000)P |= 0b10000000; //set N
      if (temp == 0) P |= 0b00000010; //set Z
      if (temp >= 0)P |= 0b00000001; //set C
      return 5; //TODO Page Boundary

    //============================================================

    case 0xE0: //CPX Immediate
      temp = X - arg1;
      PC += 2;
      P = P & 0b01111100; //clear previous nzc
      if (A & 0b10000000)P |= 0b10000000; //set N
      if (temp == 0) P |= 0b00000010; //set Z
      if (temp >= 0)P |= 0b00000001; //set C
      return 2;

    case 0xE4: //CPX Zero Page
      temp = X - MEMORY[arg1];
      PC += 2;
      P = P & 0b01111100; //clear previous nzc
      if (temp & 0b10000000)P |= 0b10000000; //set N
      if (temp == 0) P |= 0b00000010; //set Z
      if (temp >= 0)P |= 0b00000001; //set C
      return 3;

    case 0xEC: //CPX Absolute
      temp = X - MEMORY[(((uint16_t)arg2) << 8) + arg1];
      PC += 3;
      P = P & 0b01111100; //clear previous nzc
      if (temp & 0b10000000)P |= 0b10000000; //set N
      if (temp == 0) P |= 0b00000010; //set Z
      if (temp >= 0)P |= 0b00000001; //set C
      return 4;

    //============================================================

    case 0xC0: //CPY Immediate
      temp = Y - arg1;
      PC += 2;
      P = P & 0b01111100; //clear previous nzc
      if (A & 0b10000000)P |= 0b10000000; //set N
      if (temp == 0) P |= 0b00000010; //set Z
      if (temp >= 0)P |= 0b00000001; //set C
      return 2;

    case 0xC4: //CPY Zero Page
      temp = Y - MEMORY[arg1];
      PC += 2;
      P = P & 0b01111100; //clear previous nzc
      if (temp & 0b10000000)P |= 0b10000000; //set N
      if (temp == 0) P |= 0b00000010; //set Z
      if (temp >= 0)P |= 0b00000001; //set C
      return 3;

    case 0xCC: //CPY Absolute
      temp = Y - MEMORY[(((uint16_t)arg2) << 8) + arg1];
      PC += 3;
      P = P & 0b01111100; //clear previous nzc
      if (temp & 0b10000000)P |= 0b10000000; //set N
      if (temp == 0) P |= 0b00000010; //set Z
      if (temp >= 0)P |= 0b00000001; //set C
      return 4;

    //============================================================

    case 0xC6: //DEC Zero Page
      MEMORY[arg1]--;
      PC += 2;
      P = P & 0b01111101; //clear previous nz
      if (MEMORY[arg1] & 0b10000000)P |= 0b10000000; //set N
      if (MEMORY[arg1] == 0) P |= 0b00000010; //set Z
      return 5;

    case 0xD6: //DEC Zero Page,X
      MEMORY[arg1]--;
      PC += 2;
      P = P & 0b01111101; //clear previous nz
      if (MEMORY[arg1] & 0b10000000)P |= 0b10000000; //set N
      if (MEMORY[arg1] == 0) P |= 0b00000010; //set Z
      return 6;

    case 0xCE: //DEC Absolute
      MEMORY[(((uint16_t)arg2) << 8) + arg1]--;
      PC += 3;
      P = P & 0b01111101; //clear previous nz
      if (MEMORY[(((uint16_t)arg2) << 8) + arg1] & 0b10000000)P |= 0b10000000; //set N
      if (MEMORY[(((uint16_t)arg2) << 8) + arg1] == 0) P |= 0b00000010; //set Z
      return 6;

    case 0xDE: //DEC Absolute,X
      MEMORY[(((uint16_t)arg2) << 8) + arg1 + X]--;
      PC += 3;
      P = P & 0b01111101; //clear previous nz
      if (MEMORY[(((uint16_t)arg2) << 8) + arg1 + X] & 0b10000000)P |= 0b10000000; //set N
      if (MEMORY[(((uint16_t)arg2) << 8) + arg1 + X] == 0) P |= 0b00000010; //set Z
      return 7; //TODO Page Boundary

    //============================================================

    case 0x49: //EOR  Immediate
      A ^= arg1;
      PC += 2;
      P = P & 0b01111101; //clear previous nz
      if (A & 0b10000000)P |= 0b10000000; //set N
      if (A == 0) P |= 0b00000010; //set Z

      return 2;

    case 0x45: //EOR Zero Page
      A ^= MEMORY[arg1];
      PC += 2;
      P = P & 0b01111101; //clear previous nz
      if (A & 0b10000000)P |= 0b10000000; //set N
      if (A == 0) P |= 0b00000010; //set Z

      return 3;

    case 0x55: //EOR Zero Page,X
      A ^= MEMORY[arg1 + X];
      PC += 2;
      P = P & 0b01111101; //clear previous nz
      if (A & 0b10000000)P |= 0b10000000; //set N
      if (A == 0) P |= 0b00000010; //set Z

      return 4;

    case 0x4D: //EOR Absolute
      A ^= MEMORY[(((uint16_t)arg2) << 8) + arg1];
      PC += 3;
      P = P & 0b01111101; //clear previous nz
      if (A & 0b10000000)P |= 0b10000000; //set N
      if (A == 0) P |= 0b00000010; //set Z

      return 4;

    case 0x5D: //EOR Absolute,X
      A ^= MEMORY[(((uint16_t)arg2) << 8) + arg1 + X];
      PC += 3;
      P = P & 0b01111101; //clear previous nz
      if (A & 0b10000000)P |= 0b10000000; //set N
      if (A == 0) P |= 0b00000010; //set Z

      return 4; //TODO Page Boundary

    case 0x59: //EOR Absolute,Y
      A ^= MEMORY[(((uint16_t)arg2) << 8) + arg1 + Y];
      PC += 3;
      P = P & 0b01111101; //clear previous nz
      if (A & 0b10000000)P |= 0b10000000; //set N
      if (A == 0) P |= 0b00000010; //set Z
      if (arg1 + arg2 > 255)P |= 0x1; //set C
      return 4; //TODO Page Boundary

    case 0x41: //EOR (Indirect,X)
      A ^= MEMORY[MEMORY[(((uint16_t)arg2) << 8) + arg1 + X]];
      PC += 2;
      P = P & 0b01111101; //clear previous nz
      if (A & 0b10000000)P |= 0b10000000; //set N
      if (A == 0) P |= 0b00000010; //set Z

      return 6;

    case 0x51: //EOR (Indirect),Y
      A ^= MEMORY[MEMORY[(((uint16_t)arg2) << 8) + arg1] + Y];
      PC += 2;
      P = P & 0b01111101; //clear previous nz
      if (A & 0b10000000)P |= 0b10000000; //set N

      if (A == 0) P |= 0b00000010; //set Z

      return 5; //TODO Page Boundary

    //============================================================

    case 0x18: //CLC
      P = P & 0b11111110; //clear C
      PC += 1;
      return 2;

    case 0x38: //SEC
      P = P | 0b00000001; //set C
      PC += 1;
      return 2;

    case 0x58: //CLI
      P = P & 0b11111011; //clear I
      PC += 1;
      return 2;

    case 0x78: //SEI
      P = P | 0b00000100; //set I
      PC += 1;
      return 2;

    case 0xB8: //CLV
      P = P & 0b10111111; //clear V
      PC += 1;
      return 2;

    case 0xD8: //CLD
      P = P & 0b11110111; //clear D
      PC += 1;
      return 2;

    case 0xF8: //SED
      P = P | 0b00001000; //set D
      PC += 1;
      return 2;

    //============================================================

    case 0xE6: //INC Zero Page
      MEMORY[arg1]++;
      PC += 2;
      P = P & 0b01111101; //clear previous nz
      if (MEMORY[arg1] & 0b10000000)P |= 0b10000000; //set N
      if (MEMORY[arg1] == 0) P |= 0b00000010; //set Z
      return 5;

    case 0xF6: //INC Zero Page,X
      MEMORY[arg1]++;
      PC += 2;
      P = P & 0b01111101; //clear previous nz
      if (MEMORY[arg1] & 0b10000000)P |= 0b10000000; //set N
      if (MEMORY[arg1] == 0) P |= 0b00000010; //set Z
      return 6;

    case 0xEE: //INC Absolute
      MEMORY[(((uint16_t)arg2) << 8) + arg1]++;
      PC += 3;
      P = P & 0b01111101; //clear previous nz
      if (MEMORY[(((uint16_t)arg2) << 8) + arg1] & 0b10000000)P |= 0b10000000; //set N
      if (MEMORY[(((uint16_t)arg2) << 8) + arg1] == 0) P |= 0b00000010; //set Z
      return 6;

    case 0xFE: //INC Absolute,X
      MEMORY[(((uint16_t)arg2) << 8) + arg1 + X]++;
      PC += 3;
      P = P & 0b01111101; //clear previous nz
      if (MEMORY[(((uint16_t)arg2) << 8) + arg1 + X] & 0b10000000)P |= 0b10000000; //set N
      if (MEMORY[(((uint16_t)arg2) << 8) + arg1 + X] == 0) P |= 0b00000010; //set Z
      return 7; //TODO Page Boundary

    //============================================================

    case 0x4C: //JMP Absolute    //poscul
      PC = (((uint16_t)arg2) << 8) + arg1;
      return 3;

    case 0x6C: //JMP Indirect
      PC = MEMORY[(((uint16_t)arg2) << 8) + arg1];//TODO THis is not correct
      return 3;


    //============================================================

    case 0xA9: //LDA Immediate
      A = arg1;
      PC += 2;
      P = P & 0b01111101; //clear previous nz
      if (A & 0b10000000)P |= 0b10000000; //set N
      if (A == 0) P |= 0b00000010; //set Z
      return 2;

    case 0xA5: //LDA Zero Page
      A = MEMORY[arg1];
      PC += 2;
      P = P & 0b01111101; //clear previous nz
      if (A & 0b10000000)P |= 0b10000000; //set N
      if (A == 0) P |= 0b00000010; //set Z
      return 3;

    case 0xB5: //LDA Zero Page,X
      A = MEMORY[arg1 + X];
      PC += 2;
      P = P & 0b01111101; //clear previous nz
      if (A & 0b10000000)P |= 0b10000000; //set N
      if (A == 0) P |= 0b00000010; //set Z
      return 4;

    case 0xAD: //LDA Absolute
      A = MEMORY[(((uint16_t)arg2) << 8) + arg1];
      PC += 3;
      P = P & 0b01111101; //clear previous nz
      if (A & 0b10000000)P |= 0b10000000; //set N
      if (A == 0) P |= 0b00000010; //set Z
      return 4;

    case 0xBD: //LDA Absolute,X
      A = MEMORY[(((uint16_t)arg2) << 8) + arg1 + X];
      PC += 3;
      P = P & 0b01111101; //clear previous nz
      if (A & 0b10000000)P |= 0b10000000; //set N
      if (arg1 > 0 && arg2 > 0 && A < 0) P |= 0b01000000; //set V
      if (A == 0) P |= 0b00000010; //set Z
      return 4; //TODO Page Boundary

    case 0xB9: //LDA Absolute,Y
      A = MEMORY[(((uint16_t)arg2) << 8) + arg1 + Y];
      PC += 3;
      P = P & 0b01111101; //clear previous nz
      if (A & 0b10000000)P |= 0b10000000; //set N
      if (A == 0) P |= 0b00000010; //set Z
      return 4; //TODO Page Boundary

    case 0xA1: //LDA (Indirect,X)
      A = MEMORY[MEMORY[(((uint16_t)arg2) << 8) + arg1 + X]];
      PC += 2;
      P = P & 0b01111101; //clear previous nz
      if (A & 0b10000000)P |= 0b10000000; //set N
      if (A == 0) P |= 0b00000010; //set Z
      return 6;

    case 0xB1: //LDA (Indirect),Y
      A = MEMORY[MEMORY[(((uint16_t)arg2) << 8) + arg1] + Y];
      PC += 2;
      P = P & 0b01111101; //clear previous nz
      if (A & 0b10000000)P |= 0b10000000; //set N
      if (A == 0) P |= 0b00000010; //set Z
      return 5; //TODO Page Boundary

    //============================================================

    case 0xA2: //LDX Immediate
      X = arg1;
      PC += 2;
      P = P & 0b01111101; //clear previous nz
      if (X & 0b10000000)P |= 0b10000000; //set N
      if (X == 0) P |= 0b00000010; //set Z
      return 2;

    case 0xA6: //LDX Zero Page
      X = MEMORY[arg1];
      PC += 2;
      P = P & 0b01111101; //clear previous nz
      if (X & 0b10000000)P |= 0b10000000; //set N
      if (X == 0) P |= 0b00000010; //set Z
      return 3;

    case 0xB6: //LDX Zero Page,Y
      X = MEMORY[arg1 + Y];
      PC += 2;
      P = P & 0b01111101; //clear previous nz
      if (X & 0b10000000)P |= 0b10000000; //set N
      if (X == 0) P |= 0b00000010; //set Z
      return 4;

    case 0xAE: //LDX Absolute
      X = MEMORY[(((uint16_t)arg2) << 8) + arg1];
      PC += 3;
      P = P & 0b01111101; //clear previous nz
      if (X & 0b10000000)P |= 0b10000000; //set N
      if (X == 0) P |= 0b00000010; //set Z
      return 4;


    case 0xBE: //LDX Absolute,Y
      X = MEMORY[(((uint16_t)arg2) << 8) + arg1 + Y];
      PC += 3;
      P = P & 0b01111101; //clear previous nz
      if (X & 0b10000000)P |= 0b10000000; //set N
      if (X == 0) P |= 0b00000010; //set Z
      return 4; //TODO Page Boundary

    //============================================================

    case 0xA0: //LDY Immediate
      Y = arg1;
      PC += 2;
      P = P & 0b01111101; //clear previous nz
      if (Y & 0b10000000)P |= 0b10000000; //set N
      if (Y == 0) P |= 0b00000010; //set Z
      return 2;

    case 0xA4: //LDY Zero Page
      Y = MEMORY[arg1];
      PC += 2;
      P = P & 0b01111101; //clear previous nz
      if (Y & 0b10000000)P |= 0b10000000; //set N
      if (Y == 0) P |= 0b00000010; //set Z
      return 3;

    case 0xB4: //LDY Zero Page,X
      Y = MEMORY[arg1 + X];
      PC += 2;
      P = P & 0b01111101; //clear previous nz
      if (Y & 0b10000000)P |= 0b10000000; //set N
      if (Y == 0) P |= 0b00000010; //set Z
      return 4;

    case 0xAC: //LDY Absolute
      Y = MEMORY[(((uint16_t)arg2) << 8) + arg1];
      PC += 3;
      P = P & 0b01111101; //clear previous nz
      if (Y & 0b10000000)P |= 0b10000000; //set N
      if (Y == 0) P |= 0b00000010; //set Z
      return 4;


    case 0xBC: //LDY Absolute,X
      Y = MEMORY[(((uint16_t)arg2) << 8) + arg1 + X];
      PC += 3;
      P = P & 0b01111101; //clear previous nz
      if (Y & 0b10000000)P |= 0b10000000; //set N
      if (Y == 0) P |= 0b00000010; //set Z
      return 4; //TODO Page Boundary


    //============================================================

    case 0x4A: //LSR Accumulator
      P |= (A & 0b10000000); //set C
      A >> 1;
      PC += 1;
      P = P & 0b01111100; //clear previous nzc
      if (A & 0b10000000)P |= 0b10000000; //set N
      if (A == 0) P |= 0b00000010; //set Z
      return 2;

    case 0x46: //LSR Zero Page
      P |= (MEMORY[arg1] & 0b10000000); //set C
      MEMORY[arg1] >> 1;
      PC += 2;
      P = P & 0b01111100; //clear previous nzc
      if (MEMORY[arg1] & 0b10000000)P |= 0b10000000; //set N
      if (MEMORY[arg1] == 0) P |= 0b00000010; //set Z
      return 5;

    case 0x56: //LSR Zero Page,X
      P |= (MEMORY[arg1 + X] & 0b10000000); //set C
      MEMORY[arg1 + X] >> 1;
      PC += 2;
      P = P & 0b01111100; //clear previous nzc
      if (MEMORY[arg1 + X] & 0b10000000)P |= 0b10000000; //set N
      if (MEMORY[arg1 + X] == 0) P |= 0b00000010; //set Z
      return 6;

    case 0x4E: //LSR Absolute
      P |= (MEMORY[(((uint16_t)arg2) >> 8) + arg1] & 0b10000000); //set C
      MEMORY[arg1] >> 1;
      PC += 3;
      P = P & 0b01111100; //clear previous nzc
      if (MEMORY[arg1] & 0b10000000)P |= 0b10000000; //set N
      if (MEMORY[arg1] == 0) P |= 0b00000010; //set Z
      return 6;

    case 0x5E: //LSR Absolute,X
      P |= (MEMORY[(((uint16_t)arg2) >> 8) + arg1 + X] & 0b10000000); //set C
      MEMORY[arg1 + X] >> 1;
      PC += 3;
      P = P & 0b01111100; //clear previous nzc
      if (MEMORY[arg1 + X] & 0b10000000)P |= 0b10000000; //set N
      if (MEMORY[arg1 + X] == 0) P |= 0b00000010; //set Z
      return 7;



    //============================================================

    case 0xEA: //NOP
      PC += 1;
      return 2;

    //============================================================

    case 0x09: //ORA Immediate
      A |= arg1;
      PC += 2;
      P = P & 0b01111101; //clear previous nz
      if (A & 0b10000000)P |= 0b10000000; //set N
      if (A == 0) P |= 0b00000010; //set Z
      return 2;

    case 0x05: //ORA Zero Page
      A |= MEMORY[arg1];
      PC += 2;
      P = P & 0b01111101; //clear previous nz
      if (A & 0b10000000)P |= 0b10000000; //set N
      if (A == 0) P |= 0b00000010; //set Z
      return 3;

    case 0x15: //ORA Zero Page,X
      A |= MEMORY[arg1 + X];
      PC += 2;
      P = P & 0b01111101; //clear previous nz
      if (A & 0b10000000)P |= 0b10000000; //set N
      if (A == 0) P |= 0b00000010; //set Z
      return 4;

    case 0x0D: //ORA Absolute
      A |= MEMORY[(((uint16_t)arg2) << 8) + arg1];
      PC += 3;
      P = P & 0b01111101; //clear previous nz
      if (A & 0b10000000)P |= 0b10000000; //set N
      if (A == 0) P |= 0b00000010; //set Z
      return 4;

    case 0x1D: //ORA Absolute,X
      A |= MEMORY[(((uint16_t)arg2) << 8) + arg1 + X];
      PC += 3;
      P = P & 0b01111101; //clear previous nz
      if (A & 0b10000000)P |= 0b10000000; //set N
      if (A == 0) P |= 0b00000010; //set Z
      return 4; //TODO Page Boundary

    case 0x19: //ORA Absolute,Y
      A |= MEMORY[(((uint16_t)arg2) << 8) + arg1 + Y];
      PC += 3;
      P = P & 0b01111101; //clear previous nz
      if (A & 0b10000000)P |= 0b10000000; //set N
      if (A == 0) P |= 0b00000010; //set Z
      return 4; //TODO Page Boundary

    case 0x01: //ORA (Indirect,X)
      A |= MEMORY[MEMORY[(((uint16_t)arg2) << 8) + arg1 + X]];
      PC += 2;
      P = P & 0b01111101; //clear previous nz
      if (A & 0b10000000)P |= 0b10000000; //set N
      if (A == 0) P |= 0b00000010; //set Z
      return 6;

    case 0x11: //ORA (Indirect),Y
      A |= MEMORY[MEMORY[(((uint16_t)arg2) << 8) + arg1] + Y];
      PC += 2;
      P = P & 0b01111101; //clear previous nz
      if (A & 0b10000000)P |= 0b10000000; //set N
      if (A == 0) P |= 0b00000010; //set Z
      return 5; //TODO Page Boundary

    //============================================================

    case 0xAA: //TAX
      X = A;
      PC += 1;
      P = P & 0b01111101; //clear previous nz
      if (X & 0b10000000)P |= 0b10000000; //set N
      if (X == 0) P |= 0b00000010; //set Z
      return 2;

    case 0x8A: //TXA
      A = X;
      PC += 1;
      P = P & 0b01111101; //clear previous nz
      if (A & 0b10000000)P |= 0b10000000; //set N
      if (A == 0) P |= 0b00000010; //set Z
      return 2;

    case 0xCA: //DEX
      X--;
      PC += 1;
      P = P & 0b01111101; //clear previous nz
      if (X & 0b10000000)P |= 0b10000000; //set N
      if (X == 0) P |= 0b00000010; //set Z
      return 2;

    case 0xE8: //INX
      X++;
      PC += 1;
      P = P & 0b01111101; //clear previous nz
      if (X & 0b10000000)P |= 0b10000000; //set N
      if (X == 0) P |= 0b00000010; //set Z
      return 2;

    case 0xA8: //TAY
      Y = A;
      PC += 1;
      P = P & 0b01111101; //clear previous nz
      if (Y & 0b10000000)P |= 0b10000000; //set N
      if (Y == 0) P |= 0b00000010; //set Z
      return 2;

    case 0x98: //TYA
      A = Y;
      PC += 1;
      P = P & 0b01111101; //clear previous nz
      if (A & 0b10000000)P |= 0b10000000; //set N
      if (A == 0) P |= 0b00000010; //set Z
      return 2;

    case 0x88: //DEY
      Y--;
      PC += 1;
      P = P & 0b01111101; //clear previous nz
      if (Y & 0b10000000)P |= 0b10000000; //set N
      if (Y == 0) P |= 0b00000010; //set Z
      return 2;

    case 0xC8: //INY
      Y++;
      PC += 1;
      P = P & 0b01111101; //clear previous nz
      if (Y & 0b10000000)P |= 0b10000000; //set N
      if (Y == 0) P |= 0b00000010; //set Z
      return 2;

    //============================================================

    //TODO ROTATE

    //============================================================

    case 0xE9: //SBC Immediate
      tosub = arg1;
      astemp = A - tosub - (1 - (P & 0b1));
      PC += 2;
      P = P & 0b00111100; //clear previous nvzc
      if (A < 0 && tosub > 0 && astemp > 0) P |= 0b01000000; //set V
      if (astemp > 255)P |= 0b00000001; //set C
      A = astemp & 0b0000000011111111;
      if (A & 0b10000000)P |= 0b10000000; //set N
      if (A == 0) P |= 0b00000010; //set Z

      return 2;

    case 0xE5: //SBC Zero Page
      tosub = MEMORY[arg1];
      astemp = A - tosub - (1 - (P & 0b1));
      PC += 2;
      P = P & 0b00111100; //clear previous nvzc
      if (A < 0 && tosub > 0 && astemp > 0) P |= 0b01000000; //set V
      if (astemp > 255)P |= 0b00000001; //set C
      A = astemp & 0b0000000011111111;
      if (A & 0b10000000)P |= 0b10000000; //set N
      if (A == 0) P |= 0b00000010; //set Z
      return 3;

    case 0xF5: //SBC Zero Page,X
      tosub = MEMORY[arg1 + X];
      astemp = A - tosub - (1 - (P & 0b1));
      PC += 2;
      P = P & 0b00111100; //clear previous nvzc
      if (A < 0 && tosub > 0 && astemp > 0) P |= 0b01000000; //set V
      if (astemp > 255)P |= 0b00000001; //set C
      A = astemp & 0b0000000011111111;
      if (A & 0b10000000)P |= 0b10000000; //set N
      if (A == 0) P |= 0b00000010; //set Z
      return 4;

    case 0xED: //SBC Absolute
      tosub = MEMORY[(((uint16_t)arg2) << 8) + arg1];
      astemp = A - tosub - (1 - (P & 0b1));
      PC += 3;
      P = P & 0b00111100; //clear previous nvzc
      if (A < 0 && tosub > 0 && astemp > 0) P |= 0b01000000; //set V

      if (astemp > 255)P |= 0b00000001; //set C
      A = astemp & 0b0000000011111111;
      if (A & 0b10000000)P |= 0b10000000; //set N
      if (A == 0) P |= 0b00000010; //set Z
      return 4;

    case 0xFD: //SBC Absolute,X
      tosub = MEMORY[(((uint16_t)arg2) << 8) + arg1 + X];
      astemp = A - tosub - (1 - (P & 0b1));
      PC += 3;
      P = P & 0b00111100; //clear previous nvzc
      if (A < 0 && tosub > 0 && astemp > 0) P |= 0b01000000; //set V
      if (astemp > 255)P |= 0b00000001; //set C
      A = astemp & 0b0000000011111111;
      if (A & 0b10000000)P |= 0b10000000; //set N
      if (A == 0) P |= 0b00000010; //set Z
      return 4; //TODO Page Boundary

    case 0xF9: //SBC Absolute,Y
      tosub = MEMORY[(((uint16_t)arg2) << 8) + arg1 + Y];
      astemp = A - tosub - (1 - (P & 0b1));
      PC += 3;

      P = P & 0b00111100; //clear previous nvzc
      if (A < 0 && tosub > 0 && astemp > 0) P |= 0b01000000; //set V
      if (astemp > 255)P |= 0b00000001; //set C
      A = astemp & 0b0000000011111111;
      if (A & 0b10000000)P |= 0b10000000; //set N
      if (A == 0) P |= 0b00000010; //set Z
      return 4; //TODO Page Boundary

    case 0xE1: //SBC (Indirect,X)
      tosub = MEMORY[MEMORY[(((uint16_t)arg2) << 8) + arg1 + X]];
      astemp = A - tosub - (1 - (P & 0b1));
      PC += 2;
      P = P & 0b00111100; //clear previous nvzc
      if (A < 0 && tosub > 0 && astemp > 0) P |= 0b01000000; //set V
      if (astemp > 255)P |= 0b00000001; //set C
      A = astemp & 0b0000000011111111;
      if (A & 0b10000000)P |= 0b10000000; //set N
      if (A == 0) P |= 0b00000010; //set Z
      return 6;

    case 0xF1: //SBC (Indirect),Y
      tosub = MEMORY[MEMORY[(((uint16_t)arg2) << 8) + arg1] + Y];
      astemp = A - tosub - (1 - (P & 0b1));
      PC += 2;
      P = P & 0b00111100; //clear previous nvzc
      if (A < 0 && tosub > 0 && astemp > 0) P |= 0b01000000; //set V
      if (astemp > 255)P |= 0b00000001; //set C
      A = astemp & 0b0000000011111111;
      if (A & 0b10000000)P |= 0b10000000; //set N
      if (A == 0) P |= 0b00000010; //set Z
      return 5; //TODO Page Boundary

    //============================================================


    case 0x85: //STA Zero Page
      if (arg1 == 0x02) {
        WSYNC = 1;
      }
      MEMORY[arg1] = A;
      PC += 2;
      return 3;

    case 0x95: //STA Zero Page,X
      MEMORY[arg1 + X] = A;
      PC += 2;
      return 4;

    case 0x8D: //STA Absolute
      MEMORY[(((uint16_t)arg2) << 8) + arg1] = A;
      PC += 3;
      return 4;

    case 0x9D: //STA Absolute,X
      MEMORY[(((uint16_t)arg2) << 8) + arg1 + X] = A;
      PC += 3;
      return 5;

    case 0x99: //STA Absolute,Y
      toadd = MEMORY[(((uint16_t)arg2) << 8) + arg1 + Y];
      PC += 3;
      return 5;

    case 0x81: //STA (Indirect,X)
      MEMORY[MEMORY[(((uint16_t)arg2) << 8) + arg1 + X]] = A;
      PC += 2;
      return 6;

    case 0x91: //STA (Indirect),Y
      MEMORY[MEMORY[(((uint16_t)arg2) << 8) + arg1] + Y] = A;
      PC += 2;
      return 6;

    //============================================================

    //TODO Stack Instructions

    //============================================================


    case 0x86: //STX Zero Page
      MEMORY[arg1] = X;
      PC += 2;
      return 3;

    case 0x96: //STX Zero Page,Y
      MEMORY[arg1 + Y] = X;
      PC += 2;
      return 4;

    case 0x8E: //STX Absolute
      MEMORY[(((uint16_t)arg2) << 8) + arg1] = X;
      PC += 3;
      return 4;

    //============================================================

    case 0x84: //STY Zero Page
      MEMORY[arg1] = Y;
      PC += 2;
      return 3;

    case 0x94: //STY Zero Page,X
      MEMORY[arg1 + X] = Y;
      PC += 2;
      return 4;

    case 0x8C: //STY Absolute
      MEMORY[(((uint16_t)arg2) << 8) + arg1] = Y;
      PC += 3;
      return 4;

    default:
      PC += 1;
      return 0;

  }
}
