
 PARAMETER VERSION = 2.2.0


BEGIN OS
 PARAMETER OS_NAME = standalone
 PARAMETER OS_VER = 6.0
 PARAMETER PROC_INSTANCE = sensor_actor_sensor_actor_mb
 PARAMETER stdin = sensor_actor_axi_uartlite_remote_control
 PARAMETER stdout = sensor_actor_axi_uartlite_remote_control
END


BEGIN PROCESSOR
 PARAMETER DRIVER_NAME = cpu
 PARAMETER DRIVER_VER = 2.5
 PARAMETER HW_INSTANCE = sensor_actor_sensor_actor_mb
END


BEGIN DRIVER
 PARAMETER DRIVER_NAME = uartlite
 PARAMETER DRIVER_VER = 3.2
 PARAMETER HW_INSTANCE = mdm_copter
END

BEGIN DRIVER
 PARAMETER DRIVER_NAME = bram
 PARAMETER DRIVER_VER = 4.1
 PARAMETER HW_INSTANCE = sensor_actor_axi_bram_ctrl_sa
END

BEGIN DRIVER
 PARAMETER DRIVER_NAME = gpio
 PARAMETER DRIVER_VER = 4.2
 PARAMETER HW_INSTANCE = sensor_actor_axi_gpio_debug_leds_sa
END

BEGIN DRIVER
 PARAMETER DRIVER_NAME = iic
 PARAMETER DRIVER_VER = 3.3
 PARAMETER HW_INSTANCE = sensor_actor_axi_iic_bu
END

BEGIN DRIVER
 PARAMETER DRIVER_NAME = iic
 PARAMETER DRIVER_VER = 3.3
 PARAMETER HW_INSTANCE = sensor_actor_axi_iic_imu
END

BEGIN DRIVER
 PARAMETER DRIVER_NAME = iic
 PARAMETER DRIVER_VER = 3.3
 PARAMETER HW_INSTANCE = sensor_actor_axi_iic_motors
END

BEGIN DRIVER
 PARAMETER DRIVER_NAME = intc
 PARAMETER DRIVER_VER = 3.5
 PARAMETER HW_INSTANCE = sensor_actor_axi_intc_sa
END

BEGIN DRIVER
 PARAMETER DRIVER_NAME = spi
 PARAMETER DRIVER_VER = 4.2
 PARAMETER HW_INSTANCE = sensor_actor_axi_spi_battery_guard
END

BEGIN DRIVER
 PARAMETER DRIVER_NAME = tmrctr
 PARAMETER DRIVER_VER = 4.1
 PARAMETER HW_INSTANCE = sensor_actor_axi_timer_sa
END

BEGIN DRIVER
 PARAMETER DRIVER_NAME = uartlite
 PARAMETER DRIVER_VER = 3.2
 PARAMETER HW_INSTANCE = sensor_actor_axi_uartlite_remote_control
END

BEGIN DRIVER
 PARAMETER DRIVER_NAME = ppm_interpreter
 PARAMETER DRIVER_VER = 1.0
 PARAMETER HW_INSTANCE = sensor_actor_ppm_interpreter_remote_control
END

BEGIN DRIVER
 PARAMETER DRIVER_NAME = bram
 PARAMETER DRIVER_VER = 4.1
 PARAMETER HW_INSTANCE = sensor_actor_sensor_actor_local_memory_dlmb_bram_if_cntlr
END

BEGIN DRIVER
 PARAMETER DRIVER_NAME = bram
 PARAMETER DRIVER_VER = 4.1
 PARAMETER HW_INSTANCE = sensor_actor_sensor_actor_local_memory_ilmb_bram_if_cntlr
END

BEGIN DRIVER
 PARAMETER DRIVER_NAME = sysmon
 PARAMETER DRIVER_VER = 7.3
 PARAMETER HW_INSTANCE = sensor_actor_xadc_sa
END


