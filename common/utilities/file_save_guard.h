#pragma once

#include <QFile>

#include "utilities_global.h"


class QString;


namespace utilities {

class UTILITIES_EXPORT FileSaveGuard {
public:
    FileSaveGuard(const QString& filename);
    ~FileSaveGuard();

private:
    Q_DISABLE_COPY(FileSaveGuard)

    QFile temp_file_;
};


} // namespace utilities
