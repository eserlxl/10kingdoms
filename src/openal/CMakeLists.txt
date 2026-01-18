# OpenAL audio library
add_library(audio_openal STATIC
  openal_audio.cpp
  wav_stream.cpp
)

target_include_directories(audio_openal PRIVATE
  ${CMAKE_SOURCE_DIR}/include
  ${OpenAL_INCLUDE_DIRS}
)

target_compile_options(audio_openal PRIVATE
  ${GLOBAL_CFLAGS}
  "-include" "${CMAKE_BINARY_DIR}/include/config.h"
)

# Link OpenAL to the static library (needed for symbols)
# Use INTERFACE so dependencies propagate to consumers
if(OpenAL_FOUND)
  target_link_libraries(audio_openal INTERFACE OpenAL::OpenAL)
else()
  target_link_libraries(audio_openal INTERFACE ${OpenAL_LIBRARIES})
endif()
