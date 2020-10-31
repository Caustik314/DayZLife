modded class LoadingScreen
{
	void LoadingScreen(DayZGame game)
	{
		m_ImageLogoMid.LoadImageFile(0, "DayZLife/layout/MainMenu/DayZLifeLogo.edds");
		m_ImageLogoMid.SetImage(0);
		m_ImageLogoMid.SetFlags(WidgetFlags.SOURCEALPHA | WidgetFlags.BLEND | WidgetFlags.STRETCH);
		m_ImageLogoCorner.LoadImageFile(0, "DayZLife/layout/MainMenu/DayZLifeLogo.edds");
		m_ImageLogoCorner.SetImage(0);
		m_ImageLogoCorner.SetFlags(WidgetFlags.SOURCEALPHA | WidgetFlags.BLEND | WidgetFlags.STRETCH);
				
		m_ImageLogoMid.Show(false);
		m_ImageLogoCorner.Show(false);
		
		m_ImageWidgetBackground.LoadImageFile(0, "DayZLife/layout/MainMenu/DayZLifeLogo.edds");
		m_ImageWidgetBackground.SetImage(0);
		m_ImageWidgetBackground.SetFlags(WidgetFlags.SOURCEALPHA | WidgetFlags.BLEND | WidgetFlags.STRETCH);
		m_ImageWidgetBackground.GetPos(back_x, back_y);

		m_ModdedWarning.Show(false);	
		
		float x, y;
		m_ImageLogoMid.GetPos(x, y);
		m_ModdedWarning.SetPos(x, y - 40);
		
		int s_x, s_y;
		GetScreenSize(s_x, s_y);
		m_ProgressLoading.SetSize(s_x, 6);
	}
	
	override void Show()
	{
		super.Show();
		m_ImageLogoMid.Show(false);
		m_ImageLogoCorner.Show(false);
	}
	
	float back_x, back_y;

	// todo
	void FindAnUpdateLoopSoYOuCanRunThis(float val)
	{
		super.SetProgress(val);
		
		float time_delta = m_DayZGame.GetTickTime() - m_LastProgressUpdate;
		
		float duration = 10;
		time_delta /= 1000;
		
		float normalized_time = (1 / duration) * time_delta;
		normalized_time = Math.Clamp(normalized_time, 0, 1);
		float pos_x = Math.Lerp(back_x, back_x + 50, normalized_time);
		float pos_y = Math.Lerp(back_y, back_y + 50, normalized_time);
		Print(pos_x);
		Print(pos_y);
		m_ImageWidgetBackground.SetPos(pos_x, pos_y);
		m_ImageWidgetBackground.Update();
	}
};
