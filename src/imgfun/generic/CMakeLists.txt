# Generic imgfun library (C++ implementation)
add_library(imgfun_generic STATIC
  CRC.cpp
  IB.cpp
  IB2.cpp
  IB_32.cpp
  IB_A.cpp
  IB_AR.cpp
  IB_AT.cpp
  IB_ATD.cpp
  IB_ATDM.cpp
  IB_ATR.cpp
  IB_ATRD.cpp
  IB_ATRDM.cpp
  IB_DW.cpp
  IB_R.cpp
  IB_RD.cpp
  IB_T.cpp
  IB_TD.cpp
  IB_TDM.cpp
  IB_TR.cpp
  IB_TRD.cpp
  IB_TRDM.cpp
  IC.cpp
  IC_R.cpp
  IJ_T.cpp
  IR.cpp
  IR_A.cpp
  IR_AM.cpp
  IR_BAR.cpp
  IR_M.cpp
  I_BAR.cpp
  I_BLACK.cpp
  I_EMASK.cpp
  I_EREMAP.cpp
  I_FONT.cpp
  I_FREMAP.cpp
  I_LINE.cpp
  I_PIXEL.cpp
  I_READ.cpp
  I_SNOW.cpp
)

target_include_directories(imgfun_generic PRIVATE
  ${CMAKE_SOURCE_DIR}/include
)

target_compile_options(imgfun_generic PRIVATE
  ${GLOBAL_CFLAGS}
)

# Compile definitions (replaces config.h)
target_compile_definitions(imgfun_generic PRIVATE
  ${PROJECT_COMPILE_DEFINITIONS}
)
