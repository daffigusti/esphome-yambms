#pragma once

#include "esphome/core/component.h"
#include "esphome/components/sensor/sensor.h"
#include "esphome/components/uart/uart.h"
#include "battery_pack.h"

namespace esphome
{
  namespace khadafi_bms
  {

    class KhadafiBMS : public sensor::Sensor, public PollingComponent, public uart::UARTDevice
    {
    public:
      KhadafiBMS() : PollingComponent(2000) {} // Poll every 1000ms
      void set_uart(uart::UARTComponent *uart) { uart_ = uart; }
      void set_update_interval(uint32_t update_interval) { update_interval_ = update_interval; }

      void setup() override;
      void update() override;
      void loop() override;
      void dump_config() override;
      std::vector<uint8_t> createSystemInfo(const BatteryPack &aggregatedPack);

    protected:
      uart::UARTComponent *uart_;
      uint32_t update_interval_;

    private:
      BatteryPack battery_pack_;
      std::vector<uint8_t> buffer_;
      void process_message();
      void decode_pylon_message(const std::vector<uint8_t> &message);
      std::vector<uint8_t> prepareSendFrame(uint8_t address, uint8_t cid1, uint8_t cid2, const std::vector<uint8_t> &data);
      std::vector<uint8_t> createChecksum(const std::vector<uint8_t> &sendFrame);
      std::vector<uint8_t> createLengthCheckSum(uint16_t length);
      // You'll need to implement this or replace it with appropriate logic
      std::vector<uint8_t> createBatteryInformation(const BatteryPack &aggregatedPack);
      std::vector<uint8_t> createAlarms(const BatteryPack &pack);
      std::array<uint8_t, 2> convertByteToAsciiBytes(uint8_t value)
      {
        std::array<uint8_t, 2> bytes;
        char ascii[3];
        sprintf(ascii, "%02X", value);
        bytes[0] = ascii[0];
        bytes[1] = ascii[1];
        return bytes;
      }
      std::vector<uint8_t> convertStringToAsciiBytes(const std::string &value, size_t noOfCharacters)
      {
        // create byte vector (2 bytes per ascii char representation)
        std::vector<uint8_t> bytes(noOfCharacters * 2);
        size_t byteIdx = 0;
        size_t charIdx = 0;

        while (byteIdx < bytes.size())
        {
          // check if there are enough characters in the string
          if (charIdx < value.length())
          {
            // translate the next character to ascii bytes
            std::array<uint8_t, 2> hex = convertByteToAsciiBytes(static_cast<uint8_t>(value[charIdx++]));
            bytes[byteIdx++] = hex[0];
            bytes[byteIdx++] = hex[1];
          }
          else
          {
            // otherwise fill it up with ascii bytes 0x30 ('0')
            bytes[byteIdx++] = 0x30;
            bytes[byteIdx++] = 0x30;
          }
        }

        return bytes;
      }
      std::array<uint8_t, 4> convertShortToAsciiBytes(uint16_t value)
      {
        uint8_t first = (value & 0xFF00) >> 8;
        uint8_t second = value & 0x00FF;

        std::array<uint8_t, 4> data;
        std::array<uint8_t, 2> firstBytes = convertByteToAsciiBytes(first);
        std::array<uint8_t, 2> secondBytes = convertByteToAsciiBytes(second);

        data[0] = firstBytes[0];
        data[1] = firstBytes[1];
        data[2] = secondBytes[0];
        data[3] = secondBytes[1];

        return data;
      }
    };
  } // namespace khadafi_bms
} // namespace esphome