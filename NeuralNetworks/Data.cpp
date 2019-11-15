/*
 * Node.cpp
 *
 *  Created on: Mar 20, 2018
 *      Author: Joel
 */

#include "Data.h"
#include <iostream>

//funct is the function the node will use
Node::Node(double (*funct)(double)) {
	this->funct = funct;
	value = 0;
	bias = 0;
}

Node::~Node() {
	inputs.clear();
}
/*
 * adds all of the previous nodes together
 */
void Node::add() {
	double sum = 0;
	for (int i = 0; i < getInputs(); i++) {
		sum += inputs[i].getWeight() * inputs[i].getPrev()->value;
	}
	value = funct(sum + bias);
}
/*
 * sum() functions the same as add() but does not change the value with the option to use bias and there is no function
 * bias is if a bias is wantedt o be added to the value
 * returns the sum the the nodes
 */
double Node::sum(bool bias)const{
	double sum = 0;
	for(int i = 0;i<getInputs();i++){//sum the previous nodes
		sum += inputs[i].getWeight() * inputs[i].getPrev()->value;
	}
	if(bias)//add bias
		return sum+bias;
	return sum;
}
//resets the value to 0 if ever necessary
void Node::reset() {
	value = 0;
}
//size is the number of nodes in th layer
Layer::Layer(int size) {
	this->size = size;
	prev = 0;
	next = 0;
}

Layer::~Layer() {
	nodes.clear();
}

Connection::Connection(double weight, Node* prev, Node* next) {
	this->weight = weight;
	this->prev = prev;
	this->next = next;
}

