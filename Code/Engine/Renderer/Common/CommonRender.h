#pragma once

//////////////////////////////////////////////////////////////////////
class CRenderer;
extern CRenderer* gRenDev;

class CShader;

struct GlobalResources
{
	static ID3DShaderResourceView* FontAtlasRV;

	static ID3DShaderResourceView* WiteTextureRV;
	static ID3DShaderResourceView* GreyTextureRV;

	static ID3D11SamplerState* LinearSampler;

	static ID3D10EffectTechnique* BoxTechnique;
	static ID3D10EffectTechnique* MeshTechnique;

	static ID3DInputLayout* VERTEX_FORMAT_P3F_C4B_T2F_Layout;

	static _smart_ptr<CShader> SpriteShader;
	static _smart_ptr<CShader> TexturedQuadShader;

	//static _smart_ptr<CShader> GrayScale




	static ID3D11BlendState* FontBlendState;

};
