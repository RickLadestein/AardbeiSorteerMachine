/*
 ****************************************************************************
 * TITEL          Camera
 * OMSCHRIJVING   Deze klasse implementatie maakt een foto met de camera als er op de spatiebalk wordt gedrukt
 * BESTAND        Camera.cpp
 * VERSIE		  2
 * AUTEUR         Jimmy Hagens (2123658) en Stijn van Hest(2134863)
 *
 ****************************************************************************
 */
#include "Camera.h"


 /**
 * MEMBER-FUNCTIE IMPLEMENTATIE: GetImage()
 */
Mat Camera::GetImage(int Controleloop) {

	VideoCapture cap;

	int i;
	for (i = 1; i < 255; i++) {
		cap.open(i);
		if (cap.isOpened())break;
	}

	// Breedte en hooogte van het scherm aanpassen naar die van de camera. 
	int dWidth = cap.set(CAP_PROP_FRAME_WIDTH, 1280);
	int dHeight = cap.set(CAP_PROP_FRAME_HEIGHT, 960);

	// Window maken waarin het live beeld komt
	namedWindow("MyVideo", WINDOW_FREERATIO);

	// Continue loop waarin een beeld wordt opgehaald en wordt getoond in het window
	Mat frame;
	Mat Foto;
	int loop = 1;
	while (loop == 1)
	{
		// Lees een nieuw frame
		bool bSuccess = cap.read(frame);

		// Het tonen van beeld
		if (frame.empty()) {
			continue;
		}
		imshow("MyVideo", frame);

		// als de spatiebalk ingedrukt wordt, maak een foto
		char c = waitKey(1);
		switch (c) {
		case ' ':
			destroyWindow("MyVideo");
			frame.copyTo(Foto);
			loop = 0;
		}
	}
	cap.release();
	return Foto;
}