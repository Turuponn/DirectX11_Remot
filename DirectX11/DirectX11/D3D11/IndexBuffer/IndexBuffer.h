#pragma once



/************************************************************
* File Name	: IndexBuffer.h
* Description	: インデックスバッファ作成クラス
* Date		:	2018-7-12.
* Author	:	鶴崎
****************************************************/

struct ID3D11Device;
struct ID3D11Buffer;

class IndexBuffer {
public:

	IndexBuffer();
	virtual ~IndexBuffer();

	/// <summary>
	/// インデックスバッファを作成する
	/// </summary>
	/// <param name="device"></param> 
	/// <param name="buffsize"> 1インデックスに対するバッファのサイズ </param>
	/// <param name="fecedata"> 1インデックスに対するデータサイズ </param>
	/// <param name="indexbuffer"> [out] インデックスバッファを返す</param>
	void CreateIndexBuffer(ID3D11Device*& device, const unsigned int buffsize, void* fecedata, ID3D11Buffer** indexbuffer);


private:

};