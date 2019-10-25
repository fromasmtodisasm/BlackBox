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

function init(  )
	Console:CreateKeyBind("r", "shader reload");
	Console:CreateKeyBind("d", [[@retrigger_value(r_displayinfo)]]);
	Console:CreateKeyBind("q", "@Game:Stop()");
	Console:CreateKeyBind("w", "toogle_viewport_drag");

	Console:CreateKeyBind("num1", "@load1()");
	Console:CreateKeyBind("num2", "@load2()");
end

init()

