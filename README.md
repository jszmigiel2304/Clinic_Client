###################################################
# Clinic_Client (https://github.com/jszmigiel2304/Clinic_Client)
Utworzona w Qt/C++, w modelu MVC aplikacja kliencka, kt�ra ��czy si� z serwerem Clinic_Server(https://github.com/jszmigiel2304/Clinic_Serwer).
Za jej pomoc� u�ytkownik ��czy si� z serwerem, i z tamt�d pobiera potrzebne dane.

Wykorzystuje programowanie wielow�tkowe. Poza g�ownym w�tkiem programu, klasy [c_loggeduserthread.h, c_processescontrollerthread.h, c_sessioncontrollerthread.h] dzia�aj� we w�asnych w�tkach.

Clinic_Client poza mo�liwo�ci� zalogowanie, wy�wietlenia informacji o u�ytkowniu, nie posiada wi�cej funkcjonalno�ci.
Wszytskie funkcjonalno�ci, z za�o�enia, powinny by� wkonowyane w odr�bnie uruchamianych modu�ach (klasa [c_moduleProcess]), jak np.:
Clinic_Client_User_Profile (https://github.com/jszmigiel2304/Clinic_Client_User_Profile),
przez klikni�cie odpowiedniego "kafelka" w g��wnym oknie(wy�wietlaj� si� dopiero po zalogowaniu).
Informacje o modu�ach, kt�re si� pojawi� w g��wnym oknie mo�na edytowa� w [config.ini]

##########################################################

G�owne klasy aplikacji to:

c_ClinicClient: to tu inicjalizowane s� wszystkie potrzebne klasy,
--------------------------------------------------------------
c_connectionToServerController: klasa odpowiadaj�ca za po��cznie z serwerem i komunikacj� z nim,
w listach:
  QList<myStructures::packet> packetsToSend;
  QList<myStructures::packet> waitingForReplyPackets;
  QList<myStructures::packet> waitingForReceiveConfirmation;
przechowywane s� informacje, kt�re pakiety s� do wys�ania, kt�re s� ju� wys�ane i oczekuj� na odpowied�, a po okre�lonym czasie nast�puje ponowne ich wys�anie
--------------------------------------------------------------
c_actionExecutive: w tej klasie przetwarzane s� wszystkie �adania i przygotowywane odpowiedzi
--------------------------------------------------------------
c_myLocalServer: dziedziczy z QLocalServer, konfiguruje, uruchamia, i przetwarza przychodz�ce po��cznia (tj. ��cz�ce si� z serwerem modu�y)
--------------------------------------------------------------
c_moduleProcess: obiekty tej klasy odpowiadaj� obecnie uruchomionym modu�om
c_moduleProcessConnection: klasa odpowiadaj�ca za komunikacj� z pod��czonym c_moduleProcess
--------------------------------------------------------------
c_processesController: tu przechowywane s� informacje o aktualnie uruchomionych modu�ach [QList<c_moduleProcess *> OpenedModulesProcesses; QList<c_moduleProcessConnection *> openedModuleProcessConnections;] i ich po��czeniach wykorzystywanym do komunikacji z Clinic_Client
--------------------------------------------------------------
pary klas
c_loggedUser i c_loggedUserThread
c_processesController i c_processesControllerThread
c_SessionController i c_SessionControllerThread
to klasy dzia�aj�ce we w�asnych w�tkach. W klasach z ko�c�wk� 'Thread odbywa si� wi�kszo�� operacji.
loggedUser: odpowiada za dzia�anie zwi�zane z u�ytkownikem
c_SessionController: odpowiada za dzia�anie sesji
processesController: odpowiada dzia�ania zwi�zane z uruchomionymi modu�ami
--------------------------------------------------------------
Dodatkowo:
c_Parser: tutaj przygotowywane s� pakiety do wys�ania do serwera

c_waitingLoop: klasa, kt�ra pozwala na wymuszenie oczekiwania do momentu, a� aplikacja nie otrzyma ich np serwera. Pomaga w rozwi�zywaniu problemu asynchronicznegootrzymywania danych z serwera.
waitingCondition: obiekty tej klasy reprezentuj� warunki jakie musz� by� spe�nione, �eby waitingLoop m�g� przerwa� oczekiwanie

