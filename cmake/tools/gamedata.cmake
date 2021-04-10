#[[ A specially curated CMAKE file designed to handle
    a data compression archive file and automatically move it
    to the bin folder, which can then be read by ASGE.

    NB: It's probably best not to change these scripts
    unless you have a good reason to do so ]]

## creates a compressed version of the game data ready for publishing ##
function(add_gamedata TARGET GAMEDATA_FOLDER)
  
  if(TARGET "${TARGET}-GD")
    return()
  endif()
  
  if(GAMEDATA_FOLDER)
    get_target_property(TARGET_OUTPUT_DIR ${TARGET} RUNTIME_OUTPUT_DIRECTORY)
    
    if(WIN32)
      add_custom_target(
          ${TARGET}-GD
          COMMAND "${CMAKE_SOURCE_DIR}/tools/7zip/win32/7za" -tzip a
          "${TARGET_OUTPUT_DIR}/game.dat" "${GAMEDATA_FOLDER}/*"
          WORKING_DIRECTORY "${GAMEDATA_FOLDER}"
          BYPRODUCTS "${TARGET_OUTPUT_DIR}/game.dat"
          COMMENT "creating data archive")
    
    else()
      add_custom_target(
          ${TARGET}-GD
          COMMAND ${CMAKE_COMMAND} -E tar cfv "${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/game.dat" --format=zip -- *
          WORKING_DIRECTORY ${GAMEDATA_FOLDER}
          BYPRODUCTS "${TARGET_OUTPUT_DIR}/game.dat"
          COMMENT "creating data archive")
    endif()
  
  else()
    message(WARNING "No game data folder has been specified")
  endif()

endfunction()
