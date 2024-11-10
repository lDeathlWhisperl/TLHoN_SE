# Additional clean files
cmake_minimum_required(VERSION 3.16)

if("${CONFIG}" STREQUAL "" OR "${CONFIG}" STREQUAL "Debug")
  file(REMOVE_RECURSE
  "CMakeFiles\\TLHoN_IC_autogen.dir\\AutogenUsed.txt"
  "CMakeFiles\\TLHoN_IC_autogen.dir\\ParseCache.txt"
  "TLHoN_IC_autogen"
  )
endif()
