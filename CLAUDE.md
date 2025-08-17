# CLAUDE.md

This file provides guidance to Claude Code (claude.ai/code) when working with code in this repository.

## Project Overview

YamBMS (Yet another multi-BMS Merging Solution) is an ESPHome-based application that monitors Battery Management Systems (BMS) and communicates with inverters via CAN bus protocol. It supports multiple BMS models (JK, JBD, Seplos, etc.) and can merge data from multiple BMS units into a single interface for inverters.

## Architecture

### Core Components
- **ESPHome Configuration**: YAML-based configuration files for different board types and BMS models
- **Packages System**: Modular YAML packages for boards, BMS types, shunts, and communication protocols
- **Custom Components**: C++ components in `components/` directory for specialized BMS communication
- **Communication Protocols**: CAN bus (PYLON/SMA/Victron/LuxPower) and RS485 Modbus

### Directory Structure
- `packages/`: Modular YAML packages for boards, BMS, shunts, and YamBMS core
  - `board/`: Board-specific configurations (ESP32, ESP32-S3, etc.)
  - `bms/`: BMS-specific packages for different manufacturers
  - `shunt/`: Shunt sensor packages (Victron SmartShunt, Junctek KH-F)
  - `yambms/`: Core YamBMS functionality and communication protocols
- `examples/`: Example configurations for single-node and multi-node setups
- `components/`: Custom ESPHome components (currently contains khadafi_bms)
- `documents/`: Comprehensive documentation including setup guides and protocols

### Configuration Types
- **Local Packages (LP)**: Uses local `packages/` directory (e.g., `YamBMS_LP_*.yaml`)
- **Remote Packages (RP)**: Downloads packages from GitHub (e.g., `YamBMS_RP_*.yaml`)
- **Template**: `YamBMS_Remote_Packages_example.yaml` serves as the base template

## Development Commands

### ESPHome Commands
```bash
# Compile configuration
esphome compile <config_file>.yaml

# Upload to device
esphome upload <config_file>.yaml

# View logs
esphome logs <config_file>.yaml

# Clean build files
esphome clean <config_file>.yaml
```

### Testing
- No specific test framework is included
- Testing is typically done with hardware BMS units and inverters
- Demo firmware available: `firmware/YamBMS_DEMO_ESP32.factory.bin`

## Key Configuration Parameters

### YamBMS Core Settings (in substitutions section)
- `yambms_battery_chemistry`: '1' (LFP), '2' (Li-ion), '3' (LTO)
- `yambms_cell_count`: Number of cells in series (typically 16 for 51.2V systems)
- `yambms_bulk_v`: Bulk/Absorption voltage (e.g., '55.2' for LFP 16S)
- `yambms_float_v`: Float voltage (e.g., '53.6' for LFP 16S)
- `yambms_max_requested_charge_current`: Maximum charge current in Amperes
- `yambms_max_requested_discharge_current`: Maximum discharge current in Amperes

### Required Files
- `secrets.yaml`: WiFi credentials and other sensitive configuration
- Board selection: Uncomment exactly one board in packages section
- BMS selection: Import at least one BMS package with sequential `bms_id` starting from 1

## Communication Protocols

### CAN Bus Support
- **PYLON**: Standard protocol used by many inverters
- **SMA**: SMA-specific protocol
- **Victron**: Victron Energy protocol  
- **LuxPower**: LuxPower/EG4 protocol

### Supported BMS Types
- **JK BMS**: BLE, UART, RS485 Modbus variants (JK-B, JK-PB)
- **JBD BMS**: BLE and UART communication
- **Seplos BMS**: V1/V2 and V3 RS485 variants
- **Basen BMS**: RS485 communication
- **DEYE CAN**: Direct CAN module communication

### Multi-Node Architecture
- **Single-node**: One ESP32 handles multiple BMS (max 3 BLE, multiple RS485)
- **Multi-node**: Multiple ESP32s connected via RS485 Modbus network
- **Modbus roles**: Client (main YamBMS) and Server (individual BMS nodes)

## Important Notes

### ESPHome Version Requirements
- Minimum ESPHome version: 2025.6.0
- Some features require specific ESPHome versions (check changelog)

### Hardware Considerations
- ESP32 with ≥8MB flash recommended for multiple BMS monitoring
- CAN transceiver required for inverter communication (TJA1050/TJA1051/SN65HVD230)
- RS485 transceivers for multi-node setups
- Optional: 48V to 5V DC-DC converter for BMS-powered operation

### Critical Configuration
- BMS voltage calibration is essential for proper YamBMS operation
- `bms_id` must be numbered sequentially starting from 1
- Only one board package should be uncommented
- Battery chemistry and cell count must match physical battery configuration

### Development Workflow
1. Start with `YamBMS_Remote_Packages_example.yaml` as template
2. Configure board type and interfaces in packages section
3. Add BMS and optional shunt packages
4. Set battery parameters in substitutions
5. Compile and upload via ESPHome
6. Monitor via ESPHome logs and Home Assistant integration