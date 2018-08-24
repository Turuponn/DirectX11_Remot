#pragma once

#include <memory>
#include <tchar.h>

struct ID3D11VertexShader;
class D3D11DeviceAndSwapChainAndContextManager;


struct ID3D10Blob;
typedef ID3D10Blob ID3DBlob;

class VertexShadaManager {
public:
	VertexShadaManager();
	virtual ~VertexShadaManager();


	/**
	* @brief 頂点シェーダ作成する
	* @param filepath シェーダファイルまでのパス
	* @param funcname エントリポイント
	*/
	void CreateVSShada(std::shared_ptr<D3D11DeviceAndSwapChainAndContextManager>& device, const TCHAR* filepath, const char* funcname);
	/**
	* @brief 頂点シェーダコンパイルしたのを返す
	*/
	ID3DBlob*& GetVertexCompileShada();
	/**
	* @brief 頂点シェーダを返す
	*/
	ID3D11VertexShader*& GetVertexShada();


private:
	
	ID3D11VertexShader* _vertexbuff;
	ID3DBlob* _vertexCompileShada;
};
