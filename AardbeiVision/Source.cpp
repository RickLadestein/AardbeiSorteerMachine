#include <iostream>
#include <iostream>
#include <vector>
#include <chrono>

#include "opencv2/opencv.hpp"
#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <chrono>

#include "Camera.h"

int Aanbei;

int main() 
{
	// Maak een foto
	cout << "Druk op de spatiebalk om een foto te nemen" << endl;
	Camera foto;
	Mat CapsuleHouderOrg = foto.GetImage(Aanbei);
}