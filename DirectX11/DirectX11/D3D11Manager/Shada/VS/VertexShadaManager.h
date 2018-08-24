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
	* @brief ���_�V�F�[�_�쐬����
	* @param filepath �V�F�[�_�t�@�C���܂ł̃p�X
	* @param funcname �G���g���|�C���g
	*/
	void CreateVSShada(std::shared_ptr<D3D11DeviceAndSwapChainAndContextManager>& device, const TCHAR* filepath, const char* funcname);
	/**
	* @brief ���_�V�F�[�_�R���p�C�������̂�Ԃ�
	*/
	ID3DBlob*& GetVertexCompileShada();
	/**
	* @brief ���_�V�F�[�_��Ԃ�
	*/
	ID3D11VertexShader*& GetVertexShada();


private:
	
	ID3D11VertexShader* _vertexbuff;
	ID3DBlob* _vertexCompileShada;
};
