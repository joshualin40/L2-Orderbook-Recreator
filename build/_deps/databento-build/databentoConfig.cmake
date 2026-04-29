set(databento_VERSION 0.55.0)


####### Expanded from @PACKAGE_INIT@ by configure_package_config_file() #######
####### Any changes to this file will be overwritten by the next CMake run ####
####### The input file was databentoConfig.cmake.in                            ########

get_filename_component(PACKAGE_PREFIX_DIR "${CMAKE_CURRENT_LIST_DIR}/../../../" ABSOLUTE)

macro(set_and_check _var _file)
  set(${_var} "${_file}")
  if(NOT EXISTS "${_file}")
    message(FATAL_ERROR "File or directory ${_file} referenced by variable ${_var} does not exist !")
  endif()
endmacro()

macro(check_required_components _NAME)
  foreach(comp ${${_NAME}_FIND_COMPONENTS})
    if(NOT ${_NAME}_${comp}_FOUND)
      if(${_NAME}_FIND_REQUIRED_${comp})
        set(${_NAME}_FOUND FALSE)
      endif()
    endif()
  endforeach()
endmacro()

####################################################################################

# Reuse FindZstd
list(APPEND CMAKE_MODULE_PATH "${CMAKE_CURRENT_LIST_DIR}")

# Add dependencies here so end-user doesn't have to
include(CMakeFindDependencyMacro)
find_dependency(date)
find_dependency(zstd)
if(NOT TARGET zstd::libzstd)
  if(TARGET zstd::libzstd_shared)
    add_library(zstd::libzstd ALIAS zstd::libzstd_shared)
  elseif(TARGET zstd::libzstd_static)
    add_library(zstd::libzstd ALIAS zstd::libzstd_static)
  endif()
endif()
find_dependency(httplib)
find_dependency(nlohmann_json)
find_dependency(Threads)

include("${CMAKE_CURRENT_LIST_DIR}/databentoTargets.cmake")

check_required_components(databento)
