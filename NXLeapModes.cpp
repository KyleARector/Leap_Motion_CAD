// To those reading this, know that this is an earlier version of my final code, but should be good enough to give you a basis to work from, should you so choose.
// Cheers!
// - Kyle

#include <iostream>
#include "Leap.h"
#include <Windows.h>

using namespace Leap;

// Set checks to ensure buttons not held down when not needed. 
// Saves memory
int check = 0;
int shiftCheck = 0;
int loopCount = 2;
float oldScale = 1.00;
int condSet = 1;
int mode = 0;
int changeTime = 10;

class WindListener : public Listener {
  public:
    virtual void onInit(const Controller&);
    virtual void onConnect(const Controller&);
    virtual void onDisconnect(const Controller&);
    virtual void onExit(const Controller&);
    virtual void onFrame(const Controller&);

  private:
    Frame             lastFrame;
};

void WindListener::onInit(const Controller& controller) {
  std::cout << "Initialized" << std::endl;
}

void WindListener::onConnect(const Controller& controller) {
  std::cout << "Connected" << std::endl;  
  controller.enableGesture(Gesture::TYPE_SCREEN_TAP);
}

void WindListener::onDisconnect(const Controller& controller) {
  std::cout << "Disconnected" << std::endl;
}

void WindListener::onExit(const Controller& controller) {
  std::cout << "Exited" << std::endl;
}

void WindListener::onFrame(const Controller& controller) {
  const Frame frame = controller.frame();

  if (!frame.hands().empty()) {			
	
	const HandList hands = frame.hands();
	const GestureList gestures = frame.gestures();	
	const Pointable pointable = frame.pointables()[0];
	const Screen screen = controller.calibratedScreens()[0];
	
	// Begin Single Hand Code
	if ( hands.count() == 1){

		// Get the first hand
		const Hand hand = frame.hands()[0];

		// Check if the hand has any fingers
		const FingerList fingers = hand.fingers();
		if (!fingers.empty()) {

		

		// Move cursor

		float xCoord = screen.intersect(pointable, true, 1.0F).x * screen.widthPixels();
		float yCoord = screen.heightPixels() - (screen.intersect(pointable, true, 1.0F).y * screen.heightPixels());

		SetCursorPos(xCoord, yCoord);

		// End move cursor 

		std::cout << "Mode: " << mode << " Change Time: " << changeTime << std::endl;

		}// End if empty fingers
	 }// End Single Hand Code

	 // Begin Double Hand Code
	 else if ( hands.count() == 2){

		 // If the middle button and the shift key are both held down...
		 
			//Middle mouse button
			INPUT ipm;
			ipm.type = INPUT_MOUSE;
			ipm.mi.time = 0;
			ipm.mi.dwExtraInfo = 0;


			ipm.mi.dwFlags = MOUSEEVENTF_MIDDLEUP;
			SendInput(1, &ipm, sizeof(INPUT));

			// Shift Key
			INPUT ipk;
			ipk.type = INPUT_KEYBOARD;
			ipk.ki.wScan = 0;
			ipk.ki.time = 0;
			ipk.ki.dwExtraInfo = 0;

			ipk.ki.wVk = 0x10;
			ipk.ki.dwFlags = KEYEVENTF_KEYUP;
			SendInput(1, &ipk, sizeof(INPUT));

			check = 0;
			shiftCheck = 0;
			// Let them up and reset the condition check	

		// Compares scale factor to previous used frame
		float scaleFactor = frame.scaleFactor(lastFrame);
		
		float xCoord = screen.widthPixels()/2;
		float yCoord = screen.heightPixels()/2;				

		SetCursorPos(xCoord, yCoord);

		// Prevents too fast zooming
		if ( loopCount == 2 ){ 

			// If scale factor large or small enough, begin scale
			if ((scaleFactor < 0.9500) || (scaleFactor > 1.05)){

				// If smaller, shrink
				if ( ( scaleFactor-oldScale) < 0 ){
					INPUT ip;
					ip.type = INPUT_MOUSE;
					ip.mi.time = 0;
					ip.mi.mouseData = 120;


					ip.mi.dwFlags = MOUSEEVENTF_WHEEL;
					SendInput(1, &ip, sizeof(INPUT));
				}	

				// If larger, expand
				else if ((scaleFactor-oldScale) > 0){
					INPUT ip;
					ip.type = INPUT_MOUSE;
					ip.mi.time = 0;
					ip.mi.mouseData = -120;


					ip.mi.dwFlags = MOUSEEVENTF_WHEEL;
					SendInput(1, &ip, sizeof(INPUT));
				}
			}

			loopCount = 0;

			lastFrame = frame;
			
		}
		else {
			loopCount++;
		}
		
	 }// End Double Hand Code
  }// End if empty hands
}// End object

int main() {
  // Create a sample listener and controller
  WindListener listener;
  Controller controller;

  // Have the sample listener receive events from the controller
  controller.addListener(listener);

  // Keep this process running until Enter is pressed
  std::cout << "Press Enter to quit..." << std::endl;
  std::cin.get();

  // Remove the sample listener when done
  controller.removeListener(listener);

  return 0;
}
