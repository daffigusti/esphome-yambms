#pragma once

#include <map>
#include <vector>
#include <string>
#include <algorithm>

namespace esphome
{
    namespace khadafi_bms
    {

        enum class Alarm
        {
            PACK_VOLTAGE_HIGH,
            PACK_VOLTAGE_LOW,
            CELL_VOLTAGE_HIGH,
            CELL_VOLTAGE_LOW,
            CELL_TEMPERATURE_HIGH,
            CELL_TEMPERATURE_LOW,
            CELL_VOLTAGE_DIFFERENCE_HIGH,
            TEMPERATURE_SENSOR_DIFFERENCE_HIGH,
            CHARGE_CURRENT_HIGH,
            DISCHARGE_CURRENT_HIGH,
            FAILURE_COMMUNICATION_INTERNAL,
            FAILURE_OTHER
            // ... any other alarms ...
        };

        enum class AlarmLevel
        {
            NONE,
            WARNING,
            ALARM
        };
        struct BatteryPack
        {
            std::map<Alarm, AlarmLevel> alarms;
            int type;             // 0=lithium iron, 1=ternary lithium, 2=lithium titanate
            int ratedCapacitymAh; // Capacity of each cell (1mAh)
            int ratedCellmV;      // Nominal cell voltage (1mV)

            int maxPackVoltageLimit; // Maximum total voltage (0.1V)
            int minPackVoltageLimit; // Minimum total voltage (0.1V)

            int maxPackChargeCurrent;    // Maximum total charge current (0.1A)
            int maxPackDischargeCurrent; // Maximum total discharge current (-0.1A)
            int maxChargeVoltage;        // Maximum total charge voltage (0.1V)

            int packVoltage;  // Total pack voltage (0.1 V)
            int packCurrent;  // Current in (+) or out (-) of pack (0.1 A)
            int packSOC = -1; // State Of Charge (0.1%)
            int packSOH;      // State of Health (0.1%)

            int maxCellVoltageLimit; // Maximum cell voltage limit (1mV)
            int minCellVoltageLimit; // Minimum cell voltage limit (1mV)
            int maxCellmV;           // Maximum cell voltage (1mV)
            int maxCellVNum;         // Number of cell with highest voltage
            int minCellmV;           // Minimum cell voltage (1mV)
            int minCellVNum;         // Number of cell with lowest voltage
            int cellDiffmV;          // Difference between min and max cell voltages

            int tempMax;     // Maximum temperature sensor reading (0.1C)
            int tempMin;     // Minimum temperature sensor reading (0.1C)
            int tempAverage; // Average of temp sensors (0.1C)

            int chargeDischargeStatus = 0; // 0 idle/stationary, 1 charge, 2 discharge; 3 sleep
            bool chargeMOSState;           // charging MOSFET status
            bool dischargeMOSState;        // discharge MOSFET state
            bool forceCharge;              // force charging

            int bmsHeartBeat;         // BMS life (0~255 cycles)?
            int remainingCapacitymAh; // residual capacity mAH

            int numberOfCells;                            // Cell count
            int numOfTempSensors;                         // Temp sensor count
            bool chargerState;                            // charger status 0 = disconnected 1 = connected
            bool loadState;                               // Load Status 0=disconnected 1=connected
            std::vector<bool> dIO = std::vector<bool>(8); // No information about this
            int bmsCycles;                                // charge / discharge cycles

            std::vector<int> cellVmV = std::vector<int>(1024);            // Store Cell Voltages (mV)
            std::vector<int> cellTemperature = std::vector<int>(1024);    // array of cell Temperature sensors
            std::vector<bool> cellBalanceState = std::vector<bool>(1024); // boolean array of cell balance states
            bool cellBalanceActive;                                       // boolean is cell balance active

            std::string manufacturerCode; // The manufacturer code
            std::string hardwareVersion;  // The hardware version
            std::string softwareVersion;  // The software version

            int tempMaxCellNum; // The cell with the maximum temperature
            int tempMinCellNum; // The cell with the minimum temperature

            int maxModulemV;             // The maximum module voltage (0.001V) of a pack
            int minModulemV;             // The minimum module voltage (0.001V) of a pack
            int maxModulemVNum;          // The number of the pack with the maximum voltage
            int minModulemVNum;          // The number of the pack with the minimum voltage
            int maxModuleTemp;           // The maximum module temperature (0.1C)
            int minModuleTemp;           // The minimum module temperature (0.1C)
            int maxModuleTempNum;        // The pack number with maximum module temperature
            int minModuleTempNum;        // The pack number with minimum module temperature
            int modulesInSeries;         // The number of battery modules in series
            uint8_t moduleNumberOfCells; // The number of cells in a module
            int moduleVoltage;           // The module voltage (1V)
            int moduleRatedCapacityAh;   // The rated capacity of the module (1Ah)

            std::map<Alarm, AlarmLevel> getAlarms(const std::vector<AlarmLevel> &levels) const
            {
                std::map<Alarm, AlarmLevel> result;
                for (const auto &[alarm, level] : alarms)
                {
                    if (std::find(levels.begin(), levels.end(), level) != levels.end())
                    {
                        result[alarm] = level;
                    }
                }
                return result;
            }

            void setAlarm(Alarm alarm, AlarmLevel level)
            {
                alarms[alarm] = level;
            }

            AlarmLevel getAlarmLevel(Alarm alarm) const
            {
                auto it = alarms.find(alarm);
                return (it != alarms.end()) ? it->second : AlarmLevel::NONE;
            }
        };

    } // namespace khadafi_bms
} // namespace esphome
