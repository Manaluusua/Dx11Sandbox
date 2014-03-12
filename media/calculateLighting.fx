
Texture2D<float4> albedo : register(t0); 
Texture2D<float4> normal : register(t1); 
Texture2D<float4> specular : register(t2); 
Texture2D<float> depth : register(t3); 
 
RWTexture2D<float4> output : register(u0); 
 
#define BLOCK_SIZE 16 
[numthreads(BLOCK_SIZE,BLOCK_SIZE,1)] 
void MainCS( 
 uint3 groupId : SV_GroupID, 
 uint3 groupThreadId : SV_GroupThreadID, 
 uint groupIndex: SV_GroupIndex, 
 uint3 dispatchThreadId : SV_DispatchThreadID) 
{ 

	float4 a = albedo.Load(uint3(dispatchThreadId.xy, 0)); 
	output[dispatchThreadId.xy] = a;
} 



technique11 UnlitTextureScreen
{
	
    pass P0
    {

        SetVertexShader( NULL );
        SetGeometryShader( NULL );
        SetPixelShader( NULL );
		SetComputeShader( CompileShader( cs_5_0, MainCS() ) );
    }
}

