-- : ./MenuScreens/Options/Sound.lua
----------------------------------------------------------------------------------------------

UI.PageOptionsSound=
{
	SpeakerConfigIndex = {},
	SampleRateIndex = {},
	GUI=
	{

		soundfxvolumetext=
		{
			skin = UI.skins.Label,
			left = 200, top = 176,
			width = 112,

			text=Localize("SoundVolume"),
		},

		soundfxvolume=
		{
			skin = UI.skins.HScrollBar,

			left = 320, top = 176,
			width = 322, height = 24,

			tabstop = 1,

			user =
			{
				soundSfxCue = Sound:LoadSound( "Sounds/Menu/Click.wav",SOUND_UNSCALABLE),
			},

			OnChanged = function( Sender )
				setglobal( "s_SFXVolume", Sender:GetValue() );	-- 0..1
				local sound = UI.PageOptionsSound.GUI.soundfxvolume.user.soundSfxCue;
				if( sound ~= nil ) then
					Sound:SetSoundVolume( sound, getglobal( "s_SFXVolume" ) * 255.0 );
					Sound:SetSoundLoop( sound, 0 );
					Sound:PlaySound( sound );
				end;
			end,
		},


		OnActivate= function(Sender)
			UI.PageOptionsSound.GUI.soundfxvolume:SetValue(getglobal("s_SFXVolume"));

		end,
	},

	RefreshPage = function( self )
		GotoPage( "Options" );
		UI.PageOptions.GUI.VideoOptions.OnCommand( UI.PageOptions.GUI.SoundOptions );
	end,

	ApplySettings = function( self )
	end,
}

-------------------------------------------------------------------------
function UI.PageOptionsSound.ResetToDefaults()
	UI.PageOptionsSound.GUI.musicvolume:SetValue(0.6);
	UI.PageOptionsSound.GUI.soundfxvolume:SetValue(1.0);
	UI.PageOptionsSound.GUI.speakersetup:SelectIndex(2);
	UI.PageOptionsSound.GUI.doppler:SetChecked(0);
	UI.PageOptionsSound.GUI.hwmix:SetChecked(1);
	UI.PageOptionsSound.GUI.eax:SetChecked(1);
	UI.PageOptionsSound.GUI.music:SetChecked(1);
	UI.PageOptionsSound.GUI.musicquality:SelectIndex(2);
	UI.PageOptionsSound.GUI.speakersetup:SelectIndex(2);
	UI.PageOptionsSound.GUI.compatiblemode:SetChecked(0);

	UI.PageOptionsSound.GUI.musicvolume:OnChanged(UI.PageOptionsSound.GUI.musicvolume);
	UI.PageOptionsSound.GUI.soundfxvolume:OnChanged(UI.PageOptionsSound.GUI.soundfxvolume);
	UI.PageOptionsSound.GUI.hwmix:OnChanged(UI.PageOptionsSound.GUI.hwmix);
	UI.PageOptionsSound.GUI.music:OnChanged(UI.PageOptionsSound.GUI.music);
	UI.PageOptionsSound.GUI.musicquality:OnChanged(UI.PageOptionsSound.GUI.musicquality);
end


UI:CreateScreenFromTable("SoundOptions",UI.PageOptionsSound.GUI);
