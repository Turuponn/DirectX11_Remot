#pragma once

/*@note
http://uhiaha888.hatenablog.com/entry/20120901/1346479685 ��vector��sheardptr�̘b���ڂ��Ă���
*/

#include <string>
#include <memory>
#include <vector>
#include <map>
#include <unordered_map>



class D3D11DeviceAndSwapChainAndContextManager;
class Graphics;
struct WinHWND;
class Plane;
class CameraManager;
class FBXLoaderManager;





class GameEngine{
private:


public:
	GameEngine();
	virtual ~GameEngine();

	/**
	@brief ������
	@param[in] hwnd �E�B���h�E�n���h��������
	*/
	void Init(WinHWND& hwnd);
	/**
	@brief �X�V
	@param[in] hwnd �E�B���h�E�n���h��������
	*/
	void Run();
	/**
	*@brief �o�b�N�o�b�t�@�̉�ʂƓ���ւ���
	*/
	void ScreenFilp();
	/**
	* @brief�@���쐬
	* @return �n���h����Ԃ�
	*/
	const int& CreatePrimitivPlane();
	/**
	* @brief �X�V
	* @param [in] planehandle
	* @param [in] camerahandle�@�g�p���郁�C���J�����̃C���[�W�ō쐬
	*/
	void UpdatePlane(const int& planehandle, const int& camerahandle);
	/**
	* @brief�@�J�����쐬
	* @return �n���h����Ԃ�
	*/
	const int& CameraCreate();
	/**
	* @brief �X�V
	*/
	void UpdateCamera(const int& handle);
	/**
	* @brief �e�N�X�`�����[�h
	* @param filepath �t�@�C���܂ł̃p�X
	* @return 0�ȊO�̃n���h����Ԃ�
	*/
	int LoadTexture(const std::wstring& filepath);


	int LoadFBXModel(const std::string& filepath);
	void DrawFBXModel(const int handle);

private:
	



	std::shared_ptr<D3D11DeviceAndSwapChainAndContextManager> _device;
	std::shared_ptr<Graphics> _graphic;



	

	
	std::map<const int,std::shared_ptr<Plane>> _planehandles;
	std::map<const int, std::shared_ptr<CameraManager>> _camerahandles;

	
	std::map<const int, std::shared_ptr<FBXLoaderManager>> _fbxmodels;

	

};