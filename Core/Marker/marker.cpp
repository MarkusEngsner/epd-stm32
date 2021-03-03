#include "marker.h"

namespace emarker {

template <unsigned int width_, unsigned int height_>
void EPaperScreen<width_, height_>::HardReset() {
  printf("Performing e-ink hard reset...\r\n");
  HAL_GPIO_WritePin(rst_gpio_, rst_pin_, GPIO_PIN_SET);
  HAL_Delay(200);
  HAL_GPIO_WritePin(rst_gpio_, rst_pin_, GPIO_PIN_RESET);
  HAL_Delay(2);
  HAL_GPIO_WritePin(rst_gpio_, rst_pin_, GPIO_PIN_SET);
  HAL_Delay(200);
}

template <unsigned int width_, unsigned int height_>
void EPaperScreen<width_, height_>::SendCommand(Command cmd) {
  /* TODO: use RAII (struct with destructor and constructor) to handle pulling
      cs pin high/low */
  Select();
  SendCommandHelper(cmd);
  Deselect();
}

template <unsigned int width_, unsigned int height_>
void EPaperScreen<width_, height_>::SetTransmissionMode(
    EPaperScreen::TransmissionMode mode) {
  auto newPinState =
      (mode == TransmissionMode::Data) ? GPIO_PIN_SET : GPIO_PIN_RESET;
  HAL_GPIO_WritePin(dc_gpio_, dc_pin_, newPinState);
}

template <unsigned int width_, unsigned int height_>
void EPaperScreen<width_, height_>::ConfigureLUT(
    EPaperScreen::UpdateMode mode) {
  // TODO: some kind of check to now if chipselect is needed
  // TODO: make table be reference, currently not possible due to
  // HAL_SPI_Transmit
  auto table = (mode == UpdateMode::Full) ? kLutFullUpdate : kLutPartialUpdate;
  SendCommand(Command::WriteLUT);
  Select();
  SetTransmissionMode(TransmissionMode::Data);
  HAL_SPI_Transmit(hspi_, table.data(), table.size(), 1000);
  Deselect();
}

template <unsigned int width_, unsigned int height_>
void EPaperScreen<width_, height_>::SendCommand(Command cmd, uint8_t data) {
  Select();
  SendCommandHelper(cmd);
  SendData(data);
  Deselect();
}

template <unsigned int width_, unsigned int height_>
void EPaperScreen<width_, height_>::SendCommandHelper(Command cmd) {
  SetTransmissionMode(TransmissionMode::Command);
  auto byte = static_cast<uint8_t>(cmd);
  HAL_SPI_Transmit(hspi_, &byte, 1, 1000);
}

template <unsigned int width_, unsigned int height_>
void EPaperScreen<width_, height_>::InitializeDisplay() {
  printf("Initializing display...\r\n");
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
  printf("Initialization done.\r\n");
}

template <unsigned int width_, unsigned int height_>
void EPaperScreen<width_, height_>::SendData(uint8_t data) {
  SetTransmissionMode(TransmissionMode::Data);
  Select();
  HAL_SPI_Transmit(hspi_, &data, 1, 1000);
  Deselect();
}

template <unsigned int width_, unsigned int height_>
template <size_t N>
void EPaperScreen<width_, height_>::SendData(std::array<uint8_t, N> data) {
  SetTransmissionMode(TransmissionMode::Data);
  Select();
  HAL_SPI_Transmit(hspi_, data.data(), data.size(), 1000);
  Deselect();
}

template <unsigned int width_, unsigned int height_>
template <size_t N>
void EPaperScreen<width_, height_>::SendCommand(Command cmd,
                                                std::array<uint8_t, N> data) {
  Select();
  SendCommandHelper(cmd);
  SendData(data);
  Deselect();
}

template <unsigned int width_, unsigned int height_>
void EPaperScreen<width_, height_>::ClearDisplay() {
  FillDisplay(0xFF);
}

template <unsigned int width_, unsigned int height_>
void EPaperScreen<width_, height_>::FillDisplay(uint8_t pattern) {
  constexpr auto width_in_bytes = width_ / 8;
  printf("FillDisplay with 0x%X\r\n", pattern);
  //  constexpr auto width_in_bytes = (width_ % 8 == 0) ? width_ / 8 : width_ /
  //  8 + 1;
  SetWindow(0, 0, width_ - 8, height_ - 1);
  std::array<uint8_t, width_in_bytes * height_> data{};
  std::fill(data.begin(), data.end(), pattern);
  SetCursor(0, 0);
  SendCommand(Command::WriteRAM);
  Select();
  SetTransmissionMode(TransmissionMode::Data);
  HAL_SPI_Transmit(hspi_, data.data(), data.size(), 1000);
  Deselect();
  TurnOnDisplay();
  printf("FillDisplay() done.\r\n");
}

template <unsigned int width_, unsigned int height_>
void EPaperScreen<width_, height_>::PrintFull(
    paintbrush::Canvas<width_, height_> canvas) {
  // Perhaps just send in array of bytes to keep interfaces decoupled
  constexpr auto width_in_bytes = width_ >> 3;
  // Due to indices starting at 0, x_end and y_end have to be reduced by 1 byte
  SetWindow(0, 0, width_ - 8, height_ - 1);
  SetCursor(0, 0);
  // TODO: turn into a single function call instead (perhaps send command with
  // byte buffer?)
  SendCommand(Command::WriteRAM);
  Select();
  SetTransmissionMode(TransmissionMode::Data);
  HAL_SPI_Transmit(hspi_, canvas.RawData(), width_in_bytes * height_, 1000);
  Deselect();

  TurnOnDisplay();
  printf("PrintFull() done.\r\n");
}

template <unsigned int width_, unsigned int height_>
void EPaperScreen<width_, height_>::SetWindow(uint16_t x_start,
                                              uint16_t y_start, uint16_t x_end,
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
template <unsigned int width_, unsigned int height_>
void EPaperScreen<width_, height_>::SetCursor(uint16_t x, uint16_t y) {
  SendCommand(Command::SetRAMCounterX, (x >> 3) & 0xFF);
  std::array y_data = {static_cast<uint8_t>(y & 0xFF),
                       static_cast<uint8_t>((y >> 8) & 0xFF)};
  SendCommand(Command::SetRAMCounterY, y_data);
}
template <unsigned int width_, unsigned int height_>
void EPaperScreen<width_, height_>::TurnOnDisplay() {
  SendCommand(Command::DisplayUpdate2, 0xC4);
  SendCommand(Command::MasterActivation);
  SendCommand(Command::NOP);

  // check until not busy anymore
  // this can take up to 1.5s for a full refresh, so it is very important!
  // should be implemented in a more elegant,
  //  non-blocking way: perhaps through some action queue?
  WaitUntilNotBusy();
}
template <unsigned int width_, unsigned int height_>
void EPaperScreen<width_, height_>::Select() {
  HAL_GPIO_WritePin(cs_gpio_, cs_pin_, GPIO_PIN_RESET);
}
template <unsigned int width_, unsigned int height_>
void EPaperScreen<width_, height_>::Deselect() {
  HAL_GPIO_WritePin(cs_gpio_, cs_pin_, GPIO_PIN_SET);
}
template <unsigned int width_, unsigned int height_>
void EPaperScreen<width_, height_>::Sleep() {
  SendCommand(Command::DeepSleepMode, 0x01);
}
template <unsigned int width_, unsigned int height_>
void EPaperScreen<width_, height_>::WakeUp() {
  // It seems that deep sleep is truly deep:
  // The only way to wake it up is by performing a hard-reset
  // Looking at 3.1 Normal Operation Flow, this seems to be the case
  HardReset();
}
template <unsigned int width_, unsigned int height_>
void EPaperScreen<width_, height_>::WaitUntilNotBusy() {
  while (HAL_GPIO_ReadPin(busy_gpio_, busy_pin_) == GPIO_PIN_SET) {
    HAL_Delay(100);
  }
}

}  // namespace emarker
