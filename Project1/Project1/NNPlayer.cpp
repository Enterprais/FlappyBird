#include "NNPlayer.h"



NNPlayer::NNPlayer(
	double alpha = 0.10, // ������� ������� �������� ����� �� �����
	double eta = 0.15, // �������� ��������
	unsigned numhiddenNeurons = 6
) : Net({ 1, numhiddenNeurons, 2 })
{
	m_eta = eta;
	m_alpha = alpha;
}

NNPlayer::NNPlayer() : Net({ 2, 6, 1 })
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
	return resultVals.back() > 0.5;
}
