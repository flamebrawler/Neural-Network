/*
 * NeuralNetwork.h
 *
 *  Created on: Mar 18, 2018
 *      Author: Joel
 */

#ifndef NEURALNETWORK_H_
#define NEURALNETWORK_H_

#include <vector>
#include "Data.h"
#include <iostream>
#include <fstream>
#include "BmpParser.h"

using std::vector;

typedef double (*funct)(double);

class Layer;
class Node;
class Connection;

struct match{
	int in,out;
	double* question;
	double* answer;
};

class NeuralNetwork {
protected:
	int inputs;
	int outputs;
	int hiddenNodes;
	int hiddenlayers;

	funct function;
	funct endfunct;

	vector<Layer> layers;
public:
	NeuralNetwork();
	virtual ~NeuralNetwork();

	double* getResult()const;

	inline funct getFunct()const{return function;}
	inline funct getEnd()const{return endfunct;}
	inline int getInputs()const{return inputs;}
	inline int getOutputs()const{return outputs;}
	inline int getHN()const{return hiddenNodes;}
	inline int getHL()const{return hiddenlayers;}
	inline const Layer& getLayer(int pos)const{return layers[pos];}

	void fill();

	void run(double input);
	void run(const double inputs[]);

	void copyInfo(const NeuralNetwork& net);
	void copyVals(const NeuralNetwork& net);

	friend std::ostream& operator<<(std::ostream& out,const NeuralNetwork& net);
	friend std::ofstream& operator<<(std::ofstream& out,const NeuralNetwork& net);

	friend void print(NeuralNetwork &net, std::string filename);
	void readNNW(ifstream& f);
};
/*
 * unsupervised learning network that will be implemented at a later date
 */
class unsupervised:public NeuralNetwork{
private:

public:
	unsupervised();
	unsupervised(int inputs,int outputs,int function = 1,int endfunct = 1);
};

class supervised:public NeuralNetwork{
private:
	double *error;
	void learn(Layer* l,const double data[],const double intended[]);
	void learn2(Layer* l,const double data[],const double intended[]);
	double* backpropogate(const Layer& l,const Node& tested,const double* intended)const;
public:
	supervised();
	supervised(int inputs,int hiddenNodes,int outputs,int hiddenlayers,int function = 1,int endfunct = 1);

	double getError(const double intended[]);

	void learn(match* &p,int size);
	void learn(double data,double intended);
	void learn(const double data[],double intended);
	void learn(double data,double const intended[]);
	void learn(const double data[],const double intended[]);
};
double* convertfile(string name,int inputs);
double* wordtonum(std::string word, unsigned int size);

void print(const NeuralNetwork &net, std::string filename);

match* load(std::string file,int&counter);

double ReLUs(double x);
double dReLUs(double x);
double sigmoid(double x) ;
double dsigmoid(double x);
double none(double x);


#endif /* NEURALNETWORK_H_ */
