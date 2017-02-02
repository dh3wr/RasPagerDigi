#include <arpa/inet.h>
#include <fcntl.h>
#include <linux/i2c-dev.h>
#include <stdio.h>
#include <sys/ioctl.h>
#include <unistd.h>
#include <wiringPi.h>
#include <wiringPiI2C.h>

#include <iostream>
#include <sstream>
#include <string>
#include <iomanip>  // ::std::setw
#include <ios>      // ::std::fixed

#include "menus/info/hardware.h"
#include "menus/info/temperature.h"
#include "menus/info/network.h"
#include "menus/info/ntp.h"
#include "menus/menuiteminfo.h"
#include "menus/menuiteminforotator.h"
#include "menus/menuitemokback.h"
#include "menus/settings/dnseingabe.h"
#include "menus/settings/gatewayeingabe.h"
#include "menus/settings/ipadresseeingabe.h"
#include "menus/settings/ipeingabe.h"
#include "menus/settings/subneteingabe.h"
#include "menus/system/linuxupdate.h"
#include "menus/system/restart.h"
#include "menus/system/settime.h"
#include "menus/system/shutdown.h"
#include "tools/networkcontrol.h"
#include "tools/numberhandler.h"
#include "tools/raspagerdigiextension.h"
#include "tools/systemcontrol.h"
#include "tools/serverprocess.h"
#include "tools/ProcessManager.h"

// Not yet working
//#include "websocket/WebSocketServer.h"

// Instead include the server here
#include "websocket/server_ws.hpp"
#include "json_spirit.h"

using namespace std;

#define PROG_VERSION	"0.0.13"
#define COPYRIGHTZEILE1	"RasPagerDigi by DH3WR"
#define COPYRIGHTZEILE2	"DF6EF, Delissen 0.0.13"

#define TASTERDELAY_MS	50

bool skipDisplaySetup;
string master, activeslots;
//static ProcessManager g_pm;

// WEBSOCKETSERVER
typedef SimpleWeb::SocketServer<SimpleWeb::WS> WsServer;
// END WEBSOCKETSERVER


int main(int argc, char** argv) {
    skipDisplaySetup = false; // TRUE: Allows usage without Display attatched.
    cout << "  ____           ____                        ____            _             _ \n |  _ \\ __ _ ___|  _ \\ __ _  __ _  ___ _ __ / ___|___  _ __ | |_ _ __ ___ | |\n | |_) / _` / __| |_) / _` |/ _` |/ _ \\ '__| |   / _ \\| '_ \\| __| '__/ _ \\| |\n |  _ < (_| \\__ \\  __/ (_| | (_| |  __/ |  | |__| (_) | | | | |_| | | (_) | |\n |_| \\_\\__,_|___/_|   \\__,_|\\__, |\\___|_|   \\____\\___/|_| |_|\\__|_|  \\___/|_|\n                            |___/                                      v" << PROG_VERSION << "\n" << std::endl;

    NetworkControl myNetworkControl;
    SystemControl mySystemControl;
    activeslots = "0123456789ABCDEF";
    master = "DB0ABC";
	static RaspagerDigiExtension myExtension(skipDisplaySetup);
    static OneWire myOneWire;

// WEBSOCKETSERVER
	WebSocketServer wsv(myExtension);

    thread server_thread(std::bind(WebSocketServer::run, &wsv));
    //Wait for server to start so that the client can connect
    this_thread::sleep_for(chrono::seconds(1));
// END WEBSOCKETSERVER


    // Hauptmenü
    cout << "Bereite Menus vor..." << std::endl;
    MenuItemOkBack myMainMenu("Hauptmenu", &myExtension);

    // Infos vorbereiten (für spätere Nutzung)
    InfoNetwork myInfoNetwork(&myNetworkControl, &myExtension);
    InfoNtp myInfoNtp(&myNetworkControl, &myExtension);
    InfoHardware myInfoHardware(&myExtension);
    InfoTemperature myInfoTemperature(&myExtension, &myOneWire);

    // Infos
    MenuItemOkBack myMenuStats("Infos und Statistiken", &myExtension);
    myMenuStats.addSubmenuItem(&myInfoNetwork);
    myMenuStats.addSubmenuItem(&myInfoNtp);
    myMenuStats.addSubmenuItem(&myInfoHardware);
    myMenuStats.addSubmenuItem(&myInfoTemperature);
    myMenuStats.addSubmenuItem(new MenuItemInfo("Kernel-Version", mySystemControl.readKernelVersion(), &myExtension));
    myMenuStats.addSubmenuItem(new MenuItemInfo(COPYRIGHTZEILE1, COPYRIGHTZEILE2, &myExtension));
    myMenuStats.addSubmenuItem(new MenuItemInfo("RasPagerControl-Version", PROG_VERSION, &myExtension));


    // Einstellungen
    MenuItemOkBack myMenuEinstellungen("Einstellungen", &myExtension);
    myMenuEinstellungen.addSubmenuItem(new MenuIpAdresseEingabe("IP-Adresse", &myNetworkControl, &myExtension));
    myMenuEinstellungen.addSubmenuItem(new MenuSubnetEingabe("Subnetz-Maske", &myNetworkControl, &myExtension));
    myMenuEinstellungen.addSubmenuItem(new MenuGatewayEingabe("Gateway", &myNetworkControl, &myExtension));
    myMenuEinstellungen.addSubmenuItem(new MenuDnsEingabe("DNS-Server 1", &myNetworkControl, 1, &myExtension));
//    myMenuEinstellungen.addSubmenuItem(new MenuDnsEingabe("DNS-Server 2", &myNetworkControl, 2, &myExtension));

    // System
    MenuItemOkBack myMenuSystem("System", &myExtension);
    myMenuSystem.addSubmenuItem(new MenuLinuxUpdate("Pakete aktualisieren", false, &myExtension));
    myMenuSystem.addSubmenuItem(new MenuLinuxUpdate("Linux aktualisieren", true, &myExtension));
    myMenuSystem.addSubmenuItem(new MenuSetTime("Zeit setzen (NTP)", &myExtension));
    myMenuSystem.addSubmenuItem(new MenuRestart("Neustart", &myExtension));
    myMenuSystem.addSubmenuItem(new MenuShutdown("Herunterfahren", &myExtension));

    // Hauptmenü zusammenbauen, aktiv setzen und ausgeben
    myMainMenu.addSubmenuItem(&myMenuStats);
    myMainMenu.addSubmenuItem(&myMenuEinstellungen);
    myMainMenu.addSubmenuItem(&myMenuSystem);
    myMainMenu.setActive(true);
    cout << "Hauptmenu bereit!" << std::endl;

    // Screensaver
    cout << "Bereite Screensaver vor..." << std::endl;
    MenuItemInfoRotator myScreensaverMenu("Screensaver", &myExtension);
    myScreensaverMenu.addSubmenuItem(&myInfoNetwork);
    myScreensaverMenu.addSubmenuItem(&myInfoNtp);
    myScreensaverMenu.addSubmenuItem(&myInfoHardware);

	// Testing of LEDs
    myExtension.setPTTLED(true);
    myExtension.setMasterConnectedLED(true);

	cout << "Screensaver bereit!" << std::endl;


	//auto p = unique_ptr<IProcess>(new ServerProcess(myExtension, 12345));
	//g_pm.add(move(p));

    int count = 0;
    int count2 = 0;
	int countMeasurementsCyclic = 0;
	myExtension.setOutputPower_Watt(4.3);

/*	while (1) {

		// Read ADC Values
		double fwdpwr = myExtension.readFwdPwr();
		double revpwr = myExtension.readRevPwr();
		double voltage = myExtension.readVoltage();
		double current = myExtension.readCurrent();
		::std::cout << ::std::fixed
		<< ::std::setw( 12 ) << voltage << " V  "
		<< ::std::setw( 12 ) << current << " A  "
		<< ::std::setw( 12 ) << fwdpwr << " Fwd W  "
		<< ::std::setw( 12 ) << revpwr << " Rev W  " << "\n";

		usleep(1000*200);
	}
*/
    while (1) {
        // Tastererkennung
        if (!skipDisplaySetup && !myScreensaverMenu.isRunning()) {
            int taster = myExtension.readButton();
            if (taster != TASTERNOTPRESSED) {
                myMainMenu.button(taster);
            }
            usleep(TASTERDELAY_MS * 1000);
        }

        // Spannung und Temperatur aktuell halten
        if (count2 >= 50) {
            myInfoHardware.printMenu();
            count2 = 0;
        }
        count2++;

		myExtension.MakeMeasurementCyclic();

// WEBSOCKETSERVER
		// Read ADC Values
		wsdata wsd;
		wsd.fwdpwr = myExtension.readFwdPwr();
		wsd.revpwr = myExtension.readRevPwr();
		wsd.swr = myExtension.readSWR();
		wsd.voltage = myExtension.readVoltage();
		wsd.current = myExtension.readCurrent();
		wsd.fwdpwrmean = myExtension.readMeanFwdPwr();
		wsd.revpwrmean = myExtension.readMeanRevPwr();
		wsd.swrmean = myExtension.readMeanSWR();

		wsv.SendData(wsd);

// END WEBSOCKETSERVER


		// Keep Measurements for Fwd und Rev Power as well as SWR up-to-date_order
		if (countMeasurementsCyclic >= 20)
		{
			countMeasurementsCyclic = 0;

			// Read ADC Values
			double fwdpwr = myExtension.readFwdPwr();
			double revpwr = myExtension.readRevPwr();
			double swr = myExtension.readSWR();
			double voltage = myExtension.readVoltage();
			double current = myExtension.readCurrent();

			::std::cout << ::std::fixed
			<< "actual: "
			<< ::std::setw( 12 ) << voltage << " V  "
			<< ::std::setw( 12 ) << current << " A  "
			<< ::std::setw( 12 ) << fwdpwr << " Fwd W  "
			<< ::std::setw( 12 ) << revpwr << " Rev W  "
			<< ::std::setw( 12 ) << swr << " SWR" << endl;

			double fwdpwrmean = myExtension.readMeanFwdPwr();
			double revpwrmean = myExtension.readMeanRevPwr();
			double swrmean = myExtension.readMeanSWR();

			::std::cout << ::std::fixed
			<< "Mean:                                   "
			<< ::std::setw( 12 ) << fwdpwrmean << " Fwd W  "
			<< ::std::setw( 12 ) << revpwrmean << " Rev W  "
			<< ::std::setw( 12 ) << swrmean << " SWR" << endl << endl;

// WEBSOCKETSERVER
			// Get Temps and send to browser
			wstemp temp;
			temp.INTemp = myOneWire.readTemp(IN);
			temp.OUTTemp = myOneWire.readTemp(OUT);
			temp.APRSTemp = myOneWire.readTemp(APRS);
			temp.PATemp = myOneWire.readTemp(PA);
			temp.ExtTemp1 = myOneWire.readTemp(TEMPEXT1);
			temp.ExtTemp2 = myOneWire.readTemp(TEMPEXT2);
			temp.ExtTemp3 = myOneWire.readTemp(TEMPEXT3);
			temp.ExtTemp4 = myOneWire.readTemp(TEMPEXT4);

		wsd.SendTemp(temp);

// END WEBSOCKETSERVER

		}
		countMeasurementsCyclic++;

        /*
		// Hauptmenü nach Screensaver-Durchlauf anzeigen
        if (!myScreensaverMenu.isRunning() && myMainMenu.active && count == 0) {
            myMainMenu.printMenu(true);
        }

        // Screensaver aktivieren
        if (count >= 600 && !myScreensaverMenu.isRunning() && myMainMenu.active) {
            myMainMenu.setActive(false);
            myScreensaverMenu.run();
            count = 0;
            myMainMenu.setActive(true);
        } else if (!myScreensaverMenu.isRunning() && myMainMenu.active) {
            count++;
        } else if (myScreensaverMenu.isRunning() || (!myScreensaverMenu.isRunning() && !myMainMenu.active)) {
            count = 0;
        }
		*/
    }

	//g_pm.shutdown();
	//g_pm.wait();

// WEBSOCKETSERVER
	server_thread.join();
// END WEBSOCKETSERVER

    return 0;
}
