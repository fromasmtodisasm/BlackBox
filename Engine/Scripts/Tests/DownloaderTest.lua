Downloader = 
{
	--sound = Sound:LoadSound("sounds/rain/rainbasicheavy1.wav"),
	sound = Sound:LoadSound("sounds/doors/open.wav"),
	
}

function Downloader.Call(url, _file)
	System:Log("134") 
	BannerCfgDownload = System:CreateDownload();
	BannerCfgDownload.OnComplete = function() System:Log("Download Completed"); Sound:PlaySound(Downloader.sound)
		--os.execute("explorer.exe " .. _file); 
	end;
	BannerCfgDownload.OnError = function() System:Error("Download Error"); end;
	BannerCfgDownload:Download(url, _file);
end

function Downloader.Test()
	Downloader.Call("http://ya.ru", "tmp.bin")
end
