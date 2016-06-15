set(_root "${CMAKE_CURRENT_LIST_DIR}/../../..")
get_filename_component(_root ${_root} ABSOLUTE)

set(MSC_LIBRARIES
  ${_root}/libs/x64/libmsc.so
  CACHE INTERNAL "" FORCE
)

set(MSC_INCLUDE_DIRS
  ${_root}/include
  CACHE INTERNAL "" FORCE
)

# qi_persistent_set(MSC_DEPENDS "")
export_lib(MSC)
