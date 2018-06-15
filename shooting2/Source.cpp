#include <string>
#include "XLibrary11.hpp"

using namespace XLibrary11;

enum Mode
{
	Titel,
	Game,
};
//�����_���ȍ��W���擾����
Float3 GetRandomPosition()
{
	return Float3(
		-250.0,
		rand() / (float)RAND_MAX * 300.0 - 150.0,
		0.0
	);
}
//�����蔻��
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
		//��������
		return true;
	}
	//�������ĂȂ�
	return false;
}
int MAIN()
{
	Mode mode = Titel;//�ŏ��̓^�C�g����ʂɂ���

	Camera camera;
	camera.color = Float4(0.0, 0.0, 0.0, 0.1);

	const float playerSpeed = 3.0; //���@�̑���

	Sprite player(L"player.png");//���@�̉摜
	player.scale = 2.0;
	float playerVelocity = playerSpeed;

	const int enemyNum = 1; //�G�̐�
	const float enemySpeed = 2.5;//�G�̑���

	Sprite enemy(L"enemy.png");//�G�̉摜
	enemy.scale = 2.0;

	Float3 enemyPosition[enemyNum];//�G�̍��W

	const int bulletNum = 200;//�e�̐�
	const float bulletSpeed = 10.0;//�e�̑���
	int bulletIndex = 0;//���ɔ��˂���e�̔ԍ�
	//���ɔ��˂���܂ł̃J�E���g
	int bulletInterval = 0;

	Sprite bullet(L"bullet.png");//�e�̉摜
	bullet.scale = 1.0;

	Float3 bulletPosition[bulletNum];//�e�̍��W
	float bulletRadian[bulletNum];//�e�̊p�x

	Text titeleText(L"�V���|�e�B���O", 32.0);
	titeleText.scale = 2.0;
	titeleText.color = Float4(0.0, 1.0, 1.0, 1.0);

	int score = 0;//�X�R�A

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

				//�G�̍��W��������
				for (int i = 0; i < enemyNum; i++)
				{
					enemyPosition[i] = GetRandomPosition();
				}

				//�e�̍��W��������
				for (int i = 0; i < bulletNum; i++)
				{	
					//�͂邩�ޕ��ɔ�΂�
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

			//���@�̈ړ��̏���

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

			
			//���@�̕`��
			player.Draw();

			bulletInterval++;
			if (App::GetKey(VK_SPACE))
			{
				if (bulletInterval > 40.0)
				{
					bulletInterval = 0;
					//�e���v���C���[�̍��W�Ɗp�x�ɍ��킹��
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
				//�G�𓮂�������
				enemyPosition[i] += Float3(
					cosf(enemyRadian),
					sinf(enemyRadian),
					0.0
				) *enemySpeed;
				enemy.position = enemyPosition[i];

				float hitRange = enemy.GetSize().x / 2.0 *
					enemy.scale.x;
				//�e�Ƃ̓����蔻��
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

				//���@�Ƃ̓����蔻��
				if (IsHit(
					enemyPosition[i],
					player.position,
					hitRange
				))
				{
					mode = Titel;
				}
				//�G��`��
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

				//�e�̕`��
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
