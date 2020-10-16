get_filename_component(editableheader_CURRENTDIR ${CMAKE_CURRENT_LIST_FILE} PATH)

include_directories("${editableheader_CURRENTDIR}")

list(APPEND SOURCES "${editableheader_CURRENTDIR}/EditableHeader.cpp"
  "${editableheader_CURRENTDIR}/EditHeaderDialog.cpp"
)

list(APPEND HEADERS "${editableheader_CURRENTDIR}/EditHeaderInterface.h"
)

list(APPEND Q_HEADERS "${editableheader_CURRENTDIR}/EditableHeader.h"
  "${editableheader_CURRENTDIR}/EditHeaderDialog.h"
)

list(APPEND FORMS "${editableheader_CURRENTDIR}/EditHeaderDialog.ui")
