#pragma once

#include "d3drmobject_impl.h"
#include "d3drmrenderer.h"
#include "miniwin/d3drm.h"

#include <SDL3/SDL.h>
#include <vector>

struct DeferredDrawCommand {
	DWORD meshId;
	D3DRMMATRIX4D modelViewMatrix;
	Matrix3x3 normalMatrix;
	Appearance appearance;
	float depth;
};

class Direct3DRMDeviceImpl;
class Direct3DRMFrameImpl;

struct Direct3DRMViewportImpl : public Direct3DRMObjectBaseImpl<IDirect3DRMViewport> {
	Direct3DRMViewportImpl(DWORD width, DWORD height, Direct3DRMRenderer* renderer);
	HRESULT Render(IDirect3DRMFrame* group) override;
	/**
	 * @brief Blit the render back to our backbuffer
	 */
	HRESULT ForceUpdate(int x, int y, int w, int h) override;
	HRESULT Clear() override;
	HRESULT SetCamera(IDirect3DRMFrame* camera) override;
	HRESULT GetCamera(IDirect3DRMFrame** camera) override;
	HRESULT SetProjection(D3DRMPROJECTIONTYPE type) override;
	D3DRMPROJECTIONTYPE GetProjection() override;
	HRESULT SetFront(D3DVALUE z) override;
	D3DVALUE GetFront() override;
	HRESULT SetBack(D3DVALUE z) override;
	D3DVALUE GetBack() override;
	HRESULT SetField(D3DVALUE field) override;
	D3DVALUE GetField() override;
	DWORD GetWidth() override;
	DWORD GetHeight() override;
	HRESULT Transform(D3DRMVECTOR4D* screen, D3DVECTOR* world) override;
	HRESULT InverseTransform(D3DVECTOR* world, D3DRMVECTOR4D* screen) override;
	HRESULT Pick(float x, float y, LPDIRECT3DRMPICKEDARRAY* pickedArray) override;
	void CloseDevice();

private:
	HRESULT RenderScene();
	void CollectLightsFromFrame(IDirect3DRMFrame* frame, D3DRMMATRIX4D parentMatrix, std::vector<SceneLight>& lights);
	void CollectMeshesFromFrame(IDirect3DRMFrame* frame, D3DRMMATRIX4D parentMatrix);
	void BuildViewFrustumPlanes();
	void UpdateProjectionMatrix();
	Direct3DRMRenderer* m_renderer;
	std::vector<DeferredDrawCommand> m_deferredDraws;
	D3DCOLOR m_backgroundColor = 0xFF000000;
	DWORD m_width;
	DWORD m_height;
	D3DRMMATRIX4D m_viewProjectionwMatrix;
	D3DRMMATRIX4D m_viewMatrix;
	D3DRMMATRIX4D m_projectionMatrix;
	D3DRMMATRIX4D m_inverseProjectionMatrix;
	IDirect3DRMFrame* m_rootFrame = nullptr;
	IDirect3DRMFrame* m_camera = nullptr;
	D3DVALUE m_front = 1.f;
	D3DVALUE m_back = 10.f;
	D3DVALUE m_field = 0.5f;
	Plane m_frustumPlanes[6];
};

struct Direct3DRMViewportArrayImpl
	: public Direct3DRMArrayBaseImpl<IDirect3DRMViewport, Direct3DRMViewportImpl, IDirect3DRMViewportArray> {
	using Direct3DRMArrayBaseImpl::Direct3DRMArrayBaseImpl;
};
