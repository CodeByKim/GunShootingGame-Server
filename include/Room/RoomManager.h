#pragma once
#include "Library.h"
#include "Room.h"
#include "RoomPlayer.h"

class RoomManager : public Singleton<RoomManager>
{
private:
	vector<Room*> rooms;
	map<RoomPlayer*, Room*> joinRoomMap;

public:
	VOID Initialize();
	VOID Release();

	VOID AddRoom(Room* room);
	Room* FindRoom(wstring roomID);
	VOID RemoveRoom(Room* room);

	BOOL ExistRoomName(wstring roomName);
	INT GetRoomCount();
	vector<Room*>& GetRooms();

	Room* GetJoinRoom(RoomPlayer* roomPlayer);
	VOID MapRoomToRoomPlayer(RoomPlayer* roomPlayer, Room* room);
	VOID RemoveJoinRoom(RoomPlayer* roomPlayer);
};