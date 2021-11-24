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

int main() 
{
	// Loop variabelen
	int MainLoop = 1;
	int ControleLoop = 0;

	while (MainLoop == 1) {
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
			

			waitKey(0);
			destroyAllWindows;
		}
		//bij keuze q stopt het programma
		else if (keuze == 'q') {
			MainLoop = 0;
		}
		// bij een keuze anders dan c,q of i geeft hij een foutmelding en mag je opnieuw invoeren
		else
			cout << "Verkeerde invoer, probeer opnieuw." << endl;
		waitKey(1500);


	}
}
