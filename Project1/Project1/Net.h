#pragma once

#include "LibHeaders.h"
#include "Neuron.h"

typedef std::vector<unsigned> TopologyType;

class Net
{
public:
	Net(std::vector<unsigned> topology);
	void feedForward(const std::vector<double>& inputVals);
	void backProp(const std::vector<double>& targetVals);
	void getResults(std::vector<double>& resultVals) const;
	double getRecentAverageError() { return m_error; }
	void showWeights();
protected:
	std::vector<Layer> m_layers;
	double m_error;
};
