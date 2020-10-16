#ifndef PRINTUTILS_H
#define PRINTUTILS_H

class QPrinter;
    /**
    * @brief Functions for simpole widget printing
    *
    * @fn printWidget
    * @param widget
    * @param printer
    */
bool printWidget(QWidget* widget, QPrinter *printer);

#endif
