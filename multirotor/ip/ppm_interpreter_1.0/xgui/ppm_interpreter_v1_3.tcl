# Definitional proc to organize widgets for parameters.
proc init_gui { IPINST } {
  set Component_Name  [  ipgui::add_param $IPINST -name "Component_Name" -display_name {Component Name}]
  set_property tooltip {Component Name} ${Component_Name}
  #Adding Page
  set Page_0  [  ipgui::add_page $IPINST -name "Page 0" -display_name {Page 0}]
  set_property tooltip {Page 0} ${Page_0}
  set C_S00_AXI_DATA_WIDTH  [  ipgui::add_param $IPINST -name "C_S00_AXI_DATA_WIDTH" -parent ${Page_0} -display_name {C S00 AXI DATA WIDTH}]
  set_property tooltip {Width of S_AXI data bus} ${C_S00_AXI_DATA_WIDTH}
  set C_S00_AXI_ADDR_WIDTH  [  ipgui::add_param $IPINST -name "C_S00_AXI_ADDR_WIDTH" -parent ${Page_0} -display_name {C S00 AXI ADDR WIDTH}]
  set_property tooltip {Width of S_AXI address bus} ${C_S00_AXI_ADDR_WIDTH}
  set C_S00_AXI_BASEADDR  [  ipgui::add_param $IPINST -name "C_S00_AXI_BASEADDR" -parent ${Page_0} -display_name {C S00 AXI BASEADDR}]
  set_property tooltip {C S00 AXI BASEADDR} ${C_S00_AXI_BASEADDR}
  set C_S00_AXI_HIGHADDR  [  ipgui::add_param $IPINST -name "C_S00_AXI_HIGHADDR" -parent ${Page_0} -display_name {C S00 AXI HIGHADDR}]
  set_property tooltip {C S00 AXI HIGHADDR} ${C_S00_AXI_HIGHADDR}
  set syncTime  [  ipgui::add_param $IPINST -name "syncTime" -parent ${Page_0} -display_name {syncTime}]
  set_property tooltip {minimum Time for synchronization for PPM Signal. realTime= syncTime* 10ns => 250000 * 10ns = 2500000ns = 2,5ms} ${syncTime}
  set timeoutTime  [  ipgui::add_param $IPINST -name "timeoutTime" -parent ${Page_0} -display_name {timeoutTime}]
  set_property tooltip {minimum time until timeout appear. Must be bigger then the sending time of one signal. realTime= timeoutTime *10ns => 2100000ns = 2,1ms => when this time is reached after transmission the transmission is done.} ${timeoutTime}


}

proc update_PARAM_VALUE.syncTime { PARAM_VALUE.syncTime } {
	# Procedure called to update syncTime when any of the dependent parameters in the arguments change
}

proc validate_PARAM_VALUE.syncTime { PARAM_VALUE.syncTime } {
	# Procedure called to validate syncTime
	return true
}

proc update_PARAM_VALUE.timeoutTime { PARAM_VALUE.timeoutTime } {
	# Procedure called to update timeoutTime when any of the dependent parameters in the arguments change
}

proc validate_PARAM_VALUE.timeoutTime { PARAM_VALUE.timeoutTime } {
	# Procedure called to validate timeoutTime
	return true
}

proc update_PARAM_VALUE.C_S00_AXI_DATA_WIDTH { PARAM_VALUE.C_S00_AXI_DATA_WIDTH } {
	# Procedure called to update C_S00_AXI_DATA_WIDTH when any of the dependent parameters in the arguments change
}

proc validate_PARAM_VALUE.C_S00_AXI_DATA_WIDTH { PARAM_VALUE.C_S00_AXI_DATA_WIDTH } {
	# Procedure called to validate C_S00_AXI_DATA_WIDTH
	return true
}

proc update_PARAM_VALUE.C_S00_AXI_ADDR_WIDTH { PARAM_VALUE.C_S00_AXI_ADDR_WIDTH } {
	# Procedure called to update C_S00_AXI_ADDR_WIDTH when any of the dependent parameters in the arguments change
}

proc validate_PARAM_VALUE.C_S00_AXI_ADDR_WIDTH { PARAM_VALUE.C_S00_AXI_ADDR_WIDTH } {
	# Procedure called to validate C_S00_AXI_ADDR_WIDTH
	return true
}

proc update_PARAM_VALUE.C_S00_AXI_BASEADDR { PARAM_VALUE.C_S00_AXI_BASEADDR } {
	# Procedure called to update C_S00_AXI_BASEADDR when any of the dependent parameters in the arguments change
}

proc validate_PARAM_VALUE.C_S00_AXI_BASEADDR { PARAM_VALUE.C_S00_AXI_BASEADDR } {
	# Procedure called to validate C_S00_AXI_BASEADDR
	return true
}

proc update_PARAM_VALUE.C_S00_AXI_HIGHADDR { PARAM_VALUE.C_S00_AXI_HIGHADDR } {
	# Procedure called to update C_S00_AXI_HIGHADDR when any of the dependent parameters in the arguments change
}

proc validate_PARAM_VALUE.C_S00_AXI_HIGHADDR { PARAM_VALUE.C_S00_AXI_HIGHADDR } {
	# Procedure called to validate C_S00_AXI_HIGHADDR
	return true
}


proc update_MODELPARAM_VALUE.C_S00_AXI_DATA_WIDTH { MODELPARAM_VALUE.C_S00_AXI_DATA_WIDTH PARAM_VALUE.C_S00_AXI_DATA_WIDTH } {
	# Procedure called to set VHDL generic/Verilog parameter value(s) based on TCL parameter value
	set_property value [get_property value ${PARAM_VALUE.C_S00_AXI_DATA_WIDTH}] ${MODELPARAM_VALUE.C_S00_AXI_DATA_WIDTH}
}

proc update_MODELPARAM_VALUE.C_S00_AXI_ADDR_WIDTH { MODELPARAM_VALUE.C_S00_AXI_ADDR_WIDTH PARAM_VALUE.C_S00_AXI_ADDR_WIDTH } {
	# Procedure called to set VHDL generic/Verilog parameter value(s) based on TCL parameter value
	set_property value [get_property value ${PARAM_VALUE.C_S00_AXI_ADDR_WIDTH}] ${MODELPARAM_VALUE.C_S00_AXI_ADDR_WIDTH}
}

proc update_MODELPARAM_VALUE.syncTime { MODELPARAM_VALUE.syncTime PARAM_VALUE.syncTime } {
	# Procedure called to set VHDL generic/Verilog parameter value(s) based on TCL parameter value
	set_property value [get_property value ${PARAM_VALUE.syncTime}] ${MODELPARAM_VALUE.syncTime}
}

proc update_MODELPARAM_VALUE.timeoutTime { MODELPARAM_VALUE.timeoutTime PARAM_VALUE.timeoutTime } {
	# Procedure called to set VHDL generic/Verilog parameter value(s) based on TCL parameter value
	set_property value [get_property value ${PARAM_VALUE.timeoutTime}] ${MODELPARAM_VALUE.timeoutTime}
}

