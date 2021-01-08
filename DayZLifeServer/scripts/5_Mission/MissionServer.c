modded class MissionServer {

    private ref DZLEventManager manager;
    private ref DZLBuilderManager builderManager;
	private ref Timer paycheckTimer;
	private ref DZLPayCheckController paycheckController;

	void MissionServer() {
        DebugMessageDZL("Load DayZLifeServer");
        paycheckTimer = new Timer;
	}

	override void OnInit() {
	    super.OnInit();
		Object databaseObject = DZLObjectFinder.GetObjectsAtStatic(DZLDatabase.POSITION, DZLDatabase.POSITION);
		DZLDatabase database;
		if (databaseObject) {
			DZLDatabase database = DZLDatabase.Cast(databaseObject);
			DZLDatabaseLayer.Get().SetDatabase(database);

            manager = new DZLEventManager;
            builderManager = new DZLBuilderManager;
            paycheckController = new DZLPayCheckController;


            DZLLockedHouses.OnServerStart();
            paycheckTimer.Run(60, paycheckController, "Check", null, true);
            DZLLicenceCheck.Get();
            builderManager.Create();
        }
    }

	override PlayerBase OnClientNewEvent(PlayerIdentity identity, vector pos, ParamsReadContext ctx){
		PlayerBase player = super.OnClientNewEvent(identity, pos, ctx);
		
        GetGame().RPCSingleParam(player, DAY_Z_LIFE_NEW_SPAWN_CLIENT, new Param1<string>(""), true, identity);
		
		return player;
	}
}
