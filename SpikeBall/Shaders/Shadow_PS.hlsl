#include "Shadow_Header.hlsli"

//	�x�[�X�e�N�X�`��
Texture2D baseTex : register(t0);
//  Z�l�e�N�X�`��
Texture2D zTex : register(t1);
//  �e�N�X�`���T���v��
SamplerState sam : register(s0);


float4 main(PS_INPUT input) : SV_TARGET
{
	//	�x�[�X�e�N�X�`���̎擾
    float4 col = baseTex.Sample(sam, input.Texture);
	
    //float4 col = lightColor;

	//  ���C�g�ڐ���Z�l���Z�o
	float z = input.CalculatablePos.z / input.CalculatablePos.w;

	// �ˉe��Ԃ�XY���W���e�N�X�`�����W�ɕϊ�
	float2 TransTexCoord;
	TransTexCoord.x = (1.0f + input.LightViewPos.x / input.LightViewPos.w) * 0.5f;
	TransTexCoord.y = (1.0f - input.LightViewPos.y / input.LightViewPos.w) * 0.5f;

	//  �e�N�X�`���͈̔͊O�ɂ���Ƃ��͕`�悵�Ȃ�
	if(TransTexCoord.x < 0.001f || 0.999f < TransTexCoord.x ||
		TransTexCoord.y < 0.001f || 0.999f < TransTexCoord.y)
		return col;
	
	// �e�N�X�`������Z�l���o
	float SM_Z = zTex.Sample(sam, TransTexCoord).x;
	
	// �Z�o�_���V���h�E�}�b�v��Z�l�����傫����Ήe�Ɣ��f
	if (z > SM_Z + 0.005f)
	{
		col.rgb = col.rgb * 0.5f;  
	}
	

	//  �����o�[�g�g�U
    float4 lDir = normalize(lightDir);
    float4 normal = normalize(input.Normal);
    float t = dot(normal, lDir);
    t = min(1.0f, t);
    col.rgb = col.rgb * t;
	
	return col;
}