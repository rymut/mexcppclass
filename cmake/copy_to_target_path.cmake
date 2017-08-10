function(COPY_TO_TARGET_PATH TARGET_NAME FILE_DIR FILE_NAME)
  add_custom_command(TARGET ${TARGET_NAME} POST_BUILD
    COMMAND "${CMAKE_COMMAND}" -E copy
      "${FILE_DIR}/${FILE_NAME}"
      "$<TARGET_FILE_DIR:${TARGET_NAME}>/${FILE_NAME}"
    COMMENT "Copying to ${TARGET_NAME} directory")
endfunction(COPY_TO_TARGET_PATH)
