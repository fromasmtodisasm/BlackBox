-- different usefull development functions

function retrigger_value( name )
	local var
	if Game:GetVariable(name) == 1 then
		var = 0
	else
		var = 1
	end
	Game:SetVariable(name, var)
end

function load1()
	Game:LoadPlayerPos(0, "BloomTest")
	Console:ExecuteString("r_cam_w 500")
	Console:ExecuteString("r_cam_h 500")
end

function load2()
	Game:LoadPlayerPos(1, "BloomTest")
	Console:ExecuteString("r_cam_w 1000")
	Console:ExecuteString("r_cam_h 1000")
end

function setFog( r,g,b )
	Game:SetVariable("fogR", r)	
	Game:SetVariable("fogG", g)	
	Game:SetVariable("fogB", b)	
end

function setvar(n,v)
	-- Game:SetVariable(n,v)	
	Console:ExecuteString(n .. " " .. v)
end

function addcommand( name, src, help )
	Game:AddCommand(name, src, 0, help)
end



function set2dvec(nx,ny, x,y )
	setvar(nx,x)	
	setvar(ny,y)
end

function execstr( str )
	Console:ExecuteString(str)
end


function init()
	Console:CreateKeyBind("ctrl_r", "shader reload");
	Console:CreateKeyBind("ctrl_d", [[#retrigger_value("r_displayinfo")]]);
	--Console:CreateKeyBind("q", "#Game:Stop()");
	Console:CreateKeyBind("ctrl_w", "toogle_viewport_drag");

	Console:CreateKeyBind("ctrl_num1", "#load1()");
	Console:CreateKeyBind("ctrl_num2", "#load2()");

	Console:CreateKeyBind("ctrl_a", [[#retrigger_value("r_aspect")]]);
	--Console:CreateKeyBind("c", "Console:Show(false)")  
	Console:CreateKeyBind("ctrl_b", "#setFog(0,0,0)") 
	Console:CreateKeyBind("ctrl_g", "#setFog(0,0.01,0.01)")  

	Console:CreateKeyBind("xi_b", "quit")  
	--Console:CreateKeyBind("xi_back", "#Game:gotoMenu()")  


	--addcommand("set_cs", [[set2dvec("r_cam_w", "r_cam_h", %1, %2)]], "Set size of camera")
	--addcommand("set_ws", [[set2dvec("r_Width", "r_Height", %1, %2)]], "Set size of window")
	addcommand("relaunch", [[Game:SendMessage("Relaunch")]], "Relaunch game")
	addcommand("screen_shot", [[System:ScreenShot("screen_shots/ss.png")]]) 

	addcommand("level", [[Game:LoadLevel(%1)]]) 

	addcommand("connect", [[Client:Connect(%1,%2)]]) 
	addcommand("send", 
	[[
		if Client:Send(%1) then
			Console:PrintLine(Client:Response())
		end
	]]) 

	Console:CreateKeyBind("f5", "screen_shot") 
	Console:CreateKeyBind("ctrl_s", "scene load test") 
	Console:CreateKeyBind("ctrl_p", "reload_scripts") 
	--Console:CreateKeyBind("ctrl_c", "#Game:gotoMenu()") 
	Console:CreateKeyBind("f7", "#Game:gotoGame()") 
	Console:CreateKeyBind("ctrl_q", "#Game:Quit()") 


	setFog(0,0,0)

	setvar("MouseSenitivity", 2)

	Console:Show(true)
	--Console:PrintLine("!!!!!!!!!!!!!!!!!!!!!!!!!!!!")
end

Test = {

}

function Test:OnInit( )
	setFog(0, 0.01, 0.01)
end


init()
Console:PrintLine("TestChanges")

