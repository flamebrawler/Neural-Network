/*
 * NeuralNetwork.cpp
 *
 *  Created on: Mar 18, 2018
 *      Author: Joel
 */

#include "NeuralNetwork.h"
#include <iostream>
#include <math.h>
#include <time.h>
#include "Parser.h"

using namespace std;

//default constructor
NeuralNetwork::NeuralNetwork() {
	this->inputs = 1;
	this->hiddenNodes = 0;
	this->hiddenlayers = 0;
	this->outputs = 1;
	function = 0;
	endfunct = 0;
}
/*
 * converts from a string to a method usable by the network
 * word is the string to ocnvert
 * size is the length of the longest word usable
 * returns 26*size values to input into a network
 */
double* wordtonum(string word, unsigned int size) {
	double* f = new double[size * 26];

	for (unsigned int i = 0; i < size; i++) {
		if (word[i] <= 'Z' && word[0] >= 'A') //make lowercase
			word[i] -= 'A' - 'a';
		if (word[i] <= 'z' && word[i] >= 'a') //convert to number in alphabet
			word[i] -= 'a' - 1;
		else
			word[i] = 0;

		for (int l = 0; l < 26; l++) { //make node for each letter in alphabet
			if (l + 1 == word[i])
				f[i * 26 + l] = 1;
			else
				f[i * 26 + l] = 0;

		}
		if (word.size() == i + 1)
			word += '0';
	}
	return f;
}
supervised::supervised() {
	error = 0;
}
/*
 * supervised constructor
 * inputs is the inputs of the new network
 * hidden nodes is the nodes in each of the hidden layers
 * outputs is the nodes in the last layer
 * hidden layers is the number of layers in total without the first and last
 * function is the function that all of the nodes will have except for the last layer
 * endfunct is the function that the last layer will have
 * 1 is sigmoid
 * 2 is relu
 * 0 is nothing
 */
supervised::supervised(int inputs, int hiddenNodes, int outputs,
		int hiddenlayers, int function, int endfunct) {
	this->inputs = inputs;
	this->hiddenNodes = hiddenNodes;
	this->hiddenlayers = hiddenlayers;
	this->outputs = outputs;
	error = 0;
	if (function == 1) //sets the functions of the network
		this->function = sigmoid;
	else if (function == 2)
		this->function = ReLUs;
	else
		this->function = none;
	if (endfunct == 1)
		this->endfunct = sigmoid;
	else if (endfunct == 2)
		this->endfunct = ReLUs;
	else
		this->endfunct = none;
	fill();
}

NeuralNetwork::~NeuralNetwork() {
	layers.clear();
}
/*
 * runs the network once
 * inputs is the data to input into the network
 */
void NeuralNetwork::run(double input) {
	if (inputs == 1) {
		int finlayer = hiddenlayers + 1;

		//set inputs
		layers[0].nodes[0].value = input;
		//evalutate values
		for (int l = 1; l <= finlayer; l++) { //layer
			for (unsigned int m = 0; m < layers[l].getSize(); m++) { //nodes in the layer
				layers[l].nodes[m].add(); //add nodes from previous layer
			}
		}
	} else{
		std::cerr << "not enough inputs to neural network";
		exit(1);
	}
}
/*
 * runs the network once
 * inputs is the data to input into the network
 */
void NeuralNetwork::run(const double inputs[]) {

	int finlayer = hiddenlayers + 1;

	//set inputs
	for (int i = 0; i < this->inputs; i++) {
		layers[0].nodes[i].value = inputs[i];

	}
	//evalutate values
	for (int l = 1; l <= finlayer; l++) { //layer
		for (unsigned int m = 0; m < layers[l].getSize(); m++) { //nodes in the layer
			layers[l].nodes[m].add(); //add nodes from previous layer
		}
	}
}

/*
 * fills the neural network with layers, nodes and connections with a random weight based on original network's variables
 */
void NeuralNetwork::fill() {

	if (!layers.empty()) {
		//cout << "rewriting network" << endl;
		layers.clear();
	}
	for (int l = 0; l < hiddenlayers + 2; l++) { //make layers and nodes

		int nodeNum;
		if (!l)
			nodeNum = inputs;
		else if (l == hiddenlayers + 1)
			nodeNum = outputs;
		else
			nodeNum = hiddenNodes;

		layers.push_back(Layer(nodeNum));
		for (int n = 0; n < nodeNum; n++) {
			if (l > hiddenlayers) //last layer
				layers[l].nodes.push_back(Node(endfunct));
			else
				//every other layer
				layers[l].nodes.push_back(Node(function));
		}
	}
	for (unsigned int l = 1; l < layers.size(); l++) { //link the layers
		layers[l].prev = &layers[l - 1];
	}
	//create connections
	srand(time(NULL));
	for (int l = 1; l < hiddenlayers + 2; l++) { //for each layer
		for (unsigned int a = 0; a < layers[l].getSize(); a++) { //for each node
			for (unsigned int n = 0; n < layers[l - 1].getSize(); n++) { //for each previos node

				Node* prev = &layers[l - 1].nodes[n];
				Node* next = NULL;

				if (l != hiddenlayers + 1)
					next = &layers[l + 1].nodes[n];
				double weight;
				weight = ((double) (rand() % 1000) / 500.0 - 1);
				Connection c = Connection(weight, prev, next);
				layers[l].nodes[a].inputs.push_back(c);
			}
		}
	}

}
/*
 * returns the values of the final nodes
 */
double* NeuralNetwork::getResult() const {
	double* results = new double[outputs];
	for (int i = 0; i < outputs; i++) {
		results[i] = layers.back().nodes[i].getVal();
	}
	return results;
}

/*
 * overloadings of learn so that anything can be inputted
 * data is the data inputted to the network
 * intended is the output data
 */
void supervised::learn(const double data[],const double intended[]) {
	learn(&layers.back(), data, intended);
}
/*
 * overloadings of learn so that anything can be inputted
 * data is the data inputted to the network
 * intended is the output data
 */
void supervised::learn(double data, double intended) {
	if (inputs == 1 && outputs == 1) {
		double in[] = { data };
		double out[] = { intended };
		learn(&layers.back(), in, out);
	} else{
		std::cerr << "data does not fit";
		exit(1);
	}

}
/*
 * overloadings of learn so that anything can be inputted
 * data is the data inputted to the network
 * intended is the output data
 */
void supervised::learn(double data, const double intended[]) {
	if (inputs == 1) {
		double in[] = { data };
		learn(&layers.back(), in, intended);
	} else{
		std::cerr << "data does not fit";
		exit(1);
	}
}
/*
 * overloadings of learn so that anything can be inputted
 * data is the data inputted to the network
 * intended is the output data
 */
void supervised::learn(const double data[], double intended) {
	if (outputs == 1) {
		double out[] = { intended };
		learn(&layers.back(), data, out);
	} else{
		std::cerr << "data does not fit";
		exit(1);
	}
}
/*
 * uses data inputted and calculates slope for every bias and weight
 * input final layer of neural network adn network with uncalculated slopes
 * l is tyhe layer to look at
 * data is the input vector
 * intended is the output vector
 */
void supervised::learn(Layer* l,const double data[], const double intended[]) {
	supervised original; //copy the network
	original.copyInfo(*this);
	original.copyVals(*this);

	double dw = 0.1;
	double db = 0.1;
	original.run(data);
	original.getError(intended);

	unsigned int layer = 0;
	while (&layers[layer] != l)
		layer++;
	const int maxn = l->getSize();
	const int maxc = l->getPrevSize();
	supervised dupl;
	dupl.copyInfo(original);

	for (int n = 0; n < maxn; n++) {
		//test bias
		double biasSlope = 0;

		dupl.copyVals(original);
		dupl.layers[layer].nodes[n].bias += db;
		dupl.run(data); //find how different it is
		dupl.getError(intended);
		double de;
		if (layer == layers.size() - 1) { //if last layer dont get average since each weight only affects 1 error
			de = dupl.error[n] - original.error[n];
			biasSlope = de / db;

		} else { //if not last layer get average

			for (int i = 0; i < outputs; i++) {
				de = dupl.error[i] - original.error[i];
				biasSlope += de / db;

			}
			biasSlope /= outputs;
		}

		l->nodes[n].bias -= 2 * biasSlope;

		for (int c = 0; c < maxc; c++) {
			//test weight
			dupl.copyVals(original);
			dupl.layers[layer].nodes[n].inputs[c].weight += dw;
			dupl.run(data); //find how different it is
			dupl.getError(intended);

			double slope = 0;
			double de;

			if (layer == layers.size() - 1) { //if last layer dont get average since each weight only affects 1 error
				de = dupl.error[n] - original.error[n];
				slope = de / dw;
			} else { //if not last layer get average
				for (int i = 0; i < outputs; i++) {
					de = dupl.error[i] - original.error[i];
					slope += de / dw;
				}
				slope /= outputs;
			}
			l->nodes[n].inputs[c].weight -= 2 * slope; //decrease weight if slope is height and vice versa
		}
	}

	if (l->getPrevSize()) {
		learn(l->prev, data, intended);
	}
}

/*
 * l - the layer being tested
 * node - the node being tested or the connections behind it
 * bias - whether a bias is being tested or a weight
 * intended - the correct answer component
 */
double* supervised::backpropogate(const Layer& layer, const Node& tested,
		const double* intended) const {

	double product = 1; //dv/db = 1
	double productW = tested.getfunct()(tested.sum(1)); //dv/dw = f

	const Node* output = &layers.back().getNode(0);
	const Layer* l = &layer;

	if (l->next) { //if the layer is not the last so changes wont directly affect result
		product *= dsigmoid(l->getNode(0).sum(1)); //df/dv
		for (l = l->next; l != &layers.back(); l = l->next) {
			product *= l->getNode(0).getInput(0).getWeight(); //dv/df
			product *= dsigmoid(l->getNode(0).sum(1)); //df/dv
		}

	} else { //if the change is only affecting one output
		int num = 0;
		while (output != &tested) { //get output equivelent to current neuron
			num++;
			output = &layers.back().getNode(num);
		};
		product *= dsigmoid(output->sum(1)); //df/dv
		double de = product
				* (output->getfunct()(output->sum(1)) - intended[num])
				/ outputs; //average of de/df*df/dw
		double* result = new double[2]; //return two variables
		result[0] = productW * de;
		result[1] = 0;
		//cout<<result[0]<<endl;
		return result;
	}
	double* average = new double[2];

	for (int num = 0; num < outputs;
			num++, output = &layers.back().getNode(num)) { //dc/df
		average[0] += product * productW
				* (output->getfunct()(output->sum(1)) - intended[num]);
		average[1] += product
				* (output->getfunct()(output->sum(1)) - intended[num]);
	}
	average[0] /= outputs;
	average[1] /= outputs;
	return average;
}

/*
 * learn lets the supervised networklearn from many input and output values at one time and randomizes the order of it
 * p is the array of matches that will be used as inputs and answer values
 * size is the number of test data used
 */
void supervised::learn(match* &p, int size) {
	//shuffle
	srand(time(NULL));
	for (int i = size - 1; i > 0; i--) {
		int j = rand() % (i + 1);
		match temp = p[i];
		p[i] = p[j];
		p[j] = temp;
	}

	if (p[0].in == inputs && p[0].out == outputs) { //check if the data fits the network
		for (int i = 0; i < size; i++) {
			learn(p[i].question, p[i].answer); //learn from training example
			cout << "done " << (i + 1) << " of " << size << endl;
		}
	} else
		cerr << "input/output does not fit";
}
/*
 * method of learning using backpropogation
 * l is the layer being looked at
 * data is the input verctor
 * intended is the output vector
 */
void supervised::learn2(Layer* l, const double data[], const double intended[]) {
	supervised original; //make a copy of the network
	original.copyInfo(*this);
	original.copyVals(*this);
	original.run(data);

	unsigned int layer = 1;
	while (&layers[layer] != l)
		layer++;

	for (unsigned int n = 0; n < l->getSize(); n++) {
		//test bias
		double* slope = original.backpropogate(original.getLayer(layer),
				original.getLayer(layer).nodes[n], intended);

		l->nodes[n].bias -= 2 * slope[1];
		//test weights

		for (unsigned int c = 0; c < l->getPrevSize(); c++)
			l->nodes[n].inputs[c].weight -= 2 * slope[0]; //decrease weight if slope is height and vice versa
		delete[] slope;
	}

	if (l->getPrevSize()) {
		learn(l->prev, data, intended);
	}
}
/*
 * gets the defference between each othe outputs and the correct results
 * intended is the outputs that they should be
 * returns the average of the error
 */
double supervised::getError(const double intended[]) {
	double sum;
	error = new double[outputs];
	for (int i = 0; i < outputs; i++) {
		double cost = pow(layers.back().getNode(i).value - intended[i], 2);
		sum += cost;
		error[i] = cost;
	}
	return sum;
}
/*
 * copys the info and the structure from another network
 * net is the network to copy from
 */
void NeuralNetwork::copyInfo(const NeuralNetwork& net) {
	inputs = net.inputs;
	hiddenNodes = net.hiddenNodes;
	hiddenlayers = net.hiddenlayers;
	outputs = net.outputs;
	fill();
}
/*
 * copy the values from another compatible netowrks this includes funcitons weights and biases.
 * structure must be the same
 * net is the network ot copy from
 */
void NeuralNetwork::copyVals(const NeuralNetwork& net) {
	if (inputs == net.inputs && hiddenNodes == net.hiddenNodes
			&& hiddenlayers == net.hiddenlayers && outputs == net.outputs) { //check compatibility
		for (unsigned int l = 0; l < net.layers.size(); l++) {
			for (unsigned int n = 0; n < net.getLayer(l).getSize(); n++) { //copy values
				layers[l].nodes[n].bias = net.getLayer(l).getNode(n).getbias();
				layers[l].nodes[n].funct =
						net.getLayer(l).getNode(n).getfunct();
				for (unsigned int c = 0; c < net.getLayer(l).getPrevSize();
						c++) {
					layers[l].nodes[n].inputs[c].weight =
							net.getLayer(l).getNode(n).getInput(c).getWeight();
				}
			}
		}
	} else{
		std::cerr << "incompatible networks";
		exit(1);
	}
}
/*
 * inputs the name of a file and outputs a useable peice of data for networks from the types .txt, .bmp
 * will return 0 if file is not of the specied types
 * name is the name of the file to load
 * inputs is the inputs of the network to use it for and the size of the array
 * returns the double of the inputted values that will be useable
 */
double* convertfile(string name, int inputs) {
	double* result = new double[inputs];
	if (contains(name, ".txt")) {
		char input[inputs / 26];
		ifstream sub(name.c_str(), ios::in);
		if(sub.fail()){
			cerr<<"file not loaded";
			exit(1);
		}
		sub.get(input, inputs / 26);
		result = wordtonum(input, inputs / 26); //convert to useable data
		sub.close();
	} else if (contains(name, ".bmp")) {
		pixel** pixels = read(name); //load

		result = new double[inputs];
		int bytes = pixels[0]->bytes; //get rid of alpha value
		if (bytes > 3)
			bytes = 3;
		for (int i = 0;i < getHeight(name) * getWidth(name) * bytes&& i < inputs / bytes; i++) {
			result[i] = ((double) pixels[(i - (i % bytes)) / bytes]->get(
					i % bytes)) / 255.0; //convert from pixel to char to double
		}

	}else{
		delete[] result;
		return 0;
	}
	return result;
}
/*
 * load reads a .og file to read other files to input to the network
 * file is the name of the .og file to read
 * returns array of matches with inputs and outputs
 */
match* load(std::string file, int&counter) {
	counter = 0;
	match* list; //the array to output
	vector<match> v; //the array to add matches to
	ifstream f(file.c_str(), ios::in);
	string temp; //temporary string to read to from the file

	int in = 0; //data for the matches
	int out = 0;
	double* ques;
	double* ans;

	bool line = 1; //if data is inputted by line of the file or just by each file

	getline(f, temp);

	if (contains(temp, "/line")) //pick how to get the data
		line = 1;
	else if (contains(temp, "/file"))
		line = 0;

	getline(f, temp);

	string* s = parse(temp, " ");
	if (getD(s[0]) == 2) {
		in = (int) getD(s[1]);
		out = (int) getD(s[2]);
	}

	delete[] s;
	while (getline(f, temp)) {

		s = parse(temp, " ");
		if (getD(s[0]) == out + 1) { //if there is the right amount of inputs
			if (line) { //can only be text if it is looking at each line
				ifstream sub(s[1].c_str(), ios::in);
				char temp2[in / 26];

				if (!sub.fail()) {
					while (sub.getline(temp2, in / 26)) { //read each line
						ans = new double[out];
						for (int i = 0; i < out; i++)
							ans[i] = getD(s[i + 1]);
						ques = wordtonum(temp2, in / 26);

						match p; //pair the data
						p.answer = ans;
						p.question = ques;
						p.in = in;
						p.out = out;
						counter++;
						v.push_back(p);
					}
				} else {
					cerr << "could not open file: " << s[1];
					exit(1);
				}
				sub.close();
			} else { //looks at data as if the whole file is the input
				match p;

				ans = new double[out];
				for (int i = 0; i < out; i++)
					ans[i] = getD(s[i + 1]);


				ques = convertfile(s[1],in);
				if(!ques){//if the type is not supported
					cerr<<"file could not be recognized"<<endl;
					exit(1);
				}
				//set the data of the match
				p.answer = ans;
				p.question = ques;
				p.in = in;
				p.out = out;
				counter++;
				v.push_back(p);
			}

		} else {
			cerr
					<< "improper use of .og file subfiles: please use a file then the number of outputs";
			exit(1);
		}
		delete[] s;

	}
	//convert from vector to array
	list = new match[v.size()];
	for (unsigned int i = 0; i < v.size(); i++) {
		list[i] = v[i];
	}
	v.clear();
	f.close();
	return list;

}

/*
 * << operator allows for printing of a network into a stream such as cout
 */
std::ostream& operator<<(std::ostream& file, const NeuralNetwork& net) {
	file << "info:" << net.inputs << " " << net.outputs << " "
			<< net.hiddenNodes << " " << net.hiddenlayers << std::endl;
//show dimensions

	file << "size " << net.getLayer(0).getSize() << " "
			<< net.layers[0].getPrevSize() << std::endl;
	//for first layer without connections
	for (unsigned int i = 0; i < net.getLayer(0).getSize(); i++) { //make numeric values for each function
		int f = 0;
		if (net.layers[0].getNode(i).getfunct() == ReLUs)
			f = 1;
		else if (net.getLayer(0).getNode(i).getfunct() == sigmoid)
			f = 2;
		file << "n " << net.getLayer(0).getNode(i).getbias() << " " << f
				<< std::endl;
	}
	//for every ther layer
	for (unsigned int i = 1; i < net.layers.size(); i++) {
		file << "size " << net.getLayer(i).getSize() << " "
				<< net.getLayer(i).getPrevSize() << std::endl;

		for (unsigned int n = 0; n < net.getLayer(i).getSize(); n++) { //print nodes
			int f = 0;
			if (net.getLayer(i).getNode(n).getfunct() == ReLUs)
				f = 1;
			else if (net.getLayer(i).getNode(n).getfunct() == sigmoid)
				f = 2;
			file << "n " << net.getLayer(i).getNode(n).getbias() << " " << f
					<< std::endl;

			for (unsigned int c = 0; c < net.getLayer(i).getPrevSize(); c++)
				file << "c "
						<< net.getLayer(i).getNode(n).getInput(c).getWeight()
						<< std::endl; //print connections

		}
	}
	return file;
}
/*
 * < operator allows printing of a network to a file( a binary nnw file)
 */
std::ofstream& operator<<(std::ofstream& f, const NeuralNetwork& net) {
	if (!f.fail()) {
		f.write((char*) &net.inputs, 4); //print network info
		f.write((char*) &net.outputs, 4);
		f.write((char*) &net.hiddenNodes, 4);
		f.write((char*) &net.hiddenlayers, 4);

		for (int l = 0; l < net.hiddenlayers + 2; l++) { //for each layer

			for (unsigned int n = 0; n < net.getLayer(l).getSize(); n++) { //for each node
				char funct = 0; //print the function in numeric value
				if (net.getLayer(l).getNode(n).getfunct() == ReLUs)
					funct = 1; //reLU function
				else if (net.getLayer(l).getNode(n).getfunct() == sigmoid)
					funct = 2; //sigmoid function
				f.write((char*) &funct, 1); //funciton used
				double b = net.getLayer(l).getNode(n).getbias();
				f.write((char*) &b, sizeof(double)); //bias of node

				for (unsigned int c = 0; c < net.getLayer(l).getPrevSize();
						c++) {
					double w =
							net.getLayer(l).getNode(n).getInput(c).getWeight();
					f.write((char*) &w, sizeof(double)); //print conections weight
				}
			}
		}
	} else
		cerr << "file not opened";
	return f;
}

/*
 * readNNW reads a nnw file so that the network becomes what it reads
 * filename is the name of the file to read from
 */
void NeuralNetwork::readNNW(ifstream& f) {
//NeuralNetwork net;
	f.read((char*) &inputs, sizeof(int));
	f.read((char*) &outputs, sizeof(int));
	f.read((char*) &hiddenNodes, sizeof(int));
	f.read((char*) &hiddenlayers, sizeof(int));
	fill();
	for (int l = 0; l < hiddenlayers + 2; l++) { //for each layer

		for (unsigned int n = 0; n < getLayer(l).getSize(); n++) { //for each node

			try {
				char funct = 0;
				f.read((char*) &funct, 1); //funciton used
				f.exceptions(
						ifstream::eofbit | ifstream::failbit
								| ifstream::badbit); //throw errors
				if (funct == 1)
					layers[l].nodes[n].funct = ReLUs;
				else if (funct == 2)
					layers[l].nodes[n].funct = sigmoid;

				f.read((char*) &layers[l].nodes[n].bias, sizeof(double)); //bias of node
				double t = 1337;
				for (unsigned int c = 0; c < getLayer(l).getPrevSize(); c++) {
					f.read((char*) &t, sizeof(double)); //print conections weight
					layers[l].nodes[n].inputs[c].weight = t;
				}
			} catch (std::exception const & e) {
				cerr << "error: only " << f.gcount() << " could be read"
						<< endl;
				cerr << "something went wrong when loading a file" << endl;
				cerr << "Potential fix: use binary input";
				exit(1);
			}
		}
	}
}
/*
 * print prints the network to a nnw file
 * filenam eis the name of the file to write to
 */
void print(NeuralNetwork &net, std::string filename) {
	fstream f(filename.c_str(), ios::out | ios::binary);
	if (!f.fail()) {
		f.write((char*) &net.inputs, 4);
		f.write((char*) &net.outputs, 4);
		f.write((char*) &net.hiddenNodes, 4);
		f.write((char*) &net.hiddenlayers, 4);

		for (int l = 0; l < net.hiddenlayers + 2; l++) { //for each layer

			for (unsigned int n = 0; n < net.getLayer(l).getSize(); n++) { //for each node
				char funct = 0;
				if (net.getLayer(l).getNode(n).getfunct() == ReLUs)
					funct = 1;
				else if (net.getLayer(l).getNode(n).getfunct() == sigmoid)
					funct = 2;
				f.write((char*) &funct, 1); //funciton used
				double b = net.getLayer(l).getNode(n).getbias();
				f.write((char*) &b, sizeof(double)); //bias of node

				for (unsigned int c = 0; c < net.getLayer(l).getPrevSize();
						c++) {
					double w =
							net.getLayer(l).getNode(n).getInput(c).getWeight();
					f.write((char*) &w, sizeof(double)); //print conections weight
				}
			}
		}
	} else
		cerr << "file not opened";
}

//makes negative values 0
double ReLUs(double x) {
	return 1 / 2 * (/*abs(x) + */x);
}

//fits in range of 0 to 1
double sigmoid(double x) {
	return 1 / (1 + exp(-x));
}

double none(double x) {
	return x;
}
//derivative of sigmoid
double dsigmoid(double x) {
	return sigmoid(x) * (1 - sigmoid(x));
}
//derivative of relu
double dReLUs(double x) {
	if (x >= 0)
		return 1;
	else
		return 0;
}
