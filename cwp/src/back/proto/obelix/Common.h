#ifndef COMMON_H
#define COMMON_H

//! Namespace contains classes to work with obelix protocol
namespace obelix
{
    //! Protocol name
    const char XML_PROTO[] = "obelix";
    /*! \name Error codes in responses */
    //@{
    const char XMLERR_OK = 0;        /*!< no errors */
    const char XMLERR_ERROR = 1;    /*!< general error */
    //@}
}

#endif // COMMON_H
