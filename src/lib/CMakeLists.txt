remake_add_library(
  epos PREFIX OFF
  LINK ${M_LIBRARY} ${TULIBS_LIBRARIES} ${LIBCAN_LIBRARIES}
)
remake_add_headers()
