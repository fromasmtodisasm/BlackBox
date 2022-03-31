#pragma once
#include "IRender.hpp"

struct SRender2DImageDescription;

struct IRenderAuxGeom
{
	virtual void           DrawAABB(Legacy::Vec3 min, Legacy::Vec3 max, const UCol& col)                                                                                 = 0;
	virtual void           DrawTriangle(const Legacy::Vec3& v0, const UCol& colV0, const Legacy::Vec3& v1, const UCol& colV1, const Legacy::Vec3& v2, const UCol& colV2) = 0;
	virtual void           DrawLine(const Legacy::Vec3& v0, const UCol& colV0, const Legacy::Vec3& v1, const UCol& colV1, float thickness = 1.0f)                        = 0;
	virtual void           DrawLines(const Legacy::Vec3* v, uint32 numPoints, const UCol& col, float thickness = 1.0f)                                                   = 0;
	//! Adds a 2D image that should be drawn on the screen to an internal render list.
	//! The function supports placing images in stereo 3D space.
	//! The stereo params are the same that are used for the scene. A value of 0 is handled as a special case and always places the image on the screen plane.
	//! image.stereoDepth - Places image in stereo 3d space. The depth is specified in camera space.
	virtual void           PushImage(const SRender2DImageDescription& image)                                                                                             = 0;
	static IRenderAuxGeom* GetAux()
	{
		return Env::Renderer() ? Env::Renderer()->GetIRenderAuxGeom() : Env::AuxGeomRenderer();
	}
	void DrawQuad(const Legacy::Vec3& v0, const UCol& colV0, const Legacy::Vec3& v1, const UCol& colV1, const Legacy::Vec3& v2, const UCol& colV2, const Legacy::Vec3& v3, const UCol& colV3)
	{
		DrawTriangle(v0, colV0, v1, colV1, v2, colV2);
		DrawTriangle(v0, colV0, v2, colV2, v3, colV3);
	}

	virtual void DrawMesh(CVertexBuffer* pVertexBuffer, SVertexStream* pIndices, glm::mat4 transform, int texture) = 0;
	virtual void Flush()                                                                                           = 0;
};

// 2D Images added to the rendering frame
struct SRender2DImageDescription
{
	float        x = 0;
	float        y = 0;
	float        z = 0;
	float        w = 0;
	float        h = 0;
	Legacy::Vec2 uv[2]; //!< Texture UV coordinates
	float        angle       = 0;
	float        stereoDepth = 0; //!< Places image in stereo 3d space. The depth is specified in camera space.
	UCol         color;

	uint32       textureId = 0;
	uint32       targetId  = 0;

	// SDisplayContextKey displayContextKey;
	// SAuxGeomRenderFlags renderFlags = e_Def2DImageRenderflags;
};

// Helper class to abstract pushing 2d images for rendering
class IRenderAuxImage
{
	struct S2DImage
	{
		float        x = 0;
		float        y = 0;
		float        z = 0;
		float        w = 0;
		float        h = 0;
		Legacy::Vec2 uv[2]; // Texture UV coordinates
		float        angle       = 0;
		float        stereoDepth = 0;
		ColorB       color;

		uint32       textureId = 0;
		uint32       targetId  = 0;
	};

public:
	static void Draw2dImage(float xpos, float ypos, float w, float h, int texture_id, float s0 = 0, float t0 = 0, float s1 = 1, float t1 = 1, float angle = 0,
	                        float r = 1, float g = 1, float b = 1, float a = 1, float z = 1, float stereoDepth = 0)
	{
		if (Env::Renderer())
		{
			SRender2DImageDescription img;
			img.x           = xpos;
			img.y           = ypos;
			img.z           = z;
			img.w           = w;
			img.h           = h;
			img.textureId   = texture_id;
			// img.targetId = 0;
			img.uv[0].x     = s0;
			img.uv[0].y     = t0;
			img.uv[1].x     = s1;
			img.uv[1].y     = t1;
			img.angle       = angle;
			img.color       = UCol((uint8)(r * 255), (uint8)(r * 255), (uint8)(r * 255), (uint8)(r * 255));
			img.stereoDepth = stereoDepth;

			IRenderAuxGeom::GetAux()->PushImage(img);
		}
	}
	static void Draw2dImage(float xpos, float ypos, float w, float h, int texture_id, float s0, float t0, float s1, float t1, float angle, const ColorF& col)
	{
		if (Env::Renderer())
		{
			SRender2DImageDescription img;
			img.x           = xpos;
			img.y           = ypos;
			img.z           = 1.f;
			img.w           = w;
			img.h           = h;
			img.textureId   = texture_id;
			// img.targetId = 0;
			img.uv[0].x     = s0;
			img.uv[0].y     = t0;
			img.uv[1].x     = s1;
			img.uv[1].y     = t1;
			img.angle       = angle;
			img.color       = UCol(col);
			img.stereoDepth = 0.f;

			IRenderAuxGeom::GetAux()->PushImage(img);
		}
	}
	//! Draws a image using the current matrix.
	static void DrawImage(float xpos, float ypos, float w, float h, int texture_id, float s0, float t0, float s1, float t1, float r, float g, float b, float a, bool filtered = true)
	{
		if (Env::Renderer())
		{
			SRender2DImageDescription img;
			img.x           = xpos;
			img.y           = ypos;
			img.z           = 1.f;
			img.w           = w;
			img.h           = h;
			img.textureId   = texture_id;
			// img.targetId = 0;
			img.uv[0].x     = s0;
			img.uv[0].y     = t0;
			img.uv[1].x     = s1;
			img.uv[1].y     = t1;
			img.angle       = 0.f;
			img.color       = UCol((uint8)(r * 255), (uint8)(r * 255), (uint8)(r * 255), (uint8)(r * 255));
			img.stereoDepth = 0.f;

			IRenderAuxGeom::GetAux()->PushImage(img);
		}
	}
};
