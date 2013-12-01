cmake_minimum_required(VERSION 2.6)

include(ReMake)

remake_project(
  libepos
  VERSION 0.3
  RELEASE alpha
  SUMMARY "Driver library for maxon EPOS/EPOS2 motor controllers."
  AUTHOR "Ralf Kaestner"
  CONTACT "ralf.kaestner@gmail.com"
  HOME "http://github.com/kralf/libepos"
  LICENSE "GNU Lesser General Public License (LGPL)"
  PREFIX epos-
  INSTALL /usr
)

remake_debian_get_alternatives(
  libcan.so
  OUTPUT LIBCAN_ALTERNATIVES
)

remake_component(
  udev
  CONFIGURATION_DESTINATION /etc/udev
)
remake_add_directories(
  rules
  COMPONENT udev
)
remake_doc(
  man INSTALL share
  html
)

remake_project_get(DOCUMENTATION_DESTINATION DESTINATION)

remake_pack_deb(
  DEPENDS tulibs libcan
)
remake_pack_deb(
  COMPONENT utils
  DESCRIPTION "utilities"
  DEPENDS libepos
  EXTRA debian/utils/*
)
remake_pack_deb(
  COMPONENT dev
  DESCRIPTION "development headers"
  DEPENDS libepos tulibs-dev libcan-dev
)
remake_pack_deb(
  COMPONENT udev
  DESCRIPTION "udev rules"
  DEPENDS udev
  EXTRA debian/rules/*
)
remake_pack_deb(
  ARCH all
  COMPONENT doc
  DESCRIPTION "documentation"
)

remake_distribute_deb(
  DISTRIBUTION lucid
  SECTION libs
  EXCLUDE /vendor/
  UPLOAD ppa:kralf/asl
  DEPENDS tulibs-dev libcan-dev remake pkg-config doxygen
  PASS CMAKE_BUILD_TYPE LIBEPOS_GIT_REVISION
)
remake_distribute_deb(
  DISTRIBUTION precise
  SECTION libs
  EXCLUDE /vendor/
  UPLOAD ppa:kralf/asl
  DEPENDS tulibs-dev libcan-dev remake pkg-config doxygen
  PASS CMAKE_BUILD_TYPE LIBEPOS_GIT_REVISION
)
