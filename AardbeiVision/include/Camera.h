/*
 ****************************************************************************
 * TITEL          Camera
 * OMSCHRIJVING   De klasse definitie voor de Camera klasse
 * BESTAND        Camera.h
 * VERSIE		  1
 * AUTEUR         Jimmy Hagens(2123658) en Stijn van Hest(2134863)
 *
 ****************************************************************************
 */
#pragma once
#include <iostream>
#include <sstream>
#include <string>
#include <vector>
#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>
using namespace cv;
using namespace std;

/*
* CLASS Beeldopslag
* DOEL Definitie van de klasse Beeldopslag
*/
class Camera {
private:
	
public:
	/**
	* FUNCTIE: GetImage(void)
	* DOEL: Roept de camera op en als er op a wordt gedrukt wordt er een plaatje gemaakt
	* INPUT: geen
	* OUTPUT: geen
	* RETURN: Foto
	*/
	Mat GetImage(int Aanbei);
};
