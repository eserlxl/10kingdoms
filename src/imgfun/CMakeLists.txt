# imgfun library - choose between generic and x86 assembly versions
if(ENABLE_ASM)
  add_subdirectory(x86)
else()
  add_subdirectory(generic)
endif()
