#ifndef DISCRETISATIONFACTORY_H
#define DISCRETISATIONFACTORY_H

#include "SerializeDeserializeJson.h"
#include "Discretisations.h"
#include <memory>
#include "Matrix.h"
#include <map>

class DiscretisationFactory{
	public:
	DiscretisationFactory(SerializeDeserializeJson& jsonTree,
			        const Matrix<std::string>& originalObservations,
        			Matrix<int>& discretisedObservation,
				std::unordered_map<std::string,int>& observationsMap,
        			std::map<std::pair<int,int>, std::string>& observationsMapR);

	std::unique_ptr<Discretisations> create(const std::string& nodeName, unsigned int row);

	unsigned int getMethodIndex(const std::string& methodS);
	SerializeDeserializeJson& jsonTree_;	
        const Matrix<std::string>& originalObservations_;
        Matrix<int>& discretisedObservations_;
        std::unordered_map<std::string,int>& observationsMap_;
        std::map<std::pair<int,int>, std::string>& observationsMapR_;
	std::unordered_map<std::string, unsigned int> methodIndex_;
};

#endif
