#ifdef __cplusplus
// C++
	#pragma once
	#pragma pack(push,4)

	#define hlsl_cbuffer(name) struct alignas(16) HLSL_ ## name
	#define hlsl_cbuffer_register(name, reg, slot) \
	  enum { SLOT_ ## name = slot };               \
	  struct alignas(16) HLSL_ ## name

	#define hlsl_uint(member)     uint member
	#define hlsl_float(member)    float member
	#define hlsl_float2(member)   Legacy::Vec2 member
	#define hlsl_float3(member)   Legacy::Vec3 member
	#define hlsl_float4(member)   Legacy::Vec4 member
#if 0
	#define hlsl_matrix44(member) Matrix44 member
#else
	#define hlsl_matrix44(member) D3DXMATRIX member
#endif
	#define hlsl_matrix34(member) Matrix34 member
#else //__cplusplus
// HLSL
	#define hlsl_cbuffer(name)                      cbuffer name
	#define hlsl_cbuffer_register(name, reg, float) cbuffer name: reg
	#define hlsl_uint(member)                       uint member
	#define hlsl_float(member)                      float member
	#define hlsl_float2(member)                     float2 member
	#define hlsl_float3(member)                     float3 member
	#define hlsl_float4(member)                     float4 member
	#define hlsl_matrix44(member)                   float4x4 member
	#define hlsl_matrix34(member)                   float3x4 member
#endif //__cplusplus

hlsl_cbuffer_register(Light, register(b1), 1)
{
	hlsl_float3(Color);
	hlsl_float3(Pos);
};

hlsl_cbuffer_register(PerDrawCB, register(b3), 3)
{
	hlsl_matrix44(Model);
	hlsl_matrix44(World);
	hlsl_matrix44(MVP);
	hlsl_matrix44(MV);

	bool       ApplyGrayScale;
};

hlsl_cbuffer_register(MaterialCB, register(b4), 4)
{
	hlsl_float3(albedo);
	hlsl_float(metallic);
	hlsl_float(roughness);
	hlsl_float(ao);
};

