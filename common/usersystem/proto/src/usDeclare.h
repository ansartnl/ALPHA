#ifndef DECLARE_H
#define DECLARE_H

#include <QtCore/QFlags>

namespace UserSystem
{

enum Permission {
    None    = 0x00000000,

    Read    = 0x00000001,
    Write   = 0x00000002,
    Rewrite = 0x00000004,
    List    = 0x00000008,
    Execute = 0x00000010,

    ReadAttributes      = 0x00000020,
    WriteAttributes     = 0x00000040,
    ChangeAttributes    = 0x00000080,

    ReadExAttributes    = 0x00000100,
    WriteExAttributes   = 0x00000200,
    ChangeExAttributes  = 0x00000400,

    ReadPermissions     = 0x00000800,
    AddPermissions      = 0x00001000,
    RemovePermissions   = 0x00002000,

    AddUser         = 0x00004000,
    RemoveUser      = 0x00008000,
    SetPassword     = 0x00010000,
    ChangePassword  = 0x00020000,
    ChangeUserInfo  = 0x00040000,

    Reserved1   = 0x00080000,
    Reserved2   = 0x00100000,
    Reserved3   = 0x00200000,
    Reserved4   = 0x00400000,

    User1   = 0x00800000,
    User2   = 0x01000000,
    User3   = 0x02000000,
    User4   = 0x04000000,
    User5   = 0x08000000,
    User6   = 0x10000000,
    User7   = 0x20000000,
    User8   = 0x40000000,

    Root    = 0x80000000,

    All     = 0x7FFFFFFF,
    AllRoot = 0xFFFFFFFF
};
typedef QFlags<Permission> Permissions;
Q_DECLARE_OPERATORS_FOR_FLAGS(Permissions)

namespace Request
{

enum Action {
    Logout,
    ChangePassword,
    SetPassword,
    SetDescription,
    AddUser,
    RemoveUser,
    AddUserToGroup,
    RemoveUserFromGroup,
    SetUserGroup
};

enum Result {
    Ok,
    NoUser,
    WrongPassword,
    NotEnoughPrivileges,
    AlreadyExists,
    NotExists,
    RemoveSelf
};

} // namespace

namespace GroupSignatures
{

const char s_signature[] = "f63g5hg9";
const char s_group_open[] = "IG17pU";
const char s_group_close[] = "IJF17pU";
const char s_name_open[] = "IG114eU";
const char s_name_close[] = "IJF114eU";
const char s_permissions_open[] = "IG116sU";
const char s_permissions_close[] = "IJF116sU";
const char s_users_open[] = "IG121sU";
const char s_users_close[] = "IJF121sU";
const char s_user_open[] = "IG121rU";
const char s_user_close[] = "IJF121rU";

} // namespace

namespace GroupsSignatures
{

const char s_signature[] = "19t4e93m";
const char s_groups_open[] = "IGN7sV";
const char s_groups_close[] = "IbGN7sV";
const char s_group_open[] = "IGN7pV";
const char s_group_close[] = "IbGN7pV";

} // namespace

namespace UserSignatures
{

const char s_signature[] = "fr426jgs";
const char s_user_open[] = "IR121rP";
const char s_user_close[] = "IJU121rP";
const char s_name_open[] = "IR114eP";
const char s_name_close[] = "IJU114eP";
const char s_pass_open[] = "IR116sP";
const char s_pass_close[] = "IJU116sP";
const char s_desc_open[] = "IR14nP";
const char s_desc_close[] = "IJU14nP";
const char s_groups_open[] = "IR17sP";
const char s_groups_close[] = "IJU17sP";
const char s_group_open[] = "IR17pP";
const char s_group_close[] = "IJU17pP";

} // namespace

namespace UsersSignatures
{

const char s_signature[] = "hj6q2mto";
const char s_users_open[] = "IUN21sH";
const char s_users_close[] = "IzUN21sH";
const char s_user_open[] = "IUN21rH";
const char s_user_close[] = "IzUN21rH";

} // namespace

} // namespace

#endif /* DECLARE_H */
