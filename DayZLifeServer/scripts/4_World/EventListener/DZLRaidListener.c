class DZLRaidListener
{
    void DZLRaidListener() {
        GetDayZGame().Event_OnRPC.Insert(HandleEventsDZL);
    }

    void ~DZLRaidListener() {
        GetDayZGame().Event_OnRPC.Remove(HandleEventsDZL);
    }

    void HandleEventsDZL(PlayerIdentity sender, Object target, int rpc_type, ParamsReadContext ctx) {
        if (rpc_type == DAY_Z_LIFE_RAID_DOOR) {
            autoptr Param4<PlayerBase, Building, int, EntityAI> paramRaidDoor;
            if (ctx.Read(paramRaidDoor)){
                DZLHouse dzlHouse = new DZLHouse(paramRaidDoor.param2);
                dzlHouse.UnLookDoor(paramRaidDoor.param3);
                DZLLockedHouses houses = new DZLLockedHouses();
                houses.Remove(dzlHouse);

                paramRaidDoor.param4.SetHealth(0);
            }
        } else if (rpc_type == DAY_Z_LIFE_GET_DZL_BUILDING_RAID_DOOR) {
            autoptr Param2<PlayerBase, Building> paramRaidDoorDZLBuilding;
            if (ctx.Read(paramRaidDoorDZLBuilding)){
                PlayerBase raider = paramRaidDoorDZLBuilding.param1;
                Building building = paramRaidDoorDZLBuilding.param2;
                DZLHouse dzlHouseRaid = new DZLHouse(building);

                if (dzlHouse.HasAlarmSystem() && dzlHouseRaid.GetHouseAlarm().message) {
                    array<Man> players = new array<Man>;
                    GetGame().GetPlayers(players);

                    if (players) {
                        foreach(Man player: players) {
							PlayerBase currentPlayer = PlayerBase.Cast(player);
                            if (raider == currentPlayer) continue;

                            if (dzlHouseRaid.IsOwner(currentPlayer)) {
                                DZLSendMessage(currentPlayer.GetIdentity(), dzlHouseRaid.GetHouseAlarm().GetMessage(raider, dzlHouseRaid));
                            }
                        }
                    }
                }

                GetGame().RPCSingleParam(raider, DAY_Z_LIFE_GET_DZL_BUILDING_RAID_DOOR_RESPONSE, new Param1<ref DZLHouse>(new DZLHouse(building)), true, sender);
            }
        }
    }
}
