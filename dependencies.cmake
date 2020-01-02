MESSAGE("Installer: display deps:")
file(
	GET_RUNTIME_DEPENDENCIES 
	UNRESOLVED_DEPENDENCIES_VAR MY_UNRESOLVED_DEPS 
	RESOLVED_DEPENDENCIES_VAR MY_RESOLVED_DEP 
	PRE_EXCLUDE_REGEXES "api-ms-win" "ext-ms"
	EXECUTABLES D:/philipp/dev/install/Debug/bin/AstroCameraRemote.exe)
	
MESSAGE(STATUS "MY_UNRESOLVED_DEPS: ${${MY_UNRESOLVED_DEPS}}  ${MY_UNRESOLVED_DEPS}")
MESSAGE(STATUS "MY_RESOLVED_DEPS: ${${MY_RESOLVED_DEPS}}  ${MY_RESOLVED_DEPS}")

foreach(DEPENDENCY_FILE ${MY_UNRESOLVED_DEPS})
   get_filename_component("${MY_BINARY_LOCATION}" "${DEPENDENCY_FILE}" "" "" resolved_file)
  message("resolved_file='${resolved_file}'")
endforeach()