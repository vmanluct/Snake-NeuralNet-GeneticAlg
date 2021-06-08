#include "NeuralNet.h"

NeuralNet::~NeuralNet()
{
	/*delete whi;
	delete whh;
	delete who;*/
}

void NeuralNet::mutate(float mr)
{
	//Adjust all weights between layers depending on mr
	whi.mutate(mr);
	whh.mutate(mr);
	who.mutate(mr);
}

 MatrixXf NeuralNet::output(float *inputsArr)
{
	 //Get a column vector with values of input nodes
	MatrixHandler inputs = who.singleColumnMatrixFromArray(inputsArr);
	
	//Add a bias neuron 
	MatrixHandler inputsBias = inputs.addBias();

	//Multiply input values with weights to the hidden layer
	MatrixHandler hiddenInputs = whi.dot(inputsBias);

	//Use activation function to get value between 0 and 1
	MatrixHandler hiddenOutputs = hiddenInputs.activate();

	//Add bias neuron to hidden layer
	MatrixHandler hiddenOutputsBias = hiddenOutputs.addBias();

	//Multiply values of first hidden layer with weights to the second hidden layer
	MatrixHandler hiddenInputs2 = whh.dot(hiddenOutputsBias);
	MatrixHandler hiddenOutputs2 = hiddenInputs2.activate();
	MatrixHandler hiddenOutputsBias2 = hiddenOutputs2.addBias();

	//Multiply values in the second hidden layer with weights to the output layer
	MatrixHandler outputInputs = who.dot(hiddenOutputsBias2);

	//Use activation function to get values
	MatrixHandler outputs = outputInputs.activate();

	//Return column of output values
	return outputs.matrix;
}

NeuralNet NeuralNet::crossover(NeuralNet partner)
{
	//Create a new neural network where weights are picked from both parents
	NeuralNet child = NeuralNet(iNodes, hNodes, oNodes);
	child.whi = whi.crossover(partner.whi);
	child.whh = whh.crossover(partner.whh);
	child.who = who.crossover(partner.who);
	return child;
}

NeuralNet NeuralNet::clone()
{
	//Create a deep copy of the neural network
	NeuralNet clone = NeuralNet(iNodes, hNodes, oNodes);
	clone.whi = whi.clone();
	clone.whh = whh.clone();
	clone.who = who.clone();

	return clone;
}

//Write neural network to file
bool NeuralNet::writeRecordToFile(std::string file_name)
{
	//Get sizes of the different layers
	float* inpArr = whi.toArray();
	int inpSize = whi.rows * whi.cols;
	
	float* hidArr = whh.toArray();
	int hidSize = whh.rows * whh.cols;

	float* outArr = who.toArray();
	int outSize = who.rows * who.cols;

	std::ofstream file;
	file.open(file_name);
	for (int count = 0; count < inpSize; count++) {
		file << inpArr[count] << " ";
	}
	file << '\n';
	for (int count = 0; count < hidSize; count++) {
		file << hidArr[count] << " ";
	}
	file << '\n';
	for (int count = 0; count < outSize; count++) {
		file << hidArr[count] << " ";
	}
	file.close();
	return true;
}

//Read neural network from file
bool NeuralNet::readFiletoNetwork(std::string file_name)
{
	//Get sizes of the different layers
	float* inpArr = whi.toArray();
	int inpSize = whi.rows * whi.cols;

	float* hidArr = whh.toArray();
	int hidSize = whh.rows * whh.cols;

	float* outArr = who.toArray();
	int outSize = who.rows * who.cols;

	std::ifstream file(file_name);
	if (file.is_open()) {
		while (!file.eof()) {
			for (int i = 0; i < inpSize; i++) {
				file >> inpArr[i];
			}
			whi.fromArray(inpArr);
			for (int i = 0; i < hidSize; i++) {
				file >> hidArr[i];
			}
			whh.fromArray(hidArr);
			for (int i = 0; i < outSize; i++) {
				file >> outArr[i];
			}
			who.fromArray(outArr);
		}
		return true;
	}
	else {
		std::cout << "Did not open file";
		return false;
	}
}

void NeuralNet::drawInputNodes()
{
	//Set shape of all input nodes
	for (int i = 0; i < iNodes; i++) {
		inNodes[i].setRadius(8);
		inNodes[i].setFillColor(Color::White);
		inNodes[i].setPosition(10, i * 40 + 100);
	}
}

void NeuralNet::drawInToHidWeights(RenderTarget& target)
{
	//Connect line between input nodes and hidden layer
	int count = 0;
	for (int i = 0; i < iNodes; i++) {
		for (int j = 0; j < hNodes; j++) {
			inHidWeights[count][0].position = sf::Vector2f(inNodes[i].getPosition().x + 8, inNodes[i].getPosition().y + 8);
			inHidWeights[count][1].position = sf::Vector2f(hidNodes[j].getPosition().x + 8, hidNodes[j].getPosition().y + 8);
			if (whi.matrix(j, i) < 0) {
				inHidWeights[count][0].color = Color::Red;
				inHidWeights[count][1].color = Color::Red;
			}
			else {
				inHidWeights[count][0].color = lightBlue;
				inHidWeights[count][1].color = lightBlue;
			}
			target.draw(inHidWeights[count], 2, sf::Lines);
			count++;
		}
	}
}

void NeuralNet::drawHiddenNodes()
{
	//Set shape of all hidden nodes
	for (int i = 0; i < hNodes; i++) {
		hidNodes[i].setRadius(8);
		hidNodes[i].setFillColor(Color::White);
		hidNodes[i].setPosition(410, i * 40 + 100);
	}
}


void NeuralNet::drawHidToOutWeights(RenderTarget& target)
{
	//Connect lines between hidden nodes and output nodes
	int count = 0;
	for (int i = 0; i < hNodes; i++) {
		for (int j = 0; j < oNodes; j++) {
			hidOutWeights[count][0].position = sf::Vector2f(hidNodes[i].getPosition().x + 8, hidNodes[i].getPosition().y + 8);
			hidOutWeights[count][1].position = sf::Vector2f(outNodes[j].getPosition().x + 8, outNodes[j].getPosition().y + 8);
			if (who.matrix(j, i) < 0) {
				hidOutWeights[count][0].color = Color::Red;
				hidOutWeights[count][1].color = Color::Red;
			}
			else {
				hidOutWeights[count][0].color = lightBlue;
				hidOutWeights[count][1].color = lightBlue;
			}
			target.draw(hidOutWeights[count], 2, sf::Lines);
			count++;
		}
	}
}


void NeuralNet::drawOutputNodes()
{
	//Set shape for all output nodes
	for (int i = 0; i < oNodes; i++) {
		outNodes[i].setRadius(8);
		outNodes[i].setFillColor(Color::White);
		outNodes[i].setPosition(810, i * 80 + 200);
	}
}



void NeuralNet::renderInNodes(RenderTarget& target)
{
	for (int i = 0; i < iNodes; i++) {
		target.draw(inNodes[i]);
	}
}


void NeuralNet::renderHidNodes(RenderTarget& target)
{
	for (int i = 0; i < hNodes + 1; i++) {
		target.draw(hidNodes[i]);
	}

}

void NeuralNet::renderOutNodes(RenderTarget& target)
{
	for (int i = 0; i < oNodes; i++) {
		target.draw(outNodes[i]);
	}
}

void NeuralNet::renderNet(RenderTarget& target)
{
	this->drawInputNodes();
	this->drawHiddenNodes();
	this->drawOutputNodes();
	this->renderInNodes(target);
	this->renderHidNodes(target);
	this->renderOutNodes(target);
	this->drawInToHidWeights(target);
	this->drawHidToOutWeights(target);
}
