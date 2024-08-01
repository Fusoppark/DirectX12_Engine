#include "pch.h"
#include "Camera.h"
#include "Transform.h"
#include "Scene.h"
#include "SceneManager.h"
#include "GameObject.h"
#include "MeshRenderer.h"
#include "Engine.h"


Matrix Camera::S_MatView;
Matrix Camera::S_MatProjection;

Camera::Camera() : Component(COMPONENT_TYPE::CAMERA)
{
}

Camera::~Camera()
{
}

void Camera::FinalUpdate()
{
	_matView = GetTransform()->GetLocalToWorldMatrix().Invert();

	float width = static_cast<float>(GEngine->GetWindow().width);
	float height = static_cast<float>(GEngine->GetWindow().height);

	if (_type == PROJECTION_TYPE::PERSPECTIVE)
		_matProjection = ::XMMatrixPerspectiveFovLH(_fov, width / height, _near, _far);
	else
		_matProjection = ::XMMatrixOrthographicLH(width * _scale, height * _scale, _near, _far);

	_frustum.FinalUpdate();
}

bool Camera::IsVisible(const shared_ptr<GameObject>& obj)
{
	if (!(obj->GetMeshRenderer()))
		return false;

	if (IsCulled(obj->GetLayerIndex()))
		return false;

	if (obj->GetCheckFrustum())
	{
		if (_frustum.ContainsSphere(
			obj->GetTransform()->GetWorldPosition(),
			obj->GetTransform()->GetBoundingSphereRadius()) == false)
		{
			return false;
		}
	}

	return true;
}

void Camera::Render_Deferred()
{
	shared_ptr<Scene> scene = GET_SINGLE(SceneManager)->GetActiveScene();
	const vector<shared_ptr<GameObject>>& gameObjects = scene->GetGameObjects3D();

	S_MatView = _matView;
	S_MatProjection = _matProjection;

	for (auto& obj : gameObjects)
	{
		if (obj && IsVisible(obj)) obj->GetMeshRenderer()->Render();
	}
}

void Camera::Render_ForwardUI()
{
	shared_ptr<Scene> scene = GET_SINGLE(SceneManager)->GetActiveScene();
	const vector<shared_ptr<GameObject>>& gameObjects = scene->GetGameObjectsUI();

	S_MatView = _matView;
	S_MatProjection = _matProjection;

	for (auto& obj : gameObjects)
	{
		if (obj && IsVisible(obj)) obj->GetMeshRenderer()->Render();
	}
}

/*
void Camera::SortGameObject()
{
	shared_ptr<Scene> scene = GET_SINGLE(SceneManager)->GetActiveScene();
	const vector<shared_ptr<GameObject>>& gameObjects = scene->GetGameObjects();

	_vecForward.clear();
	_vecDeferred.clear();

	for (auto& gameObject : gameObjects)
	{
		if (gameObject->GetMeshRenderer() == nullptr)
			continue;

		if (IsCulled(gameObject->GetLayerIndex()))
			continue;

		if (gameObject->GetCheckFrustum())
		{
			if (_frustum.ContainsSphere(
				gameObject->GetTransform()->GetWorldPosition(),
				gameObject->GetTransform()->GetBoundingSphereRadius()) == false)
			{
				continue;
			}
		}

		SHADER_TYPE shaderType = gameObject->GetMeshRenderer()->GetMaterial()->GetShader()->GetShaderType();
		switch (shaderType)
		{
		case SHADER_TYPE::DEFERRED:
			_vecDeferred.push_back(gameObject);
			break;
		case SHADER_TYPE::FORWARD:
			_vecForward.push_back(gameObject);
			break;
		}
	}
}
*/