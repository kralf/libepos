remake_set(LIBEPOS_UTILS_TARGETS
  baud_rate bit_rate control current error home init input
  oscillate position position_profile position_profile_eval sensor
  velocity velocity_profile velocity_profile_eval version
  CACHE INTERNAL "List of utiltity binary targets")
remake_add_documentation(
  TARGETS ${LIBEPOS_UTILS_TARGETS}
  ARGS --man-output=%OUTPUT%
    --man-title="${REMAKE_PROJECT_NAME} Utilities Documentation"
    --project-name="${REMAKE_PROJECT_NAME}"
    --project-version="${REMAKE_PROJECT_VERSION}"
    --project-authors="${REMAKE_PROJECT_AUTHORS}"
    --project-contact="${REMAKE_PROJECT_CONTACT}"
    --project-home="${REMAKE_PROJECT_HOME}"
    --project-license="${REMAKE_PROJECT_LICENSE}"
  LINK_ALTERNATIVES libcan.so
  OUTPUT_FILE man1/%EXECUTABLE%.%ALTERNATIVE%.1.gz
  MAKE_DIRECTORIES
  TYPES man
  COMPONENT utils
  INSTALL ${LIBEPOS_DOCUMENTATION_DESTINATION})
