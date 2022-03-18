#include "CGF/stdafx.h"
#include "CGF/ChunkFileReader.h"
#include <BlackBox/System/File/CryFile.h>

#include "CGFImporter.h"
#include "CGF/CgfDump.h"

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
					    "-mesh",
					    (char*)file.GetAdjustedFilename()};

					std::vector<char> buf(file.GetLength());
					file.Read(buf.data(), file.GetLength());

					auto                        mapping = _smart_ptr(new MemoryBlob((void*)buf.data(), file.GetLength()));
					CCgfDump<MemoryBlob> dumper(mapping);
					dumper.Dump(3, argv);
				}
			}
		}
	}
} // namespace Assimp
