#include <BlackBox/Renderer/BaseRenderer.hpp>
#include <BlackBox/System/ConsoleRegistration.h>

#include <SDL2/SDL.h>

int RenderCVars::CV_r_GetScreenShot;

CRenderer::~CRenderer()
{
	SAFE_DELETE(m_RenderAuxGeom);
	SAFE_DELETE(m_BufferManager);
	SAFE_DELETE(m_VertexBuffer);
}

int CRenderer::EnumDisplayFormats(SDispFormat* formats)
{
	static int displayInUse = 0; /* Only using first display */
	int numModes			  = 0;
	int i					  = 0;
	//TODO: FIX IT
#if SFML_WINDOW
	auto videoModes = sf::VideoMode::getFullscreenModes();

	numModes = static_cast<int>(videoModes.size());
	if (formats != nullptr)
	{
		for (auto& mode : videoModes)
		{
			formats[i].m_BPP	= mode.bitsPerPixel;
			formats[i].m_Width	= mode.width;
			formats[i].m_Height = mode.height;
			i++;
		}
	}
#endif
	//#elif SDL_WINDOW
	SDL_DisplayMode mode;
	Uint32 f;

	gEnv->pLog->Log("SDL_GetNumVideoDisplays(): %i", SDL_GetNumVideoDisplays());

	numModes = SDL_GetNumDisplayModes(displayInUse);
	if (numModes < 1)
	{
		gEnv->pLog->Log("SDL_GetNumDisplayModes failed: %s", SDL_GetError());
		return 1;
	}
	gEnv->pLog->Log("SDL_GetNumDisplayModes: %i", numModes);

	if (formats != nullptr)
	{
		for (i = 0; i < numModes; ++i)
		{
			if (SDL_GetDisplayMode(displayInUse, i, &mode) != 0)
			{
				gEnv->pLog->Log("SDL_GetDisplayMode failed: %s", SDL_GetError());
				return 1;
			}
			f = mode.format;

			formats[i].m_BPP	= SDL_BITSPERPIXEL(f);
			formats[i].m_Width	= mode.w;
			formats[i].m_Height = mode.h;
		}
	}

	//#endif

	return numModes;
}

void RenderCVars::InitCVars()
{
#if 0
	REGISTER_CVAR(r_Width, 800, VF_NULL, "Window Width");
	REGISTER_CVAR(r_Height, 600, VF_NULL, "Window Height");
	REGISTER_CVAR(r_Fullscreen, 600, VF_NULL);
	REGISTER_CVAR(r_Fullscreen, 600, VF_NULL);
#endif
	REGISTER_CVAR(r_MSAA, r_MSAA, VF_DUMPTODISK | VF_READONLY, "");
	REGISTER_CVAR(r_MSAA_samples, r_MSAA_samples, VF_DUMPTODISK, "");

	REGISTER_CVAR(r_Vsync, r_Vsync, VF_DUMPTODISK, "");
	REGISTER_CVAR(r_DisplayIndex, r_DisplayIndex, VF_DUMPTODISK, "");
	//REGISTER_CVAR3("r_GetScreenShot", CV_r_GetScreenShot, 0, VF_NULL,
	REGISTER_CVAR3("r_getscreenshot", CV_r_GetScreenShot, 0, VF_NULL,
				   "To capture one screenshot (variable is set to 0 after capturing)\n"
				   "0=do not take a screenshot (default), 1=save a screenshot (together with .HDR if enabled), 2=save a screenshot");
}

RenderCVars::~RenderCVars()
{
}

CVertexBuffer* CRenderer::CreateBuffer(int vertexCount, int vertexFormat, const char* szSource, bool bDynamic /* = false*/)
{
	return m_BufferManager->Create(vertexCount, vertexFormat, szSource, bDynamic);
}

void CRenderer::ReleaseBuffer(CVertexBuffer* bufptr)
{
	m_BufferManager->Release(bufptr);
}

void CRenderer::DrawBuffer(CVertexBuffer* src, SVertexStream* indicies, int numindices, int offsindex, int prmode, int vert_start /* = 0*/, int vert_stop /* = 0*/, CMatInfo* mi /* = NULL*/)
{
	m_BufferManager->Draw(src, indicies, numindices, offsindex, prmode, vert_start, vert_stop, mi);
}

void CRenderer::UpdateBuffer(CVertexBuffer* dest, const void* src, int vertexcount, bool bUnLock, int nOffs /* = 0*/, int Type /* = 0*/)
{
	m_BufferManager->Update(dest, src, vertexcount, bUnLock, nOffs, Type);
}

void CRenderer::CreateIndexBuffer(SVertexStream* dest, const void* src, int indexcount)
{
	m_BufferManager->Create(dest, src, indexcount);
}

void CRenderer::UpdateIndexBuffer(SVertexStream* dest, const void* src, int indexcount, bool bUnLock /* = true*/)
{
	m_BufferManager->Update(dest, src, indexcount, bUnLock);
}

void CRenderer::ReleaseIndexBuffer(SVertexStream* dest)
{

}

