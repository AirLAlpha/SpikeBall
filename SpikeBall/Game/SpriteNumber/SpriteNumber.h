/**********************************************
 *
 *  SpriteNumber.h
 *  SpriteNumber.cpp�̃w�b�_�[�t�@�C��
 *
 *  ����ҁF���ە��m
 *  ������F2023/03/31
 *
 **********************************************/
#pragma once


class SpriteNumber
{
public:
	//	�A���J�[
	enum class Anchor
	{
		LEFT	= 0,	//	��
		RIGHT	= 1,	//	�E
		CENTER	= 2,	//	����
	};

public:
	//	�e�N�X�`���p�X
	static const wchar_t* const		TEXTURE_PATH;

	//	�e�N�X�`���̕�����
	static const DirectX::SimpleMath::Vector2	TEXTURE_SPLIT_COUNT;
	//	�����̊��o
	static const DirectX::SimpleMath::Vector2	NUMBER_GAP;
	//	�A�E�g���C���̃I�t�Z�b�g
	static const DirectX::SimpleMath::Vector2	OUTLINE_OFFSET[8];


private:
	//	�X�v���C�g�o�b�`
	std::unique_ptr<DirectX::SpriteBatch>				m_spriteBatch;

	//	�e�N�X�`��
	Microsoft::WRL::ComPtr<ID3D11ShaderResourceView>	m_texture;
	//	�e�N�X�`���T�C�Y
	DirectX::SimpleMath::Vector2						m_textureSize;

	//	�F
	DirectX::XMVECTORF32	m_color;
	//	�A�E�g���C���F
	DirectX::XMVECTORF32	m_outlineColor;

private:
	//	���l�̕`��
	void DrawNumberBase(
		const int& num,
		const DirectX::SimpleMath::Vector2& pos,
		const Anchor& anchor,
		const DirectX::SimpleMath::Vector2& offset = DirectX::SimpleMath::Vector2::Zero,
		const int& digits = -1,
		const DirectX::XMVECTORF32& color = DirectX::Colors::White
	);


public:
	SpriteNumber(ID3D11Device* device, ID3D11DeviceContext* context);
	~SpriteNumber();

	//	�`��J�n
	void Begin(const DirectX::CommonStates& states);
	//	�`��I��
	void End();

	//	���l�̕`��
	void DrawNumber(
		const int& num,
		const DirectX::SimpleMath::Vector2& pos,
		const Anchor& anchor,
		const DirectX::SimpleMath::Vector2& offset = DirectX::SimpleMath::Vector2::Zero,
		const int& outlineWidth = 0,
		const int& digits = -1
	);

	//	���l�̕`��iRECT�w��Łj
	void DrawNumber(
		const RECT& rect,
		const DirectX::SimpleMath::Vector2& pos,
		const Anchor& anchor,
		const DirectX::SimpleMath::Vector2& offset = DirectX::SimpleMath::Vector2::Zero,
		const int& outlineWidth = 0
	);


public:
	//	�F�̐ݒ�
	void SetColor(const DirectX::XMVECTORF32& color) { m_color = color; }
	//	�A�E�g���C���F�̐ݒ�
	void SetOutlineColor(const DirectX::XMVECTORF32& color) { m_outlineColor = color; }

	//	�e�N�X�`���T�C�Y�̎擾
	DirectX::SimpleMath::Vector2 GetTextureSize() { return m_textureSize; }

};
