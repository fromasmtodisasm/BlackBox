#include <assimp/BaseImporter.h>
#include <assimp/Importer.hpp>
#include <assimp/IOSystem.hpp>

#include <BlackBox/Core/smartptr.hpp>

namespace Assimp
{
	class CgfImporter : public BaseImporter, public _reference_target_t
	{
	public:
		CgfImporter()
		    : BaseImporter()
		{
		}
		~CgfImporter() {
		}

		bool                          CanRead(const std::string& filename, IOSystem* pIOHandler, bool checkSig) const override;

		// Inherited via BaseImporter
		virtual const aiImporterDesc* GetInfo() const override;
		virtual void                  InternReadFile(const std::string& pFile, aiScene* pScene, IOSystem* pIOHandler) override;
	};

} // namespace Assimp