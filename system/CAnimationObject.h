#include	"CAnimationObject.h"

void CAnimationObject::Init()
{
	// ボーンコンビネーション行列初期化
	m_BoneCombMatrix.Create();
}

void CAnimationObject::Update(float dt)
{
	float ticksPerSecond = 24.0f; // ※本来はデータから取得推奨

	// 時間経過をTick単位に変換して加算
	m_CurrentFrame += dt * ticksPerSecond;

	// 【削除】勝手にリセットしない（メッシュ側でループ制御するため）
	// float duration = 100.0f;
	// m_CurrentFrame = fmod(m_CurrentFrame, duration);

	int frame = static_cast<int>(m_CurrentFrame);

	// アニメーションメッシュ更新
	m_AnimMesh->Update(m_BoneCombMatrix, frame);
}

void CAnimationObject::BlendUpdate(float dt)
{
	float ticksPerSecond = 24.0f; // ※本来はデータから取得推奨

	// dt は呼び出し元で加工された「経過時間(秒)」なので、ここでフレーム進行量に変換
	m_CurrentFrame += dt * ticksPerSecond;

	// 【削除】勝手にリセットしない。これがあるとアニメーションが途切れたり飛んだりします。
	// float duration = 100.0f;
	// m_CurrentFrame = fmod(m_CurrentFrame, duration);

	int frame = static_cast<int>(m_CurrentFrame);

	// アニメーションメッシュ更新
	m_AnimMesh->UpdateBlended(m_BoneCombMatrix, frame);
}

void CAnimationObject::Draw()
{
	// ボーンコンビネーションを定数バッファへ反映させる
	m_BoneCombMatrix.Update();

	// 定数バッファGPUへセット
	m_BoneCombMatrix.SetGPU();

	// メッシュ描画
	m_AnimMesh->Draw();
}