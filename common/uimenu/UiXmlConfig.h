#ifndef _UI_IO_H__
#define _UI_IO_H__

#include "UiModel.h"

#include <QtCore/QStringList>
#include <QtGui/QIcon>
#include <QtGui/QAction>


namespace Uim {

namespace UiXmlConfig
{
    //! Read configuration file in xml format containing structure of
    //! menu bar and toolbar.
    /*!
        \return Empty string if file was successfully read, error string otherwise.
    */
    QString read(const QString &path, UiModel &model);

    //! Save structure of menu bar and toolbar into the configuration file
    //! in xml format.
    /*!
        \return Empty string if file was successfully written, error string otherwise.
    */
    QString write(const QString &path, const UiModel &model);
}

}   // namespace Uim

#endif // _UI_IO_H__
