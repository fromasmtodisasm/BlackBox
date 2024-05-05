-- : ./MenuScreens/Options/BindWaitForKey.lua
--
-- control options menu page
--

UI.PageWaitForKey =
{
	GUI =
	{
		darkeningpane =
		{
			classname = "static",
			rect = "0 0 800 600",
			color = "0 0 0 128",
			zorder = 50,
		},

		darkerpane =
		{
			classname = "static",
			left = 200,
			top = 140,
			width = 580,
			height = 259,
			color = "0 0 0 128",
			zorder = 55,


			halign = UIALIGN_CENTER,
			fontsize = 18,
		},

		commandtext =
		{
			classname = "static",
			left = 200,
			top = 140,
			width = 580,
			height = 259,
			color = "0 0 0 128",
			zorder = 55,
			halign = UIALIGN_CENTER,
			text = Localize("PressAKeyToBind"),

			fontsize = 16,
		},

		-------------------------------------------------------------------------

		OnActivate = function(Sender)
			UI.PageWaitForKey.GUI.commandtext:SetText(Localize("PressAKeyToBind") .. " " .. UI.PageWaitForKey.Action
				.desc);
		end,

		OnUpdate = function(self)
			local PressedKey = Input:GetXKeyPressedName();
			if PressedKey then
				System:Log("PressedKey: " .. tostring(PressedKey))
			end
			if (PressedKey and (tostring(PressedKey) ~= "esc") and (tostring(PressedKey) ~= System:GetConsoleKeyName()) and (tostring(PressedKey) ~= "pause")) then
				Input:BindActionMultipleMaps(UI.PageWaitForKey.Action.name, PressedKey, UI.PageWaitForKey.iBindNumber);
				System:Log("Bound key: " .. tostring(PressedKey) ..
					" to action: " ..
					UI.PageWaitForKey.Action.name .. " on bind number: " .. UI.PageWaitForKey.iBindNumber);

				UI.PageOptionsControl:FillBindList();

				UI:DeactivateScreen(self);
			end
		end,
	},

	------------------------------------------------------------------------

	Action = nil, -- stores the Action {desc="", name=""}
}

-------------------------------------------------------------------------
function UI.PageWaitForKey:SetLabel(Action, iNumber)
	self.Action = Action;
	self.iBindNumber = iNumber;
end

UI:CreateScreenFromTable("BindWaitForKey", UI.PageWaitForKey.GUI);
