#include "pch.h"
#include "Game.h"
#include "Engine.h"
#include "SceneManager.h"
#include "TestScene_01.h"
#include "Scene.h"


void Game::Init(const WindowInfo& info)
{
	GEngine->Init(info);

	shared_ptr<TestScene_01> testScene = make_shared<TestScene_01>();
	testScene->Load(L"start loading");

	GET_SINGLE(SceneManager)->LoadScene(testScene);
}

void Game::Update()
{
	GEngine->Update();
}