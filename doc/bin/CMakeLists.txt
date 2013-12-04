remake_set(LIBEPOS_UTILS_ALTERNATIVE_TARGETS
  baud_rate bit_rate control current error home init input oscillate
  position position_profile sensor velocity velocity_profile version
  CACHE INTERNAL "List of utiltity binary targets")
remake_add_documentation(
  TARGETS ${LIBEPOS_UTILS_ALTERNATIVE_TARGETS}
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
remake_add_documentation(
  TARGETS position_profile_eval velocity_profile_eval
    spline_to_interpolated_position interpolated_position_eval
  ARGS --man-output=%OUTPUT%
    --man-title="${REMAKE_PROJECT_NAME} Utilities Documentation"
    --project-name="${REMAKE_PROJECT_NAME}"
    --project-version="${REMAKE_PROJECT_VERSION}"
    --project-authors="${REMAKE_PROJECT_AUTHORS}"
    --project-contact="${REMAKE_PROJECT_CONTACT}"
    --project-home="${REMAKE_PROJECT_HOME}"
    --project-license="${REMAKE_PROJECT_LICENSE}"
  OUTPUT_FILE man1/%EXECUTABLE%.1.gz
  MAKE_DIRECTORIES
  TYPES man
  COMPONENT utils
  INSTALL ${LIBEPOS_DOCUMENTATION_DESTINATION})
