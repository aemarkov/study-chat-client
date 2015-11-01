#include "Chat.h"

ChatOnlyInfo::ChatOnlyInfo(const Chat& chat)
{
	Id = chat.Id;
	//TODO: Возможно, нужно глубокое копирвоание
}