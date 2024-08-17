#include "pch.h"
#include "TestScene_01.h"

#include "Scene.h"
#include "SceneManager.h"

#include "GameObject.h"
#include "MeshRenderer.h"
#include "Engine.h"
#include "Material.h"
#include "Transform.h"
#include "Camera.h"
#include "TestCameraScript.h"
#include "Resources.h"
#include "Light.h"


void TestScene_01::Load(const wstring& path)
{
#pragma region Camera
	{
		shared_ptr<GameObject> camera = make_shared<GameObject>();
		camera->AddComponent(make_shared<Transform>());
		camera->AddComponent(make_shared<Camera>()); // Near=1, Far=1000, FOV=45µµ
		camera->AddComponent(make_shared<TestCameraScript>());
		camera->GetTransform()->SetLocalPosition(Vec3(-100.f, 160.f, -100.f));
		camera->GetTransform()->SetLocalRotation(Vec3(3.14f / 8, 3.14f/4, 0.f));
		uint8 layerIndex = GET_SINGLE(SceneManager)->LayerNameToIndex(L"UI");
		camera->GetCamera()->SetCullingMaskLayerOnOff(layerIndex, true);
		AddGameObject3D(camera);
	}
#pragma endregion

#pragma region UI_Camera
	{
		shared_ptr<GameObject> camera = make_shared<GameObject>();
		camera->SetName(L"Orthographic_Camera");
		camera->AddComponent(make_shared<Transform>());
		camera->AddComponent(make_shared<Camera>()); // Near=1, Far=1000, 800*600
		camera->GetTransform()->SetLocalPosition(Vec3(0.f, 0.f, 0.f));
		camera->GetCamera()->SetProjectionType(PROJECTION_TYPE::ORTHOGRAPHIC);
		uint8 layerIndex = GET_SINGLE(SceneManager)->LayerNameToIndex(L"UI");
		camera->GetCamera()->SetCullingMaskAll(); // ´Ù ²ô°í
		camera->GetCamera()->SetCullingMaskLayerOnOff(layerIndex, false); // UI¸¸ ÂïÀ½
		AddGameObject3D(camera);
	}
#pragma endregion

#pragma region SkyBox
	{
		
		shared_ptr<GameObject> skybox = make_shared<GameObject>();
		skybox->AddComponent(make_shared<Transform>());
		skybox->SetCheckFrustum(false);
		shared_ptr<MeshRenderer> meshRenderer = make_shared<MeshRenderer>();
		{
			shared_ptr<Mesh> sphereMesh = GET_SINGLE(Resources)->LoadSphereMesh();
			meshRenderer->SetMesh(sphereMesh);
		}
		{
			shared_ptr<Shader> shader = GET_SINGLE(Resources)->Get<Shader>(L"Skybox");
			shared_ptr<Texture> texture = GET_SINGLE(Resources)->Load<Texture>(L"Sky01", L"..\\Resource\\Texture\\space_skybox.jpg");
			shared_ptr<Material> material = make_shared<Material>();
			material->SetShader(shader);
			material->SetTexture(0, texture);
			meshRenderer->SetMaterial(material);
		}
		skybox->AddComponent(meshRenderer);
		AddGameObjectUI(skybox);
		
	}
#pragma endregion

#pragma region Object
	{
		for (int i = 0; i < 9; i++)
		{
			float x_pos = 320 - 110 * (i % 3);
			float z_pos = 320 - 110 * (i / 3);

			shared_ptr<GameObject> obj = make_shared<GameObject>();
			obj->AddComponent(make_shared<Transform>());
			obj->GetTransform()->SetLocalScale(Vec3(100.f, 100.f, 100.f));
			obj->GetTransform()->SetLocalPosition(Vec3(x_pos, 0.f, z_pos));
			shared_ptr<MeshRenderer> meshRenderer = make_shared<MeshRenderer>();
			{
				shared_ptr<Mesh> sphereMesh = GET_SINGLE(Resources)->LoadSphereMesh();
				meshRenderer->SetMesh(sphereMesh);
			}
			{
				shared_ptr<Shader> shader = GET_SINGLE(Resources)->Get<Shader>(L"Deferred_Transparent");
				shared_ptr<Texture> texture = GET_SINGLE(Resources)->Load<Texture>(L"Glass", L"..\\Resource\\Texture\\Glass_Window_basecolor.jpg");
				shared_ptr<Texture> texture2 = GET_SINGLE(Resources)->Load<Texture>(L"Glass_Normal", L"..\\Resource\\Texture\\Glass_Window_normal.jpg");
				shared_ptr<Material> material = make_shared<Material>();
				material->SetShader(shader);
				material->SetTexture(0, texture);
				material->SetTexture(1, texture2);
				meshRenderer->SetMaterial(material);
			}
			obj->AddComponent(meshRenderer);
			AddGameObject3D(obj);
		}
		
	}
#pragma endregion

#pragma region UI_Test
	for (int32 i = 0; i < 6; i++)
	{
		shared_ptr<GameObject> sphere = make_shared<GameObject>();
		sphere->SetLayerIndex(GET_SINGLE(SceneManager)->LayerNameToIndex(L"UI")); // UI
		sphere->AddComponent(make_shared<Transform>());
		sphere->GetTransform()->SetLocalScale(Vec3(200.f, 200.f, 200.f));
		sphere->GetTransform()->SetLocalPosition(Vec3(-700.f + (i * 220), 500.f, 500.f));
		shared_ptr<MeshRenderer> meshRenderer = make_shared<MeshRenderer>();
		{
			shared_ptr<Mesh> mesh = GET_SINGLE(Resources)->LoadRectangleMesh();
			meshRenderer->SetMesh(mesh);
		}
		{
			shared_ptr<Shader> shader = GET_SINGLE(Resources)->Get<Shader>(L"Texture");

			shared_ptr<Texture> texture;

			if (i < 5)
			{
				//wstring textureName = L"AdditionalDepthBuffer_" + std::to_wstring(i);
				wstring textureName = L"LayerRenderResult_" + std::to_wstring(i);
				texture = GET_SINGLE(Resources)->Get<Texture>(textureName);
			}
			else 
			{
				texture = GET_SINGLE(Resources)->Get<Texture>(L"InitialZeroDepthBuffer");
			}
			

			shared_ptr<Material> material = make_shared<Material>();
			material->SetShader(shader);
			material->SetTexture(0, texture);
			meshRenderer->SetMaterial(material);
		}
		sphere->AddComponent(meshRenderer);
		AddGameObjectUI(sphere);
	}
#pragma endregion

#pragma region Directional Light
	{
		shared_ptr<GameObject> light = make_shared<GameObject>();
		light->AddComponent(make_shared<Transform>());
		//light->GetTransform()->SetLocalPosition(Vec3(0.f, 150.f, 150.f));
		light->AddComponent(make_shared<Light>());
		light->GetLight()->SetLightDirection(Vec3(0, 0, 1.f));
		light->GetLight()->SetLightType(LIGHT_TYPE::DIRECTIONAL_LIGHT);
		light->GetLight()->SetDiffuse(Vec3(1.f, 0.8f, 0.8f));
		light->GetLight()->SetAmbient(Vec3(0.8f, 0.64f, 0.64f));
		light->GetLight()->SetSpecular(Vec3(0.5f, 0.25f, 0.25f));

		AddGameObject3D(light);
	}
#pragma endregion

#pragma region Point Light
	{
		shared_ptr<GameObject> light = make_shared<GameObject>();
		light->AddComponent(make_shared<Transform>());
		light->GetTransform()->SetLocalPosition(Vec3(0.f, 100.f, 150.f));
		light->AddComponent(make_shared<Light>());
		//light->GetLight()->SetLightDirection(Vec3(-1.f, -1.f, 0));
		light->GetLight()->SetLightType(LIGHT_TYPE::DIRECTIONAL_LIGHT); // ÀÓ½Ã
		light->GetLight()->SetDiffuse(Vec3(0.0f, 1.0f, 1.0f));
		light->GetLight()->SetAmbient(Vec3(0.0f, 0.8f, 0.8f));
		light->GetLight()->SetSpecular(Vec3(0.0f, 0.3f, 0.3f));
		light->GetLight()->SetLightRange(200.f);

		AddGameObject3D(light);
	}
#pragma endregion

#pragma region Spot Light
	{
		shared_ptr<GameObject> light = make_shared<GameObject>();
		light->AddComponent(make_shared<Transform>());
		light->GetTransform()->SetLocalPosition(Vec3(75.f, 0.f, 150.f));
		light->AddComponent(make_shared<Light>());
		light->GetLight()->SetLightDirection(Vec3(-1.f, 0, 0));
		light->GetLight()->SetLightType(LIGHT_TYPE::SPOT_LIGHT);
		light->GetLight()->SetDiffuse(Vec3(0.0f, 0.f, 0.5f));
		light->GetLight()->SetAmbient(Vec3(0.0f, 0.0f, 0.1f));
		light->GetLight()->SetSpecular(Vec3(0.0f, 0.0f, 0.1f));
		light->GetLight()->SetLightRange(200.f);
		light->GetLight()->SetLightAngle(3.14f / 2);

		AddGameObject3D(light);
	}
#pragma endregion



}
