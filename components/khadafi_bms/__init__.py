import esphome.codegen as cg
import esphome.config_validation as cv
from esphome.components import uart
from esphome.const import CONF_ID, CONF_UPDATE_INTERVAL

DEPENDENCIES = ["uart"]
AUTO_LOAD = ["sensor"]

khadafi_bms_ns = cg.esphome_ns.namespace("khadafi_bms")
KhadafiBMS = khadafi_bms_ns.class_("KhadafiBMS", cg.PollingComponent, uart.UARTDevice)

CONF_KHADAFI_BMS_ID = "khadafi_bms_id"

CONFIG_SCHEMA = cv.Schema(
    {
        cv.GenerateID(): cv.declare_id(KhadafiBMS),
        cv.Required(CONF_UPDATE_INTERVAL): cv.update_interval,
        cv.Required(uart.CONF_UART_ID): cv.use_id(uart.UARTComponent),
    }
).extend(cv.COMPONENT_SCHEMA)


async def to_code(config):
    var = cg.new_Pvariable(config[CONF_ID])
    await cg.register_component(var, config)
    uart_component = await cg.get_variable(config[uart.CONF_UART_ID])
    cg.add(var.set_uart(uart_component))

    cg.add(var.set_update_interval(config[CONF_UPDATE_INTERVAL]))
