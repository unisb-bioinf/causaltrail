#include "DiscretiseFloor.h"

DiscretiseFloor::DiscretiseFloor(unsigned int row,
                                const Matrix<std::string>& originalObservations,
                                Matrix<int>& discretisedObservations)
	:Discretisations(row,originalObservations,discretisedObservations)
{
}

void DiscretiseFloor::apply(){
//        for(unsigned int col = 0; col < originalObservations_.getColCount();
  //          col++) {
    //            float value = getNumber(col, row);
//                int dvalue = floor(value);
  //              observations_.setData(dvalue, col, row);
    //    }
}
