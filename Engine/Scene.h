#pragma once

class GameObject;


class Scene
{

public:
	void Awake();
	void Start();
	void Update();
	void LateUpdate();
	void FinalUpdate();

public:
	void Render();

	void RenderLayer(uint32 layer);
	void RenderObjects(uint32 layer);
	void RenderLights(uint32 layer);
	void RenderLayerEnd(uint32 layer);

	void RenderFinal();

public:
	void Load(const wstring& path);

public:
	void AddGameObject3D(shared_ptr<GameObject> gameObject);
	void AddGameObjectUI(shared_ptr<GameObject> gameObject);
	void RemoveGameObject(shared_ptr<GameObject> gameObject);

	const vector<shared_ptr<GameObject>>& GetGameObjects3D() { return _gameObjects3D; }
	const vector<shared_ptr<GameObject>>& GetGameObjectsUI() { return _gameObjectsUI; }

private:
	void PushLightData();

private:
	
	vector<shared_ptr<GameObject>>		_gameObjects3D;
	vector<shared_ptr<GameObject>>		_gameObjectsUI;

	vector<shared_ptr<class Camera>>	_cameras;
	vector<shared_ptr<class Light>>		_lights;
};
