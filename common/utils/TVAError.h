#ifndef TVAERROR_H
#define TVAERROR_H

#include <QtCore/QString>

//! Namespace contains service classes
//! Error Codes
//
static const int S_OK = 0;
static const int S_FALSE = 1;
static const int E_FAIL = -1;
static const int E_OUTOFMEMORY = -1000;

//! Class for errors
class Error
{
    public:

        //! Copy contructor
        Error(const Error &ref);
        /*! Constructor of error
         \param aczFile name of file, where error occurred
         \param iLine number of the line in src code
         \param sReason the reason of an error
         \param iRes    the cod of an error
         */
        Error(const char* file, int line, const QString& reason, int result = E_FAIL);
        //! Destructor
        virtual ~Error(void);
        //! Get reason
        /*!
         \return reason
         */
        const QString& reason() const;
        //! Get error code
        /*!
         \return error code
         */
        int code() const;
        //! Get file name where error occurred
        /*!
         \return file name where error occurred
         */
        const QString& file() const;
        //! Get line in src code
        /*!
         \return line in src code
         */
        int line() const;
        //! Add reason
        /*!
         \param sReason additional reason
         */
        void setReason(const QString& sReason);

    private:
        QString mFile;
        int mLine;
        QString mReason;
        int mCode;
};

/** Macros for throwing */
#define TVA_THROW(description, result) {throw Error(__FILE__, __LINE__, description, result);}

#define TVA_CHECK(condition, description, code) \
    {if(!(condition)) TVA_THROW(description, code)}

#define TVA_RETHROWALL \
    catch (const Error&) { throw; } \
    catch (...) { TVA_THROW("CATCH(...)", E_FAIL); }

#define TVA_CATCHALL_TRY try{try

#define TVA_CATCHALL(name) \
    TVA_RETHROWALL \
    } \
    catch(Error& name)

#define TVA_TRAPALL \
    TVA_RETHROWALL \
        }catch(const Error&)

#endif //TVAERROR_H
