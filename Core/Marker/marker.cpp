//
// Created by markus on 2021-02-21.
//

#include "marker.h"

namespace emarker {
void EPaperScreen::HardReset() {
  HAL_GPIO_WritePin(rst_gpio_, rst_pin_, GPIO_PIN_SET);
  HAL_Delay(200);
  HAL_GPIO_WritePin(rst_gpio_, rst_pin_, GPIO_PIN_RESET);
  HAL_Delay(2);
  HAL_GPIO_WritePin(rst_gpio_, rst_pin_, GPIO_PIN_SET);
  HAL_Delay(200);
}

void EPaperScreen::SendCommand(Command cmd) {
  /* TODO: use RAII (struct with destructor and constructor) to handle pulling
      cs pin high/low */
  Select();
  SendCommandHelper(cmd);
  Deselect();
}

void EPaperScreen::SetTransmissionMode(EPaperScreen::TransmissionMode mode) {
  auto newPinState =
      (mode == TransmissionMode::Data) ? GPIO_PIN_SET : GPIO_PIN_RESET;
  HAL_GPIO_WritePin(dc_gpio_, dc_pin_, newPinState);
}
void EPaperScreen::ConfigureLUT(EPaperScreen::UpdateMode mode) {
  // TODO: some kind of check to now if chipselect is needed
  // TODO: make table be reference, currently not possible due to
  // HAL_SPI_Transmit
  auto table = (mode == UpdateMode::Full) ? kLutFullUpdate : kLutPartialUpdate;
  SendCommand(Command::WriteLUT);
  HAL_SPI_Transmit(hspi_, table.data(), table.size(), 1000);
}
void EPaperScreen::SendCommand(Command cmd, uint8_t data) {
  Select();
  SendCommandHelper(cmd);
  SendData(data);
  Deselect();
}
void EPaperScreen::SendCommandHelper(Command cmd) {
  SetTransmissionMode(TransmissionMode::Command);
  auto byte = static_cast<uint8_t>(cmd);
  HAL_SPI_Transmit(hspi_, &byte, 1, 1000);
}
void EPaperScreen::InitializeDisplay() {
  HAL_GPIO_WritePin(dc_gpio_, dc_pin_, GPIO_PIN_RESET);
  HAL_GPIO_WritePin(cs_gpio_, cs_pin_, GPIO_PIN_RESET);
  HAL_GPIO_WritePin(rst_gpio_, rst_pin_, GPIO_PIN_SET);

  HardReset();
  std::array<uint8_t, 3> driverData{
      static_cast<uint8_t>((height_ - 1) & 0xFF),
      static_cast<uint8_t>(((height_ - 1) >> 8) & 0xFF), 0x00};
  SendCommand(Command::DriverOutput, driverData);

  SendCommand(Command::BoosterSoftStart,
              std::array<uint8_t, 3>{0xD7, 0xD6, 0x9D});

  SendCommand(Command::WriteVCOM, 0xA8);
  SendCommand(Command::SetDummyLinePeriod, 0x1A);
  SendCommand(Command::SetGateLineWidth, 0x08);
  SendCommand(Command::BorderWaveform, 0x03);
  SendCommand(Command::DataEntryMode, 0x03);
  ConfigureLUT(UpdateMode::Full);
}

void EPaperScreen::SendData(uint8_t data) {
  SetTransmissionMode(TransmissionMode::Data);
  HAL_SPI_Transmit(hspi_, &data, 1, 1000);
}

template <size_t N>
void EPaperScreen::SendData(std::array<uint8_t, N> data) {
  SetTransmissionMode(TransmissionMode::Data);
  HAL_SPI_Transmit(hspi_, data.data(), data.size(), 1000);
}
template <size_t N>
void EPaperScreen::SendCommand(Command cmd, std::array<uint8_t, N> data) {
  Select();
  SendCommandHelper(cmd);
  SendData(data);
  Deselect();
}
void EPaperScreen::ClearDisplay() {
  const auto width_in_bytes = width_ / 8;
  SetWindow(0, 0, width_, height_);

  for (uint16_t row = 0; row < height_; row++) {
    SetCursor(0, row);
    SendCommand(Command::WriteRAM);
    Select();
    for (uint16_t column = 0; column < width_in_bytes; column++) {
      SendData(0x2A);
    }
    Deselect();
  }

  TurnOnDisplay();

  // for every row (height):
  // set cursor
  // send WriteRAM command
  // for every column (width / 8): send 0xFF

  // turnondisplay
}
void EPaperScreen::SetWindow(uint16_t x_start, uint16_t y_start, uint16_t x_end,
                             uint16_t y_end) {
  std::array x_data = {static_cast<uint8_t>((x_start >> 3) & 0xFF),
                       static_cast<uint8_t>((x_end >> 3) & 0xFF)};
  SendCommand(Command::SetRAMPosX, x_data);
  std::array y_data = {static_cast<uint8_t>(y_start & 0xFF),
                       static_cast<uint8_t>((y_start >> 8) & 0xFF),
                       static_cast<uint8_t>(y_end & 0xFF),
                       static_cast<uint8_t>((y_end >> 8) & 0xFF)};
  SendCommand(Command::SetRAMPosY, y_data);
}
void EPaperScreen::SetCursor(uint16_t x, uint16_t y) {
  SendCommand(Command::SetRAMCounterX, (x >> 3) & 0xFF);
  std::array y_data = {static_cast<uint8_t>(y & 0xFF),
                       static_cast<uint8_t>((y >> 8) & 0xFF)};
  SendCommand(Command::SetRAMCounterY, y_data);
}
void EPaperScreen::TurnOnDisplay() {
  SendCommand(Command::DisplayUpdate2, 0xC4);
  SendCommand(Command::MasterActivation);
  SendCommand(Command::NOP);

  // check until not busy anymore
}
void EPaperScreen::Select() {
  HAL_GPIO_WritePin(cs_gpio_, cs_pin_, GPIO_PIN_RESET);
}
void EPaperScreen::Deselect() {
  HAL_GPIO_WritePin(cs_gpio_, cs_pin_, GPIO_PIN_SET);
}
void EPaperScreen::Sleep() {
  SendCommand(Command::DeepSleepMode, 0x01);
}

}  // namespace emarker
