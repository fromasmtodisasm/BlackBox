// Copyright 2001-2019 Crytek GmbH / Crytek Group. All rights reserved.
#pragma once

#include "EditorCommonAPI.h"
#if 0
//#include <CrySandbox/CryFunction.h>
#else
//////////////////////////////////////////////////////////////////////////

namespace CryFunctionPrivate
{
template<typename Signature> struct FunctionWrapper
{
	static std::function<Signature> Cast(const std::function<Signature>& f) { return f; }
};

//0 argument
template<typename Ret>
struct FunctionWrapper<Ret()>
{
	static std::function<Ret()> Cast(const std::function<Ret()>& f) { return f; }

	template<typename OtherRet>
	static std::function<Ret()> Cast(const std::function<OtherRet()>& f) { return [=]() { return static_cast<Ret>(f()); }; }
};

template<>
struct FunctionWrapper<void()>
{
	static std::function<void()> Cast(const std::function<void()>& f) { return f; }

	template<typename OtherRet>
	static std::function<void()> Cast(const std::function<OtherRet()>& f) { return [=]() { f(); }; }
};

//1 argument
template<typename Ret, typename Arg1>
struct FunctionWrapper<Ret(Arg1)>
{
	static std::function<Ret(Arg1)> Cast(const std::function<Ret(Arg1)>& f) { return f; }

	template<typename OtherRet, typename OtherArg1>
	static std::function<Ret(Arg1)> Cast(const std::function<OtherRet(OtherArg1)>& f) { return [=](Arg1 arg1) { return static_cast<Ret>(f(static_cast<OtherArg1>(arg1))); }; }

	template<typename OtherRet>
	static std::function<Ret(Arg1)> Cast(const std::function<OtherRet()>& f) { return [=](Arg1 arg1) { return static_cast<Ret>(f()); }; }
};

template<typename Arg1>
struct FunctionWrapper<void(Arg1)>
{
	static std::function<void(Arg1)> Cast(const std::function<void(Arg1)>& f) { return f; }

	template<typename OtherRet, typename OtherArg1>
	static std::function<void(Arg1)> Cast(const std::function<OtherRet(OtherArg1)>& f) { return [=](Arg1 arg1) { f(static_cast<OtherArg1>(arg1)); }; }

	template<typename OtherRet>
	static std::function<void(Arg1)> Cast(const std::function<OtherRet()>& f) { return [=](Arg1 arg1) { f(); }; }
};

//2 arguments
template<typename Ret, typename Arg1, typename Arg2>
struct FunctionWrapper<Ret(Arg1, Arg2)>
{
	static std::function<Ret(Arg1, Arg2)> Cast(const std::function<Ret(Arg1, Arg2)>& f) { return f; }

	template<typename OtherRet, typename OtherArg1, typename OtherArg2>
	static std::function<Ret(Arg1, Arg2)> Cast(const std::function<OtherRet(OtherArg1, OtherArg2)>& f) { return [=](Arg1 arg1, Arg2 arg2) { return static_cast<Ret>(f(static_cast<OtherArg1>(arg1), static_cast<OtherArg2>(arg2))); }; }

	template<typename OtherRet, typename OtherArg1>
	static std::function<Ret(Arg1, Arg2)> Cast(const std::function<OtherRet(OtherArg1)>& f) { return [=](Arg1 arg1, Arg2 arg2) { return static_cast<Ret>(f(static_cast<OtherArg1>(arg1))); }; }

	template<typename OtherRet>
	static std::function<Ret(Arg1, Arg2)> Cast(const std::function<OtherRet()>& f) { return [=](Arg1 arg1, Arg2 arg2) { return static_cast<Ret>(f()); }; }
};

template<typename Arg1, typename Arg2>
struct FunctionWrapper<void(Arg1, Arg2)>
{
	static std::function<void(Arg1, Arg2)> Cast(const std::function<void(Arg1, Arg2)>& f) { return f; }

	template<typename OtherRet, typename OtherArg1, typename OtherArg2>
	static std::function<void(Arg1, Arg2)> Cast(const std::function<OtherRet(OtherArg1, OtherArg2)>& f) { return [=](Arg1 arg1, Arg2 arg2) { f(static_cast<OtherArg1>(arg1), static_cast<OtherArg2>(arg2)); }; }

	template<typename OtherRet, typename OtherArg1>
	static std::function<void(Arg1, Arg2)> Cast(const std::function<OtherRet(OtherArg1)>& f) { return [=](Arg1 arg1, Arg2 arg2) { f(static_cast<OtherArg1>(arg1)); }; }

	template<typename OtherRet>
	static std::function<void(Arg1, Arg2)> Cast(const std::function<OtherRet()>& f) { return [=](Arg1 arg1, Arg2 arg2) { f(); }; }
};

//3 arguments
template<typename Ret, typename Arg1, typename Arg2, typename Arg3>
struct FunctionWrapper<Ret(Arg1, Arg2, Arg3)>
{
	static std::function<Ret(Arg1, Arg2, Arg3)> Cast(const std::function<Ret(Arg1, Arg2, Arg3)>& f) { return f; }

	template<typename OtherRet, typename OtherArg1, typename OtherArg2, typename OtherArg3>
	static std::function<Ret(Arg1, Arg2, Arg3)> Cast(const std::function<OtherRet(OtherArg1, OtherArg2, OtherArg3)>& f)
	{
		return [=](Arg1 arg1, Arg2 arg2, Arg3 arg3) { return static_cast<Ret>(f(static_cast<OtherArg1>(arg1), static_cast<OtherArg2>(arg2), static_cast<OtherArg3>(arg3))); };
	}

	template<typename OtherRet, typename OtherArg1, typename OtherArg2>
	static std::function<Ret(Arg1, Arg2, Arg3)> Cast(const std::function<OtherRet(OtherArg1, OtherArg2)>& f) { return [=](Arg1 arg1, Arg2 arg2, Arg3 arg3) { return static_cast<Ret>(f(static_cast<OtherArg1>(arg1), static_cast<OtherArg2>(arg2))); }; }

	template<typename OtherRet, typename OtherArg1>
	static std::function<Ret(Arg1, Arg2, Arg3)> Cast(const std::function<OtherRet(OtherArg1)>& f) { return [=](Arg1 arg1, Arg2 arg2, Arg3 arg3) { return static_cast<Ret>(f(static_cast<OtherArg1>(arg1))); }; }

	template<typename OtherRet>
	static std::function<Ret(Arg1, Arg2, Arg3)> Cast(const std::function<OtherRet()>& f) { return [=](Arg1 arg1, Arg2 arg2, Arg3 arg3) { return static_cast<Ret>(f()); }; }
};

template<typename Arg1, typename Arg2, typename Arg3>
struct FunctionWrapper<void(Arg1, Arg2, Arg3)>
{
	static std::function<void(Arg1, Arg2, Arg3)> Cast(const std::function<void(Arg1, Arg2, Arg3)>& f) { return f; }

	template<typename OtherRet, typename OtherArg1, typename OtherArg2, typename OtherArg3>
	static std::function<void(Arg1, Arg2, Arg3)> Cast(const std::function<OtherRet(OtherArg1, OtherArg2, OtherArg3)>& f)
	{
		return [=](Arg1 arg1, Arg2 arg2, Arg3 arg3) { f(static_cast<OtherArg1>(arg1), static_cast<OtherArg2>(arg2), static_cast<OtherArg3>(arg3)); };
	}

	template<typename OtherRet, typename OtherArg1, typename OtherArg2>
	static std::function<void(Arg1, Arg2, Arg3)> Cast(const std::function<OtherRet(OtherArg1, OtherArg2)>& f) { return [=](Arg1 arg1, Arg2 arg2, Arg3 arg3) { f(static_cast<OtherArg1>(arg1), static_cast<OtherArg2>(arg2)); }; }

	template<typename OtherRet, typename OtherArg1>
	static std::function<void(Arg1, Arg2, Arg3)> Cast(const std::function<OtherRet(OtherArg1)>& f) { return [=](Arg1 arg1, Arg2 arg2, Arg3 arg3) { f(static_cast<OtherArg1>(arg1)); }; }

	template<typename OtherRet>
	static std::function<void(Arg1, Arg2, Arg3)> Cast(const std::function<OtherRet()>& f) { return [=](Arg1 arg1, Arg2 arg2, Arg3 arg3) { f(); }; }
};

//4 arguments
template<typename Ret, typename Arg1, typename Arg2, typename Arg3, typename Arg4>
struct FunctionWrapper<Ret(Arg1, Arg2, Arg3, Arg4)>
{
	static std::function<Ret(Arg1, Arg2, Arg3, Arg4)> Cast(const std::function<Ret(Arg1, Arg2, Arg3, Arg4)>& f) { return f; }

	template<typename OtherRet, typename OtherArg1, typename OtherArg2, typename OtherArg3, typename OtherArg4>
	static std::function<Ret(Arg1, Arg2, Arg3, Arg4)> Cast(const std::function<OtherRet(OtherArg1, OtherArg2, OtherArg3, OtherArg4)>& f)
	{
		return [=](Arg1 arg1, Arg2 arg2, Arg3 arg3, Arg4 arg4) { return static_cast<Ret>(f(static_cast<OtherArg1>(arg1), static_cast<OtherArg2>(arg2), static_cast<OtherArg3>(arg3), static_cast<OtherArg4>(arg4))); };
	}

	template<typename OtherRet, typename OtherArg1, typename OtherArg2, typename OtherArg3>
	static std::function<Ret(Arg1, Arg2, Arg3, Arg4)> Cast(const std::function<OtherRet(OtherArg1, OtherArg2, OtherArg3)>& f)
	{
		return [=](Arg1 arg1, Arg2 arg2, Arg3 arg3, Arg4 arg4) { return static_cast<Ret>(f(static_cast<OtherArg1>(arg1), static_cast<OtherArg2>(arg2), static_cast<OtherArg3>(arg3))); };
	}

	template<typename OtherRet, typename OtherArg1, typename OtherArg2>
	static std::function<Ret(Arg1, Arg2, Arg3, Arg4)> Cast(const std::function<OtherRet(OtherArg1, OtherArg2)>& f) { return [=](Arg1 arg1, Arg2 arg2, Arg3 arg3, Arg4 arg4) { return static_cast<Ret>(f(static_cast<OtherArg1>(arg1), static_cast<OtherArg2>(arg2))); }; }

	template<typename OtherRet, typename OtherArg1>
	static std::function<Ret(Arg1, Arg2, Arg3, Arg4)> Cast(const std::function<OtherRet(OtherArg1)>& f) { return [=](Arg1 arg1, Arg2 arg2, Arg3 arg3, Arg4 arg4) { return static_cast<Ret>(f(static_cast<OtherArg1>(arg1))); }; }

	template<typename OtherRet>
	static std::function<Ret(Arg1, Arg2, Arg3, Arg4)> Cast(const std::function<OtherRet()>& f) { return [=](Arg1 arg1, Arg2 arg2, Arg3 arg3, Arg4 arg4) { return static_cast<Ret>(f()); }; }
};

template<typename Arg1, typename Arg2, typename Arg3, typename Arg4>
struct FunctionWrapper<void(Arg1, Arg2, Arg3, Arg4)>
{
	static std::function<void(Arg1, Arg2, Arg3, Arg4)> Cast(const std::function<void(Arg1, Arg2, Arg3, Arg4)>& f) { return f; }

	template<typename OtherRet, typename OtherArg1, typename OtherArg2, typename OtherArg3, typename OtherArg4>
	static std::function<void(Arg1, Arg2, Arg3, Arg4)> Cast(const std::function<OtherRet(OtherArg1, OtherArg2, OtherArg3, OtherArg4)>& f)
	{
		return [=](Arg1 arg1, Arg2 arg2, Arg3 arg3, Arg4 arg4) { f(static_cast<OtherArg1>(arg1), static_cast<OtherArg2>(arg2), static_cast<OtherArg3>(arg3), static_cast<OtherArg4>(arg4)); };
	}

	template<typename OtherRet, typename OtherArg1, typename OtherArg2, typename OtherArg3>
	static std::function<void(Arg1, Arg2, Arg3, Arg4)> Cast(const std::function<OtherRet(OtherArg1, OtherArg2, OtherArg3)>& f)
	{
		return [=](Arg1 arg1, Arg2 arg2, Arg3 arg3, Arg4 arg4) { f(static_cast<OtherArg1>(arg1), static_cast<OtherArg2>(arg2), static_cast<OtherArg3>(arg3)); };
	}

	template<typename OtherRet, typename OtherArg1, typename OtherArg2>
	static std::function<void(Arg1, Arg2, Arg3, Arg4)> Cast(const std::function<OtherRet(OtherArg1, OtherArg2)>& f) { return [=](Arg1 arg1, Arg2 arg2, Arg3 arg3, Arg4 arg4) { f(static_cast<OtherArg1>(arg1), static_cast<OtherArg2>(arg2)); }; }

	template<typename OtherRet, typename OtherArg1>
	static std::function<void(Arg1, Arg2, Arg3, Arg4)> Cast(const std::function<OtherRet(OtherArg1)>& f) { return [=](Arg1 arg1, Arg2 arg2, Arg3 arg3, Arg4 arg4) { f(static_cast<OtherArg1>(arg1)); }; }

	template<typename OtherRet>
	static std::function<void(Arg1, Arg2, Arg3, Arg4)> Cast(const std::function<OtherRet()>& f) { return [=](Arg1 arg1, Arg2 arg2, Arg3 arg3, Arg4 arg4) { f(); }; }
};

//5 arguments
template<typename Ret, typename Arg1, typename Arg2, typename Arg3, typename Arg4, typename Arg5>
struct FunctionWrapper<Ret(Arg1, Arg2, Arg3, Arg4, Arg5)>
{
	static std::function<Ret(Arg1, Arg2, Arg3, Arg4, Arg5)> Cast(const std::function<Ret(Arg1, Arg2, Arg3, Arg4, Arg5)>& f) { return f; }

	template<typename OtherRet, typename OtherArg1, typename OtherArg2, typename OtherArg3, typename OtherArg4, typename OtherArg5>
	static std::function<Ret(Arg1, Arg2, Arg3, Arg4, Arg5)> Cast(const std::function<OtherRet(OtherArg1, OtherArg2, OtherArg3, OtherArg4, OtherArg5)>& f)
	{
		return [=](Arg1 arg1, Arg2 arg2, Arg3 arg3, Arg4 arg4, Arg5 arg5) { return static_cast<Ret>(f(static_cast<OtherArg1>(arg1), static_cast<OtherArg2>(arg2), static_cast<OtherArg3>(arg3), static_cast<OtherArg4>(arg4), static_cast<OtherArg5>(arg5))); };
	}

	template<typename OtherRet, typename OtherArg1, typename OtherArg2, typename OtherArg3, typename OtherArg4>
	static std::function<Ret(Arg1, Arg2, Arg3, Arg4, Arg5)> Cast(const std::function<OtherRet(OtherArg1, OtherArg2, OtherArg3, OtherArg4)>& f)
	{
		return [=](Arg1 arg1, Arg2 arg2, Arg3 arg3, Arg4 arg4, Arg5 arg5) { return static_cast<Ret>(f(static_cast<OtherArg1>(arg1), static_cast<OtherArg2>(arg2), static_cast<OtherArg3>(arg3), static_cast<OtherArg4>(arg4))); };
	}

	template<typename OtherRet, typename OtherArg1, typename OtherArg2, typename OtherArg3>
	static std::function<Ret(Arg1, Arg2, Arg3, Arg4, Arg5)> Cast(const std::function<OtherRet(OtherArg1, OtherArg2, OtherArg3)>& f)
	{
		return [=](Arg1 arg1, Arg2 arg2, Arg3 arg3, Arg4 arg4, Arg5 arg5) { return static_cast<Ret>(f(static_cast<OtherArg1>(arg1), static_cast<OtherArg2>(arg2), static_cast<OtherArg3>(arg3))); };
	}

	template<typename OtherRet, typename OtherArg1, typename OtherArg2>
	static std::function<Ret(Arg1, Arg2, Arg3, Arg4, Arg5)> Cast(const std::function<OtherRet(OtherArg1, OtherArg2)>& f) { return [=](Arg1 arg1, Arg2 arg2, Arg3 arg3, Arg4 arg4, Arg5 arg5) { return static_cast<Ret>(f(static_cast<OtherArg1>(arg1), static_cast<OtherArg2>(arg2))); }; }

	template<typename OtherRet, typename OtherArg1>
	static std::function<Ret(Arg1, Arg2, Arg3, Arg4, Arg5)> Cast(const std::function<OtherRet(OtherArg1)>& f) { return [=](Arg1 arg1, Arg2 arg2, Arg3 arg3, Arg4 arg4, Arg5 arg5) { return static_cast<Ret>(f(static_cast<OtherArg1>(arg1))); }; }

	template<typename OtherRet>
	static std::function<Ret(Arg1, Arg2, Arg3, Arg4, Arg5)> Cast(const std::function<OtherRet()>& f) { return [=](Arg1 arg1, Arg2 arg2, Arg3 arg3, Arg4 arg4, Arg5 arg5) { return static_cast<Ret>(f()); }; }
};

template<typename Arg1, typename Arg2, typename Arg3, typename Arg4, typename Arg5>
struct FunctionWrapper<void(Arg1, Arg2, Arg3, Arg4, Arg5)>
{
	static std::function<void(Arg1, Arg2, Arg3, Arg4, Arg5)> Cast(const std::function<void(Arg1, Arg2, Arg3, Arg4, Arg5)>& f) { return f; }

	template<typename OtherRet, typename OtherArg1, typename OtherArg2, typename OtherArg3, typename OtherArg4, typename OtherArg5>
	static std::function<void(Arg1, Arg2, Arg3, Arg4, Arg5)> Cast(const std::function<OtherRet(OtherArg1, OtherArg2, OtherArg3, OtherArg4, OtherArg5)>& f)
	{
		return [=](Arg1 arg1, Arg2 arg2, Arg3 arg3, Arg4 arg4, Arg5 arg5) { f(static_cast<OtherArg1>(arg1), static_cast<OtherArg2>(arg2), static_cast<OtherArg3>(arg3), static_cast<OtherArg4>(arg4), static_cast<OtherArg5>(arg5)); };
	}

	template<typename OtherRet, typename OtherArg1, typename OtherArg2, typename OtherArg3, typename OtherArg4>
	static std::function<void(Arg1, Arg2, Arg3, Arg4, Arg5)> Cast(const std::function<OtherRet(OtherArg1, OtherArg2, OtherArg3, OtherArg4)>& f)
	{
		return [=](Arg1 arg1, Arg2 arg2, Arg3 arg3, Arg4 arg4, Arg5 arg5) { f(static_cast<OtherArg1>(arg1), static_cast<OtherArg2>(arg2), static_cast<OtherArg3>(arg3), static_cast<OtherArg4>(arg4)); };
	}

	template<typename OtherRet, typename OtherArg1, typename OtherArg2, typename OtherArg3>
	static std::function<void(Arg1, Arg2, Arg3, Arg4, Arg5)> Cast(const std::function<OtherRet(OtherArg1, OtherArg2, OtherArg3)>& f)
	{
		return [=](Arg1 arg1, Arg2 arg2, Arg3 arg3, Arg4 arg4, Arg5 arg5) { f(static_cast<OtherArg1>(arg1), static_cast<OtherArg2>(arg2), static_cast<OtherArg3>(arg3)); };
	}

	template<typename OtherRet, typename OtherArg1, typename OtherArg2>
	static std::function<void(Arg1, Arg2, Arg3, Arg4, Arg5)> Cast(const std::function<OtherRet(OtherArg1, OtherArg2)>& f) { return [=](Arg1 arg1, Arg2 arg2, Arg3 arg3, Arg4 arg4, Arg5 arg5) { f(static_cast<OtherArg1>(arg1), static_cast<OtherArg2>(arg2)); }; }

	template<typename OtherRet, typename OtherArg1>
	static std::function<void(Arg1, Arg2, Arg3, Arg4, Arg5)> Cast(const std::function<OtherRet(OtherArg1)>& f) { return [=](Arg1 arg1, Arg2 arg2, Arg3 arg3, Arg4 arg4, Arg5 arg5) { f(static_cast<OtherArg1>(arg1)); }; }

	template<typename OtherRet>
	static std::function<void(Arg1, Arg2, Arg3, Arg4, Arg5)> Cast(const std::function<OtherRet()>& f) { return [=](Arg1 arg1, Arg2 arg2, Arg3 arg3, Arg4 arg4, Arg5 arg5) { f(); }; }
};

//6 arguments
template<typename Ret, typename Arg1, typename Arg2, typename Arg3, typename Arg4, typename Arg5, typename Arg6>
struct FunctionWrapper<Ret(Arg1, Arg2, Arg3, Arg4, Arg5, Arg6)>
{
	static std::function<Ret(Arg1, Arg2, Arg3, Arg4, Arg5, Arg6)> Cast(const std::function<Ret(Arg1, Arg2, Arg3, Arg4, Arg5, Arg6)>& f) { return f; }
};

template<typename Arg1, typename Arg2, typename Arg3, typename Arg4, typename Arg5, typename Arg6>
struct FunctionWrapper<void(Arg1, Arg2, Arg3, Arg4, Arg5, Arg6)>
{
	static std::function<void(Arg1, Arg2, Arg3, Arg4, Arg5, Arg6)> Cast(const std::function<void(Arg1, Arg2, Arg3, Arg4, Arg5, Arg6)>& f) { return f; }
};

}

//Intended breach of CryENGINE coding standard to match c++ standard for casting, i.e. static_cast/dynamic_cast etc
template<typename Sig1, typename Sig2>
inline std::function<Sig1> function_cast(const std::function<Sig2>& f)
{
	return CryFunctionPrivate::FunctionWrapper<Sig1>::Cast(f);
}

#if 0
template<typename Sig1, typename Function, typename std::enable_if<IsCallable<Function>::value, int>::type* = 0>
inline std::function<Sig1> function_cast(const Function& f)
{
	typedef typename SFuncInfo<Function>::Signature Sig2;
	return CryFunctionPrivate::FunctionWrapper<Sig1>::Cast(std::function<Sig2>(f));
}
#endif
#endif

#include <QDialog>
#include <QString>
#include <functional>

class QCustomTitleBar;
class QGridLayout;
/**
 * Dialog base class
 *
 * TODO : Handle buttons generically, so they are always placed the same and named the same across all dialogs
 */
class EDITOR_COMMON_API CEditorDialog : public QDialog
{
	Q_OBJECT;
public:
	//! dialogNameId must be unique in the codebase, as this will be used as the key to save its settings and personalization
	CEditorDialog(const QString& dialogNameId, QWidget* parent = nullptr, bool saveSize = true);

	//! Convenience method to store shared properties in personalization manager
	void AddPersonalizedSharedProperty(const QString& propName, std::function<QVariant()>& store, std::function<void(const QVariant&)>& restore);
	//! Convenience method to store project properties in personalization manager
	void AddPersonalizedProjectProperty(const QString& propName, std::function<QVariant()>& store, std::function<void(const QVariant&)>& restore);

	//! Convenience method to store shared properties in personalization manager
	template<typename Store, typename Restore>
	void AddPersonalizedSharedProperty(const QString& propName, Store& store, Restore& restore)
	{
		AddPersonalizedSharedProperty(propName, function_cast<QVariant()>(store), function_cast<void(const QVariant&)>(restore));
	}

	//! Convenience method to store project properties in personalization manager
	template<typename Store, typename Restore>
	void AddPersonalizedProjectProperty(const QString& propName, Store& store, Restore& restore)
	{
		AddPersonalizedProjectProperty(propName, function_cast<QVariant()>(store), function_cast<void(const QVariant&)>(restore));
	}

	const QString& GetDialogName() const { return m_dialogNameId; }

	static QVariantMap s_config;

	//! Prevents the dialog from being resized, must be used after all the layout has been filled up
	void SetResizable(bool resizable);

	//! Shows the dialog and waits until the dialog exits. Returns true if the dialog succeeded (accepted), false if it was closed or cancelled.
	bool Execute();

	//! Brings the dialog window to the front, being on top of all other windows
	void Raise();

	//! Sets visible and show on top of other windows. Default position is in the center of the main monitor.
	void Popup();
	void Popup(const QPoint &position, const QSize& size);

	//! This will position the window much like a tool window is expected, and cascade them, so several dialogs can be layed out without overlap.
	//! Note that the default dialog position is the center of the screen
	void SetPosCascade();

	//! Hides the dialog when it is finished by any means. Only call this once.
	void SetHideOnClose();

	//! Hides the close button and prevents closing using ESC. Only call this once.
	void SetDoNotClose();

	//! Must be called after being shown //TODO : this should be saved and always work...
	void SetTitle(const QString& title);

protected:
	virtual void changeEvent(QEvent*) override;
	virtual void showEvent(QShowEvent* event) override;
	virtual bool nativeEvent(const QByteArray &eventType, void *message, qintptr *result) override;
	virtual void keyPressEvent(QKeyEvent* keyEvent) override;

	bool             m_layoutWrapped;
	QCustomTitleBar* m_titleBar;
	QGridLayout*     m_grid;
	bool             m_saveSize;
	bool			 m_resizable;
	bool			 m_canClose;
	QString			 m_dialogNameId;
};
