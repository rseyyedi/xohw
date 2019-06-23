set_property PACKAGE_PIN T22 [get_ports {debug_led_sensor_actor_tri_o[0]}]
set_property PACKAGE_PIN T21 [get_ports {debug_led_sensor_actor_tri_o[1]}]
set_property IOSTANDARD LVCMOS33 [get_ports {debug_led_sensor_actor_tri_o[1]}]
set_property IOSTANDARD LVCMOS33 [get_ports {debug_led_sensor_actor_tri_o[0]}]

set_property PACKAGE_PIN U22 [get_ports {debug_led_flight_controller_tri_o[0]}]
set_property PACKAGE_PIN U21 [get_ports {debug_led_flight_controller_tri_o[1]}]
set_property IOSTANDARD LVCMOS33 [get_ports {debug_led_flight_controller_tri_o[0]}]
set_property IOSTANDARD LVCMOS33 [get_ports {debug_led_flight_controller_tri_o[1]}]
# MOTORS
set_property PACKAGE_PIN W12 [get_ports iic_motorboard_sda_io]
set_property PACKAGE_PIN V12 [get_ports iic_motorboard_scl_io]
set_property IOSTANDARD LVCMOS33 [get_ports iic_motorboard_scl_io]
set_property IOSTANDARD LVCMOS33 [get_ports iic_motorboard_sda_io]
# IMU
set_property PACKAGE_PIN W11 [get_ports iic_imu_sda_io]
set_property PACKAGE_PIN W10 [get_ports iic_imu_scl_io]
set_property IOSTANDARD LVCMOS33 [get_ports iic_imu_scl_io]
set_property IOSTANDARD LVCMOS33 [get_ports iic_imu_sda_io]
# BU

set_property PACKAGE_PIN V10 [get_ports iic_bu_sda_io]
set_property PACKAGE_PIN V9 [get_ports iic_bu_scl_io]
set_property IOSTANDARD LVCMOS33 [get_ports iic_bu_scl_io]
set_property IOSTANDARD LVCMOS33 [get_ports iic_bu_sda_io]

# PPM
set_property PACKAGE_PIN W8 [get_ports ppm]
set_property IOSTANDARD LVCMOS33 [get_ports ppm]

set_property PACKAGE_PIN Y11 [get_ports {spi_battery_guard_ss_io[0]}]
set_property PACKAGE_PIN AA11 [get_ports spi_battery_guard_sck_io]
set_property PACKAGE_PIN Y10 [get_ports spi_battery_guard_io0_io]
set_property PACKAGE_PIN AA9 [get_ports spi_battery_guard_io1_io]
set_property PACKAGE_PIN AB11 [get_ports {spi_battery_guard_ss_io[1]}]
set_property IOSTANDARD LVCMOS33 [get_ports {spi_battery_guard_ss_io[1]}]
set_property IOSTANDARD LVCMOS33 [get_ports {spi_battery_guard_ss_io[0]}]
set_property IOSTANDARD LVCMOS33 [get_ports spi_battery_guard_io0_io]
set_property IOSTANDARD LVCMOS33 [get_ports spi_battery_guard_io1_io]
set_property IOSTANDARD LVCMOS33 [get_ports spi_battery_guard_sck_io]

set_property PACKAGE_PIN AB10 [get_ports uart_remote_control_txd]
set_property PACKAGE_PIN AB9 [get_ports uart_remote_control_rxd]
set_property IOSTANDARD LVCMOS33 [get_ports uart_remote_control_rxd]
set_property IOSTANDARD LVCMOS33 [get_ports uart_remote_control_txd]

set_property PULLUP true [get_ports iic_motorboard_sda_io]
set_property PULLUP true [get_ports iic_bu_scl_io]
set_property PULLUP true [get_ports iic_bu_sda_io]
set_property PULLUP true [get_ports iic_imu_scl_io]
set_property PULLUP true [get_ports iic_imu_sda_io]
set_property PULLUP true [get_ports iic_motorboard_scl_io]
