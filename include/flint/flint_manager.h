#ifndef FLINT_MANAGER_H_
#define FLINT_MANAGER_H_

#include <string>
using namespace std;

/**
 * Main User Apis
 */
class FlintManager {
public:
    void connect();

    void disconnect();

    void reconnect();

    bool isConnected();

    bool isConnecting();

    void setApplicationId(string id);

    string getApplicationId();

    void requestStatus();

    void sendMessage(string namespace, string message); // TODO

    void launchApplication(string url); // TODO

    void launchApplication(string url, bool relaunchIfRunning); // TODO

    void launchApplication(string url, bool relaunchIfRunning, bool useIpc); // TODO

    void joinApplication(sring url); // TODO

    void joinApplication(); // TODO

    void leaveApplication(); // TODO

    void stopApplication();

    void setVolume(double volume);

    double getVolume();

    void setMute(bool mute);

    bool isMute();

    ApplicationMetadata *getApplicationMetadata(); // TODO

    string getApplicationStatus(); // TODO

    void setMessageReceivedCallbacks(); // TODO
    
    void removeMessageReceivedCallbacks(); // TODO
}

#endif // FLINT_MANAGER_H_
