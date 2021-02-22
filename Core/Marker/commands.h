//
// Created by markus on 2021-02-21.
//

#include <cstdint>

#ifndef CPPFROMBLINKY_COMMANDS_H
namespace emarker {
enum class Command : std::uint8_t {
  DriverOutput = 0x01,
  BoosterSoftStart = 0x0C,
  DeepSleepMode = 0x10,
  DataEntryMode = 0x11,
  SWReset = 0x12,
  TemperatureSensor = 0x1A,
  MasterActivation = 0x20,
  DisplayUpdate1 = 0x21,
  DisplayUpdate2 = 0x22,
  WriteRAM = 0x24,
  WriteVCOM = 0x2C,
  WriteLUT = 0x32,
  SetDummyLinePeriod = 0x3A,
  SetGateLineWidth = 0x3B,
  BorderWaveform = 0x3C,
  SetRAMPosX = 0x44,
  SetRAMPosY = 0x45,
  SetRAMCounterX = 0x4E,
  SetRAMCounterY = 0x4F,
  NOP = 0xFF,
};

}
#define CPPFROMBLINKY_COMMANDS_H

#endif  // CPPFROMBLINKY_COMMANDS_H
