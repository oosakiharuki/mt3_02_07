#include <Novice.h>
#include <Novice.h>
#include <cstdint>
#include <cassert>
#define _USE_MATH_DEFINES
#include <math.h>
#include "MyMath.h"


#include<imgui.h>
#include <algorithm>
const char kWindowTitle[] = "LC1B_03_オオサキ_ハルキ_タイトル";



Vector3 Transform(const Vector3& vector, const Matrix4x4& matrix) {
	Vector3 result{};

	result.x = vector.x * matrix.m[0][0] + vector.y * matrix.m[1][0] + vector.z * matrix.m[2][0] + 1.0f * matrix.m[3][0];
	result.y = vector.x * matrix.m[0][1] + vector.y * matrix.m[1][1] + vector.z * matrix.m[2][1] + 1.0f * matrix.m[3][1];
	result.z = vector.x * matrix.m[0][2] + vector.y * matrix.m[1][2] + vector.z * matrix.m[2][2] + 1.0f * matrix.m[3][2];
	float w = vector.x * matrix.m[0][3] + vector.y * matrix.m[1][3] + vector.z * matrix.m[2][3] + 1.0f * matrix.m[3][3];
	assert(w != 0.0f);
	result.x /= w;
	result.y /= w;
	result.z /= w;

	return result;
}

Vector3 Add(const Vector3& v1, const Vector3& v2) {
	Vector3 result;
	result.x = v1.x + v2.x;
	result.y = v1.y + v2.y;
	result.z = v1.z + v2.z;
	return result;
}

Vector3 Subtract(const Vector3& v1, const Vector3& v2) {
	Vector3 result{};
	result.x = v1.x - v2.x;
	result.y = v1.y - v2.y;
	result.z = v1.z - v2.z;
	return result;
}

float Length(const Vector3& v) {
	float result;
	result = (float)sqrt((v.x * v.x) + (v.y * v.y) + (v.z * v.z));
	return result;
}


struct AABB {
	Vector3 min;
	Vector3 max;
};

struct Segment {
	Vector3 origin;//始点
	Vector3 diff;  //終点　差分ベクトル
};

void DrawGrid(const Matrix4x4& viewProjectionMatrix, const Matrix4x4& viewportMatrix) {
	const float kGridHandleWidth = 2.0f;
	const uint32_t kSubdivision = 10;
	const float kGridEvery = (kGridHandleWidth * 2.0f) / float(kSubdivision);

	for (uint32_t xIndex = 0; xIndex <= kSubdivision; ++xIndex) {
		float x = -kGridHandleWidth + (xIndex * kGridEvery);
		unsigned int color = 0xAAAAAAFF;
		Vector3 start{ x,0.0f,-kGridHandleWidth };
		Vector3 end{ x,0.0f,kGridHandleWidth };

		Vector3 startScreen = Transform(Transform(start, viewProjectionMatrix), viewportMatrix);
		Vector3 endScreen = Transform(Transform(end, viewProjectionMatrix), viewportMatrix);

		if (x == 0.0f) {
			color = BLACK;
		}
		Novice::DrawLine((int)startScreen.x, (int)startScreen.y, (int)endScreen.x, (int)endScreen.y, color);
	}
	for (uint32_t zIndex = 0; zIndex <= kSubdivision; ++zIndex) {
		float z = -kGridHandleWidth + (zIndex * kGridEvery);
		unsigned int color = 0xAAAAAAFF;
		Vector3 start{ -kGridHandleWidth,0.0f,z };
		Vector3 end{ kGridHandleWidth,0.0f,z };

		Vector3 startScreen = Transform(Transform(start, viewProjectionMatrix), viewportMatrix);
		Vector3 endScreen = Transform(Transform(end, viewProjectionMatrix), viewportMatrix);

		if (z == 0.0f) {
			color = BLACK;
		}
		Novice::DrawLine((int)startScreen.x, (int)startScreen.y, (int)endScreen.x, (int)endScreen.y, color);

	}
}




void DrawaAABB(const AABB& aabb, const Matrix4x4& viewProjectionMatrix, const Matrix4x4& viewportMatrix, uint32_t color) {

	Vector3 Bottom[4] = {
		{ aabb.min.x,aabb.min.y,aabb.min.z },
		{ aabb.max.x,aabb.min.y,aabb.min.z },
		{ aabb.min.x,aabb.min.y,aabb.max.z },
		{ aabb.max.x,aabb.min.y,aabb.max.z }
	};

	Vector3 Top[4] = {
		{ aabb.min.x,aabb.max.y,aabb.min.z },
		{ aabb.max.x,aabb.max.y,aabb.min.z },
		{ aabb.min.x,aabb.max.y,aabb.max.z },
		{ aabb.max.x,aabb.max.y,aabb.max.z }
	};

	Vector3 BottomScreen[4] = {};
	Vector3 TopScreen[4] = {};


	for (uint32_t i = 0; i < 4; i++) {
		BottomScreen[i] = Transform(Transform(Bottom[i], viewProjectionMatrix), viewportMatrix);
		TopScreen[i] = Transform(Transform(Top[i], viewProjectionMatrix), viewportMatrix);
	}


	Novice::DrawLine((int)BottomScreen[0].x, (int)BottomScreen[0].y, (int)BottomScreen[1].x, (int)BottomScreen[1].y, color);
	Novice::DrawLine((int)BottomScreen[0].x, (int)BottomScreen[0].y, (int)BottomScreen[2].x, (int)BottomScreen[2].y, color);
	Novice::DrawLine((int)BottomScreen[1].x, (int)BottomScreen[1].y, (int)BottomScreen[3].x, (int)BottomScreen[3].y, color);
	Novice::DrawLine((int)BottomScreen[2].x, (int)BottomScreen[2].y, (int)BottomScreen[3].x, (int)BottomScreen[3].y, color);

	Novice::DrawLine((int)TopScreen[0].x, (int)TopScreen[0].y, (int)TopScreen[1].x, (int)TopScreen[1].y, color);
	Novice::DrawLine((int)TopScreen[0].x, (int)TopScreen[0].y, (int)TopScreen[2].x, (int)TopScreen[2].y, color);
	Novice::DrawLine((int)TopScreen[1].x, (int)TopScreen[1].y, (int)TopScreen[3].x, (int)TopScreen[3].y, color);
	Novice::DrawLine((int)TopScreen[2].x, (int)TopScreen[2].y, (int)TopScreen[3].x, (int)TopScreen[3].y, color);

	Novice::DrawLine((int)TopScreen[0].x, (int)TopScreen[0].y, (int)BottomScreen[0].x, (int)BottomScreen[0].y, color);
	Novice::DrawLine((int)TopScreen[1].x, (int)TopScreen[1].y, (int)BottomScreen[1].x, (int)BottomScreen[1].y, color);
	Novice::DrawLine((int)TopScreen[2].x, (int)TopScreen[2].y, (int)BottomScreen[2].x, (int)BottomScreen[2].y, color);
	Novice::DrawLine((int)TopScreen[3].x, (int)TopScreen[3].y, (int)BottomScreen[3].x, (int)BottomScreen[3].y, color);
}





bool IsCollision(const AABB& aabb, const Segment segment) {

	Vector3 t1 = {
		(aabb.min.x - segment.origin.x) / segment.diff.x,
		(aabb.min.y - segment.origin.y) / segment.diff.y,
		(aabb.min.z - segment.origin.z) / segment.diff.z
	};

	Vector3 t2 = {
		(aabb.max.x - segment.origin.x) / segment.diff.x,
		(aabb.max.y - segment.origin.y) / segment.diff.y,
		(aabb.max.z - segment.origin.z) / segment.diff.z
	};

	Vector3 tNear = {
		min(t1.x, t2.x),
		min(t1.y, t2.y),
		min(t1.z, t2.z)
	};

	Vector3 tFar = {
		max(t1.x, t2.x),
		max(t1.y, t2.y),
		max(t1.z, t2.z)
	};


	float tmin = max(max(tNear.x, tNear.y), tNear.z);
	float tmax = min(min(tFar.x, tFar.y), tFar.z);

	if (tmin <= tmax) {
		return true;
	}
	return false;
}


// Windowsアプリでのエントリーポイント(main関数)
int WINAPI WinMain(HINSTANCE, HINSTANCE, LPSTR, int) {

	// ライブラリの初期化
	Novice::Initialize(kWindowTitle, 1280, 720);

	// キー入力結果を受け取る箱
	char keys[256] = {0};
	char preKeys[256] = {0};

	AABB aabb{
		.min{ -0.5f,-0.5f,-0.5f },
		.max{ 0.5f,0.5f,0.5f }
	};

	Segment segment{
		.origin{-0.7f,0.3f,0.0f},
		.diff{2.0f,-0.5f,0.0f}
	};

	MyMath* myMath_ = new MyMath();

	Vector3 cameraPosition = { 0.0f ,0.0f,-20.0f };
	Vector3 cameraTranslate = { 0.0f,-1.0f,-6.49f };
	Vector3 cameraRotate = { -0.26f,0.26f,0.0f };

	uint32_t color1 = WHITE;
	uint32_t color2 = WHITE;

	// ウィンドウの×ボタンが押されるまでループ
	while (Novice::ProcessMessage() == 0) {
		// フレームの開始
		Novice::BeginFrame();

		// キー入力を受け取る
		memcpy(preKeys, keys, 256);
		Novice::GetHitKeyStateAll(keys);

		///
		/// ↓更新処理ここから
		///


		Matrix4x4 worldMatrix = myMath_->MakeAffineMatrix({ 1.0f,1.0f,1.0f }, cameraRotate, cameraTranslate);
		Matrix4x4 cameraMatrix = myMath_->MakeAffineMatrix({ 1.0f,1.0f,1.0f }, { 0.0f,0.0f,0.0f }, cameraPosition);
		Matrix4x4 viewMatrix = myMath_->Inverse(cameraMatrix);
		Matrix4x4 projectionMatrix = myMath_->MakePerspectiveFovMatrix(0.45f, float(1280.0f) / float(720.0f), 0.1f, 100.0f);
		Matrix4x4 WorldViewProjectionMatrix = myMath_->Multiply(worldMatrix, myMath_->Multiply(viewMatrix, projectionMatrix));
		Matrix4x4 viewportMatrix = myMath_->MakeViewportMatrix(0, 0, float(1280.0f), float(720.0f), 0.0f, 1.0f);

		Vector3 start = Transform(Transform(segment.origin, WorldViewProjectionMatrix), viewportMatrix);
		Vector3 end = Transform(Transform(Add(segment.origin, segment.diff), WorldViewProjectionMatrix), viewportMatrix);





		///
		/// ↑更新処理ここまで
		///

		///
		/// ↓描画処理ここから
		///

		DrawGrid(WorldViewProjectionMatrix, viewportMatrix);

		DrawaAABB(aabb, WorldViewProjectionMatrix, viewportMatrix, color1);

		Novice::DrawLine((int)start.x, (int)start.y, (int)end.x, (int)end.y, color2);

		if (IsCollision(aabb,segment)) {
			color1 = RED;
		}
		else {
			color1 = WHITE;
		}



		ImGui::Begin("window");
		ImGui::DragFloat3("CameraTranslate", &cameraTranslate.x, 0.01f);
		ImGui::DragFloat3("CameraRotate", &cameraRotate.x, 0.01f);

		ImGui::DragFloat3("box min", &aabb.min.x, 0.01f);
		ImGui::DragFloat3("box max", &aabb.max.x, 0.01f);

		ImGui::DragFloat3("sen start", &segment.origin.x, 0.01f);
		ImGui::DragFloat3("sen end", &segment.diff.x, 0.01f);

		ImGui::End();

		///
		/// ↑描画処理ここまで
		///

		// フレームの終了
		Novice::EndFrame();

		// ESCキーが押されたらループを抜ける
		if (preKeys[DIK_ESCAPE] == 0 && keys[DIK_ESCAPE] != 0) {
			break;
		}
	}

	// ライブラリの終了
	Novice::Finalize();
	return 0;
}
