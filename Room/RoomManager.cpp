#include "RoomManager.h"

VOID RoomManager::Initialize()
{
	this->rooms.clear();
}

VOID RoomManager::Release()
{
	this->rooms.clear();
}

VOID RoomManager::AddRoom(Room* room)
{
	this->rooms.push_back(room);
}

INT RoomManager::GetRoomCount()
{
	return (INT)this->rooms.size();
}

vector<Room*>& RoomManager::GetRooms()
{
	return this->rooms;
}

Room* RoomManager::FindRoom(wstring roomID)
{
	for (INT i = 0; i < (INT)this->rooms.size(); i++)
	{
		if (rooms[i]->GetRoomID() == roomID)
			return rooms[i];
	}

	return NULL;
}

VOID RoomManager::RemoveRoom(Room* room)
{
	for (INT i = 0; i < (INT)this->rooms.size(); i++)
	{
		if (this->rooms[i]->GetRoomID() == room->GetRoomID())
		{
			this->rooms.erase(this->rooms.begin() + i);
			room->Release();
		}
	}
}

BOOL RoomManager::ExistRoomName(wstring roomName)
{
	for (INT i = 0; i < (INT)this->rooms.size(); i++)
	{
		if (rooms[i]->GetName() == roomName)
			return TRUE;
	}
	return FALSE;
}

Room* RoomManager::GetJoinRoom(RoomPlayer* roomPlayer)
{
	return this->joinRoomMap[roomPlayer];
}

VOID RoomManager::MapRoomToRoomPlayer(RoomPlayer* roomPlayer, Room* room)
{
	this->joinRoomMap.insert(make_pair(roomPlayer, room));
}

VOID RoomManager::RemoveJoinRoom(RoomPlayer* roomPlayer)
{
	this->joinRoomMap.erase(roomPlayer);
}