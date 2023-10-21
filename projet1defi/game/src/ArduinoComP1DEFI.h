#pragma once

/* Cette librarie sert d'interface entre votre projet et la librarie de bas niveau 
   appel�e libserialport qui permet de communiquer avec la carte Arduino. */

/** Structure annonyme qui contient les donn�es de la connection. 
    Vous n'avez pas besoin de vous en soucier. */
struct ArduinoComData;

/** Structure SerialCom.
    C'est la donn�e que vous aller passer aux diff�rentes fonctions. */
typedef struct SerialCom {
	struct ArduinoComData* p_aCom; // Pas besoin de comprendre ce que c'est.
} SerialCom;

/** D�marre une connexion avec la carte arduino.
 *  
 * @param portName une string donnant le nom du port o� est branch� la carte arduino.
 * Ex : "COM3"
 * @param baudrate la vitesse de transmission du canal. 
 * Celle-ci soit �tre la m�me que dans le Serial.begin(...) de la carte.
 * 
 * @return <<la connexion>>. Il faudra affecter cette connexion dans une variable de type SerialCom.
 * 
 * Exemple d'utilisation :
 * SerialCom sCom = startArduinoConnection("COM3", 9600);
 */
SerialCom startArduinoConnection(const char* portName, int baudrate);

/** Ferme une connexion avec la carte arduino.
 *
 * @param sCom la connexion � fermer.
 * 
 * Une fois ferm�, il n'est plus possible d'utiliser la connexion.
 */
int closeArduinoConnection(SerialCom sCom);

/** R�cup�re le dernier message re�u de la carte Arduino
 *
 * @param sCom la connexion avec la carte.
 * @param start le premier caract�re du message � recevoir.
 * @param end le dernier caract�re du message � recevoir.
 *
 * @return la string (type char* en C) correspondant au message si un nouveau message � �t� re�u,
 *         NULL sinon.
 * 
 * Exemple d'utilisation :
 * char* monMessageRecu = arduinoReceiveLatestMsg(sCom, '<', '>');
 * (monMessageRecu peut correspondre � "<100,200,300>" par exemple)
 */
char* arduinoReceiveLatestMsg(SerialCom sCom, char start, char end);

/** Envoie un message � la carte Arduino
 *
 * @param sCom la connexion avec la carte.
 * @param msg le message � envoyer.
 *
 * @return -1 s'il y a eu une erreur, 0 sinon.
 * 
 * Exemple d'utilisation :
 * arduinoSendMsg(sCom, "Toto");
 */
int arduinoSendMsg(SerialCom sCom, char* msg);



/** R�cup�re un message re�u de la carte Arduino (pas forc�ment le dernier)
 *
 * Le message re�u est le premier dans la file d'attente de reception. 
 * 
 * @param sCom la connexion avec la carte.
 * @param start le premier caract�re du message � recevoir.
 * @param end le dernier caract�re du message � recevoir.
 *
 * @return la string (type char* en C) correspondant au message si un nouveau message � �t� re�u,
 *         NULL sinon.
 * 
 */
char* arduinoReceiveMsg(SerialCom sCom, char start, char end);