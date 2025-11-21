#include	"CAnimationObject.h"

void CAnimationObject::Init()
{
	// ボーンコンビネーション行列初期化
	m_BoneCombMatrix.Create();							// 20240723 
}

void CAnimationObject::Update(float dt)
{
	// 修正: 本来はアニメーションデータから取得すべき値
	// Assimpのデフォルトや多くのアニメーションは24や30、60など
	// 実際には m_AnimMesh->GetTicksPerSecond() のような関数を作って取得するのがベストです
	float ticksPerSecond = 24.0f; // 仮の値（データに合わせて取得するように実装してください）
	float duration = 100.0f;      // 仮の値（データに合わせて取得するように実装してください）

	// 時間経過をTick単位に変換して加算
	m_CurrentFrame += dt * ticksPerSecond;

	// ループ処理: 期間を超えたら0に戻す
	m_CurrentFrame = fmod(m_CurrentFrame, duration);

	int frame = static_cast<int>(m_CurrentFrame);

	// アニメーションメッシュ更新
	m_AnimMesh->Update(m_BoneCombMatrix, frame);
}

void CAnimationObject::BlendUpdate(float dt)
{
	// 1. Update関数と同様に、秒数(dt)をアニメーションフレーム単位に変換する
	// 本来はロードしたデータから取得するのがベストですが、Updateに合わせて24.0fとします
	float ticksPerSecond = 24.0f;
	float duration = 100.0f; // 必要に応じてデータの長さを取得してください

	// dt は呼び出し元で加工された「経過時間(秒)」なので、ここでフレーム進行量に変換
	m_CurrentFrame += dt * ticksPerSecond;

	// 2. ループ処理を追加 (これがないと一定時間後にアニメーションが止まる/おかしくなる)
	m_CurrentFrame = fmod(m_CurrentFrame, duration);

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

