remake_project_set(CAN_INTERFACE cpc CACHE STRING
  "The motor control's CAN interface as supported by libcan [cpc|serial].")

remake_find_library(
  m math.h
  PACKAGE libm
)
remake_find_library(
  tulibs-config config.h
  PACKAGE tulibs
)
remake_find_library(
  tulibs-timer timer.h
  PACKAGE tulibs
)
remake_project_get(CAN_INTERFACE)
remake_find_library(
  can-${CAN_INTERFACE} can.h
  PACKAGE libcan
)
remake_var_name(CAN_LIBRARY_VAR can-${CAN_INTERFACE} LIBRARY)

remake_add_library(
  epos PREFIX OFF
  LINK ${M_LIBRARY} ${TULIBS_CONFIG_LIBRARY} ${TULIBS_TIMER_LIBRARY}
    ${${CAN_LIBRARY_VAR}}
)
remake_add_headers()
