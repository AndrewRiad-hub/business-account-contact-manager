#ifndef ACTION_H
#define ACTION_H

#include "Account.h"
#include "Contact.h"

enum class ActionType
{
    AddedAccount,
    DeletedAccount,
    AddedContact,
    DeletedContact
};

struct Action
{
    ActionType type;
    Account account;
    int accountID;
    int contactID;
    Contact contact;
};

#endif
