struct GlobalResources
{
	static ID3D10ShaderResourceView* FontAtlasRV;

	static ID3D10ShaderResourceView* WiteTextureRV;
	static ID3D10ShaderResourceView* GreyTextureRV;

	static ID3D10SamplerState* LinearSampler;

	static ID3D10EffectTechnique* BoxTechnique;
	static ID3D10EffectTechnique* MeshTechnique;

};
