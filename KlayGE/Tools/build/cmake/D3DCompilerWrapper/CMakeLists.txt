IF(KLAYGE_PLATFORM_WINDOWS)
	SET(SOURCE_FILES
		${KLAYGE_PROJECT_DIR}/Tools/src/D3DCompilerWrapper/D3DCompilerWrapper.cpp
		${KLAYGE_PROJECT_DIR}/Tools/src/D3DCompilerWrapper/D3DCompilerWrapper.py
	)

	SETUP_TOOL(D3DCompilerWrapper)
ELSE()
	FIND_PACKAGE(PythonInterp)

	FIND_PATH(WINEGCC_PATH "winegcc"
		PATHS /usr/local/bin opt/local/sbin /usr/bin /bin /usr/sbin /sbin /usr/local/bin)

	IF(NOT WINEGCC_PATH)
		MESSAGE(FATAL_ERROR "Could NOT find winegcc")
	ENDIF()

	ADD_CUSTOM_TARGET(D3DCompilerWrapper ALL
		COMMAND "${PYTHON_EXECUTABLE}" "${KLAYGE_PROJECT_DIR}/Tools/src/D3DCompilerWrapper/D3DCompilerWrapper.py" "${KLAYGE_PROJECT_DIR}/Tools/src/D3DCompilerWrapper/D3DCompilerWrapper.cpp" "${KLAYGE_BIN_DIR}/D3DCompilerWrapper${CMAKE_DEBUG_POSTFIX}"
	)
ENDIF()
