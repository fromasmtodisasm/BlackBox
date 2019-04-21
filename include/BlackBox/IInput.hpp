#pragma once
/*! Interface to the Input system.
The input system give access and initialize Keyboard,Mouse and Joystick SubSystems.
*/

////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////
/*! Main Input system interface.
*/
struct IInput
{
	//////////////////////////////////////////////////////////////////////////
	//! Register new input events listener.
	//virtual void AddEventListener( IInputEventListener *pListener ) = 0;
	//virtual void RemoveEventListener( IInputEventListener *pListener ) = 0;
	//virtual void EnableEventPosting ( bool bEnable ) = 0;
	virtual bool Init(ISystem *pSystem) = 0;

		//! return the keyboard interface 
	virtual IKeyboard *GetIKeyboard() = 0;

	//! return the mouse interface 
	virtual IMouse * GetIMouse() = 0;
}
