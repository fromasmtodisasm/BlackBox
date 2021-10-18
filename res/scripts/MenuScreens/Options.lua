--
-- options menu page
--

UI.PageOptions=
{
	GUI=
	{
		GameOptions=
		{
			skin = UI.skins.TopMenuButton,
			width = 146,
			
			greyedcolor = UI.szTabAdditiveColor,
			greyedblend = UIBLEND_ADDITIVE,
			
			text = Localize("GameOptions"),
			
			tabstop = 101,
		},
		
		VideoOptions=
		{
			skin = UI.skins.TopMenuButton,
			left = 345, width = 146,
			
			greyedcolor = UI.szTabAdditiveColor,
			greyedblend = UIBLEND_ADDITIVE,
			
			text = Localize("VideoOptions"),
			
			tabstop = 102,
		},
		
		SoundOptions=
		{
			skin = UI.skins.TopMenuButton,
			left = 490, width = 146,
			
			greyedcolor = UI.szTabAdditiveColor,
			greyedblend = UIBLEND_ADDITIVE,
			
			text = Localize("SoundOptions"),
			
			tabstop = 103,
		},

		ControlOptions=
		{
			skin = UI.skins.TopMenuButton,
			left = 635, width = 145,
			
			greyedcolor = UI.szTabAdditiveColor,
			greyedblend = UIBLEND_ADDITIVE,
			
			text = Localize("ControlOptions"),
			
			tabstop = 104,
		},
		
		OnActivate = function(Sender)
			Sender.ControlOptions.OnCommand = UI.PageOptions.CommonOnCommand;
			Sender.SoundOptions.OnCommand = UI.PageOptions.CommonOnCommand;
			Sender.VideoOptions.OnCommand = UI.PageOptions.CommonOnCommand;
			Sender.GameOptions.OnCommand = UI.PageOptions.CommonOnCommand;
			
			Sender.ControlOptions.skin.OnLostFocus(Sender.ControlOptions);
			Sender.SoundOptions.skin.OnLostFocus(Sender.SoundOptions);
			Sender.VideoOptions.skin.OnLostFocus(Sender.VideoOptions);
			Sender.GameOptions.skin.OnLostFocus(Sender.GameOptions);
			
			Sender.GameOptions:OnCommand(Sender.GameOptions);
		end
	},
	
	CommonOnCommand = function(Sender)
		UI:EnableWidget("GameOptions", "Options");
		UI:EnableWidget("VideoOptions", "Options");
		UI:EnableWidget("SoundOptions", "Options");
		UI:EnableWidget("ControlOptions", "Options");
		UI:DisableWidget(Sender);
		UI:DeactivateScreen("GameOptions");
		UI:DeactivateScreen("VideoOptions");
		UI:DeactivateScreen("SoundOptions");
		UI:DeactivateScreen("ControlOptions");
		UI:ActivateScreen(Sender:GetName());
	end
		
	------------------------------------------------------------------------	
};

AddUISideMenu(UI.PageOptions.GUI,
{
	{ "MainMenu", Localize("MainMenu"), "$MainScreen$", 0},
});
UI:CreateScreenFromTable("Options",UI.PageOptions.GUI);