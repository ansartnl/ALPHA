#include "file_save_guard.h"

#include <QString>


namespace utilities {


FileSaveGuard::FileSaveGuard(const QString& filename)
    : temp_file_(filename) {
    if (temp_file_.exists()) {
        QString tmp_filename = filename + "-";
        while (!temp_file_.rename(tmp_filename))
            tmp_filename += "-";
    } else {
        temp_file_.setFileName("");
    }
}

FileSaveGuard::~FileSaveGuard() {
    if (temp_file_.exists())
        temp_file_.remove();
}


} // namespace utilities