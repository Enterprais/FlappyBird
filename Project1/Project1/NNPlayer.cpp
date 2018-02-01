#include "NNPlayer.h"
#include <iostream>



NNPlayer::NNPlayer(
	double alpha = 0.10, // влияние старого значения весов на новое
	double eta = 0.15, // скорость обучения
	unsigned numhiddenNeurons = 6
) : Net({ 1, numhiddenNeurons, 2 })
{
	m_eta = eta;
	m_alpha = alpha;
}

NNPlayer::NNPlayer() : Net({ 2, 1, 1 })
{
	m_eta = 0.15;
	m_alpha = 0.10;
}


NNPlayer::~NNPlayer()
{
}

void NNPlayer::learn()
{
	Neuron::setEta(m_eta);
	Neuron::setAlpha(m_alpha);
	Net::backProp({ 1 });
}

bool NNPlayer::needToJump(ReturnValue input)
{
  	Net::feedForward({ input.Height, input.Distance });
	std::vector<double> resultVals;
	Net::getResults(resultVals);
	std::cout << "Result neuro val: " << resultVals[0] << std::endl;

	return resultVals[0] > 0.5;
}
