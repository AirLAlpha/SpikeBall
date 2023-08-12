/**********************************************
 *
 *  SpriteNumber.cpp
 *  �X�v���C�g�ɂ�鐔���̕`��
 *
 *  ����ҁF���ە��m
 *  ������F2023/03/31
 *
 **********************************************/
#include "pch.h"
#include "SpriteNumber.h"
#include "Utilities/UtilityFunction.h"
#include "WICTextureLoader.h"

using namespace DirectX;

//	�e�N�X�`���p�X
const wchar_t* const		SpriteNumber::TEXTURE_PATH = L"Resources/Textures/numberx128.png";
//	�e�N�X�`���̕�����
const SimpleMath::Vector2	SpriteNumber::TEXTURE_SPLIT_COUNT = SimpleMath::Vector2(11.0f, 1.0f);

//	�����̊��o
const SimpleMath::Vector2	SpriteNumber::NUMBER_GAP = SimpleMath::Vector2(96.0f, 0.0f);

//	�A�E�g���C��
const SimpleMath::Vector2	SpriteNumber::OUTLINE_OFFSET[8] =
{
	SimpleMath::Vector2(0, -1),		//	��
	SimpleMath::Vector2(1, -1),		//	�E��
	SimpleMath::Vector2(0, 1),		//	�E
	SimpleMath::Vector2(1, 1),		//	�E��
	SimpleMath::Vector2(1, 0),		//	��
	SimpleMath::Vector2(-1, 1),		//	����
	SimpleMath::Vector2(-1, 0),		//	��
	SimpleMath::Vector2(-1, -1),	//	����
};


//	�R���X�g���N�^
SpriteNumber::SpriteNumber(ID3D11Device* device, ID3D11DeviceContext* context):
	m_color(Colors::White),
	m_outlineColor(Colors::Black)
{
	//	�X�v���C�g�o�b�`�̍쐬
	m_spriteBatch = std::make_unique<SpriteBatch>(context);

	//	�e�N�X�`���̓ǂݍ���
	Microsoft::WRL::ComPtr<ID3D11Resource> resource;
	DX::ThrowIfFailed(
		CreateWICTextureFromFile(
			device,
			TEXTURE_PATH,
			resource.ReleaseAndGetAddressOf(),
			m_texture.ReleaseAndGetAddressOf()
		)
	);

	//	�e�N�X�`���T�C�Y�̎擾
	Microsoft::WRL::ComPtr<ID3D11Texture2D> texture;
	resource.As(&texture);
	D3D11_TEXTURE2D_DESC desc;
	texture->GetDesc(&desc);
	//	�T�C�Y��ێ�
	m_textureSize.x = static_cast<float>(desc.Width);
	m_textureSize.y = static_cast<float>(desc.Height);

	resource.Reset();
	texture.Reset();
}

//	�f�X�g���N�^
SpriteNumber::~SpriteNumber()
{
}


//--------------------------------------------------------------------------------
// �`��J�n
//--------------------------------------------------------------------------------
void SpriteNumber::Begin(const CommonStates& states)
{
	//	�`��J�n
	m_spriteBatch->Begin(SpriteSortMode_Deferred, states.NonPremultiplied());
}

//--------------------------------------------------------------------------------
// �`��I��
//--------------------------------------------------------------------------------
void SpriteNumber::End()
{
	//	�`��I��
	m_spriteBatch->End();
}

//--------------------------------------------------------------------------------
// ���l�̕`��
//--------------------------------------------------------------------------------
void SpriteNumber::DrawNumber(
	const int& num, 
	const SimpleMath::Vector2& pos, 
	const Anchor& anchor,
	const SimpleMath::Vector2& offset, 
	const int& outlineWidth, 
	const int& digits
)
{
	//	�A�E�g���C���̕`��
	if (outlineWidth > 0)
	{
		for (size_t i = 0; i < 8; i++)
		{
			DrawNumberBase(num, pos + OUTLINE_OFFSET[i] * static_cast<float>(outlineWidth), anchor, offset, digits, m_outlineColor);
		}
	}

	//	�����̕`��
	DrawNumberBase(num, pos, anchor, offset, digits, m_color);
}

//--------------------------------------------------------------------------------
// ���l�̕`��iRECT�w��Łj
//--------------------------------------------------------------------------------
void SpriteNumber::DrawNumber(
	const RECT& rect,
	const SimpleMath::Vector2& pos,
	const Anchor& anchor, 
	const SimpleMath::Vector2& offset, 
	const int& outlineWidth
)
{
	const SimpleMath::Vector2 TEXTURE_SPRITE_SIZE = SimpleMath::Vector2(m_textureSize.x / TEXTURE_SPLIT_COUNT.x, m_textureSize.y / TEXTURE_SPLIT_COUNT.y);

	//	�����ʒu���v�Z
	float anchorOffset = 0;
	if (anchor != Anchor::LEFT)
		anchorOffset -= (rect.right - rect.left) / (int)anchor;			//	�A���J�[���W�����炷

	DirectX::SimpleMath::Vector2 numPos = pos + NUMBER_GAP * anchorOffset;
	//	�I�t�Z�b�g�����Z
	numPos += offset;

	//	�A�E�g���C���̕`��
	if (outlineWidth > 0)
	{
		for (size_t i = 0; i < 8; i++)
		{
			m_spriteBatch->Draw(m_texture.Get(), numPos + OUTLINE_OFFSET[i] * static_cast<float>(outlineWidth), &rect, m_outlineColor, 0.0f);
		}
	}

	//	�`��
	m_spriteBatch->Draw(m_texture.Get(), numPos, &rect, m_color, 0.0f);
}

//--------------------------------------------------------------------------------
// ���l�̕`��
//--------------------------------------------------------------------------------
void SpriteNumber::DrawNumberBase(
	const int& num,
	const SimpleMath::Vector2& pos,
	const Anchor& anchor,
	const SimpleMath::Vector2& offset,
	const int& digits,
	const XMVECTORF32& color
)
{
	const SimpleMath::Vector2 TEXTURE_SPRITE_SIZE = SimpleMath::Vector2(m_textureSize.x / TEXTURE_SPLIT_COUNT.x, m_textureSize.y / TEXTURE_SPLIT_COUNT.y);

	//	����
	int dig = GetDigits(num);
	//	���l�𕶎���ɕϊ�
	std::string numStr = std::to_string(num);

	//	�ڕW�̌����ɖ����Ȃ��Ƃ���0���߂���
	while (dig < digits)
	{
		//	������̐擪��0��t������
		numStr = "0" + numStr;
		//	���������Z
		dig++;
	}

	//	�����̕`��
	for (int i = 0; i < dig; i++)
	{
		int n = numStr[i] - '0';		//	�ꌅ���o��

		if (0 > n || n > 9)				//	���l�� 0 ~ 9 �͈̔͊O�Ȃ�X�L�b�v
			continue;

		RECT rect;						//	�e�N�X�`���̐؂蔲���ʒu
		rect.left	= static_cast<int>(TEXTURE_SPRITE_SIZE.x * n);				//	��
		rect.right	= static_cast<LONG>(TEXTURE_SPRITE_SIZE.x * (n + 1));		//	�E
		rect.top	= 0;														//	��
		rect.bottom = static_cast<LONG>(TEXTURE_SPRITE_SIZE.y);					//	��

		//	�����ʒu���v�Z
		float anchorOffset = static_cast<float>(i);
		if (anchor != Anchor::LEFT)
			anchorOffset -= (float)dig / (int)anchor;			//	�A���J�[���W�����炷

		DirectX::SimpleMath::Vector2 numPos = pos + NUMBER_GAP * anchorOffset;
		//	�I�t�Z�b�g�����Z
		numPos += offset;

		//	�`��
		m_spriteBatch->Draw(m_texture.Get(), numPos, &rect, color, 0.0f);
	}
}