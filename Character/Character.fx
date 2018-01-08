SamplerState samp;

cbuffer CameraBuffer : register(b0)
{
    matrix _view;
    matrix _projection;
}

cbuffer WorldBuffer : register(b1)
{
    matrix _world;
};

cbuffer SunBuffer : register(b0)
{
    float3 _lightDir;
    float _padding;
};

cbuffer MaterialBuffer : register(b1)
{
    float4 _ambient;
    float4 _diffuse;
};

struct VertexInput
{
    float4 position : POSITION0;
    float4 color : COLOR0;
};

struct PixelInput
{
    float4 position : SV_POSITION;
    float4 color : COLOR0;
};

PixelInput VS(VertexInput input)
{
    PixelInput output;

    output.position = mul(input.position, _world);
    output.position = mul(output.position, _view);
    output.position = mul(output.position, _projection);

    output.color = input.color;	

    return output;
}

float4 PS(PixelInput input) : SV_TARGET
{
    return input.color;
}