#include "Chat.h"

ChatOnlyInfo::ChatOnlyInfo(const Chat& chat)
{
	Id = chat.Id;
	Users = chat.Users;
	//TODO: Возможно, нужно глубокое копирвоание
}