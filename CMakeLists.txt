cmake_minimum_required(VERSION 2.6)

include(ReMake)

remake_project(
  Tulibs
  0.1
  alpha
  "A collection of C-based Tool and Utility LIBrarieS."
  "Ralf Kaestner"
  "ralf.kaestner@gmail.com"
  "http://aslforge.ethz.ch/projects/robotks"
  "GNU Lesser General Public License (LGPL)"
  INSTALL /usr
)

remake_doc(
  man INSTALL share
  html
)

remake_pack_deb()
remake_pack_deb(COMPONENT dev)
remake_pack_deb(COMPONENT doc)
