#pragma once

#include <tchar.h>

struct ID3D11Device;
struct ID3D11VertexShader;

struct ID3D10Blob;
typedef ID3D10Blob ID3DBlob;


class VertexShada {
public:
	VertexShada();
	virtual ~VertexShada();

	/**
	@brief 頂点シェーダ作成
	@param filename　シェーダファイル名までのパス
	@param funcname  関数名
	@param[out] vertexbuffer  頂点シェーダを返します
	@param [out] compileshada コンパイルシェーダを返します
	*/
	void CreateVertexShada(const TCHAR* filenamepath, const char* funcname, ID3D11Device*& device, ID3D11VertexShader** vertexbuffer, ID3DBlob** compileshada);

private:
};
