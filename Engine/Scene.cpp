#include "pch.h"
#include "Scene.h"
#include "GameObject.h"
#include "Camera.h"
#include "Light.h"
#include "Engine.h"
#include "Resources.h"

void Scene::Awake()
{
	for (const shared_ptr<GameObject>& gameObject : _gameObjects3D)
	{
		if (gameObject) gameObject->Awake();
	}
	for (const shared_ptr<GameObject>& gameObject : _gameObjectsUI)
	{
		if (gameObject) gameObject->Awake();
	}
}

void Scene::Start()
{
	for (const shared_ptr<GameObject>& gameObject : _gameObjects3D)
	{
		if (gameObject) gameObject->Start();
	}
	for (const shared_ptr<GameObject>& gameObject : _gameObjectsUI)
	{
		if (gameObject) gameObject->Start();
	}
}

void Scene::Update()
{
	for (const shared_ptr<GameObject>& gameObject : _gameObjects3D)
	{
		if(gameObject) gameObject->Update();
	}
	for (const shared_ptr<GameObject>& gameObject : _gameObjectsUI)
	{
		if (gameObject) gameObject->Update();
	}
	
}

void Scene::LateUpdate()
{
	for (const shared_ptr<GameObject>& gameObject : _gameObjects3D)
	{
		if (gameObject) gameObject->LateUpdate();
	}
	for (const shared_ptr<GameObject>& gameObject : _gameObjectsUI)
	{
		if (gameObject) gameObject->LateUpdate();
	}
}

void Scene::FinalUpdate() 
{
	for (const shared_ptr<GameObject>& gameObject : _gameObjects3D)
	{
		if (gameObject) gameObject->FinalUpdate();
	}
	for (const shared_ptr<GameObject>& gameObject : _gameObjectsUI)
	{
		if (gameObject) gameObject->FinalUpdate();
	}
}



void Scene::Render()
{
	PushLightData();

	// SwapChain Group �ʱ�ȭ
	int8 backIndex = GEngine->GetSwapChain()->GetBackBufferIndex();
	GEngine->GetRTGroup(RENDER_TARGET_GROUP_TYPE::SWAP_CHAIN)->ClearRenderTargetView(backIndex);

	// Deferred Group �ʱ�ȭ
	GEngine->GetRTGroup(RENDER_TARGET_GROUP_TYPE::G_BUFFER)->ClearRenderTargetView();

	// Lighting Group �ʱ�ȭ
	GEngine->GetRTGroup(RENDER_TARGET_GROUP_TYPE::LIGHTING)->ClearRenderTargetView();

	// Deferred OMSet
	GEngine->GetRTGroup(RENDER_TARGET_GROUP_TYPE::G_BUFFER)->OMSetRenderTargets();

	
	shared_ptr<Camera> mainCamera = _cameras[0];	// ���� �ѹ� �����Ұ�


	mainCamera->Render_Deferred();
	GEngine->GetRTGroup(RENDER_TARGET_GROUP_TYPE::G_BUFFER)->WaitTargetToResource();

	RenderLights();
	GEngine->GetRTGroup(RENDER_TARGET_GROUP_TYPE::LIGHTING)->WaitTargetToResource();

	RenderFinal();

	mainCamera->Render_ForwardUI();

	for (auto& camera : _cameras)
	{
		if (camera == mainCamera)
			continue;

		camera->Render_ForwardUI();
	}

}

void Scene::PushLightData()
{
	LightParams lightParams = {};

	for (auto& light : _lights)
	{
		const LightInfo& lightInfo = light->GetLightInfo();

		light->SetLightIndex(lightParams.lightCount);

		lightParams.lights[lightParams.lightCount] = lightInfo;
		lightParams.lightCount++;
	}

	CONST_BUFFER(CONSTANT_BUFFER_TYPE::GLOBAL)->SetGlobalData(&lightParams, sizeof(lightParams));
}

void Scene::RenderLights()
{
	GEngine->GetRTGroup(RENDER_TARGET_GROUP_TYPE::LIGHTING)->OMSetRenderTargets();

	// ������ �׸���.
	for (auto& light : _lights)
	{
		light->Render();
	}
}

void Scene::RenderFinal()
{
	// Swapchain OMSet
	int8 backIndex = GEngine->GetSwapChain()->GetBackBufferIndex();
	GEngine->GetRTGroup(RENDER_TARGET_GROUP_TYPE::SWAP_CHAIN)->OMSetRenderTargets(1, backIndex);

	GET_SINGLE(Resources)->Get<Material>(L"Final")->PushData();
	GET_SINGLE(Resources)->Get<Mesh>(L"Rectangle")->Render();
}



void Scene::Load(const wstring& path)
{
	// ���� Scene�� File ���·� ������ ���� ���
	return;
}


void Scene::AddGameObject3D(shared_ptr<GameObject> gameObject)
{
	// camera �� light�� �� ���� �ִ� ���� ������ ��
	if (gameObject->GetCamera())
	{
		_cameras.push_back(gameObject->GetCamera());
	}
	else if (gameObject->GetLight())
	{
		_lights.push_back(gameObject->GetLight());
	}

	_gameObjects3D.push_back(gameObject);
}

void Scene::AddGameObjectUI(shared_ptr<GameObject> gameObject)
{
	_gameObjectsUI.push_back(gameObject);
}

void Scene::RemoveGameObject(shared_ptr<GameObject> gameObject)
{
	// Scene�� �� ��ġ�� camera�� ���ŵȴٸ�?
	if (gameObject->GetCamera())
	{
		auto findIt = std::find(_cameras.begin(), _cameras.end(), gameObject->GetCamera());
		if (findIt != _cameras.end())
			_cameras.erase(findIt);
	}
	if (gameObject->GetLight())
	{
		auto findIt = std::find(_lights.begin(), _lights.end(), gameObject->GetLight());
		if (findIt != _lights.end())
			_lights.erase(findIt);
	}

	auto findIt = std::find(_gameObjects3D.begin(), _gameObjects3D.end(), gameObject);
	if (findIt != _gameObjects3D.end())
		_gameObjects3D.erase(findIt);

	auto findItUI = std::find(_gameObjectsUI.begin(), _gameObjectsUI.end(), gameObject);
	if (findItUI != _gameObjectsUI.end())
		_gameObjectsUI.erase(findIt);
}