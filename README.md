###################################################
# Clinic_Client (https://github.com/jszmigiel2304/Clinic_Client)
Utworzona w Qt/C++, w modelu MVC aplikacja kliencka, która ³¹czy siê z serwerem Clinic_Server(https://github.com/jszmigiel2304/Clinic_Serwer).
Za jej pomoc¹ u¿ytkownik ³¹czy siê z serwerem, i z tamt¹d pobiera potrzebne dane.

Wykorzystuje programowanie wielow¹tkowe. Poza g³ownym w¹tkiem programu, klasy [c_loggeduserthread.h, c_processescontrollerthread.h, c_sessioncontrollerthread.h] dzia³aj¹ we w³asnych w¹tkach.

Clinic_Client poza mo¿liwoœci¹ zalogowanie, wyœwietlenia informacji o u¿ytkowniu, nie posiada wiêcej funkcjonalnoœci.
Wszytskie funkcjonalnoœci, z za³o¿enia, powinny byæ wkonowyane w odrêbnie uruchamianych modu³ach (klasa [c_moduleProcess]), jak np.:
Clinic_Client_User_Profile (https://github.com/jszmigiel2304/Clinic_Client_User_Profile),
przez klikniêcie odpowiedniego "kafelka" w g³ównym oknie(wyœwietlaj¹ siê dopiero po zalogowaniu).
Informacje o modu³ach, które siê pojawi¹ w g³ównym oknie mo¿na edytowaæ w [config.ini]

##########################################################

G³owne klasy aplikacji to:

c_ClinicClient: to tu inicjalizowane s¹ wszystkie potrzebne klasy,
--------------------------------------------------------------
c_connectionToServerController: klasa odpowiadaj¹ca za po³¹cznie z serwerem i komunikacjê z nim,
w listach:
  QList<myStructures::packet> packetsToSend;
  QList<myStructures::packet> waitingForReplyPackets;
  QList<myStructures::packet> waitingForReceiveConfirmation;
przechowywane s¹ informacje, które pakiety s¹ do wys³ania, które s¹ ju¿ wys³ane i oczekuj¹ na odpowiedŸ, a po okreœlonym czasie nastêpuje ponowne ich wys³anie
--------------------------------------------------------------
c_actionExecutive: w tej klasie przetwarzane s¹ wszystkie ¿adania i przygotowywane odpowiedzi
--------------------------------------------------------------
c_myLocalServer: dziedziczy z QLocalServer, konfiguruje, uruchamia, i przetwarza przychodz¹ce po³¹cznia (tj. ³¹cz¹ce siê z serwerem modu³y)
--------------------------------------------------------------
c_moduleProcess: obiekty tej klasy odpowiadaj¹ obecnie uruchomionym modu³om
c_moduleProcessConnection: klasa odpowiadaj¹ca za komunikacjê z pod³¹czonym c_moduleProcess
--------------------------------------------------------------
c_processesController: tu przechowywane s¹ informacje o aktualnie uruchomionych modu³ach [QList<c_moduleProcess *> OpenedModulesProcesses; QList<c_moduleProcessConnection *> openedModuleProcessConnections;] i ich po³¹czeniach wykorzystywanym do komunikacji z Clinic_Client
--------------------------------------------------------------
pary klas
c_loggedUser i c_loggedUserThread
c_processesController i c_processesControllerThread
c_SessionController i c_SessionControllerThread
to klasy dzia³aj¹ce we w³asnych w¹tkach. W klasach z koñcówk¹ 'Thread odbywa siê wiêkszoœæ operacji.
loggedUser: odpowiada za dzia³anie zwi¹zane z u¿ytkownikem
c_SessionController: odpowiada za dzia³anie sesji
processesController: odpowiada dzia³ania zwi¹zane z uruchomionymi modu³ami
--------------------------------------------------------------
Dodatkowo:
c_Parser: tutaj przygotowywane s¹ pakiety do wys³ania do serwera

c_waitingLoop: klasa, która pozwala na wymuszenie oczekiwania do momentu, a¿ aplikacja nie otrzyma ich np serwera. Pomaga w rozwi¹zywaniu problemu asynchronicznegootrzymywania danych z serwera.
waitingCondition: obiekty tej klasy reprezentuj¹ warunki jakie musz¹ byæ spe³nione, ¿eby waitingLoop móg³ przerwaæ oczekiwanie

