#ifndef CPPFROMBLINKY_MARKER_H
#define CPPFROMBLINKY_MARKER_H

#include <array>

#include "commands.h"
#include "stm32f4xx_hal.h"

namespace emarker {

// TODO: turn into template
// template class <width, height> or whatever
class EPaperScreen {
 public:
  EPaperScreen(unsigned int width, unsigned int height, SPI_HandleTypeDef *hspi,
               GPIO_TypeDef *cs_gpio, uint16_t cs_pin, GPIO_TypeDef *dc_gpio,
               uint16_t dc_pin, GPIO_TypeDef *rst_gpio, uint16_t rst_pin,
               GPIO_TypeDef *busy_gpio, uint16_t busy_pin)
      : width_(width),
        height_(height),
        hspi_(hspi),
        cs_gpio_(cs_gpio),
        cs_pin_(cs_pin),
        dc_gpio_(dc_gpio),
        dc_pin_(dc_pin),
        rst_gpio_(rst_gpio),
        rst_pin_(rst_pin),
        busy_gpio_(busy_gpio),
        busy_pin_(busy_pin) {}

  void InitializeDisplay();

  void ClearDisplay();
  void FillDisplay(uint8_t pattern);

  void Sleep();
  void WakeUp();

  void WaitUntilNotBusy();

 private:
  void HardReset();

  void SetWindow(uint16_t x_start, uint16_t y_start, uint16_t x_end, uint16_t y_end);

  void SetCursor(uint16_t x, uint16_t y);

  void TurnOnDisplay();

  void SendCommandHelper(Command cmd);
  void SendCommand(Command cmd);
  void SendCommand(Command cmd, uint8_t data);
  template<size_t N>
  void SendCommand(Command cmd, std::array<uint8_t, N> data);

  void SendData(uint8_t datum);


  template<size_t N>
  void SendData(std::array<uint8_t, N> data);

  void Select();
  void Deselect();

  enum class TransmissionMode { Data, Command };

  void SetTransmissionMode(TransmissionMode mode);

  static constexpr std::array<uint8_t, 30> kLutFullUpdate{
      0x50, 0xAA, 0x55, 0xAA, 0x11, 0x00, 0x00, 0x00, 0x00, 0x00,
      0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
      0xFF, 0xFF, 0x1F, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00};

  static constexpr std::array<uint8_t, 30> kLutPartialUpdate{
      0x10, 0x18, 0x18, 0x08, 0x18, 0x18, 0x08, 0x00, 0x00, 0x00,
      0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
      0x13, 0x14, 0x44, 0x12, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00};


  enum class UpdateMode { Full, Partial };
  void ConfigureLUT(UpdateMode mode);

  const unsigned int width_;
  const unsigned int height_;

  SPI_HandleTypeDef *hspi_;

  GPIO_TypeDef *cs_gpio_;  // communication is only possible when LOW
  uint16_t cs_pin_;

  GPIO_TypeDef *dc_gpio_;
  uint16_t dc_pin_;

  GPIO_TypeDef *rst_gpio_;
  uint16_t rst_pin_;

  GPIO_TypeDef *busy_gpio_;
  uint16_t busy_pin_;
};

}  // namespace emarker

#endif  // CPPFROMBLINKY_MARKER_H
