class DZLPlayer {
    string fileName;
    int money = 0;
    int bank = 0;
	
	ref array<ref DZLLicence> licences;

    void DZLPlayer(string playerId) {
        fileName = playerId + ".json";
        if (!Load()) {
            if (DAY_Z_LIFE_DEBUG) {
                money = 100000;
            }

            DZLPlayerIdentities idents = new DZLPlayerIdentities;
            idents.AddPlayer(playerId);
			licences = new array<ref DZLLicence>;
			
            Save();
        }
    }
	
	void AddMoneyToPlayer(int moneyCount) {
        if (!DayZGame().IsClient()) {
			money += moneyCount;
		    Save();
		}
    }
	void AddMoneyToPlayerBank(int moneyCount) {
        if (!DayZGame().IsClient()) {
			bank += moneyCount;
		    Save();
		}
    }

    void PlayerHasDied() {
        money = 0;
        Save();
    }

    void TransferFromPlayerToOtherPlayer(DZLPlayer playerTarget) {
        playerTarget.AddMoneyToPlayer(money);
        money = 0;
        Save();
    }
	
	void DepositMoneyToOtherPlayer(DZLPlayer playerTarget, int moneyToTransfer) {
		playerTarget.AddMoneyToPlayerBank(moneyToTransfer);
		
		if (money > 0) {
			if (money < moneyToTransfer) {
				moneyToTransfer -= money;
				money = 0;
			} else {
				money -= moneyToTransfer;
				moneyToTransfer = 0;
			}
		}
		
		if (moneyToTransfer > 0) {
			bank -= moneyToTransfer;
		}
		
		Save();
	}
	
	string CanBuyLicence(notnull DZLLicence licenceToBuy){
		DebugMessageDZL("licenceToBuy.price" + licenceToBuy.price.ToString());
		if(money < licenceToBuy.price) return "#not_enough_money";
		if(HasLicense(licenceToBuy)) return "#your_already_have_the_licence";
		if(!licenceToBuy.dependencyLicence) return "";
		
		foreach(DZLLicence licence: licences){
			if(licence.name == licenceToBuy.dependencyLicence){
				return "";
			}
		}
		return "#you_have_not_the_dependency_licence";
	}
	
	bool HasLicense(DZLLicence licence) {
		return -1 != licences.Find(licence);
	}
	
	void BuyLicence(DZLLicence licenceToBuy){
		money -= licenceToBuy.price;
		licences.Insert(licenceToBuy);
		Save();
	}

    private bool Load(){
        if (GetGame().IsServer() && FileExist(DAY_Z_LIFE_SERVER_FOLDER_DATA_PLAYER + fileName)) {
            JsonFileLoader<DZLPlayer>.JsonLoadFile(DAY_Z_LIFE_SERVER_FOLDER_DATA_PLAYER + fileName, this);
            return true;
        }
        return false;
    }

    private void Save(){
        if (GetGame().IsServer()) {
            CheckDZLDataSubPath(DAY_Z_LIFE_SERVER_FOLDER_DATA_PLAYER);
            JsonFileLoader<DZLPlayer>.JsonSaveFile(DAY_Z_LIFE_SERVER_FOLDER_DATA_PLAYER + fileName, this);
        }
    }
}