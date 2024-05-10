#include "common.fx"
// Copyright 2001-2019 Crytek GmbH / Crytek Group. All rights reserved.

// Shader global descriptions
float Script : STANDARDSGLOBAL
<
    string Script =
        // Determines if the shader will be made visible to UI elements
        // For example, if set the shader will be available in the Material Editor
        "Public;"
        // If set when SupportsFullDeferredShading is not set, we go through the tiled forward pass
        // In the tiled forward case, we will execute the pass defined in this file - and not Common*Pass
        "SupportsDeferredShading;"
        // Determines that the shader supports fully deferred shading, so will not go through the forward pass
        "SupportsFullDeferredShading;";
    
    string Description = "This shader is used for rendering sprites";
>;

typedef Texture2D<float4> ColorBuffer;
ColorBuffer text : register(t0);
SamplerState textSampler : register(s0) {
    Filter = MIN_MAG_MIP_LINEAR;
    AddressU = Wrap;
    AddressV = Wrap;
    AddressW = Wrap;
    MipLODBias = 0.0;
    MaxAnisotropy = 16;
    ComparisonFunc = LESS_EQUAL;
    BorderColor = float4(1.0, 1.0, 1.0, 1.0);
    MinLOD = 0;
    MaxLOD = 0;
};

struct VsOutput
{
    float4 Position : SV_Position;
    float2 TexCoords : TEXCOORD;
    float4 Color : COLOR;
};

typedef float4 vec4;
struct VsInput
{
    float3 Pos : POSITION;
    vec4 Color : COLOR0;
    float2 TC : TEXCOORD0;
};



VsOutput VSMain(VsInput IN)
{
    VsOutput Output;
    Output.Position = float4(IN.Pos.x,IN.Pos.y, 0.0, 1.0);
    Output.TexCoords = IN.TC;
    //Output.TexCoords = float2(IN.TC.x, 1.0 - IN.TC.y);
    Output.Color = IN.Color;
    return Output;
}  


float4 Font(VsOutput IN) : SV_Target0
{    
    // {{{{{{{{{{{{{{{{{
    return IN.Color * float4(1.0, 1.0, 1.0, text.Sample(textSampler, IN.TexCoords).r);
}


// same function declaration style as vertex shaders
// pixel shaders return the colour value of the pixel (hence the float4)
float4 GrayScalePS(float3 color) : SV_Target0
{
  // greyscale the pixel colour values
  // - perform a dot product between the pixel colour and the specified vector
  // - 0.222, 0.707, 0.071 is found throughout image processing for gray scale effects.
    float4 grey = dot(float3(0.222, 0.707, 0.071), color);
  
  // return the pixel colour in the form of a float4.          
    return grey;
}


float4 TexturedQuad(VsOutput IN) : SV_Target0
{    
    float4 color = IN.Color * text.Sample(textSampler, IN.TexCoords);
    return color;
}

Technique Font
<
    string description = "This is technique spesialized for font rendering";
>
{
    Pass p0<string script = "Decal";>
    {
        VertexShader = VSMain();
        PixelShader = Font();

        SetDepthStencilState(DisableDepth);
        SetBlendState(AlphaBlend);

        //BlendEnable = true;
        //SrcBlend = SRC_ALPHA;
        //DestBlend = INV_SRC_ALPHA;
        //BlendOp = ADD;
        //SrcBlendAlpha = ONE;
        //DestBlendAlpha = ZERO;
        //BlendOpAlpha = ADD;
    }
}

Technique TexturedQuad<string description = "This is technique spesialized for textured quad rendering";>
{
    Pass p0
    {
        FillMode = WireFrame;
        VertexShader = VSMain();
        PixelShader = TexturedQuad();
    }
}

