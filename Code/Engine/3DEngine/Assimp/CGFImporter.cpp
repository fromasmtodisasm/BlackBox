#include "CGF/stdafx.h"
#include "CGF/ChunkFileReader.h"
#include <BlackBox/System/File/CryFile.h>

#include "CGFImporter.h"
#include "CGF/Loader.h"

namespace Assimp
{
	aiImporterDesc GetDesc()
	{
		static aiImporterDesc desc{0};
		desc.mName       = "CryTek Geometry File Importer";
		desc.mAuthor     = "fromasmtodisasm";
		desc.mMaintainer = "fromasmtodisasm";
		desc.mComments;
		desc.mFlags;
		desc.mMinMajor;
		desc.mMinMinor;
		desc.mMaxMajor;
		desc.mMaxMinor;
		desc.mFileExtensions = "cgf";

		return desc;
	}

	static aiImporterDesc Desc = GetDesc();

	bool                  CgfImporter::CanRead(const std::string& filename, IOSystem* pIOHandler, bool checkSig) const
	{
		if (checkSig)
		{
			// Check the signature and return result
		}
		else
		{
			const std::string extension = GetExtension(filename);
			if (extension == "cgf")
			{
				return true;
			}
		}
		return false;
	}
	const aiImporterDesc* CgfImporter::GetInfo() const
	{
		return &Desc;
	}

	void CgfImporter::InternReadFile(const std::string& pFile, aiScene* pScene, IOSystem* pIOHandler)
	{
		CryLog("[CgfImporter] Load object: %s", pFile.c_str());
		if (pIOHandler->Exists(pFile.c_str()))
		{
			CryLog("Object exists");
			if (auto stream = pIOHandler->Open(pFile, "rb"); stream != nullptr)
			{
				CCryFile file(pFile.data(), "rb");
				if (file)
				{
					char* argv[] = {
					    "dummy.exe",
					    //"-mesh",
					    (char*)file.GetAdjustedFilename()};

					std::vector<char> buf(file.GetLength());
					file.Read(buf.data(), file.GetLength());

					_smart_ptr<IMemoryBlob> mapping = new CMemoryBlob((void*)buf.data(), file.GetLength());
					CLoaderCGF              loader(mapping, pScene);
					loader.Load(pFile.c_str());
				}
			}
		}
	}
} // namespace Assimp
