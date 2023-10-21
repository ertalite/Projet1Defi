/*******************************************************************************************
*
*   raylib game template
*
*   <Game title>
*   <Game description>
*
*   This game has been created using raylib (www.raylib.com)
*   raylib is licensed under an unmodified zlib/libpng license (View raylib.h for details)
*
*   Copyright (c) 2021 Ramon Santamaria (@raysan5)
*
********************************************************************************************/

/**
*  Projet 1 DEFI
*     Nom prénom : Paccini Mathys
*     Nom prénom : Louvigny Sam 
*     Numéro binôme : D
*/

#include "raylib.h"
#include "raymath.h"
#include <stdio.h>
#include <stdlib.h>
#include "ArduinoComP1DEFI.h"

//----------------------------------------------------------------------------------
// Déclarations (structures)
//----------------------------------------------------------------------------------

typedef struct RectangleEdges {
	float left;
	float right;
	float top;
	float bottom;
}RectangleEdges;

typedef struct RectangleCentered {
	Vector2 centre;
	Vector2 dimensions;

}RectangleCentered;

typedef struct Elipse {
	Vector2 centre;
	Vector2 dimensions;
}Elipse;

//----------------------------------------------------------------------------------
// Fonctions auxilaires
//----------------------------------------------------------------------------------

RectangleEdges RecToRecEdges(Rectangle rectangle) {
	RectangleEdges monRectangleE = {
		rectangle.x,
		rectangle.x + rectangle.width,
		rectangle.y,
		rectangle.y + rectangle.height,
	};
	return monRectangleE;
};

RectangleCentered RecEdgesToRecCentered(RectangleEdges mescotederec) {
	RectangleCentered monRectangleC = {
	.centre = {(mescotederec.left + mescotederec.right) / 2,(mescotederec.top + mescotederec.bottom) / 2},
	.dimensions = {mescotederec.right - mescotederec.left,mescotederec.bottom - mescotederec.top}
	};
	return monRectangleC;
};

Rectangle RecCenteredToRec(RectangleCentered rectanglecentered) {
	Rectangle monRectangleR = {
		rectanglecentered.centre.x - (rectanglecentered.dimensions.x / 2),
		rectanglecentered.centre.y - (rectanglecentered.dimensions.y / 2),
		rectanglecentered.dimensions.x,
		rectanglecentered.dimensions.y
	};
	return monRectangleR;
};

RectangleEdges RecCenteredToRecEdges(RectangleCentered rectanglecentered) {
	RectangleEdges monrectangleE = (RecToRecEdges(RecCenteredToRec(rectanglecentered)));
	return monrectangleE;
}

Rectangle RecEdgesToRec(RectangleEdges mescotesderec){
	Rectangle monrectangleR = (RecCenteredToRec(RecEdgesToRecCentered(mescotesderec)));
	return monrectangleR;
}

RectangleCentered RecToRecCentered(Rectangle rectangle) {
	RectangleCentered monrectangleC = RecEdgesToRecCentered(RecToRecEdges(rectangle));
	return monrectangleC;
}


//----------------------------------------------------------------------------------
// Fonction main
//----------------------------------------------------------------------------------

int main(void)
{
	//--------------------------------------------------------------------------------------
	// Initialization (code exécuté une fois au début du programme)
	//--------------------------------------------------------------------------------------

	InitWindow(800, 1000, "Projet 1 DEFI - Louvigny Sam - Paccini Mathys");
	InitAudioDevice();      // Initialize audio device
	SetTargetFPS(60);       // Set our game to run at 60 frames-per-second
	
	/* --> Votre code ici (A) */

	// Ouverture du port et initialisation des coordonnées du joystick

	char* portName = "COM4";

	SerialCom sCom = startArduinoConnection(portName, 9600);

	int joystickX = 512;
	int joystickY = 512;


	// Coordonnées de la voiture

	Vector2 carPos = {
		.x = 500.0,
		.y = 500.0
	};

	// Dimensions de la voiture

	Vector2 carDim = {
		.x = 60,
		.y = 120
	};

	// Vitesse de la voiture

	Vector2 vitesse = {
		.x = 0,
		.y = 0
	};

	RectangleEdges obstacleRec = { 450,550,450,550 }; // creation de l'obstacle


	//sprite de base de la voiture

	Vector2 carCenter = {
		.x = carDim.x / 2,  //Abscisse du centre de la voiture.
		.y = carDim.y / 2  // Ordonnée du centre de la voiture.
	};

	

	Color obstacleColor = BLACK;

	Color colorcarosserie = (Color) { 100, 35, 180, 255 };
	Color colorHeadlights = (Color) { 255, 250, 205, 255 };
	Color colorfeuderecul = (Color) { 237, 237, 237, 170 };

	double frottements = 5;
	double coefacceleration = 5000;



	//--------------------------------------------------------------------------------------
	// Game loop principale (code exécuté à chaque frame (60 fois par secondes))
	//--------------------------------------------------------------------------------------
	while (!WindowShouldClose())    // Detect window close button or ESC key
	{
		/*
		 * Gestion de tout ce qui n'est pas l'affichage ici
		 */

		 /* --> Votre code ici (B) */

		// Gestion des joysticks

		char* msgRecu = arduinoReceiveLatestMsg(sCom, '<', '>');

		if (msgRecu) {
			printf("%s\n", msgRecu);
			int status = sscanf(msgRecu,"< %d , %d >", &joystickX, &joystickY);
			
			if (status < 2) {
				printf("echec");

			}
		}
		else {

		}


		// Gestion des déplacements de la voiture selon l'acceleration

		Vector2 acceleration = {
		.x = 0,
		.y = 0
		};

		double frame = GetFrameTime(); 

		arduinoSendMsg(sCom, TextFormat("<%d, %d>", acceleration.x));



		

		//gestion des touches + joystick pour l'instant

		if (IsKeyDown(KEY_LEFT)|| joystickX < 500) { acceleration.x = acceleration.x - coefacceleration; }
		if (IsKeyDown(KEY_RIGHT) || joystickX > 600) { acceleration.x = acceleration.x + coefacceleration; }

		if (IsKeyDown(KEY_UP)|| joystickY < 500) { acceleration.y = acceleration.y - coefacceleration; }
		if (IsKeyDown(KEY_DOWN)|| joystickY > 600) {
			acceleration.y = acceleration.y + coefacceleration;
			colorfeuderecul = RED;
		}
		else
		{
			colorfeuderecul= (Color) { 237, 237, 237, 170 };
		}

	
		// gestion de l'acceleration 

		int ForcefrottementsX =  vitesse.x*(- frottements);
		int ForcefrottementY = vitesse.y * (-frottements);

		acceleration.x = acceleration.x + ForcefrottementsX;
		acceleration.y = acceleration.y + ForcefrottementY;
		
		vitesse.x = vitesse.x + frame * acceleration.x;
		vitesse.y = vitesse.y + frame * acceleration.y;
		

		carPos.x = carPos.x + frame * vitesse.x;
		carPos.y = carPos.y + frame * vitesse.y;

	
		// Creation du sprite de la voiture
		int headLightSize = carDim.x / 6; //Taille du rayon des phares.

		Rectangle spriteCarroserie = { carPos.x, carPos.y, carDim.x, carDim.y };

		Rectangle roue1 = { carPos.x - carDim.x / 15, carPos.y + carDim.y / 12, carDim.x / 6, carDim.y / 6 };
		Rectangle roue2 = { carPos.x - carDim.x / 15, carPos.y + carDim.y - carDim.y / 4, carDim.x / 6, carDim.y / 6 };
		Rectangle roue3 = { carPos.x + carDim.x - carDim.x / 15, carPos.y + carDim.y / 12, carDim.x / 6, carDim.y / 6 };
		Rectangle roue4 = { carPos.x + carDim.x - carDim.x / 15, carPos.y + carDim.y - carDim.y / 4, carDim.x / 6, carDim.y / 6 };

		Rectangle feuRecul1 = { carPos.x, carPos.y + carDim.y - carDim.y / 15, carDim.x / 3, carDim.y / 15 };
		Rectangle feuRecul2 = { carPos.x + carDim.x - carDim.x / 3, carPos.y + carDim.y - carDim.y / 15, carDim.x / 3, carDim.y / 15 };

		RectangleEdges hitBoxVoiture = RecToRecEdges(spriteCarroserie);

		RectangleCentered ilestcentre = RecToRecCentered(spriteCarroserie);

		Elipse spriteHabitacle = { ilestcentre.centre.x , ilestcentre.centre.y, carDim.x / 3, carDim.y / 3 };

		Elipse spritePhare1 = { spriteCarroserie.x + headLightSize,spriteCarroserie.y,headLightSize,headLightSize };

		Elipse spritePhare2 = { spriteCarroserie.x + carDim.x - headLightSize,spriteCarroserie.y,headLightSize,headLightSize };

		

		// Gestion des bordures 

		if (hitBoxVoiture.top < 0) { carPos.y = 0; }
		if (hitBoxVoiture.bottom> 1000) { carPos.y = 1000-spriteCarroserie.height; }

		if (hitBoxVoiture.left < 0) { carPos.x = 0; }
		if (hitBoxVoiture.right > 800) { carPos.x = 800-spriteCarroserie.width; }

		//gestion de l'obstacle ( A revoir : Pas bon !!)

		if ((hitBoxVoiture.bottom > obstacleRec.top && hitBoxVoiture.top < obstacleRec.bottom)
			&& (hitBoxVoiture.left < obstacleRec.right && hitBoxVoiture.right > obstacleRec.left)) {


			obstacleColor = RED;

		}
		else { obstacleColor = BLACK; }

		// Gestion de la taille de la voiture

		if (IsKeyDown(KEY_Y)) { carDim.y = carDim.y + 2; }
		if (IsKeyDown(KEY_H)) { carDim.y = carDim.y - 2; }
		if (IsKeyDown(KEY_J)) { carDim.x = carDim.x + 2; }
		if (IsKeyDown(KEY_G)) { carDim.x = carDim.x - 2; }

		/*
		 * Gestion de l'affichage ici
		 */
		BeginDrawing();
		{
			ClearBackground(RAYWHITE);
			

			/* --> Votre code ici (C) */
		
			DrawRectangleRec(RecEdgesToRec(obstacleRec), obstacleColor); //obstacle

			DrawEllipse(spritePhare1.centre.x ,spritePhare1.centre.y ,spritePhare1.dimensions.x, spritePhare1.dimensions.y, colorHeadlights); //Phare1
			DrawEllipse(spritePhare2.centre.x, spritePhare2.centre.y, spritePhare2.dimensions.x, spritePhare2.dimensions.x, colorHeadlights); //Phare1


			//DrawRectangle(carPos.x - carCenter.x, carPos.y - carCenter.y, carDim.x, carDim.y, colorcarosserie); //Carroserie

			//DrawEllipse(carPos.x - carCenter.x, carPos.y - carCenter.y, carDim.x / 3, carDim.y / 3, DARKGRAY); //Habitacle
			DrawRectangleRec(roue1, DARKGRAY);
			DrawRectangleRec(roue2, DARKGRAY);
			DrawRectangleRec(roue3, DARKGRAY);
			DrawRectangleRec(roue4, DARKGRAY);
			DrawRectangleRec(spriteCarroserie, colorcarosserie);
			DrawEllipse(spriteHabitacle.centre.x, spriteHabitacle.centre.y, spriteHabitacle.dimensions.x, spriteHabitacle.dimensions.y, DARKGRAY);
			
			DrawRectangleRec(feuRecul1, colorfeuderecul);
			DrawRectangleRec(feuRecul2, colorfeuderecul);


			// Interface mesures

			DrawFPS(10, 10);
			DrawText(TextFormat("42 : %d", 42), 10, 30, 10, BLACK);
			DrawText(TextFormat("carPos.x, carPos.y: %f %f", carPos.x, carPos.y), 10, 40, 10, BLACK);
			DrawText(TextFormat("vitesse.x, vitesse.y : %.2f, %.2f", vitesse.x, vitesse.y), 10, 50, 10, BLACK);
			DrawText(TextFormat("Paccini Mathys - Louvigny Sam "), 380, 980, 10, BLACK);

	
		}
		EndDrawing();
	}

	//--------------------------------------------------------------------------------------
	// De-Initialization (code exécuté une fois à la fin du programme)
	//--------------------------------------------------------------------------------------

	/* --> Votre code ici (D) */

	CloseAudioDevice();     // Close audio context
	CloseWindow();          // Close window and OpenGL context
	closeArduinoConnection(sCom); // Close Arduino's communications

	return 0;
}
