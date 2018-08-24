#pragma once

/*@note
http://uhiaha888.hatenablog.com/entry/20120901/1346479685 でvectorとsheardptrの話が載っている
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
	@brief 初期化
	@param[in] hwnd ウィンドウハンドルを入れる
	*/
	void Init(WinHWND& hwnd);
	/**
	@brief 更新
	@param[in] hwnd ウィンドウハンドルを入れる
	*/
	void Run();
	/**
	*@brief バックバッファの画面と入れ替える
	*/
	void ScreenFilp();
	/**
	* @brief　床作成
	* @return ハンドルを返す
	*/
	const int& CreatePrimitivPlane();
	/**
	* @brief 更新
	* @param [in] planehandle
	* @param [in] camerahandle　使用するメインカメラのイメージで作成
	*/
	void UpdatePlane(const int& planehandle, const int& camerahandle);
	/**
	* @brief　カメラ作成
	* @return ハンドルを返す
	*/
	const int& CameraCreate();
	/**
	* @brief 更新
	*/
	void UpdateCamera(const int& handle);
	/**
	* @brief テクスチャロード
	* @param filepath ファイルまでのパス
	* @return 0以外のハンドルを返す
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