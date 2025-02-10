#include "esphome/core/log.h"
#include "khadafi_bms.h"
#include <vector>
#include <string>
#include <cstdio>

namespace esphome
{
    namespace khadafi_bms
    {

        static const char *TAG = "khadafi_bms";

        void KhadafiBMS::setup()
        {
            ESP_LOGD(TAG, "Setting up KhadafiBMS sensor");
            ESP_LOGI(TAG, "KhadafiBMS sensor setup complete");
        }

        void KhadafiBMS::update()
        {
            // // Create a dummy BatteryPack for testing
            // BatteryPack dummy_pack;

            // // Fill the dummy pack with some test data
            // dummy_pack.manufacturerCode = "TESTMANUFACTURER";
            // dummy_pack.softwareVersion = "10";
            // dummy_pack.numberOfCells = 15;
            // dummy_pack.type = 0;                       // lithium iron
            // dummy_pack.ratedCapacitymAh = 100000;      // 100Ah
            // dummy_pack.ratedCellmV = 3200;             // 3.2V
            // dummy_pack.maxPackVoltageLimit = 580;      // 58.0V
            // dummy_pack.minPackVoltageLimit = 400;      // 40.0V
            // dummy_pack.maxPackChargeCurrent = 500;     // 50.0A
            // dummy_pack.maxPackDischargeCurrent = -500; // -50.0A
            // dummy_pack.packVoltage = 512;              // 51.2V
            // dummy_pack.packCurrent = 100;              // 10.0A
            // dummy_pack.packSOC = 750;                  // 75.0%
            // dummy_pack.packSOH = 980;                  // 98.0%
            // dummy_pack.maxCellmV = 3300;               // 3.3V
            // dummy_pack.minCellmV = 3200;               // 3.2V
            // dummy_pack.tempMax = 350;                  // 35.0°C
            // dummy_pack.tempMin = 250;                  // 25.0°C
            // dummy_pack.chargeDischargeStatus = 1;      // charging
            // dummy_pack.bmsCycles = 100;                // 100 cycles

            // // // Generate some dummy cell voltages
            // for (int i = 0; i < dummy_pack.numberOfCells; i++)
            // {
            //     dummy_pack.cellVmV[i] = 3200 + (i * 10); // 3.2V to 3.35V
            // }

            // // // Create and send system info using the dummy pack
            // std::vector<uint8_t> systemInfo = createSystemInfo(dummy_pack);

            // // // Send the system info frame
            // this->uart_->write_array(systemInfo);

            // Log the sent data

            // for (uint8_t byte : systemInfo)
            // {
            //     ESP_LOGD(TAG, "0x%02X", byte);
            // }

            // Create battery information using the dummy pack
            // std::vector<uint8_t> batteryInfo = createBatteryInformation(dummy_pack);

            // // Send the battery information frame
            // this->uart_->write_array(batteryInfo);
            // ESP_LOGD(TAG, "Sent dummy system info frame:");

            // Create alarms
            // std::vector<uint8_t> alarms = createAlarms(dummy_pack);

            // // Send the alarms frame
            // this->uart_->write_array(alarms);
            // ESP_LOGD(TAG, "Sent alarms frame:");
            // // Log some of the dummy data for verification
            // ESP_LOGD(TAG, "Dummy BatteryPack data:");
            // ESP_LOGD(TAG, "  Manufacturer: %s", dummy_pack.manufacturerCode.c_str());
            // ESP_LOGD(TAG, "  Software Version: %s", dummy_pack.softwareVersion.c_str());
            // ESP_LOGD(TAG, "  Number of Cells: %d", dummy_pack.numberOfCells);
            // ESP_LOGD(TAG, "  Pack Voltage: %.1fV", dummy_pack.packVoltage / 10.0f);
            // ESP_LOGD(TAG, "  Pack Current: %.1fA", dummy_pack.packCurrent / 10.0f);
            // ESP_LOGD(TAG, "  Pack SOC: %.1f%%", dummy_pack.packSOC / 10.0f);
            // ESP_LOGD(TAG, "  Max Cell Voltage: %.3fV", dummy_pack.maxCellmV / 1000.0f);
            // ESP_LOGD(TAG, "  Min Cell Voltage: %.3fV", dummy_pack.minCellmV / 1000.0f);
            // ESP_LOGD(TAG, "  Max Temperature: %.1f°C", dummy_pack.tempMax / 10.0f);
            // ESP_LOGD(TAG, "  Min Temperature: %.1f°C", dummy_pack.tempMin / 10.0f);
        }
        std::vector<uint8_t> KhadafiBMS::createAlarms(const BatteryPack &pack)
        {
            std::array<uint8_t, 8> alarms;

            auto setBit = [](uint8_t &byte, int position, bool value)
            {
                if (value)
                    byte |= (1 << position);
                else
                    byte &= ~(1 << position);
            };

            auto convertAndAssign = [this](uint8_t value, uint8_t &alarm1, uint8_t &alarm2)
            {
                std::array<uint8_t, 2> bytes = this->convertByteToAsciiBytes(value);
                alarm1 = bytes[0];
                alarm2 = bytes[1];
            };

            // warning alarms 1
            uint8_t value = 0;
            setBit(value, 7, pack.getAlarmLevel(Alarm::PACK_VOLTAGE_HIGH) == AlarmLevel::WARNING);
            setBit(value, 6, pack.getAlarmLevel(Alarm::PACK_VOLTAGE_LOW) == AlarmLevel::WARNING);
            setBit(value, 5, pack.getAlarmLevel(Alarm::CELL_VOLTAGE_HIGH) == AlarmLevel::WARNING);
            setBit(value, 4, pack.getAlarmLevel(Alarm::CELL_VOLTAGE_LOW) == AlarmLevel::WARNING);
            setBit(value, 3, pack.getAlarmLevel(Alarm::CELL_TEMPERATURE_HIGH) == AlarmLevel::WARNING);
            setBit(value, 2, pack.getAlarmLevel(Alarm::CELL_TEMPERATURE_LOW) == AlarmLevel::WARNING);
            setBit(value, 1, false);
            setBit(value, 0, pack.getAlarmLevel(Alarm::CELL_VOLTAGE_DIFFERENCE_HIGH) == AlarmLevel::WARNING);
            convertAndAssign(value, alarms[0], alarms[1]);

            // warning alarms 2
            value = 0;
            setBit(value, 7, pack.getAlarmLevel(Alarm::TEMPERATURE_SENSOR_DIFFERENCE_HIGH) == AlarmLevel::WARNING);
            setBit(value, 6, pack.getAlarmLevel(Alarm::CHARGE_CURRENT_HIGH) == AlarmLevel::WARNING);
            setBit(value, 5, pack.getAlarmLevel(Alarm::DISCHARGE_CURRENT_HIGH) == AlarmLevel::WARNING);
            setBit(value, 4, pack.getAlarmLevel(Alarm::FAILURE_COMMUNICATION_INTERNAL) == AlarmLevel::WARNING);
            setBit(value, 3, false);
            setBit(value, 2, false);
            setBit(value, 1, false);
            setBit(value, 0, false);
            convertAndAssign(value, alarms[2], alarms[3]);

            // protection alarms 1
            value = 0;
            setBit(value, 7, pack.getAlarmLevel(Alarm::PACK_VOLTAGE_HIGH) == AlarmLevel::ALARM);
            setBit(value, 6, pack.getAlarmLevel(Alarm::PACK_VOLTAGE_LOW) == AlarmLevel::ALARM);
            setBit(value, 5, pack.getAlarmLevel(Alarm::CELL_VOLTAGE_HIGH) == AlarmLevel::ALARM);
            setBit(value, 4, pack.getAlarmLevel(Alarm::CELL_VOLTAGE_LOW) == AlarmLevel::ALARM);
            setBit(value, 3, pack.getAlarmLevel(Alarm::CELL_TEMPERATURE_HIGH) == AlarmLevel::ALARM);
            setBit(value, 2, pack.getAlarmLevel(Alarm::CELL_TEMPERATURE_LOW) == AlarmLevel::ALARM);
            setBit(value, 1, false);
            setBit(value, 0, false);
            convertAndAssign(value, alarms[4], alarms[5]);

            // protection alarms 2
            value = 0;
            setBit(value, 7, false);
            setBit(value, 6, pack.getAlarmLevel(Alarm::CHARGE_CURRENT_HIGH) == AlarmLevel::ALARM);
            setBit(value, 5, pack.getAlarmLevel(Alarm::DISCHARGE_CURRENT_HIGH) == AlarmLevel::ALARM);
            setBit(value, 4, false);
            setBit(value, 3, pack.getAlarmLevel(Alarm::FAILURE_OTHER) == AlarmLevel::ALARM);
            setBit(value, 2, false);
            setBit(value, 1, false);
            setBit(value, 0, false);
            convertAndAssign(value, alarms[6], alarms[7]);

            return prepareSendFrame(0x12, 0x46, 0x63, std::vector<uint8_t>(alarms.begin(), alarms.end()));
        }

        void KhadafiBMS::loop()
        {
            while (this->uart_->available())
            {
                uint8_t byte;
                if (this->uart_->read_byte(&byte))
                {
                    // Ignore leading 0xFF bytes
                    if (buffer_.empty() && byte == 0xFF)
                    {
                        continue;
                    }

                    buffer_.push_back(byte);

                    // Check for start of message
                    if (buffer_.size() == 1 && byte != 0x7E)
                    {
                        buffer_.clear(); // Clear buffer if it doesn't start with 0x7E
                        continue;
                    }

                    // Process message when end byte is received
                    if (byte == 0x0D)
                    {
                        process_message();
                        buffer_.clear();
                    }

                    // Safety check: clear buffer if it gets too large
                    if (buffer_.size() > 100)
                    { // Adjust this size based on your expected maximum message length
                        ESP_LOGW(TAG, "Buffer overflow, clearing");
                        buffer_.clear();
                    }
                }
            }
        }

        void KhadafiBMS::process_message()
        {
            if (buffer_.size() < 9)
            { // 18 characters + EOI
                ESP_LOGW(TAG, "Incomplete message received");
                return;
            }

            // Check if it's a complete message
            if (buffer_[0] == 0x7E && buffer_.back() == 0x0D)
            {
                ESP_LOGD(TAG, "Received complete Pylon message");

                // Log the message
                std::string hex_message;
                std::string ascii_message;
                for (uint8_t b : buffer_)
                {
                    char hex[3];
                    sprintf(hex, "%02X ", b);
                    hex_message += hex;

                    if (b >= 32 && b <= 126)
                    { // Printable ASCII range
                        ascii_message += static_cast<char>(b);
                    }
                    else
                    {
                        ascii_message += '.'; // Non-printable characters
                    }
                }
                ESP_LOGD(TAG, "  HEX: %s", hex_message.c_str());
                ESP_LOGD(TAG, "  ASCII: %s", ascii_message.c_str());

                // Process the message
                decode_pylon_message(buffer_);

                // Clear the buffer
                buffer_.clear();
            }
        }

        void KhadafiBMS::decode_pylon_message(const std::vector<uint8_t> &message)
        {
            if (message.size() < 9)
            { // 9 parts * 2 characters each
                ESP_LOGW(TAG, "Message too short");
                return;
            }

            auto hex_to_byte = [](char c1, char c2)
            {
                uint8_t value = 0;
                if (c1 >= '0' && c1 <= '9')
                    value = (c1 - '0') << 4;
                else if (c1 >= 'A' && c1 <= 'F')
                    value = (c1 - 'A' + 10) << 4;
                if (c2 >= '0' && c2 <= '9')
                    value |= (c2 - '0');
                else if (c2 >= 'A' && c2 <= 'F')
                    value |= (c2 - 'A' + 10);
                return value;
            };

            ESP_LOGI(TAG, "Pylon RS485 Message decode:");
            ESP_LOGI(TAG, "SOI: 0x%02X", message[0]);
            ESP_LOGI(TAG, "Version: 0x%02X", hex_to_byte(message[1], message[2]));
            ESP_LOGI(TAG, "Address: 0x%02X", hex_to_byte(message[3], message[4]));
            ESP_LOGI(TAG, "CID1: 0x%02X", hex_to_byte(message[5], message[6]));
            ESP_LOGI(TAG, "CID2: 0x%02X", hex_to_byte(message[7], message[8]));

            uint16_t length = (hex_to_byte(message[9], message[10]) << 8) | hex_to_byte(message[11], message[12]);
            uint16_t lenid = length & 0x0FFF;        // LENID is the lower 12 bits
            uint8_t lchksum = (length >> 12) & 0x0F; // LCHKSUM is the upper 4 bits

            ESP_LOGI(TAG, "LENGTH: 0x%04X (LENID: %d, LCHKSUM: %d)", length, lenid, lchksum);

            // Verify LCHKSUM
            uint8_t calculated_lchksum = 0;
            for (int i = 0; i < 3; i++)
            {
                calculated_lchksum += (lenid >> (i * 4)) & 0x0F;
            }
            calculated_lchksum = (~calculated_lchksum + 1) & 0x0F;

            if (calculated_lchksum != lchksum)
            {
                ESP_LOGW(TAG, "LCHKSUM mismatch! Calculated: %d, Received: %d", calculated_lchksum, lchksum);
                return;
            }

            // Extract INFO
            std::string info;
            for (size_t i = 13; i < message.size() - 5; i++)
            {
                info += static_cast<char>(message[i]);
            }
            ESP_LOGI(TAG, "INFO: %s", info.c_str());

            // Calculate checksum
            uint16_t calculated_checksum = 0;
            for (size_t i = 1; i < message.size() - 5; i++)
            {
                calculated_checksum += message[i];
            }
            calculated_checksum = (~calculated_checksum + 1) & 0xFFFF; // Bitwise invert and add 1

            uint16_t received_checksum = (hex_to_byte(message[message.size() - 5], message[message.size() - 4]) << 8) |
                                         hex_to_byte(message[message.size() - 3], message[message.size() - 2]);

            ESP_LOGI(TAG, "Checksum: 0x%04X (Calculated: 0x%04X)", received_checksum, calculated_checksum);
            ESP_LOGI(TAG, "EOI: 0x%02X", message.back());

            if (calculated_checksum != received_checksum)
            {
                ESP_LOGW(TAG, "Checksum mismatch!");
                return;
            }

            BatteryPack dummy_pack;

            // Fill the dummy pack with some test data
            dummy_pack.manufacturerCode = "TESTMANUFACTURER";
            dummy_pack.softwareVersion = "10";
            dummy_pack.numberOfCells = 15;
            dummy_pack.type = 0;                       // lithium iron
            dummy_pack.ratedCapacitymAh = 100000;      // 100Ah
            dummy_pack.ratedCellmV = 3200;             // 3.2V
            dummy_pack.maxPackVoltageLimit = 580;      // 58.0V
            dummy_pack.minPackVoltageLimit = 400;      // 40.0V
            dummy_pack.maxPackChargeCurrent = 500;     // 50.0A
            dummy_pack.maxPackDischargeCurrent = -500; // -50.0A
            dummy_pack.packVoltage = 512;              // 51.2V
            dummy_pack.packCurrent = 100;              // 10.0A
            dummy_pack.packSOC = 750;                  // 75.0%
            dummy_pack.packSOH = 980;                  // 98.0%
            dummy_pack.maxCellmV = 3300;               // 3.3V
            dummy_pack.minCellmV = 3200;               // 3.2V
            dummy_pack.tempMax = 350;                  // 35.0°C
            dummy_pack.tempMin = 250;                  // 25.0°C
            dummy_pack.chargeDischargeStatus = 1;      // charging
            dummy_pack.bmsCycles = 100;                // 100 cycles

            // // Generate some dummy cell voltages
            for (int i = 0; i < dummy_pack.numberOfCells; i++)
            {
                dummy_pack.cellVmV[i] = 3200 + (i * 10); // 3.2V to 3.35V
            }
            // Decode specific message types
            if (message[5] == '4' && message[6] == '6' && message[7] == '6' && message[8] == '1')
            {
                ESP_LOGI(TAG, "Battery System INfo values message received");
                std::vector<uint8_t> systemInfo = createSystemInfo(dummy_pack);

                // // Send the system info frame
                this->uart_->write_array(systemInfo);
                // Add decoding for analog values here
            }
            else if (message[5] == '4' && message[6] == '6' && message[7] == '6' && message[8] == '2')
            {
                ESP_LOGI(TAG, "Battery INformation to analog values request");
                std::vector<uint8_t> batteryInfo = createBatteryInformation(dummy_pack);

                // Send the battery information frame
                this->uart_->write_array(batteryInfo);
                ESP_LOGD(TAG, "Sent dummy system info frame:");
                // Add decoding for the response here
            }
            else if (message[5] == '4' && message[6] == '6' && message[7] == '6' && message[8] == '3')
            {
                ESP_LOGI(TAG, "Battery Alarm to analog values request");
                // Create alarms
                std::vector<uint8_t> alarms = createAlarms(dummy_pack);

                // // Send the alarms frame
                this->uart_->write_array(alarms);
                ESP_LOGD(TAG, "Sent alarms frame:");
                // Add decoding for the response here
            }
        }

        void KhadafiBMS::dump_config()
        {
            ESP_LOGCONFIG(TAG, "KhadafiBMS:");
            ESP_LOGCONFIG(TAG, "  Update interval: %ums", this->update_interval_);
            // Log any other relevant configuration details
        }
        std::vector<uint8_t> KhadafiBMS::createSystemInfo(const BatteryPack &aggregatedPack)
        {
            std::vector<uint8_t> buffer;
            buffer.reserve(4096); // Pre-allocate space

            auto appendAsciiBytes = [this, &buffer](const std::string &str, size_t length)
            {
                std::vector<uint8_t> asciiBytes = this->convertStringToAsciiBytes(str, length);
                buffer.insert(buffer.end(), asciiBytes.begin(), asciiBytes.end());
            };

            auto appendByteAsAscii = [this, &buffer](uint8_t byte)
            {
                std::array<uint8_t, 2> asciiBytes = this->convertByteToAsciiBytes(byte);
                buffer.push_back(asciiBytes[0]);
                buffer.push_back(asciiBytes[1]);
            };

            appendAsciiBytes("Battery", 10);
            appendAsciiBytes(aggregatedPack.manufacturerCode, 20);
            appendAsciiBytes(aggregatedPack.softwareVersion, 2);
            appendByteAsAscii(aggregatedPack.numberOfCells);

            for (int i = 0; i < aggregatedPack.numberOfCells; i++)
            {
                appendAsciiBytes("Battery S/N #" + std::to_string(i), 16);
            }

            return prepareSendFrame(0x12, 0x46, 0x61, buffer);
        }
        std::vector<uint8_t> KhadafiBMS::createBatteryInformation(const BatteryPack &aggregatedPack)
        {
            std::vector<uint8_t> buffer;
            buffer.reserve(4096);

            auto appendShortAsAscii = [this, &buffer](uint16_t value)
            {
                std::array<uint8_t, 4> asciiBytes = this->convertShortToAsciiBytes(value);
                buffer.insert(buffer.end(), asciiBytes.begin(), asciiBytes.end());
            };

            auto appendByteAsAscii = [this, &buffer](uint8_t value)
            {
                std::array<uint8_t, 2> asciiBytes = this->convertByteToAsciiBytes(value);
                buffer.insert(buffer.end(), asciiBytes.begin(), asciiBytes.end());
            };

            appendShortAsAscii(static_cast<uint16_t>(aggregatedPack.packVoltage * 100));
            appendShortAsAscii(static_cast<uint16_t>(aggregatedPack.packCurrent * 10));
            appendByteAsAscii(static_cast<uint8_t>(aggregatedPack.packSOC / 10));
            appendShortAsAscii(static_cast<uint16_t>(aggregatedPack.bmsCycles));
            appendShortAsAscii(10000);                                            // maximum cycles
            appendByteAsAscii(static_cast<uint8_t>(aggregatedPack.packSOH / 10)); // average SOH
            appendByteAsAscii(static_cast<uint8_t>(aggregatedPack.packSOH / 10)); // lowest SOH
            int maxPack = 0;
            int minPack = 0;
            appendShortAsAscii(static_cast<uint16_t>(aggregatedPack.maxCellmV));
            appendByteAsAscii(static_cast<uint8_t>(maxPack));                    // battery pack with highest voltage
            appendByteAsAscii(static_cast<uint8_t>(aggregatedPack.maxCellVNum)); // cell with highest voltage

            appendShortAsAscii(static_cast<uint16_t>(aggregatedPack.minCellmV));
            appendByteAsAscii(static_cast<uint8_t>(minPack));                    // battery pack with lowest voltage
            appendByteAsAscii(static_cast<uint8_t>(aggregatedPack.minCellVNum)); // cell with lowest voltage

            appendShortAsAscii(static_cast<uint16_t>(aggregatedPack.tempAverage + 2731));

            maxPack = 0;
            minPack = 0;

            appendShortAsAscii(static_cast<uint16_t>(aggregatedPack.tempMax + 2731));
            appendByteAsAscii(static_cast<uint8_t>(maxPack));
            appendByteAsAscii(static_cast<uint8_t>(aggregatedPack.tempMaxCellNum));

            appendShortAsAscii(static_cast<uint16_t>(aggregatedPack.tempMin + 2731));
            appendByteAsAscii(static_cast<uint8_t>(minPack));
            appendByteAsAscii(static_cast<uint8_t>(aggregatedPack.tempMinCellNum));

            // MOSFET average temperature
            appendShortAsAscii(static_cast<uint16_t>(aggregatedPack.tempAverage + 2731));
            // MOSFET max temperature
            appendShortAsAscii(static_cast<uint16_t>(aggregatedPack.tempAverage + 2731));
            // MOSFET max temperature pack
            appendShortAsAscii(0);
            // MOSFET min temperature
            appendShortAsAscii(static_cast<uint16_t>(aggregatedPack.tempAverage + 2731));
            // MOSFET min temperature pack
            appendShortAsAscii(0);

            // BMS average temperature
            appendShortAsAscii(static_cast<uint16_t>(aggregatedPack.tempAverage + 2731));
            // BMS max temperature
            appendShortAsAscii(static_cast<uint16_t>(aggregatedPack.tempAverage + 2731));
            // BMS max temperature pack
            appendShortAsAscii(0);
            // BMS min temperature
            appendShortAsAscii(static_cast<uint16_t>(aggregatedPack.tempAverage + 2731));
            // BMS min temperature pack
            appendShortAsAscii(0);

            return prepareSendFrame(0x12, 0x46, 0x62, buffer);
        }

        std::vector<uint8_t> KhadafiBMS::createChecksum(const std::vector<uint8_t> &sendFrame)
        {
            uint32_t checksum = 0;

            // add all values except SOI, checksum and EOI
            for (size_t i = 1; i < sendFrame.size() - 5; i++)
            {
                checksum += sendFrame[i];
            }

            // modulo remainder of 65535
            checksum %= 65535;

            // invert
            checksum = ~checksum;
            // add 1
            checksum++;

            // extract the high and low bytes
            uint8_t high = (checksum >> 8) & 0xFF;
            uint8_t low = checksum & 0xFF;

            // convert them to ascii
            std::vector<uint8_t> data(4);
            sprintf(reinterpret_cast<char *>(data.data()), "%02X%02X", high, low);

            return data;
        }

        std::vector<uint8_t> KhadafiBMS::createLengthCheckSum(uint16_t length)
        {
            // split the first 12 bits into groups of 4 bits and accumulate
            uint8_t chksum = (length & 0x0F) + ((length >> 4) & 0x0F) + ((length >> 8) & 0x0F);

            // modulo 16 remainder
            chksum %= 16;
            // invert
            chksum = ~chksum & 0x0F;
            // and finally +1
            chksum++;

            // combine the checksum and length
            uint16_t dataValue = (chksum << 12) | (length & 0x0FFF);

            // convert to ASCII
            std::vector<uint8_t> data(4);
            sprintf(reinterpret_cast<char *>(data.data()), "%04X", dataValue);

            return data;
        }

        std::vector<uint8_t> KhadafiBMS::prepareSendFrame(uint8_t address, uint8_t cid1, uint8_t cid2, const std::vector<uint8_t> &data)
        {
            std::vector<uint8_t> sendFrame;
            sendFrame.reserve(18 + data.size()); // Pre-allocate space

            auto appendByteAsAscii = [this, &sendFrame](uint8_t byte)
            {
                std::array<uint8_t, 2> asciiBytes = this->convertByteToAsciiBytes(byte);
                sendFrame.push_back(asciiBytes[0]);
                sendFrame.push_back(asciiBytes[1]);
            };

            sendFrame.push_back(0x7E);  // Start flag
            sendFrame.push_back(0x32);  // version
            sendFrame.push_back(0x30);  // version
            appendByteAsAscii(address); // address
            appendByteAsAscii(cid1);    // command CID1
            appendByteAsAscii(cid2);    // command CID2

            // Frame Length Byte
            auto lengthChecksum = createLengthCheckSum(data.size());
            sendFrame.insert(sendFrame.end(), lengthChecksum.begin(), lengthChecksum.end());

            // data
            sendFrame.insert(sendFrame.end(), data.begin(), data.end());

            // checksum
            auto checksum = createChecksum(sendFrame);
            sendFrame.insert(sendFrame.end(), checksum.begin(), checksum.end());

            sendFrame.push_back(0x0D); // End flag

            return sendFrame;
        }

    } // namespace khadafi_bms
} // namespace esphome