class DZLBankRaidListener : Managed 
{
    ref DZLBankingConfig config;
    float time = 300;
	int timeHappened = 0;
    private ref Timer raidTimer;
    private PlayerBase playerWhoStartedRaid;

    void DZLBankRaidListener() {
        config = DZLConfig.Get().bankConfig;
        time = config.raidTimeBankInSeconds;
        GetDayZGame().Event_OnRPC.Insert(Start);
        raidTimer = new Timer;
    }

    void ~DZLBankRaidListener() {
       GetDayZGame().Event_OnRPC.Remove(Start);
    }

	void Start(PlayerIdentity sender, Object target, int rpc_type, ParamsReadContext ctx) {
		if (!DZLLicenceCheck.Get().HasActiveLicence(sender)) return;
        if (rpc_type == DAY_Z_LIFE_START_BANK_RAID) {
            autoptr Param1<PlayerBase> param;
            if (ctx.Read(param)){
				DZLBank bank = DZLDatabaseLayer.Get().GetBank();
				if (!playerWhoStartedRaid) {
					playerWhoStartedRaid = param.param1;
					bank.StartRaid();
					timeHappened = 0;
					DZLSendMessage(null, "#bank_rob_was_started");
			        raidTimer.Run(1, this, "Finish", null, true);
				} else {
				    DZLSendMessage(sender, "#raid_allready_started");
				}
			}
		}
    }

    void Finish() {
		if (time > timeHappened) {
            if (!isInNearOfBankAndLocationIsEnabled()) {
                playerWhoStartedRaid = null;
                raidTimer.Stop();
                DZLBank bank_cancel = DZLDatabaseLayer.Get().GetBank();
                bank_cancel.CancelRaid();
                DZLSendMessage(null, "#bank_rob_was_canceled");
                return;
            }
			
		    timeHappened++;
			return;
		}

        raidTimer.Stop();
		DZLBank bank = DZLDatabaseLayer.Get().GetBank();
        bank.RaidIsFinished();
        DZLPlayer dzlPlayer = DZLDatabaseLayer.Get().GetPlayer(playerWhoStartedRaid.GetIdentity().GetId());

        int money = bank.PlayerRaidBank(DZLDatabaseLayer.Get().GetPlayer(playerWhoStartedRaid.GetIdentity().GetId()), config.percentOfMoneyWhenRaid);
        DZLSendMessage(null, "#bank_rob_was_successful " + money.ToString());
		bank = DZLDatabaseLayer.Get().GetBank();
        GetGame().RPCSingleParam(playerWhoStartedRaid, DAY_Z_LIFE_PLAYER_BANK_DATA_RESPONSE, new Param1<ref DZLBank>(bank), true);
        playerWhoStartedRaid = null;
        array<Man> _players = new array<Man>;
        GetGame().GetPlayers(_players);

        if (_players) {
            foreach(Man _player: _players) {
                GetGame().RPCSingleParam(_player, DAY_Z_LIFE_PLAYER_DATA_RESPONSE, new Param1<ref DZLPlayer>(DZLDatabaseLayer.Get().GetPlayer(_player.GetIdentity().GetId())), true, _player.GetIdentity());
            }
        }
    }

    private bool isInNearOfBankAndLocationIsEnabled() {
        if (!playerWhoStartedRaid) {
            return false;
        }
        vector playerPosition = playerWhoStartedRaid.GetPosition();
        if (!playerPosition) {
            return false;
        }
        foreach(DZLBankingPosition position: config.positionOfBankingPoints) {
            if (vector.Distance(position.position, playerPosition) <= config.maximumRaidDistanceToBank){
                return position.raidIsEnabled;
            }
        }
        return false;
    }

}
