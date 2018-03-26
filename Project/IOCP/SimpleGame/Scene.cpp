#include "stdafx.h"
#include "Server.h"
#include "Renderer.h"
#include "Timer.h"
#include "Sound.h"
#include "Scene.h"

using namespace std;

Scene::Scene()
{
}

Scene::~Scene()
{
}

void Scene::buildScene()
{
#if GLRENDERON
	m_Renderer = new Renderer(WINDOW_WIDTH, WINDOW_HEIGHT);
	// Initialize Renderer
	if (!m_Renderer->IsInitialized())
	{
		std::cout << "Renderer could not be initialized.. \n";
	}
	ChessBoard = m_Renderer->CreatePngTexture("Assets/Image/chess board.png");
	ChessPiece = m_Renderer->CreatePngTexture("Assets/Image/chess piece.png");
#endif
	memset(m_Board, INVALID, sizeof(int) * 81);
	InitPieces(m_BlackPiece, TEAM::BLACK);
	InitPieces(m_WhitePiece, TEAM::WHITE);
	
	GameStatus = GAMESTATUS::RUNNING;
}

void Scene::InitPieces(vector<Object*>& pieceset, TEAM Side)
{
	Vec3i pos;
	pieceset.reserve(16);
	for (int i = 0; i < 16; ++i)
	{
		Object* piece = new Object();
		piece->setTeam(Side);
		if (Side == TEAM::WHITE)
			pos = Vec3i((i % 8) + 1, 8 - ((15 - i) / 8), 0);
		else if (Side == TEAM::BLACK)
			pos = Vec3i((i % 8) + 1, ((15 - i) / 8) + 1, 0);
		piece->setPosition(pos);
		piece->setID(i);

		if (i < 8)						piece->setType(OBJTYPE::PAWN);
		else if (i == 8 || i == 15)		piece->setType(OBJTYPE::ROCK);
		else if (i == 9 || i == 14)		piece->setType(OBJTYPE::BISHOP);
		else if (i == 10 || i == 13)	piece->setType(OBJTYPE::KNIGHT);

		m_Board[pos.x][pos.y] = piece->getID();
		pieceset.push_back(piece);
	}
	pieceset[11]->setType(OBJTYPE::KING);
	pieceset[12]->setType(OBJTYPE::QUEEN);
}

int Scene::PieceChk(int x, int y)
{
	if (x < 1 || y < 1 || x > 9 || y > 9) return false;

	if (m_Board[x][y] == INVALID)
		return true;
	else if (m_Board[x][y] != INVALID)
		return false;
}

void Scene::releaseScene()
{
	delete		m_Renderer;
	delete		g_Timer;
}

void Scene::keyinput(unsigned char key)
{
	switch (key)
	{

	default:
		break;
	}

}

void Scene::keyspcialinput(int key)
{
	switch (key)
	{
	default:
		break;
	}
}

// ��Ƽ�½�ŷ�� �����ϰ� �� �ٿ� �۷ι��� ������ �־���Ѵ�.
// �ۿ��� ������ �ȿ��� �� �� ���� �ֱ� ������
void Scene::mouseinput(int button, int state, int x, int y)
{
	if (button == GLUT_LEFT_BUTTON && state == GLUT_UP && GameStatus == GAMESTATUS::RUNNING)
	{

	}
}

void Scene::update()
{
	g_Timer->getTimeset();
	double timeElapsed = g_Timer->getTimeElapsed();

	if (GameStatus == GAMESTATUS::RUNNING) 
	{
		ProcessMsg();
	}
	else if (GameStatus == GAMESTATUS::PAUSE)
	{
		g_Timer->getTimeset();
		double timeElapsed = g_Timer->getTimeElapsed();
	}

}

void Scene::render()
{
	m_Renderer->DrawTexturedRect(0, 0, 0, 720, 1.0f, 1.0f, 1.0f, 1.0f, ChessBoard, 0.9);
	
	for (int i = 0; i < 16; ++i)
	{
		m_Renderer->DrawTexturedRectSeq(
			-360 + m_BlackPiece[i]->getPosition().x * 80,
			360 - m_BlackPiece[i]->getPosition().y * 80,
			0, PIECESIZE, 1, 1, 1, 1, ChessPiece,
			m_BlackPiece[i]->getType(),
			TEAM::BLACK,
			6, 2, 0.5);
	}
	for (int i = 0; i < 16; ++i)
	{
		m_Renderer->DrawTexturedRectSeq(
			-360 + m_WhitePiece[i]->getPosition().x * 80,
			360 - m_WhitePiece[i]->getPosition().y * 80,
			0, PIECESIZE, 1, 1, 1, 1, ChessPiece,
			m_WhitePiece[i]->getType(),
			TEAM::WHITE,
			6, 2, 0.5);
	}
	if (m_Target != nullptr)
		m_Renderer->DrawSolidRect(
			-360 + m_Target->getPosition().x * 80,
			360 - m_Target->getPosition().y * 80,
			0, 80, 0.5f, 0.5f, 0, 0.5f, 0.7);

	if (GameStatus != GAMESTATUS::STOP)
	{
	
	}
	
	else if (GameStatus == GAMESTATUS::STOP)
	{
	
	}
}

void Scene::ProcessMsg()
{
	char buf[MSGSIZE];

	while (m_Server->GetMsgqueue().size() != 0)
	{
		memcpy(buf, m_Server->GetMsgqueue().front(), MSGSIZE);

		MsgQueueLocker.lock();
		m_Server->GetMsgqueue().pop_front();
		MsgQueueLocker.unlock();

		MSGTYPE type = (MSGTYPE)buf[0];
		switch (type)
		{
		case MSGTYPE::HEARTBEAT:
			break;

		case MSGTYPE::MOVE:
		{
			char side = buf[1];
			char id = buf[2];
			int x = buf[3];
			int y = buf[4];
			if (side == TEAM::BLACK)
				m_Target = m_BlackPiece[id];
			else
				m_Target = m_WhitePiece[id];

			Vec3f pos{ x,y,0 };
			if (!PieceChk(x, y))
				continue;

			m_Board[int(m_Target->getPosition().x)][int(m_Target->getPosition().y)] = -1;
			m_Target->setPosition(pos);
			m_Board[x][y] = m_Target->getID();

			buf[0] = MSGTYPE::MOVE;
			buf[1] = m_Target->getTeam();
			buf[2] = m_Target->getID();
			buf[3] = m_Target->getPosition().x;
			buf[4] = m_Target->getPosition().y;

			//m_Server->SendMsg(buf);
			break;
		}
		case MSGTYPE::REMOVECLIENT:
			break;
		}
	}
}