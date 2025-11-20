#pragma once

#include	<memory>
#include	"gameobject.h"
#include	"../system/CAnimationMeshBlender.h"
#include	"../system/CAnimationObject.h"
#include	"../system/CShader.h"
// 追加: アニメーションデータを管理するクラス
#include	"../system/CAnimationData.h"

class player : public gameobject {

public:
	void update(uint64_t delta) override;
	void draw(uint64_t delta) override;
	void init() override;
	void dispose() override;

	// 動きのパラメータ
	const float VALUE_MOVE_MODEL = 1.0f;
	const float VALUE_ROTATE_MODEL = PI * 0.02f;
	const float RATE_ROTATE_MODEL = 0.4f;
	const float RATE_MOVE_MODEL = 0.20f;
	float speedRate = 0.0f;
private:
	const float TIME_TO_RUN_MS = 1000.0f;
	const float SPEED_RATE_IDLE = 1.0f;
	const float SPEED_RATE_RUN = 2.0f;

	float m_moveTimeCount = 0.0f;
	// アニメーションデータの管理（メッシュより先に宣言しておくと破棄順序が安全です）
	CAnimationData m_animData;

	std::unique_ptr<CAnimationMeshBlender>	m_mesh;
	std::unique_ptr<CAnimationObject>		m_model;
	std::unique_ptr<CShader>				m_shader;

	Vector3	m_move = { 0.0f,0.0f,0.0f };
	Vector3	m_destrot = { 0.0f,0.0f,0.0f };
	aiAnimation* m_pAnimIdle = nullptr;
	aiAnimation* m_pAnimRun = nullptr;
	float m_AnimBlendRate = 0.0f;
	uint64_t m_oldTime = 0;
};