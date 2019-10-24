cmake_minimum_required(VERSION 3.0.2)

if(${CMAKE_HOST_SYSTEM_NAME} MATCHES "^MSYS_NT.*$")
  set(STR_PREPROCESSED_FILE_PATH "preprocessed_file_path")
else()
  set(STR_PREPROCESSED_FILE_PATH "{preprocessed_file_path}")
endif()

message(STATUS "${ARDUINO_PREPROC_TARGET} STREQUAL ${STR_PREPROCESSED_FILE_PATH}")

if("${ARDUINO_PREPROC_TARGET}" STREQUAL ${STR_PREPROCESSED_FILE_PATH})
  set(build_dir ${ARDUINO_BUILD_PATH})
  set(preproc_flag "")
else()
  set(build_dir ${ARDUINO_BUILD_PATH}/preproc)
  set(preproc_flag -DARDUINO_PREPROC=1)
endif()

message(STATUS "build_dir ${build_dir}")

set(ENV{PATH} "${ARDUINO_COMPILER_PATH}:$ENV{PATH}")
set(preproc_script ${ARDUINO_BUILD_PATH}/preproc/preproc.sh)

if(NOT ${EXTERNAL_PROJECT_PATH_OPENTHREAD} STREQUAL "")
  set(external_ot_path -DEXTERNAL_PROJECT_PATH_OPENTHREAD=${EXTERNAL_PROJECT_PATH_OPENTHREAD})
else()
  set(external_ot_path "")
endif()

if(EXISTS ${build_dir}/_cmakefile/.NOT_CHANGED )
    message(STATUS "remote")
  file(REMOVE ${build_dir}/_cmakefile/.NOT_CHANGED )
else()
  if("${ARDUINO_PREPROC_TARGET}" STREQUAL ${STR_PREPROCESSED_FILE_PATH})
    message(STATUS "cmake")
    execute_process(
      COMMAND ${CMAKE_COMMAND} _cmakefile -DCMAKE_TOOLCHAIN_FILE=${ARDUINO_SYSTEM_PATH}/ot-rtos/cmake/arm-none-eabi.cmake -DPLATFORM_NAME=nrf52 -DWITHOUT_APPS=1 -DWITHOUT_CLI=1
      WORKING_DIRECTORY ${build_dir}
    )
  else()
    message(STATUS "cmake preproc")
    #if(NOT EXISTS ${ARDUINO_BUILD_PATH}/preproc/preproc.sh )
      message(STATUS "create preproc.sh")
      message(STATUS "${CMAKE_COMMAND} _cmakefile -DCMAKE_TOOLCHAIN_FILE=${ARDUINO_SYSTEM_PATH}/ot-rtos/cmake/arm-none-eabi.cmake -DPLATFORM_NAME=nrf52 -DWITHOUT_APPS=1 -DWITHOUT_CLI=1")
      execute_process(
	COMMAND ${CMAKE_COMMAND} _cmakefile -DCMAKE_TOOLCHAIN_FILE=${ARDUINO_SYSTEM_PATH}/ot-rtos/cmake/arm-none-eabi.cmake -DPLATFORM_NAME=nrf52 -DWITHOUT_APPS=1 -DWITHOUT_CLI=1
        WORKING_DIRECTORY ${build_dir}
	#OUTPUT_QUIET
	#ERROR_QUIET
      )
    #endif()
  endif()
endif()

if("${ARDUINO_PREPROC_TARGET}" STREQUAL ${STR_PREPROCESSED_FILE_PATH})
  message(STATUS "build ${build_dir}")
  execute_process(
    COMMAND ${CMAKE_COMMAND} --build ${build_dir} -- V=1 VERBOSE=1
    WORKING_DIRECTORY ${build_dir}
    RESULT_VARIABLE retcode
  )
else()
  message(STATUS "preproc bash ${preproc_script} ${ARDUINO_PREPROC_SOURCE} ${ARDUINO_PREPROC_TARGET}")
  execute_process(
    COMMAND bash ${preproc_script} ${ARDUINO_PREPROC_SOURCE} ${ARDUINO_PREPROC_TARGET}
    WORKING_DIRECTORY ${build_dir}
    RESULT_VARIABLE retcode
  )
endif()

# remove symlinks (Prevent system file deletion by cleanup on Arduino IDE exit)
file(GLOB_RECURSE sc_links ${build_dir}/*)
foreach(l ${sc_links})
  if(IS_SYMLINK ${l})
    file(REMOVE ${l})
  endif()
endforeach()


if(${retcode})
  message(FATAL_ERROR)
endif()

