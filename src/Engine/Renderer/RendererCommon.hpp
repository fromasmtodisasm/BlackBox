class CShader;

struct GlobalResources
{
	static ID3D10ShaderResourceView* FontAtlasRV;

	static ID3D10ShaderResourceView* WiteTextureRV;
	static ID3D10ShaderResourceView* GreyTextureRV;

	static ID3D10SamplerState* LinearSampler;

	static ID3D10EffectTechnique* BoxTechnique;
	static ID3D10EffectTechnique* MeshTechnique;

	static ID3D10InputLayout* VERTEX_FORMAT_P3F_C4B_T2F_Layout;

	static _smart_ptr<CShader> SpriteShader;
	static _smart_ptr<CShader> TexturedQuadShader;




	static ID3D10BlendState* FontBlendState;

};
