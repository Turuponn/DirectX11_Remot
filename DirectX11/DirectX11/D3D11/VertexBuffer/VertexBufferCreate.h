#pragma once


struct ID3D11Device;
struct ID3D11Buffer;


class VertexBufferCreate {
public:
	VertexBufferCreate();
	virtual ~VertexBufferCreate();

	/**
	* @brief ���_�o�b�t�@�쐬
	* @param [out] vertexbuffer ���_�o�b�t�@��Ԃ�
	* @param [in] vertexdata ���_�f�[�^������
	* @detail  buffersize = onevertexsize*allvertexarrysize
	*/
	void CreateVertexBuffer(ID3D11Device*& device, const int& buffersize, void* vertexdata, ID3D11Buffer** vertexbuffer);


private:
};