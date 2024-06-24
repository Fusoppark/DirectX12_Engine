#pragma once

class Scene;

enum
{
	MAX_LAYER = 32
};

class SceneManager
{
	DECLARE_SINGLE(SceneManager);

public:
	void Update();
	void Render();
	void LoadScene(wstring sceneName);
	void LoadScene(shared_ptr<Scene> scene);

public :
	void InitLayers();
	void SetLayerName(uint8 index, const wstring& name);
	const wstring& IndexToLayerName(uint8 index) { return _layerNames[index]; }
	uint8 LayerNameToIndex(const wstring& name);

public:
	shared_ptr<Scene> GetActiveScene() { return _activeScene; }

private:
	shared_ptr<Scene> _activeScene;

	// 이건 Scene으로 가야하는 거 아닌가??
	array<wstring, MAX_LAYER> _layerNames;
	map<wstring, uint8> _layerIndex;
};

