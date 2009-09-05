cmake_minimum_required(VERSION 2.6)

include(ReMake)

remake_project(
  libepos
  VERSION 0.1
  RELEASE alpha
  SUMMARY "Driver library for the maxon EPOS motor control."
  AUTHOR "Ralf Kaestner"
  CONTACT "ralf.kaestner@gmail.com"
  HOME "http://aslforge.ethz.ch/projects/libepos"
  LICENSE "GNU Lesser General Public License (LGPL)"
  PREFIX epos-
  INSTALL /usr
)

remake_doc(
  man INSTALL share
  html
)

remake_pack_deb(tulibs libcan)
remake_pack_deb(COMPONENT dev tulibs-dev libcan-dev)
remake_pack_deb(COMPONENT doc)
