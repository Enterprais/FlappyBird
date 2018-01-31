#include "Net.h"



void Net::getResults(std::vector<double>& resultVals) const
{
	resultVals.clear();

	for (unsigned n = 0; n < m_layers.back().size() - 1; ++n) {
		resultVals.push_back(m_layers.back()[n].getOutputVal());
	}
}

void Net::showWeights()
{
	unsigned count_layer = 1;
	for (auto layer : m_layers){
		std::cout << "LayerNum: " << count_layer << std::endl;
		unsigned count_neuron = 1;
		for (auto neuron : layer) {
			std::cout << "\tNeuronNum: " << count_neuron << std::endl;
			neuron.showWeights();
			count_neuron++;
		}
		count_layer++;
	}
}



void Net::backProp(const std::vector<double>& targetVals)
{
	
	// Подсчитать RMS - среднеквадратичную ошибку
	Layer& outputLayer = m_layers.back();
	m_error = 0.0;

	for (unsigned n = 0; n < outputLayer.size() - 1; ++n)
	{
		double delta = targetVals[n] - outputLayer[n].getOutputVal();
		m_error += delta * delta;
	}
	m_error /= outputLayer.size() - 1; // get average error squared
	m_error = sqrt(m_error); // RMS


	// Подсчитать градиенты выходного слоя
	for (unsigned n = 0; n < outputLayer.size() - 1; ++n) {
		outputLayer[n].calcOutputGradients(targetVals[n]);
	}

	// Подсчитать градиенты скрытых слоев
	for (unsigned layerNum = m_layers.size() - 2; layerNum > 0; --layerNum) {
		Layer &hiddenLayer = m_layers[layerNum];
		Layer &nextLayer = m_layers[layerNum + 1];

		for (unsigned n = 0; n < hiddenLayer.size(); n++) {
			hiddenLayer[n].calcHiddenGradients(nextLayer);
		}
	}

	// изменить веса всех слоев
	for (unsigned layerNum = m_layers.size() - 1; layerNum > 0; --layerNum) {
		Layer& layer = m_layers[layerNum];
		Layer& prevLayer = m_layers[layerNum - 1];

		for (unsigned n = 0; n < layer.size(); ++n) {
			layer[n].updateInputWeights(prevLayer);
		}
	}
}

void Net::feedForward(const std::vector<double>& inputVals)
{
	assert(inputVals.size() == m_layers[0].size() - 1);

	//Иницилизируем входными значениями входныем веса
	for (unsigned i = 0; i < inputVals.size(); i++) {
		m_layers[0][i].setOutputVal(inputVals[i]);
	}

	//Forwar propagade
	for (int layerNum = 1; layerNum < m_layers.size(); ++layerNum)
	{
		Layer& prevLayer = m_layers[layerNum - 1];
		for (int n = 0; n < m_layers[layerNum].size(); n++)
		{
			m_layers[layerNum][n].feedForward(prevLayer);
		}
	}
}

Net::Net(const std::vector<unsigned> topology)
{
	unsigned numLayers = topology.size();
	for (unsigned layerNum = 0; layerNum < numLayers; layerNum++)
	{
		//std::cout << "Made Layer" << std::endl;
		m_layers.push_back(Layer());
		unsigned numOutputs = layerNum == topology.size() - 1 ? 0 : topology[layerNum + 1] + 1;
		unsigned add_bias = layerNum == topology.size() - 1 ? 1 : 1;
		// Заполням слои нейронами из топологии
		// и добавляем доп bias нейрон
		for (unsigned neuronNum = 0; neuronNum < topology[layerNum] + add_bias; ++neuronNum)
		{

			m_layers.back().push_back(Neuron(numOutputs, neuronNum));
			//std::cout << "\tMade a neuron" << neuronNum << numOutputs << std::endl;
		}

		m_layers.back().back().setOutputVal(1.0);
	}
}


