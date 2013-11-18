project(plot)
find_package( KDE4 REQUIRED )
find_package( Qt4 REQUIRED)
find_package ( OpenGL REQUIRED )
find_package ( Analitza REQUIRED )
include_directories( ${KDE4_INCLUDES} ${CMAKE_SOURCE_DIR} ${CMAKE_CURRENT_BINARY_DIR}  ${ANALITZA_INCLUDE_DIR}  ${OPENGL_INCLUDE_DIR}  )
set( plot_SRCS main.cpp mainwindow.cpp )
kde4_add_executable( plot ${plot_SRCS} )
target_link_libraries( plot ${KDE4_KDEUI_LIBS}  ${KDE4_KFILE_LIBS}
  ${ANALITZA_LIBRARY}
  ${ANALITZAGUI_LIBRARY}
  ${ANALITZAPLOT_LIBRARY}
  ${QT_QTOPENGL_LIBRARY}
  ${QT_QTXML_LIBRARY}
  ${QTMML_LIBRARY}
)
if(HAVE_OPENGL)
    target_link_libraries ( plot
        ${OPENGL_gl_LIBRARY}
        ${OPENGL_glu_LIBRARY}
    )
endif(HAVE_OPENGL)
install( TARGETS plot EXPORT DESTINATION ${INSTALL_TARGETS_DEFAULT_ARGS} )
