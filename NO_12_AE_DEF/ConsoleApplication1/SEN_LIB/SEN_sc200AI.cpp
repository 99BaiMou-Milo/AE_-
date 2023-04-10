#include"SEN_sc200AI.h"


void Set_G(VideoCapture CAM, int exprouse){

	exprouse = exprouse < 0 ? 0 : exprouse;
	exprouse = exprouse > 54000 ? 54000 : exprouse;

	CAM.set(CAP_PROP_GAIN, exprouse);
}


