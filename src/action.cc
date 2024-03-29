/*This file is for initializing the primary generator */

#include "action.hh"
/* The constructor and destructor*/
MyActionInitialization::MyActionInitialization()
{}

MyActionInitialization::~MyActionInitialization()
{}

/*function to create an instance of the primary generator and initialize it*/

void MyActionInitialization::Build() const
{
	MyPrimaryGenerator *generator =new MyPrimaryGenerator();
	SetUserAction(generator);
	
	MyRunAction *runAction = new MyRunAction();
	SetUserAction(runAction);
	
	MyEventAction *eventAction = new MyEventAction(runAction);
	SetUserAction(eventAction);
	
	MySteppingAction *steppingAction = new MySteppingAction(eventAction);
	SetUserAction(steppingAction);
}
