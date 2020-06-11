--------------------------------------------------------------------------------
-- Reads a table from a file, and returns it
--------------------------------------------------------------------------------
-- the file should contain only line of the type:
-- keyname = value
--------------------------------------------------------------------------------
function ReadTableFromFile(szFilename, LineMode)

	local hfile = assert(io.open(szFilename, "r"))

	Console:PrintLine("Begin read: "..szFilename)
	if (hfile == nil) then
		return
	end

	local iEqual;
	local tList = {};
	local szLine = hfile:read("*l");
	local szProp;
	local szValue;

	while (szLine ~= nil) do
	
		if (szLine:len() > 0) then
			if (string.sub(szLine, -1) == "\n") then
				szLine = string.sub(szLine, 1, szLine:len()-1);
			end	
		end
		
		if (szLine:len() > 0) then
			if (LineMode) then
				tinsert(tList, szLine);
			else
				if (szLine:len() > 0) then
	
					iEqual = string.find(szLine, "=", 1, 1);
	
					if (iEqual) then
						szProp = string.sub(szLine, 1, iEqual-1);
						szValue = string.sub(szLine, iEqual+1, -1);
	
						tList[szProp] = szValue;
					else
						tList[szLine] = 0;
					end
				end
			end
		end
		
		szLine = hfile:read("*l");
		System:Log("read line")
	end

	hfile:close();

	if (LineMode) then
		tList.n = nil;
	end

	return tList;
end

-------------------------------------------------------
-- Convert seconds to mm:ss string
-------------------------------------------------------
function SecondsToString(iSeconds)

	local iMinutes = floor(iSeconds / 60);

	return sprintf("%.2d:%.2d", iMinutes, iSeconds - (iMinutes * 60));
end

-------------------------------------------------------
-- C like printf
-------------------------------------------------------
function printf(...)
  System:LogToConsole(call(format,arg))
end
-------------------------------------------------------
-- C like sprintf
-------------------------------------------------------
function sprintf(...)
  return call(format,arg)
end

-- tokenize a string
-- BEWARE: this code doesn't handle special character like @ very well
-- \return a table containing the tokens
-------------------------------------------------------
function tokenize(str)
	local toks={}
	local cmd=gsub(str,"(%S+)",function (s) tinsert(toks,s) end);
	toks.n=nil;
	return toks;
end


-------------------------------------------------------
--untokenize a string
--returns a string containing the strings (other types are ignored) connected with " "
-------------------------------------------------------
function untokenize(toktable)
	local fullstring="";

	for i,tok in toktable do
		if type(tok)=="string" then
			if fullstring=="" then
				fullstring=tok;
			else
				fullstring=fullstring.." "..tok;
			end
		end
	end

	return fullstring;
end

-------------------------------------------------------
--Clone a table
-------------------------------------------------------
function new(_obj)
	if(type(_obj)=="table") then
		local newInstance={};
		for i,field in _obj do
			if(type(field)=="table") then
				newInstance[i]=new(field);
			else
				newInstance[i]=field;
			end
		end
		return newInstance;
	else
		return _obj;
	end
end

-------------------------------------------------------
--copy table source into the table dest skipping functions
-------------------------------------------------------
function merge(dest,source,recursive)
	for i,v in source do
		if(type(v)~="function") then
			if(recursive) then
				if(type(v)=="table")then
					dest[i]={};
					merge(dest[i],v,recursive);
				else
					dest[i]=v;
				end
			else
				dest[i]=v;
			end
		end
	end
end

-------------------------------------------------------
--copy table source into the table dest with functions
-------------------------------------------------------
function mergef(dest,source,recursive)
	for i,v in source do
		if(recursive) then
			if(type(v)=="table")then
				dest[i]={};
				mergef(dest[i],v,recursive);
			else
				dest[i]=v;
			end
		else
			dest[i]=v;
		end
	end
end


g_dump_tabs=0;
function dump(_class,no_func)
	if not _class then
		System:Log("$2nil");
	else
		local str="";
		for n=0,g_dump_tabs,1 do
			str=str.."  ";
		end
		for i,field in pairs(_class) do
			if(type(field)=="table") then
				g_dump_tabs=g_dump_tabs+1;
				System:Log(str.."$4"..i.."$1= {");
				dump(field);
				System:Log(str.."$1}");
				g_dump_tabs=g_dump_tabs-1;
			else
				if(type(field)=="number" ) then
					System:Log("$2"..str.."$6"..i.."$1=$8"..field);
				elseif(type(field) == "string") then
					System:Log("$2"..str.."$6"..i.."$1=$8".."\""..field.."\"");
				else
					if(not no_func)then
						if(type(field)=="function")then
							System:Log("$2"..str.."$5"..i.."()");
						else
							System:Log("$2"..str.."$7"..i.."$8<userdata>");
						end
					end
				end
			end
		end
	end
end

-------------------------------------------------------
-- dump all globals variable (that might take a while - the system checks against circles)
function gdump()
	local referenced={}
	local xdump=function (_class,no_func)
		local str="";
		for n=0,g_dump_tabs,1 do
			str=str.."  ";
		end
		for i,field in pairs(_class) do
			if(type(field)=="table") then
				if(not referenced[field])then
					g_dump_tabs=g_dump_tabs+1;
					System:Log(str..i.."= {");
					local idx=#referenced+1;
					referenced[field]=idx;
					xdumpa(field);
					System:Log(str.."}");
					g_dump_tabs=g_dump_tabs-1;
				else
					System:Log(str..i.."=referenced["..tonumber(referenced[field]).."]");
				end
			else
				if(type(field)=="number" ) then
					System:Log(str..i.."="..field);
				elseif(type(field) == "string") then
					System:Log(str..i.."=".."\""..field.."\"");
				else
					if(not no_func)then
						if(type(field)=="function")then
							System:Log(str..i.."()");
						else
							System:Log(str..i.."<userdata>");
						end
					end
				end
			end
		end
	end
	xdumpa=xdump;
	xdump(_G);
	referenced=nil;
end
-----------------------------------------------------------------------------
function dotproduct3d(a,b)
   return (a.x * b.x) + (a.y * b.y) + (a.z * b.z);
end

function LogVec(name,v)
	return format("%s = (%f %f %f)",name,v.x,v.y,v.z);
end


function CopyVector(dest,src)
	dest.x=src.x;
	dest.y=src.y;
	dest.z=src.z;
end
----------------------------------
function SumVectors(a,b)
	return {x=a.x+b.x,y=a.y+b.y,z=a.z+b.z};
end

----------------------------------
function FastSumVectors(dest,a,b)
	dest.x=a.x+b.x;
	dest.y=a.y+b.y;
	dest.z=a.z+b.z;
end

----------------------------------
function DifferenceVectors(a,b)
	return {x=a.x-b.x,y=a.y-b.y,z=a.z-b.z};
end

----------------------------------
function FastDifferenceVectors(dest,a,b)
	dest.x=a.x-b.x;
	dest.y=a.y-b.y;
	dest.z=a.z-b.z;
end

----------------------------------
function ProductVectors(a,b)
	return {x=a.x*b.x,y=a.y*b.y,z=a.z*b.z};
end

----------------------------------
function FastProductVectors(dest,a,b)
	dest.x=a.x*b.x;
	dest.y=a.y*b.y;
	dest.z=a.z*b.z;
end

----------------------------------
function LengthSqVector(a)
	return (a.x * a.x + a.y * a.y + a.z * a.z);
end

----------------------------------
function ScaleVector(a,b)
	return {x=a.x*b,y=a.y*b,z=a.z*b};
end

function ScaleVectorInPlace(a,b)
	a.x=a.x*b;
	a.y=a.y*b;
	a.z=a.z*b;
end

function ConvertToRadAngles(dest,src)
		local x=rad(src.z+180.0);
		local y=rad((-src.x)+90.0)
		local z=rad(src.y);

	  dest.x=-sin(y)*sin(x);
	  dest.y= sin(y)*cos(x);
	  dest.z=-cos(y);
end

function ConvertVectorToCameraAngles(dest,src)

		local	fForward;
		local	fYaw,fPitch;

		local temp = g_Vectors.temp;
		CopyVector( temp,src );

		NormalizeVector(temp)


		--first check for simple case
		if (temp.y==0 and temp.x==0) then
			--looking up/down
			fYaw=0;
			if (temp.z>0)then
				fPitch=90;
			else
				fPitch=270;
			end

		else
			if (temp.x~=0)then

				fYaw=atan2((temp.y),(temp.x))*(180.0/PI);

			else
				--lokking left/right
				if (temp.y>0) then
					fYaw=90;
				else
					fYaw=270;
				end
			end
			if (fYaw<0)then
				fYaw=fYaw+360;
			end

			fForward=sqrt(temp.x*temp.x+temp.y*temp.y);
			fPitch=(atan2(temp.z,fForward)*180.0/PI);
			if (fPitch<0)then
				fPitch=fPitch+360;
			end

		end

		--y = -fPitch;
		--x = fYaw;
		--z = 0;
		temp.x=-fPitch;
		temp.y=0; --can't calculate roll without an up vector
		temp.z=fYaw+90;

		--clamp again
		if (temp.x>360)then
			temp.x=temp.x-360;
		else
			if(temp.x<-360)then
				temp.x=temp.x+360;
			end
		end

		if (temp.z>360)then
			temp.z=temp.z-360;
		else
			if (temp.z<-360) then
				temp.z=temp.z+360;
			end
		end
		dest.x=temp.x
		dest.y=temp.y
		dest.z=temp.z

	end
----------------------------------
function NormalizeVector(a)
	local len=sqrt(LengthSqVector(a));
	local multiplier;
	if(len>0)then
		multiplier=1/len;
	else
		multiplier=0.0001;
	end
	a.x=a.x*multiplier;
	a.y=a.y*multiplier;
	a.z=a.z*multiplier;
end
----------------------------------
function FastScaleVector(dest,a,b)
	dest.x=a.x*b;
	dest.y=a.y*b;
	dest.z=a.z*b;
end

--linear interpolation
----------------------------------
function LerpColors(a,b,k)
	g_Vectors.tempColor[1] = a[1]+(b[1]-a[1])*k
	g_Vectors.tempColor[2] = a[2]+(b[2]-a[2])*k
	g_Vectors.tempColor[3] = a[3]+(b[3]-a[3])*k
	return g_Vectors.tempColor;
end

----------------------------------
function Lerp(a,b,k)
	return (a + (b - a)*k);
end


----------------------------------
function __max(a, b)
	if (a > b) then
		return a;
	else
		return b;
	end
end

System:Log("common lua------------------------------------------------------")
