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
		double alpha, // влияние старого значения весов на новое
		double eta, // скорость обучения
		unsigned numhiddenNeurons
	);
	~NNPlayer();
	
	// обучаться
	void learn();
	// получить результат
	bool needToJump(ReturnValue input);
	double getEta() { return m_eta; }
	double getAlpha() { return m_alpha; }
	void setEta(double new_eta) { m_eta = new_eta; }
	void setAlpha(double new_alpha) { m_alpha = new_alpha; }

private:
	// свои параметры нейронной сети
	double m_eta;
	double m_alpha;
};

