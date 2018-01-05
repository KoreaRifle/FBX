Texture2D _map;
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
    float3 normal : NORMAL0;
	float2 uv : TEXCOORD0;
};

struct PixelInput
{
	float4 position : SV_POSITION;
    float3 normal : NORMAL0;
    float2 uv : TEXCOORD0;
};

PixelInput VS(VertexInput input)
{
	PixelInput output;

	input.position.w = 1.0f;

    output.position = mul(input.position, _world);
    output.position = mul(output.position, _view);
    output.position = mul(output.position, _projection);

    output.normal = mul(input.normal, (float3x3) _world);
    output.normal = normalize(output.normal);

	output.uv = input.uv;

	return output;
}

float4 PS(PixelInput input) : SV_TARGET
{
    float4 diffuseMap = _map.Sample(samp, input.uv);
    float4 result = _ambient;

    float3 light = _lightDir * -1;
    
    float intensity = saturate(dot(input.normal, light));

    if(intensity > 0.0f)
        result += _diffuse * intensity;
    
    result = saturate(result) * diffuseMap;

    return result;
}