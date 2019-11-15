/*
 * Node.h
 *
 *  Created on: Mar 20, 2018
 *      Author: Joel
 */

#ifndef DATA_H_
#define DATA_H_

#include <vector>
#include "NeuralNetwork.h"
#include <iostream>

class NeuralNetwork;
class Connection;

typedef double (*f)(double);

class Node {
private:
	double value;
	double bias;
	f funct;
	std::vector<Connection> inputs;
public:

	Node(double (*funct)(double));
	virtual ~Node();

	inline double getVal()const {
		return value;
	}
	inline double getbias()const {
		return bias;
	}
	inline const Connection& getInput(int pos)const{
		return inputs[pos];
	}
	inline f getfunct() const{
		return funct;
	}
	inline int getInputs() const{
		return inputs.size();
	}

	void addInput(Connection c);

	void add();
	double sum(bool bias)const;
	void reset();

	friend class NeuralNetwork;
	friend class unsupervised;
	friend class supervised;
	friend class Layer;
};

class Layer {
private:
	unsigned int size;
	std::vector<Node> nodes;
	Layer* prev;
	Layer* next;
public:

	inline const Layer* getPrev() const{return prev;}
	inline const Layer* getNext() const{return next;}
	inline const Node& getNode(int pos)const{return nodes[pos];}
	inline unsigned int getSize()const {return size;}
	inline unsigned int getPrevSize() const{return nodes[0].getInputs();}

	Layer(int size);
	virtual ~Layer();

	friend class NeuralNetwork;
	friend class supervised;
	friend class unsupervised;
};

class Connection {
private:
	Node *prev, *next;
	double weight;
public:

	Connection(double weight, Node* prev, Node* next);

	inline double getWeight() const{
		return weight;
	}
	inline const Node * getPrev() const{
		return prev;
	}
	inline const Node * getNext() const{
		return next;
	}

	friend class NeuralNetwork;
	friend class supervised;
	friend class unsupervised;

};

#endif /* DATA_H_ */
