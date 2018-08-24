#pragma once


#include <memory>

struct ID3D11Buffer;
class D3D11DeviceAndSwapChainAndContextManager;

class VertexBufferManager {
public:
	VertexBufferManager();
	virtual ~VertexBufferManager();

	/**
	* @brief ���_�o�b�t�@�쐬
	* @param [in] vertexdata ���_�f�[�^
	* @param [in] onevertexsize �꒸�_������̒��_�T�C�Y
	* @param [in] vertexarrysize ���_�z��̗v�f��
	* @detail  buffersize = onevertexsize*vertexarrysize 
	*/
	void CreateVertexBuffer(std::shared_ptr<D3D11DeviceAndSwapChainAndContextManager>& device, void* vertexdata, const size_t& onevertexsize, const int& vertexarrysize);
	/**
	* @brief ���_�o�b�t�@��Ԃ�
	* @return 
	*/
	ID3D11Buffer*& GetVertexBuffer();

private:
	
	ID3D11Buffer* _vertexbuffer;

};