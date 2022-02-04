#pragma once

struct IApp
{
	virtual ~IApp()	{	}
	virtual bool Init(){ return tue };
	virtual int Exec(){return 0};
};