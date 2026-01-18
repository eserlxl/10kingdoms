# Add subdirectories for libraries
add_subdirectory(openal)
add_subdirectory(enet)
add_subdirectory(imgfun)

if(ENABLE_CURL)
  add_subdirectory(curl)
endif()

# Main executable source files
set(10KINGDOMS_SOURCES
  AM.cpp
  ConfigAdv.cpp
  CmdLine.cpp
  LocaleRes.cpp
  OAI_ACT.cpp
  OAI_ACT2.cpp
  OAI_ATTK.cpp
  OAI_BUIL.cpp
  OAI_CAP2.cpp
  OAI_CAPT.cpp
  OAI_DEFE.cpp
  OAI_DIPL.cpp
  OAI_ECO.cpp
  OAI_GRAN.cpp
  OAI_INFO.cpp
  OAI_MAIN.cpp
  OAI_MAR2.cpp
  OAI_MAR3.cpp
  OAI_MARI.cpp
  OAI_MILI.cpp
  OAI_MONS.cpp
  OAI_QUER.cpp
  OAI_SEEK.cpp
  OAI_SPY.cpp
  OAI_TALK.cpp
  OAI_TOWN.cpp
  OAI_TRAD.cpp
  OAI_UNIT.cpp
  OANLINE.cpp
  OBATTLE.cpp
  OBLOB.cpp
  OBOX.cpp
  OBULLET.cpp
  OBULLETA.cpp
  OBUTT3D.cpp
  OBUTTCUS.cpp
  OBUTTON.cpp
  OB_FLAME.cpp
  OB_HOMIN.cpp
  OB_PROJ.cpp
  OCOLTBL.cpp
  OCONFIG.cpp
  OCRC_STO.cpp
  ODATE.cpp
  ODB.cpp
  ODIR.cpp
  ODYNARR.cpp
  ODYNARRB.cpp
  OEFFECT.cpp
  OERRCTRL.cpp
  OERROR.cpp
  OEXPMASK.cpp
  OFILE.cpp
  OFILETXT.cpp
  OFIRM.cpp
  OFIRM2.cpp
  OFIRMA.cpp
  OFIRMAI.cpp
  OFIRMDIE.cpp
  OFIRMDRW.cpp
  OFIRMIF.cpp
  OFIRMIF2.cpp
  OFIRMIF3.cpp
  OFIRMRES.cpp
  OFLAME.cpp
  OFONT.cpp
  OF_BASE.cpp
  OF_BASE2.cpp
  OF_CAMP.cpp
  OF_CAMP2.cpp
  OF_FACT.cpp
  OF_FACT2.cpp
  OF_HARB.cpp
  OF_HARB2.cpp
  OF_INN.cpp
  OF_INN2.cpp
  OF_MARK.cpp
  OF_MARK2.cpp
  OF_MINE.cpp
  OF_MINE2.cpp
  OF_MONS.cpp
  OF_RESE.cpp
  OF_RESE2.cpp
  OF_WAR.cpp
  OF_WAR2.cpp
  OGAMCRED.cpp
  OGAME.cpp
  OGAMEMP.cpp
  OGAMENCY.cpp
  OGAMEND.cpp
  OGAMESET.cpp
  OGAMHALL.cpp
  OGAMMAIN.cpp
  OGAMMENU.cpp
  OGAMSCE2.cpp
  OGAMSCEN.cpp
  OGAMSING.cpp
  OGENHILL.cpp
  OGENMAP.cpp
  OGETA.cpp
  OGFILE.cpp
  OGFILE2.cpp
  OGFILE3.cpp
  OGFILEA.cpp
  OGF_REC.cpp
  OGF_V1.cpp
  OGODRES.cpp
  OGRPSEL.cpp
  OHELP.cpp
  OHILLRES.cpp
  OIMGRES.cpp
  OINFO.cpp
  OINGMENU.cpp
  OLIGHTN.cpp
  OLIGHTN2.cpp
  OLOG.cpp
  OLONGLOG.cpp
  OLZW.cpp
  OMATRIX.cpp
  OMEM.cpp
  OMISC.cpp
  OMONSRES.cpp
  OMOUSE.cpp
  OMOUSECR.cpp
  OMP_CRC.cpp
  OMUSIC.cpp
  ONATIONA.cpp
  ONATIONB.cpp
  ONEWS.cpp
  ONEWS2.cpp
  ONEWSENG.cpp
  OOPTMENU.cpp
  OPLANT.cpp
  OPLASMA.cpp
  OPOWER.cpp
  ORACERES.cpp
  ORAIN1.cpp
  ORAIN2.cpp
  ORAIN3.cpp
  ORAWRES.cpp
  OREBEL.cpp
  OREGION.cpp
  OREGIONS.cpp
  OREMOTE.cpp
  OREMOTE2.cpp
  OREMOTEM.cpp
  OREMOTEQ.cpp
  ORES.cpp
  ORESDB.cpp
  ORESX.cpp
  OROCK.cpp
  OROCKRES.cpp
  OR_AI.cpp
  OR_ECO.cpp
  OR_MIL.cpp
  OR_NAT.cpp
  OR_NEWS.cpp
  OR_RANK.cpp
  OR_SPY.cpp
  OR_TECH.cpp
  OR_TOWN.cpp
  OR_TRADE.cpp
  OSCROLL.cpp
  OSE.cpp
  OSERES.cpp
  OSFRMRES.cpp
  OSITE.cpp
  OSITEDRW.cpp
  OSKILL.cpp
  OSLIDCUS.cpp
  OSNOW1.cpp
  OSNOW2.cpp
  OSNOWG.cpp
  OSNOWRES.cpp
  OSPATH.cpp
  OSPATHBT.cpp
  OSPREDBG.cpp
  OSPREOFF.cpp
  OSPRESMO.cpp
  OSPREUSE.cpp
  OSPRITE.cpp
  OSPRITE2.cpp
  OSPRITEA.cpp
  OSPRTRES.cpp
  OSPY.cpp
  OSPY2.cpp
  OSPYA.cpp
  OSTR.cpp
  OSYS.cpp
  OSYS2.cpp
  OSYS3.cpp
  OTALKENG.cpp
  OTALKFRE.cpp
  OTALKGER.cpp
  OTALKMSG.cpp
  OTALKRE2.cpp
  OTALKRES.cpp
  OTALKSPA.cpp
  OTECHRES.cpp
  OTERRAIN.cpp
  OTORNADO.cpp
  OTOWN.cpp
  OTOWNA.cpp
  OTOWNAI.cpp
  OTOWNBLD.cpp
  OTOWNDRW.cpp
  OTOWNIF.cpp
  OTOWNIND.cpp
  OTOWNRES.cpp
  OTUTOR.cpp
  OTUTOR2.cpp
  OTownNetwork.cpp
  OUNIT.cpp
  OUNIT2.cpp
  OUNITA.cpp
  OUNITAAC.cpp
  OUNITAAT.cpp
  OUNITAC.cpp
  OUNITAI.cpp
  OUNITAM.cpp
  OUNITAT.cpp
  OUNITAT2.cpp
  OUNITAT3.cpp
  OUNITATB.cpp
  OUNITD.cpp
  OUNITDRW.cpp
  OUNITHB.cpp
  OUNITI.cpp
  OUNITIF.cpp
  OUNITIND.cpp
  OUNITM.cpp
  OUNITRES.cpp
  OUNITS.cpp
  OU_CARA.cpp
  OU_CARA2.cpp
  OU_CARAS.cpp
  OU_CARAT.cpp
  OU_CART.cpp
  OU_GOD.cpp
  OU_GOD2.cpp
  OU_MARI.cpp
  OU_MARI2.cpp
  OU_MARIF.cpp
  OU_MARIS.cpp
  OU_MARIT.cpp
  OU_MONS.cpp
  OU_VEHI.cpp
  OVBROWIF.cpp
  OVBROWSE.cpp
  OVGA.cpp
  OVGABUF.cpp
  OVGABUF2.cpp
  OVGALOCK.cpp
  OVIDEO.cpp
  OVOLUME.cpp
  OVQUEUE.cpp
  OWALLRES.cpp
  OWARPT.cpp
  OWEATHER.cpp
  OWORLD.cpp
  OWORLD_M.cpp
  OWORLD_Z.cpp
  OW_FIRE.cpp
  OW_PLANT.cpp
  OW_ROCK.cpp
  OW_SOUND.cpp
  OW_WALL.cpp
  PlayerStats.cpp
  ReplayFile.cpp
  dbglog.cpp
  file_input_stream.cpp
  file_output_stream.cpp
  file_reader.cpp
  file_util.cpp
  file_writer.cpp
  input_stream.cpp
  mem_input_stream.cpp
  misc_uuid.cpp
  output_stream.cpp
  session_desc.cpp
  player_desc.cpp
  vgautil.cpp
  vgautil2.cpp
)

# Windows resource file
if(WIN32)
  list(APPEND 10KINGDOMS_SOURCES ico.rc)
endif()

# Create executable
add_executable(10kingdoms ${10KINGDOMS_SOURCES})

# Compiler flags
target_compile_options(10kingdoms PRIVATE
  ${GLOBAL_CFLAGS}
  -DOPENAL_AL_H=${OPENAL_AL_H_VALUE}
  -DOPENAL_ALC_H=${OPENAL_ALC_H_VALUE}
)

# Include directories
target_include_directories(10kingdoms PRIVATE
  ${CMAKE_SOURCE_DIR}/include
  ${SDL2_INCLUDE_DIRS}
  ${OpenAL_INCLUDE_DIRS}
)

# Compile definitions (replaces config.h)
target_compile_definitions(10kingdoms PRIVATE
  ${PROJECT_COMPILE_DEFINITIONS}
  PACKAGE="10kingdoms"
  PACKAGE_DATA_DIR="${CMAKE_INSTALL_PREFIX}/share/10kingdoms"
  LOCALE_DIR="${CMAKE_INSTALL_PREFIX}/share/locale"
)

# Link libraries
target_link_libraries(10kingdoms PRIVATE
  audio_openal
  multiplayer
  ${SDL2_LIBRARIES}
  ${OpenAL_LIBRARIES}
)

# Link imgfun library (generic or x86)
if(ENABLE_ASM)
  target_link_libraries(10kingdoms PRIVATE imgfun_x86)
else()
  target_link_libraries(10kingdoms PRIVATE imgfun_generic)
endif()

if(ENABLE_CURL)
  target_link_libraries(10kingdoms PRIVATE WebService)
endif()

if(ENABLE_ENET)
  target_link_libraries(10kingdoms PRIVATE ${ENET_LIBRARIES})
endif()

if(INTL_LIBRARY)
  target_link_libraries(10kingdoms PRIVATE ${INTL_LIBRARY})
endif()

if(ICONV_LIBRARY)
  target_link_libraries(10kingdoms PRIVATE ${ICONV_LIBRARY})
endif()

if(PLATFORM_LIBS)
  target_link_libraries(10kingdoms PRIVATE ${PLATFORM_LIBS})
endif()

# Linker flags
target_link_options(10kingdoms PRIVATE ${GLOBAL_LDFLAGS})

# Windows resource compilation
if(WIN32)
  enable_language(RC)
  set_source_files_properties(ico.rc PROPERTIES
    COMPILE_FLAGS "-I \"${CMAKE_SOURCE_DIR}/include\""
  )
endif()

# Copy/link binary to src/ directory for compatibility with existing scripts
add_custom_command(TARGET 10kingdoms POST_BUILD
  COMMAND ${CMAKE_COMMAND} -E create_symlink
    ${CMAKE_CURRENT_BINARY_DIR}/10kingdoms
    ${CMAKE_SOURCE_DIR}/src/10kingdoms
  COMMENT "Creating compatibility symlink in src/ directory"
)
