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
remake_find_library(
  can can.h
  PACKAGE libcan
)

remake_add_library(
  epos PREFIX OFF
  LINK ${M_LIBRARY} ${TULIBS_CONFIG_LIBRARY} ${TULIBS_TIMER_LIBRARY}
    ${CAN_LIBRARY}
)
remake_add_headers()
