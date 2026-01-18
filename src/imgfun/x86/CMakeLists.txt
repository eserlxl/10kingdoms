# x86 assembly imgfun library
# Assembly files need to be compiled with jwasm
set(ASM_SOURCES
  CRC.asm
  IB.asm
  IB2.asm
  IB_32.asm
  IB_A.asm
  IB_AR.asm
  IB_AT.asm
  IB_ATD.asm
  IB_ATDM.asm
  IB_ATM.asm
  IB_ATR.asm
  IB_ATRD.asm
  IB_ATRDM.asm
  IB_DW.asm
  IB_R.asm
  IB_RD.asm
  IB_T.asm
  IB_TD.asm
  IB_TDM.asm
  IB_TM1.asm
  IB_TM2.asm
  IB_TM3.asm
  IB_TR.asm
  IB_TRD.asm
  IB_TRDM.asm
  IC.asm
  IC_R.asm
  IJ_T.asm
  IR.asm
  IR_A.asm
  IR_AM.asm
  IR_BAR.asm
  IR_M.asm
  I_BAR.asm
  I_BLACK.asm
  I_CTRL.asm
  I_EMASK.asm
  I_EREMAP.asm
  I_FONT.asm
  I_FREMAP.asm
  I_LINE.asm
  I_PIXEL.asm
  I_READ.asm
  I_SNOW.asm
  I_XOR.asm
)

# Custom command to compile assembly files with jwasm
set(ASM_OBJECTS)
foreach(asm_file ${ASM_SOURCES})
  get_filename_component(asm_name ${asm_file} NAME_WE)
  set(obj_file ${CMAKE_CURRENT_BINARY_DIR}/${asm_name}.o)
  list(APPEND ASM_OBJECTS ${obj_file})
  
  add_custom_command(
    OUTPUT ${obj_file}
    COMMAND ${JWASM_CMD} ${JWASM_FMT} -q -Fo ${obj_file} ${CMAKE_CURRENT_SOURCE_DIR}/${asm_file}
    DEPENDS ${CMAKE_CURRENT_SOURCE_DIR}/${asm_file}
    COMMENT "Assembling ${asm_file}"
  )
endforeach()

# Create library from object files
add_library(imgfun_x86 STATIC ${ASM_OBJECTS})

target_include_directories(imgfun_x86 PRIVATE
  ${CMAKE_SOURCE_DIR}/include
)

# Note: Assembly files don't use C++ compiler flags, but we set include dirs for consistency
