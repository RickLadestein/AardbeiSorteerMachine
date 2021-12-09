#define _USE_MATH_DEFINES

#include <iostream>
#include <iostream>
#include <vector>
#include <chrono>

#include "opencv2/opencv.hpp"
#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <chrono>
#include <math.h>
#include <glm/glm.hpp>

#include "Camera.h"
//#include "bewerking.h"

int main() 
{
	// Loop variabelen
	int MainLoop = 1;
	int ControleLoop = 0;
	int I = 0;

	while (MainLoop == 1) {
		I++;

		system("cls");

		cout << "Typ een 'c' gevolgd door een enter voor controle." << endl <<
			    "Typ een 'q' gevolgd door een enter om te stoppen. " << endl;

		char keuze = cin.get();
		//bij keuze c
		if (keuze == 'c') {
			ControleLoop += 1;

			// Maak een foto
			cout << "Druk op de spatiebalk om een foto te nemen" << endl;
			Camera Foto;
			Mat aanbei = Foto.GetImage(ControleLoop);
					
			resize(aanbei, aanbei, Size(), 0.2, 0.2, INTER_AREA);
			imshow("plaatje", aanbei);

			ostringstream Bestandnaam;
			Bestandnaam << "C:\\aardbei\\fake\\" "aardbei+I" ".PNG";
			imwrite(Bestandnaam.str(), aanbei);

		}
		//bij keuze q stopt het programma
		else if (keuze == 'q') {
			//MainLoop = 0;
			ControleLoop += 1;
			Mat OGimage = imread("C:\\Users\\bas_j\\OneDrive - Avans Hogeschool\\aardbei-sorteersysteem\\aardbei\\fake\\7.PNG");

			//imshow("origineel", OGimage);

			Mat splitImage[3];

			split(OGimage, splitImage);

			//imshow("blue", splitImage[0]);
			//imshow("green", splitImage[1]);
			//imshow("red", splitImage[2]);

			Mat Regions[3];

			threshold(splitImage[0], Regions[0], 0, 255, THRESH_OTSU);
			//threshold(splitImage[1], Regions[1], 0, 255, THRESH_OTSU);
			//threshold(splitImage[2], Regions[2], 0, 255, THRESH_OTSU);

			threshold(splitImage[1], Regions[1], 100, 255, THRESH_BINARY);
			threshold(splitImage[2], Regions[2], 100, 255, THRESH_BINARY);

			Mat BackGroundInv = Regions[0];
			Mat Crown = Regions[1];
			Mat Berry = Regions[2];

			//imshow("Background", BackGroundInv);
			//imshow("Crown", Crown);
			//imshow("Berry", Berry);

			Mat BackGround;

			bitwise_not(BackGroundInv, BackGround);

			Mat CrownRegion;
			Mat BerryRegion;

			bitwise_and(BackGround, Crown, CrownRegion);
			bitwise_and(BackGround, Berry, BerryRegion);

			// Beeldverwerking kroontje

			Mat element = getStructuringElement(MORPH_RECT, Size(3, 3), Point(-1, -1));
			// optimalisatie van afbeelding met erosie/dilatie

			Mat CrownErosion;
			Mat CrownDilation;

			// opening
			erode(CrownRegion, CrownErosion, element);
			dilate(CrownErosion, CrownDilation, element);

			// closing
			dilate(CrownDilation, CrownDilation, element);
			erode(CrownDilation, CrownErosion, element);

			Moments mo = moments(CrownDilation);
			auto result = Point(mo.m10 / mo.m00, mo.m01 / mo.m00);

			// beeldverwerking aardbei
			Mat BerryDilation;
			Mat BerryErosion;
			dilate(BerryRegion, BerryDilation, element);
			erode(BerryDilation, BerryErosion, element);

			imshow("Bei Gebied", BerryErosion);

			double BiggestDist = 0;
			auto FurthestPoint = Point(0, 0);
			
			for (int i = 0; i < BerryErosion.rows; i++)
			{
				for (int j = 0; j < BerryErosion.cols; j++)
				{
					int pixelvalue = BerryErosion.at<uchar>(i, j);
					if (pixelvalue > 0) {
						// afstand meten tot kroontje
						double distance = sqrt(pow((result.x - j), 2) + pow((result.y - i), 2));
						//std::cout << endl << distance;
						if (distance > BiggestDist) {
							BiggestDist = distance;
							FurthestPoint = Point(j, i);
						}
						//std::cout << result.x << ", " << result.y << " tot aan " << i << ", " << j << endl;
					}
				}
			}

			glm::vec2 StartPoint = glm::vec2(result.x, result.y);
			glm::vec2 EndPoint = glm::vec2(FurthestPoint.x, FurthestPoint.y);

			// oriëntatie-as berekenen
			double m = (StartPoint.y - EndPoint.y) / (StartPoint.x - EndPoint.x);
			double b = -(m * StartPoint.x) + StartPoint.y;
			// y = mx + b

			// Slope vertalen naar hoek voor rotatiematrix
			Mat Rotated;
			Point2f pc(BerryErosion.cols / 2., BerryErosion.rows / 2.);
			double angleRad = atan(1 / m);
			double angle = (angleRad * 180) / M_PI;

			Mat rot = getRotationMatrix2D(pc, angle, 1.0);

			warpAffine(BerryErosion, Rotated, rot, BerryErosion.size());

			Mat Black(BerryErosion.rows, BerryErosion.cols, CV_8UC1, Scalar(0));
			Mat CutSlice;
			int BiggestDiameter = 0;
			auto GrabPoint1 = Point(0, 0);
			auto GrabPoint2 = Point(0, 0);

			for (int ColIndex = 0; ColIndex < Rotated.cols; ColIndex++)
			{
				//cout << "Colom: " << i << endl;
				Mat BlackReplace = Black;
				line(BlackReplace, Point(ColIndex, 1), Point(ColIndex, Rotated.rows), Scalar(255, 255, 255), 1, LINE_AA);
				bitwise_and(BlackReplace, BerryErosion, CutSlice);
				for (int j = 0; j < CutSlice.rows -1; j++)
				{
					int PixVal = CutSlice.at<char>(j, ColIndex);
					if (PixVal != 0) {
						int HighestBorder = j;
						for (int k = CutSlice.rows - 1; k > 1; k--)
						{
							int PixVal = CutSlice.at<char>(k, ColIndex);
							if (PixVal != 0) {
								int LowestBorder = k;
								int Diameter = k - j;
								if (Diameter > BiggestDiameter) {
									BiggestDiameter = Diameter;
									GrabPoint1 = Point(j, ColIndex);
									GrabPoint2 = Point(k, ColIndex);
								}
							}
						}
					}
				}
			}

			
			//double angleRadIn = atan(1/m);
			//double angleIn = (angleRadIn * 180) / M_PI;
			Mat Invert = getRotationMatrix2D(pc, angle, 1.0);

			Mat Invert;	
			invertAffineTransform(rot, Invert);

			vector<Point2f> yourPoints;
			yourPoints.push_back(Point2f(GrabPoint1.x, GrabPoint1.y));
			yourPoints.push_back(Point2f(GrabPoint2.x, GrabPoint2.y));

			vector<Point2f> transformedPoints;

			//perspectiveTransform(yourPoints, transformedPoints, Invert);
			transform(yourPoints, transformedPoints, Invert);

			line(Rotated, Point(GrabPoint1.y, GrabPoint1.x), Point(GrabPoint2.y, GrabPoint2.x), Scalar(0, 255, 0), 2, LINE_AA);
			
			line(OGimage, Point(result.x, result.y), Point(FurthestPoint.x, FurthestPoint.y), Scalar(0, 255, 0), 2, LINE_AA);
			line(OGimage, Point(transformedPoints[0].x, transformedPoints[0].y), Point(transformedPoints[1].x, transformedPoints[1].y), Scalar(255, 0, 0), 2, LINE_AA);

			circle(OGimage, result, 4, Scalar(255, 0, 0), FILLED);
			circle(OGimage, FurthestPoint, 4, Scalar(255, 0, 0), FILLED);

			imshow("Gevonden as", OGimage);
			imshow("Rotated", Rotated);
		}

		// bij een keuze anders dan c,q of i geeft hij een foutmelding en mag je opnieuw invoeren
		else
			cout << "Verkeerde invoer, probeer opnieuw." << endl;
		waitKey(1500);
	}
}
