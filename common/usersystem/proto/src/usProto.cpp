#include "usProto.h"

namespace UserSystem
{

void registerUserSystemMetatypes()
{
    qRegisterMetaTypeStreamOperators< LoginAttempt >();
    qRegisterMetaTypeStreamOperators< LoginResult >();
    qRegisterMetaTypeStreamOperators< LogoutAttempt >();
    qRegisterMetaTypeStreamOperators< ChangePasswordAttempt >();
    qRegisterMetaTypeStreamOperators< SetPasswordAttempt >();
    qRegisterMetaTypeStreamOperators< SetDescriptionAttempt >();
    qRegisterMetaTypeStreamOperators< AddUserAttempt >();
    qRegisterMetaTypeStreamOperators< RemoveUserAttempt >();
    qRegisterMetaTypeStreamOperators< AddUserToGroupAttempt >();
    qRegisterMetaTypeStreamOperators< RemoveUserFromGroupAttempt >();
    qRegisterMetaTypeStreamOperators< SetUserGroupAttempt >();
    qRegisterMetaTypeStreamOperators< UserListAttempt >();
    qRegisterMetaTypeStreamOperators< UserListResult >();
    qRegisterMetaTypeStreamOperators< GroupListAttempt >();
    qRegisterMetaTypeStreamOperators< GroupListResult >();
    qRegisterMetaTypeStreamOperators< UserResult >();
    qRegisterMetaTypeStreamOperators< RequestAttempt >();
    qRegisterMetaTypeStreamOperators< RequestResult >();
}

} // namespace
