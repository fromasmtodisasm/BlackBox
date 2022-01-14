#pragma once

//! Iteration over table parameters.
struct Iterator
{
	const char* sKey; // This is now redundant.
	int			nKey; // This is now redundant.
	union value
	{
		bool			b;
		int				i;
		float			f;
		const char*		c;
		HSCRIPTFUNCTION sf;
		void*			p;
		IScriptObject*	o;
	};

	using Value = value;
	Value		  v;
	ScriptVarType key_type;
	ScriptVarType value_type;
	struct
	{
		bool resolvePrototypeTableAsWell;
		int	 nStackMarker1; //!< Used for traversing our own table (this is typically the table that overrides properties from prototype tables).
		int	 nStackMarker2; //!< Used after our own table is traversed; we then try to traverse the prototype table (gets retrieved via a potential metatable).
	} internal;
};



