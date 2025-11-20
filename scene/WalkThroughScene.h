#pragma once

#include <array>
#include <memory>

#include "../system/camera.h"
#include "../system/IScene.h"

#include "../system/C3DShape.h"

#include "../system/transform.h"
#include "../system/SceneClassFactory.h"
#include "../gameobject/field.h"
#include "../gameobject/player.h"
#include "../gameobject/enemy.h"
#include "../gameobject/obstacle.h"

/**
 * @brief メッシュフィールドを表示する
 */
class WalkThroughScene : public IScene {
public:
	static constexpr uint32_t ENEMYMAX = 100;
	static constexpr uint32_t OBSTACLEMAX = 10;

	/// @brief コピーコンストラクタは使用不可
	WalkThroughScene(const WalkThroughScene&) = delete;

	/// @brief 代入演算子も使用不可
	WalkThroughScene& operator=(const WalkThroughScene&) = delete;

	/**
	 * @brief コンストラクタ
	 *
	 * 
	 */
	explicit WalkThroughScene();

	/**
	 * @brief 毎フレームの更新処理
	 * @param deltatime 前フレームからの経過時間（マイクロ秒）
	 */
	void update(uint64_t deltatime) override;

	/**
	 * @brief 毎フレームの描画処理
	 * @param deltatime 前フレームからの経過時間（マイクロ秒）
	 *
	 */
	void draw(uint64_t deltatime) override;

	/**
	 * @brief シーンの初期化処理
	 *
	 */
	void init() override;

	/**
	 * @brief シーンの終了処理
	 *
	 */
	void dispose() override;

	/**
	 * @brief Directional Light
	*
		* Directional Light
	 */
	void debugDirectionalLight();

	/**
	 * @brief Free Camera
	 *
	 * Free Camera;
	 */
	void debugFreeCamera();

	/**
	 * @brief field remake
	 *
	 * field remake;
	 */
	void debugFieldRemake();

	/**
	 * @brief field undulation
	 *
	 * field remake;
	 */
	void debugFieldUnduration();

	/**
	 * @brief field height
	 *
	 * field height disp
	 */
	void debugFieldHeight();

	/**
	 * @brief get player
	 *
	 * get player object address
	 */
	player* getplayer() {
		return m_player.get();
	}

private:
	 
	 /**
	 * @brief このシーンで使用するカメラ
	 */
	FreeCamera m_camera;

	std::array<std::unique_ptr<Segment>, 3> m_segments;			// ワールド軸表示用線分
	std::array<std::unique_ptr<Segment>, 1> m_playersegment;	// ワールド軸表示用線分

	/**
	* @brief フィールド
	*/
	std::unique_ptr<field> m_field;

	/**
	* @brief プレイヤ
	*/
	std::unique_ptr<player>		m_player;		// プレイヤ

	/**
	* @brief 敵群
	*/
	std::array<std::unique_ptr<enemy>, ENEMYMAX>		m_enemies;	// 敵

	/**
	* @brief 障害物群
	*/
	std::array<std::unique_ptr<obstacle>, OBSTACLEMAX>	m_obstacles;	// 障害物

};

REGISTER_CLASS(WalkThroughScene)