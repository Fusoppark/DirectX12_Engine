#include "pch.h"
#include "SceneManager.h"
#include "Scene.h"

void SceneManager::Update()
{
	if (_activeScene == nullptr)
		return;

	_activeScene->Update();
	_activeScene->LateUpdate();
	_activeScene->FinalUpdate();
}

void SceneManager::LoadScene(wstring sceneName)
{
	// TODO : sceneName을 통해 Scene을 가져오는 로직이 필요
	// Scene 을 파일로 불러오는 로직 구현과 함께 추가 예정
\
	// shared_ptr<Scene> scene = LoadSceneByName(sceneName);
	// LoadScene(scene);
}

void SceneManager::LoadScene(shared_ptr<Scene> scene)
{
	_activeScene = scene;

	_activeScene->Awake();
	_activeScene->Start();
}

void SceneManager::Render()
{
	if (_activeScene)
		_activeScene->Render();
}

void SceneManager::InitLayers()
{
#pragma region LayerMask
	SetLayerName(0, L"Default");
	SetLayerName(1, L"UI");
#pragma endregion
}

void SceneManager::SetLayerName(uint8 index, const wstring& name)
{
	// 기존 데이터 삭제
	const wstring& prevName = _layerNames[index];
	_layerIndex.erase(prevName);

	_layerNames[index] = name;
	_layerIndex[name] = index;
}

uint8 SceneManager::LayerNameToIndex(const wstring& name)
{
	auto findIt = _layerIndex.find(name);
	if (findIt == _layerIndex.end())
		return 0;

	return findIt->second;
}
