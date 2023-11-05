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

/*-----------------------------------Voiture-----------------------------------*/

typedef struct CarPassengerCompartmentSprite {
	Elipse sprite;
	Color color;

}CarPassengerCompartmentSprite;

typedef struct CarHeadLightSprite {
	int size;
	Color color;
	Elipse sprite1;
	Elipse sprite2;

}CarHeadLightSprite;

typedef struct CarBackLightSprite {
	Color color;
	Rectangle sprite1;
	Rectangle sprite2; 

}CarBackLightSprite;

typedef struct CarWheels {
	Color color;
	Rectangle wheel1;
	Rectangle wheel2;
	Rectangle wheel3;
	Rectangle wheel4;

}CarWheels;

typedef struct CarStats {
	int width;
	int height;
	Vector2 pos;

}CarStats;

typedef struct CarSprite {
	
	Color color;

	Rectangle spriteCarroserie;
	RectangleEdges hitBox;
	RectangleCentered spriteCarrosserieCentre;

	// Habitacle
	CarPassengerCompartmentSprite passagerCompartment;

	// Phares
	CarHeadLightSprite headLights;

	// Feu arrières
	CarBackLightSprite backLights;

	// Roues
	CarWheels wheels;

}CarSprite;

/*-----------------------------------Objets-----------------------------------*/

typedef struct Obstacle {
	RectangleEdges obstacleRec;
	Color obstacleColor;

}Obstacle;

typedef struct Coin {
	Elipse sprite;
	Color color;

}Coin;

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
};

Rectangle RecEdgesToRec(RectangleEdges mescotesderec) {
	Rectangle monrectangleR = (RecCenteredToRec(RecEdgesToRecCentered(mescotesderec)));
	return monrectangleR;
};

RectangleCentered RecToRecCentered(Rectangle rectangle) {
	RectangleCentered monrectangleC = RecEdgesToRecCentered(RecToRecEdges(rectangle));
	return monrectangleC;
};

/*-----------------------------------Affichage voiture-----------------------------------*/

void drawHeadLights(CarSprite car) {

	DrawEllipse(car.headLights.sprite1.centre.x, car.headLights.sprite1.centre.y, car.headLights.sprite1.dimensions.x, car.headLights.sprite1.dimensions.y, car.headLights.color); //Phare1
	DrawEllipse(car.headLights.sprite2.centre.x, car.headLights.sprite2.centre.y, car.headLights.sprite2.dimensions.x, car.headLights.sprite2.dimensions.x, car.headLights.color); //Phare1

};

void drawPassagerCompartment(CarSprite car) {

	DrawEllipse(car.passagerCompartment.sprite.centre.x, car.passagerCompartment.sprite.centre.y, car.passagerCompartment.sprite.dimensions.x, car.passagerCompartment.sprite.dimensions.y, DARKGRAY);

};

void drawbackLights(CarSprite car) {

	DrawRectangleRec(car.backLights.sprite1, car.backLights.color);
	DrawRectangleRec(car.backLights.sprite2, car.backLights.color);

};

void drawWheels(CarSprite car) {

	DrawRectangleRec(car.wheels.wheel1, DARKGRAY);
	DrawRectangleRec(car.wheels.wheel2, DARKGRAY);
	DrawRectangleRec(car.wheels.wheel3, DARKGRAY);
	DrawRectangleRec(car.wheels.wheel4, DARKGRAY);

};

void drawCar(CarSprite car, CarStats carStats) {


	drawWheels(car);
	drawHeadLights(car);
	DrawRectangleRec(car.spriteCarroserie, car.color);
	drawPassagerCompartment(car);
	drawbackLights(car);


};

/*-----------------------------------Affichage objets-----------------------------------*/

void drawCoins(Coin coin) {

	DrawEllipse(coin.sprite.centre.x, coin.sprite.centre.y, coin.sprite.dimensions.x, coin.sprite.dimensions.y, coin.color);

};

/*-----------------------------------Collisions-----------------------------------*/

void gestionBordures(CarSprite car, CarStats carStats) {

	/* Fonction realtive à la gestion des collisions entre les bordures de la fenêtre et la voiture.
	*
	*
	*/

	if (car.hitBox.bottom > 1000) { carStats.pos.y = 1000 - car.spriteCarroserie.height; }
	if (car.hitBox.top < 0) { carStats.pos.y = 0; }
	if (car.hitBox.left < 0) { carStats.width = 0; }
	if (car.hitBox.right > 800) { carStats.pos.x = 800 - car.spriteCarroserie.width; }

};

void gestionTailleVoiture(CarStats carStats) {

	/* Fonction realtive à la gestion du changement de taille de la voiture.
	*
	*
	*/

	if (IsKeyDown(KEY_Y)) { carStats.height = carStats.height + 2; }
	if (IsKeyDown(KEY_H)) { carStats.height = carStats.height - 2; }
	if (IsKeyDown(KEY_J)) { carStats.width = carStats.width + 2; }
	if (IsKeyDown(KEY_G)) { carStats.width = carStats.width - 2; }
};

/*-----------------------------------Interfaces-----------------------------------*/

void affichageInterface(CarSprite car, Vector2 vitesse, CarStats carStats) {

	DrawFPS(10, 10);
	DrawText(TextFormat("carStats.pos.x, carStats.pos.y: %f %f", carStats.pos.x, carStats.pos.y), 10, 30, 10, BLACK);
	DrawText(TextFormat("carStats.width, carStats.height: %i %i", carStats.width, carStats.height), 10, 40, 10, BLACK);
	DrawText(TextFormat("vitesse.x, vitesse.y : %.2f, %.2f", vitesse.x, vitesse.y), 10, 50, 10, BLACK);


	DrawText(TextFormat("Paccini Mathys - Louvigny Sam "), 380, 980, 10, BLACK);

};

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

	//SerialCom sCom = startArduinoConnection(portName, 9600);

	int joystickX = 512;
	int joystickY = 512;

	// Stats de la voiture

	CarStats carStats = {

		.width = 60,
		.height = 120,

		.pos = {
			.x = 500.0,
			.y = 500.0
		},

	};

	// Création d'un obstacle

	Obstacle blackObstacle = {
		.obstacleRec = { 450,550,450,550 }, // creation de l'obstacle
		.obstacleColor = BLACK,

	};

	// Création d'un pièce

	

	Coin coin = {
		.sprite = (Elipse) { GetRandomValue(0, 800), GetRandomValue(0, 1000), 20, 20 },
		.color = YELLOW,

	};

	// Vitesse de la voiture

	Vector2 vitesse = {
		.x = 0,
		.y = 0
	};

	double frottements = 5;
	double coefAcceleration = 5000;


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

		/*char* msgRecu = arduinoReceiveLatestMsg(sCom, '<', '>');

		if (msgRecu) {
			printf("%s\n", msgRecu);
			int status = sscanf(msgRecu,"< %d , %d >", &joystickX, &joystickY);
			
			if (status < 2) {
				printf("echec");

			}
		}
		else {

		}*/

		// Sprite de la voiture
		
		CarSprite car = {

			.color = (Color) { 100, 35, 180, 255 },

			.spriteCarroserie = { carStats.pos.x, carStats.pos.y, carStats.width, carStats.height },
			.hitBox = RecToRecEdges(car.spriteCarroserie),
			.spriteCarrosserieCentre = RecToRecCentered(car.spriteCarroserie),

			.passagerCompartment = {
				.sprite = { car.spriteCarrosserieCentre.centre.x, car.spriteCarrosserieCentre.centre.y, carStats.width / 3, carStats.height / 3 },
				.color = DARKGRAY,
			},

			.headLights = {
				.size = carStats.width / 6, // Rayon des phares
				.color = (Color){ 255, 250, 205, 255 },
				.sprite1 = { car.spriteCarroserie.x + car.headLights.size, car.spriteCarroserie.y, car.headLights.size, car.headLights.size },
				.sprite2 = { car.spriteCarroserie.x + carStats.width - car.headLights.size, car.spriteCarroserie.y, car.headLights.size, car.headLights.size },
			},

			.backLights = {
				.color = (Color){ 237, 237, 237, 170 },
				.sprite1 = { carStats.pos.x, carStats.pos.y + carStats.height - carStats.height / 15, carStats.width / 3, carStats.height / 15 },
				.sprite2 = { carStats.pos.x + carStats.width - carStats.width / 3, carStats.pos.y + carStats.height - carStats.height / 15, carStats.width / 3, carStats.height / 15 },
			},

			.wheels = {
				.color = (Color) { 237, 237, 237, 170 },
				.wheel1 = { carStats.pos.x - carStats.width / 15, carStats.pos.y + carStats.height / 12, carStats.width / 6, carStats.height / 6 },
				.wheel2 = { carStats.pos.x - carStats.width / 15, carStats.pos.y + carStats.height - carStats.height / 4, carStats.width / 6, carStats.height / 6 },
				.wheel3 = { carStats.pos.x + carStats.width - carStats.width / 15, carStats.pos.y + carStats.height / 12, carStats.width / 6, carStats.height / 6 },
				.wheel4 = { carStats.pos.x + carStats.width - carStats.width / 15, carStats.pos.y + carStats.height - carStats.height / 4, carStats.width / 6, carStats.height / 6 },
			},

		};

		// Gestion des déplacements de la voiture selon l'acceleration

		Vector2 acceleration = {
		.x = 0,
		.y = 0
		};

		double frame = GetFrameTime(); 

		//arduinoSendMsg(sCom, TextFormat("<%d, %d>", acceleration.x));

		// Gestion des touches + joystick 

		if (IsKeyDown(KEY_LEFT) || joystickX < 500) { acceleration.x = acceleration.x - coefAcceleration; }
		if (IsKeyDown(KEY_RIGHT) || joystickX > 600) { acceleration.x = acceleration.x + coefAcceleration; }

		if (IsKeyDown(KEY_UP) || joystickY < 500) { acceleration.y = acceleration.y - coefAcceleration; }
		if (IsKeyDown(KEY_DOWN) || joystickY > 600) {
			acceleration.y = acceleration.y + coefAcceleration;
			car.backLights.color = RED;
		}
		else
		{
			car.backLights.color = (Color) { 237, 237, 237, 170 };
		}
	
		// Gestion de l'acceleration 

		int ForcefrottementsX =  vitesse.x * (-frottements);
		int ForcefrottementY = vitesse.y * (-frottements);

		acceleration.x = acceleration.x + ForcefrottementsX;
		acceleration.y = acceleration.y + ForcefrottementY;
		
		vitesse.x = vitesse.x + frame * acceleration.x;
		vitesse.y = vitesse.y + frame * acceleration.y;
		

		carStats.pos.x = carStats.pos.x + frame * vitesse.x;
		carStats.pos.y = carStats.pos.y + frame * vitesse.y;


		// Gestion de l'obstacle

		if ((car.hitBox.bottom > blackObstacle.obstacleRec.top && car.hitBox.top < blackObstacle.obstacleRec.bottom)
			&& (car.hitBox.left < blackObstacle.obstacleRec.right && car.hitBox.right > blackObstacle.obstacleRec.left)) {

			blackObstacle.obstacleColor = RED;

		}
		else { blackObstacle.obstacleColor = BLACK; }

		// Gestion de la taille de la voiture
		
		gestionTailleVoiture(carStats);

		// Gestion des bordures 

		gestionBordures(car, carStats);

		/*
		 * Gestion de l'affichage ici
		 */

		BeginDrawing();
		{
			ClearBackground(RAYWHITE);

			/* --> Votre code ici (C) */
		
			DrawRectangleRec(RecEdgesToRec(blackObstacle.obstacleRec), blackObstacle.obstacleColor); // Affichage de l'obstacle

			drawCar(car, carStats); // Affichage de la voiture

			drawCoins(coin);
			
			affichageInterface(car, vitesse, carStats); // Interface mesures

	
		}
		EndDrawing();
	}

	//--------------------------------------------------------------------------------------
	// De-Initialization (code exécuté une fois à la fin du programme)
	//--------------------------------------------------------------------------------------

	/* --> Votre code ici (D) */

	CloseAudioDevice();     // Close audio context
	CloseWindow();          // Close window and OpenGL context
	//closeArduinoConnection(sCom); // Close Arduino's communications

	return 0;
}
