#include "network.h"
#include <stdexcept>
#include <iostream>
#include <algorithm>
#include <functional>
#include "random.h"


void Network::resize(const size_t& resize) {

	values.resize(resize);  //Use of the resize function of the class vector
	RNG.normal(values);
}

bool Network::add_link(const size_t& a, const size_t& b) {

bool added(false);  //boolean to check if the link can be added
	if (link_possible(a,b)) {   //We check if it is possible to add the link

		links.insert(std::pair<size_t, size_t>(a,b));  //We insert a pair of size_t twice to have a bidirectional link
		links.insert(std::pair<size_t,size_t>(b,a));

		added = true;
	}

	return added;
}

size_t Network::random_connect(const double& mean_deg) {

	links.clear(); //We destroy the map in order to refill it with some random connections
	
	for(size_t i(0); i < values.size(); ++i) {
		size_t deg(RNG.poisson(mean_deg));
		for(size_t j(0); j < deg; ++j) {
			while(!add_link(i,RNG.uniform_double(0, values.size()))) continue;	
		}
	}
 return links.size()/2; //We divide the size by 2 because each link created is bidirectional.
}

size_t Network::set_values(const std::vector<double>& toset) {
	values.clear();
	
	values = toset;
	
	return values.size();
}

size_t Network::size() const {

	return values.size();
}

size_t Network::degree(const size_t& n) const {
	try {
		
		return links.count(n);
		
	} catch (std::out_of_range &e) {
		std::cerr << "The element " << n << " does not exist" << std::endl;
	}
}

double Network::value(const size_t & n) const {
	try {

			return values[n];
			
	} catch (std::out_of_range &e) {
		std::cerr << "The value does not exist" << std::endl;
	}
}

std::vector<double> Network::sorted_values() const {
	
	std::vector<double> result(values);  //We use a new vector which will be a copy of values in order not to modify values

	std::sort (result.begin(), result.end(), std::greater<double>());

	return result;
}

std::vector<size_t> Network::neighbors(const size_t& n) const {

	std::vector<size_t> ngb;
	ngb.clear(); //Making sure the vector is completely empty.

	for (auto i : links) {
		if(i.first == n) {
			ngb.push_back(i.second);
		}
	}

	return ngb;
}


bool Network::link_existant(const size_t& a,const size_t& b) {

 	for (auto i: links) {
 		if((i.first == a or i.first == b) and (i.second == b or i.second == a)) {
 			return true;
 		}
 	}

 	return false;
 }
 
bool Network::link_possible (const size_t& a,const size_t& b) {
	
	if ((a!=b) and 	//if a and b are the same, we can't establish a bidirectionnal link
	   (std::max(a,b) < values.size()) and //we check if a and b are not out of the values vector range
	   (!link_existant(a,b))) {
		   return true;
	}
	
	return false;
}
	   
	   


