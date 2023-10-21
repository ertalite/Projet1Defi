#pragma once

/* Cette librarie sert d'interface entre votre projet et la librarie de bas niveau 
   appelée libserialport qui permet de communiquer avec la carte Arduino. */

/** Structure annonyme qui contient les données de la connection. 
    Vous n'avez pas besoin de vous en soucier. */
struct ArduinoComData;

/** Structure SerialCom.
    C'est la donnée que vous aller passer aux différentes fonctions. */
typedef struct SerialCom {
	struct ArduinoComData* p_aCom; // Pas besoin de comprendre ce que c'est.
} SerialCom;

/** Démarre une connexion avec la carte arduino.
 *  
 * @param portName une string donnant le nom du port où est branché la carte arduino.
 * Ex : "COM3"
 * @param baudrate la vitesse de transmission du canal. 
 * Celle-ci soit être la même que dans le Serial.begin(...) de la carte.
 * 
 * @return <<la connexion>>. Il faudra affecter cette connexion dans une variable de type SerialCom.
 * 
 * Exemple d'utilisation :
 * SerialCom sCom = startArduinoConnection("COM3", 9600);
 */
SerialCom startArduinoConnection(const char* portName, int baudrate);

/** Ferme une connexion avec la carte arduino.
 *
 * @param sCom la connexion à fermer.
 * 
 * Une fois fermé, il n'est plus possible d'utiliser la connexion.
 */
int closeArduinoConnection(SerialCom sCom);

/** Récupère le dernier message reçu de la carte Arduino
 *
 * @param sCom la connexion avec la carte.
 * @param start le premier caractère du message à recevoir.
 * @param end le dernier caractère du message à recevoir.
 *
 * @return la string (type char* en C) correspondant au message si un nouveau message à été reçu,
 *         NULL sinon.
 * 
 * Exemple d'utilisation :
 * char* monMessageRecu = arduinoReceiveLatestMsg(sCom, '<', '>');
 * (monMessageRecu peut correspondre à "<100,200,300>" par exemple)
 */
char* arduinoReceiveLatestMsg(SerialCom sCom, char start, char end);

/** Envoie un message à la carte Arduino
 *
 * @param sCom la connexion avec la carte.
 * @param msg le message à envoyer.
 *
 * @return -1 s'il y a eu une erreur, 0 sinon.
 * 
 * Exemple d'utilisation :
 * arduinoSendMsg(sCom, "Toto");
 */
int arduinoSendMsg(SerialCom sCom, char* msg);



/** Récupère un message reçu de la carte Arduino (pas forcément le dernier)
 *
 * Le message reçu est le premier dans la file d'attente de reception. 
 * 
 * @param sCom la connexion avec la carte.
 * @param start le premier caractère du message à recevoir.
 * @param end le dernier caractère du message à recevoir.
 *
 * @return la string (type char* en C) correspondant au message si un nouveau message à été reçu,
 *         NULL sinon.
 * 
 */
char* arduinoReceiveMsg(SerialCom sCom, char start, char end);