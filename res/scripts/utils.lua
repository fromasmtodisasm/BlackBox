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
	Console:CreateKeyBind("r", "shader reload");
	Console:CreateKeyBind("d", [[@retrigger_value("r_displayinfo")]]);
	Console:CreateKeyBind("q", "@Game:Stop()");
	Console:CreateKeyBind("w", "toogle_viewport_drag");

	Console:CreateKeyBind("num1", "@load1()");
	Console:CreateKeyBind("num2", "@load2()");

	Console:CreateKeyBind("a", [[@retrigger_value("r_aspect")]]);
	Console:CreateKeyBind("b", "@setFog(0,0,0)") 
	Console:CreateKeyBind("g", "@setFog(0,0.01,0.01)")  


	addcommand("set_cs", [[set2dvec("r_cam_w", "r_cam_h", %1, %2)]], "Set size of camera")
	addcommand("set_ws", [[set2dvec("r_Width", "r_Height", %1, %2)]], "Set size of window")
	addcommand("relaunch", [[Game:SendMessage("Relaunch")]], "Relaunch game")
	addcommand("screen_shot", [[System:ScreenShot("screen_shots/ss.png")]]) 

	Console:CreateKeyBind("f5", "screen_shot") 

	setFog(0,0,0)
end

Test = {

}

function Test:OnInit( )
	setFog(0, 0.01, 0.01)
end


init()

