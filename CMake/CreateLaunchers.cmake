# - Create MSVC user file
#
#  include(CreateLaunchers) - to make these available
#
#  create_target_launcher(<targetname>
#    [ARGS <args...>]
#    [WORKING_DIRECTORY <dir>]
#
#  Original from github.com/rpavlik/cmake-modules
#

if(__create_launchers)
	return()
endif()
set(__create_launchers YES)

# We must run the following at "include" time, not at function call time,
# to find the path to this module rather than the path to a calling list file
get_filename_component(_launchermoddir
	${CMAKE_CURRENT_LIST_FILE}
	PATH)
set(_launchermoddir "${_launchermoddir}/Templates")

macro(_launcher_system_settings)
	if(CMAKE_SIZEOF_VOID_P EQUAL 8)
		set(BITS 64)
	else()
		set(BITS 32)
	endif()

	if(WIN32 AND MSVC AND MSVC_VERSION GREATER 1700)
		# Find user and system name
		set(SYSTEM_NAME $ENV{USERDOMAIN})
		set(USER_NAME $ENV{USERNAME})
		set(VCPROJ_TYPE vcxproj)
		set(USERFILE_EXTENSION user)
		set(LAUNCHER_LINESEP "\n")

		if(MSVC11)
			set(USERFILE_VC_VERSION 11.0)
		elseif(MSVC12)
			set(USERFILE_VC_VERSION 12.0)
		endif()

		if(BITS EQUAL 64)
			set(USERFILE_PLATFORM x64)
		else()
			set(USERFILE_PLATFORM Win${BITS})
		endif()
	endif()
endmacro()

macro(_launcher_process_args)
	set(_nowhere)
	set(_curdest _nowhere)
	set(_val_args
		ARGS
		WORKING_DIRECTORY)
	foreach(_arg ${_val_args})
		set(${_arg})
	endforeach()
	foreach(_element ${ARGN})
		string(REPLACE ";" "\\;" _element "${_element}")
		list(FIND _val_args "${_element}" _val_arg_find)
		if("${_val_arg_find}" GREATER "-1")
			set(_curdest "${_element}")
		else()
			list(APPEND ${_curdest} "${_element}")
		endif()
	endforeach()

	if(_nowhere)
		message(FATAL_ERROR
			"Syntax error in use of a function in CreateLaunchers!")
	endif()

	if(NOT WORKING_DIRECTORY)
		set(WORKING_DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}")
	endif()

	set(USERFILE_WORKING_DIRECTORY "${WORKING_DIRECTORY}")
	set(USERFILE_COMMAND_ARGUMENTS "${ARGS}")

endmacro()

macro(_launcher_produce_vcproj_user)
	if(MSVC)
		file(READ
			"${_launchermoddir}/perconfig.${VCPROJ_TYPE}.user.in"
			_perconfig)
		set(USERFILE_CONFIGSECTIONS)
		set(USERFILE_WORKING_DIRECTORY_BASE ${USERFILE_WORKING_DIRECTORY})
		foreach(USERFILE_CONFIGNAME ${CMAKE_CONFIGURATION_TYPES})
			set(USERFILE_WORKING_DIRECTORY "${USERFILE_WORKING_DIRECTORY_BASE}/${USERFILE_CONFIGNAME}")
			string(CONFIGURE "${_perconfig}" _temp @ONLY ESCAPE_QUOTES)
			string(CONFIGURE
				"${USERFILE_CONFIGSECTIONS}${_temp}"
				USERFILE_CONFIGSECTIONS
				ESCAPE_QUOTES)
		endforeach()

		configure_file("${_launchermoddir}/${VCPROJ_TYPE}.user.in"
			${VCPROJNAME}.${VCPROJ_TYPE}.${USERFILE_EXTENSION}
			@ONLY)
	endif()
endmacro()

function(create_target_launcher _targetname)
	_launcher_system_settings()
	_launcher_process_args(${ARGN})

	set(VCPROJNAME "${CMAKE_CURRENT_BINARY_DIR}/${_targetname}")
	_launcher_produce_vcproj_user()
endfunction()

