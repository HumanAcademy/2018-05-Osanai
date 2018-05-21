#include <string>
#include "XLibrary11.hpp"

using namespace XLibrary11;

enum Mode
{
	Titel,
	Game,
};
//ランダムな座標を取得する
Float3 GetRandomPosition()
{
	return Float3(
		-250.0,
		rand() / (float)RAND_MAX * 300.0 - 150.0,
		0.0
	);
}
//当たり判定
bool IsHit(
	Float3 position1,
	Float3 position2,
	float range
)
{
	if (position1.x - range < position2.x &&
		position1.x + range > position2.x &&
		position1.y - range < position2.y &&
		position1.y + range > position2.y)
	{
		//当たった
		return true;
	}
	//当たってない
	return false;
}
int MAIN()
{
	Mode mode = Titel;//最初はタイトル画面にする

	Camera camera;
	camera.color = Float4(0.0, 0.0, 0.0, 0.1);

	const float playerSpeed = 3.0; //自機の速さ

	Sprite player(L"player.png");//自機の画像
	player.scale = 2.0;
	float playerVelocity = playerSpeed;

	const int enemyNum = 1; //敵の数
	const float enemySpeed = 2.5;//敵の速さ

	Sprite enemy(L"enemy.png");//敵の画像
	enemy.scale = 2.0;

	Float3 enemyPosition[enemyNum];//敵の座標

	const int bulletNum = 200;//弾の数
	const float bulletSpeed = 10.0;//弾の速さ
	int bulletIndex = 0;//次に発射する弾の番号
	//次に発射するまでのカウント
	int bulletInterval = 0;

	Sprite bullet(L"bullet.png");//弾の画像
	bullet.scale = 1.0;

	Float3 bulletPosition[bulletNum];//弾の座標
	float bulletRadian[bulletNum];//弾の角度

	Text titeleText(L"シュ－ティング", 32.0);
	titeleText.scale = 2.0;
	titeleText.color = Float4(0.0, 1.0, 1.0, 1.0);

	int score = 0;//スコア

	Text scoreText(L"0", 10.0);
	scoreText.scale = 10.0;
	scoreText.color = Float4(0.0, 1.0, 1.0, 1.0);

	while (App::Refresh())
	{
		camera.Update();

		switch (mode)
		{
		case Titel:

			if (App::GetKeyDown(VK_RETURN))
			{
				player.position = 0.0;
				score = 0;
				scoreText.Create(L"0", 10.0);

				//敵の座標を初期化
				for (int i = 0; i < enemyNum; i++)
				{
					enemyPosition[i] = GetRandomPosition();
				}

				//弾の座標を初期化
				for (int i = 0; i < bulletNum; i++)
				{	
					//はるか彼方に飛ばす
					bulletPosition[i].x = 99999.0;
					bulletRadian[i] = 0.0;
				}
				mode = Game;
			}

			camera.position = 0.0;

			titeleText.Draw();

			scoreText.position = Float3(
				0.0, 200.0, 0.0
			);

				break;

		case Game:

			//自機の移動の処理

			player.position.y += playerVelocity;
			if (player.position.y < -150.0)
			{
				playerVelocity = playerSpeed;
			}
			if (player.position.y > 150.0)
			{
				playerVelocity = -playerSpeed;
			}
			player.position.x = 250.0;

			
			//自機の描画
			player.Draw();

			bulletInterval++;
			if (App::GetKey(VK_SPACE))
			{
				if (bulletInterval > 40.0)
				{
					bulletInterval = 0;
					//弾をプレイヤーの座標と角度に合わせる
					bulletPosition[bulletIndex] =
						player.position;
					bulletRadian[bulletIndex] =
						XM_PI;

					bulletIndex++;
					if (bulletIndex >= bulletNum)
						bulletIndex = 0;
				}
			}
			for (int i = 0;i < enemyNum; i++)
			{
				float enemyRadian = atan2f(
					player.position.y - enemyPosition[i].y,
					player.position.x - enemyPosition[i].x
				);
				//敵を動かす処理
				enemyPosition[i] += Float3(
					cosf(enemyRadian),
					sinf(enemyRadian),
					0.0
				) *enemySpeed;
				enemy.position = enemyPosition[i];

				float hitRange = enemy.GetSize().x / 2.0 *
					enemy.scale.x;
				//弾との当たり判定
				for (int j = 0; j < bulletNum; j++)
				{
					if (IsHit(
						enemyPosition[i],
						bulletPosition[j],
						hitRange
					))
					{
						bulletPosition[j].x = 99999.0;
						enemyPosition[i] =
							camera.position +
							GetRandomPosition();
						score++;
						scoreText.Create(
							std::to_wstring(score),
							10.0
						);
					}
				}

				//自機との当たり判定
				if (IsHit(
					enemyPosition[i],
					player.position,
					hitRange
				))
				{
					mode = Titel;
				}
				//敵を描画
				enemy.Draw();
			}

			for (int i = 0; i < bulletNum; i++)
			{
				bulletPosition[i] += Float3(
					cosf(bulletRadian[i]),
					sinf(bulletRadian[i]),
					0.0
				) * bulletSpeed;
				bullet.position = bulletPosition[i];

				//弾の描画
				bullet.Draw();
			
			}
			scoreText.position = camera.position +
				Float3(0.0, 200.0, 0.0);
			scoreText.Draw();
			break;
		}
	}
	return 0;
}
