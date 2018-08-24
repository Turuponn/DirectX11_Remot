#pragma once

struct ID3D11Device;
struct D3D11_INPUT_ELEMENT_DESC;

struct ID3D10Blob;
typedef ID3D10Blob ID3DBlob;



struct ID3D11InputLayout;

class CreateInputLayout {
public:
	CreateInputLayout();
	virtual ~CreateInputLayout();

	/**
	@brief 頂点インプットレイアウト作成
	@param inputlaout
	@param numelement inputlaoutの要素数
	@param vertex_compileshada  頂点コンパイル済みのブロブを入れます
	@param [out] vertex_inputlaout 頂点インプットレイアウトを返します
	*/
	void Create(ID3D11Device*& device, const D3D11_INPUT_ELEMENT_DESC* inputlaout, const unsigned int& numelement, ID3DBlob*& vertex_compileshada, ID3D11InputLayout** vertex_inputlaout);

private:

};
