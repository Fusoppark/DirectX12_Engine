# DX12 Deferred Engine
그래픽스 공부 목적으로 제작 중인 Deferred Engine 입니다.
[S.T.A.L.K.E.R](https://developer.nvidia.com/gpugems/gpugems2/part-ii-shading-lighting-and-shadows/chapter-9-deferred-shading-stalker) 게임 엔진을 따라가는 것을 목표로 제작하고 있습니다.

![Helios](data/screenshot_1.jpg)





## 기능 설명
인프런 [DX12 강의](https://www.inflearn.com/course/%EC%96%B8%EB%A6%AC%EC%96%BC-3d-mmorpg-2/dashboard)에서 시작하여 현재는 S.T.A.L.K.E.R 게임을 참고하며 기능을 추가하고 있습니다.
아래는 추가한 기능 목록입니다.





## 주요 기능
### Transparent(불투명) Object Rendering
[Depth Peeling](https://my.eng.utah.edu/~cs5610/handouts/order_independent_transparency.pdf)(2001, Cass Everitt)을 사용해서 불투명한 오브젝트도 렌더할 수 있도록 했습니다.
Depth 정보 저장용으로 RenderTarget을 추가해서 Depth Buffer를 대신했고, 
전체 렌더 패스를 5번 반복하여 5개의 Layer를 Alpha Blend해서 최종 렌더 결과를 도출하도록 제작했습니다.
(24.08.17)
  
### 부가 기능
24.06.25 : Log 함수를 추가했습니다.

### 기타 로직/오류 수정
24.08.01 : Forward / Deferred 로 그려질 오브젝트를 매 프레임마다 Sort하는 로직을 수정했습니다.
24.06.23 : Cull None 으로 설정되어 Lighting이 중복 연산되는 버그를 수정했습니다.






## 개발 환경
Windows 10, Visual Studio 2022 Community Edition에서 제작했습니다.






## 조작 및 설명
* `W`/`A`/`S`/`D` - 카메라 '앞'/'오른쪽'/'뒤'/'왼쪽' 이동합니다.
* `Q`/`E` - 카메라를 '아래'/'위'로 회전합니다.
* 화면 내 5개의 Textures : 순서대로 1번~5번 Layer 렌더 결과입니다.
* 





## 참고 문헌 및 자료
* [glfw](https://github.com/glfw/glfw)
* [imgui](https://github.com/ocornut/imgui)
* [ImGuizmo](https://github.com/CedricGuillemet/ImGuizmo)
* [AssetCore](https://github.com/diharaw/AssetCore) 
* [nativefiledialog](https://github.com/mlabbe/nativefiledialog)
* [VulkanMemoryAllocator](https://github.com/GPUOpen-LibrariesAndSDKs/VulkanMemoryAllocator)
* [stb](https://github.com/nothings/stb)
