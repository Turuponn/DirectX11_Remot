#pragma once

#include <vector>
#include <memory>


class IndexBuffer;
struct ID3D11Buffer;
class D3D11DeviceAndSwapChainAndContextManager;

class IndexBufferManager {
public:
	IndexBufferManager();
	virtual ~IndexBufferManager();

	/// <summary>
	/// �C���f�b�N�X�o�b�t�@���T�C�Y���쐬����
	/// </summary>
	/// <param name="device"></param>
	/// <param name="buffsize"> �o�b�t�@�̑S�T�C�Y������</param>
	/// <param name="fecedata"> �C���f�b�N�X�p�̃f�[�^�����ׂē����</param>
	void CreateIndexBuffer(std::shared_ptr<D3D11DeviceAndSwapChainAndContextManager>& device, const unsigned int buffsize, void* fecedata);
	/// <summary>
	/// 
	/// </summary>
	/// <returns> �C���f�b�N�X�o�b�t�@��Ԃ� </returns>
	ID3D11Buffer*& GetIndexBuffer();
private:
	ID3D11Buffer* _indexbuffer;
};
