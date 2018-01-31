#pragma once
#include "Net.h"

struct ReturnValue
{
	float Height = 0;
	float Distance = 0;
};

class NNPlayer : public Net
{
public:
	NNPlayer::NNPlayer();
	NNPlayer(
		double alpha, // ������� ������� �������� ����� �� �����
		double eta, // �������� ��������
		unsigned numhiddenNeurons
	);
	~NNPlayer();
	
	// ���������
	void learn();
	// �������� ���������
	bool needToJump(ReturnValue input);
	double getEta() { return m_eta; }
	double getAlpha() { return m_alpha; }
	void setEta(double new_eta) { m_eta = new_eta; }
	void setAlpha(double new_alpha) { m_alpha = new_alpha; }

private:
	// ���� ��������� ��������� ����
	double m_eta;
	double m_alpha;
};

