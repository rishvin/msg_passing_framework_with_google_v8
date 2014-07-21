Message Passing framework with google v8 engine
===============================================

Message passing framework with google's java script engine attached to it for quering.
This is an experiment by me on using google v8.

Features:
  Includes shell for querying(control)
  Includes framework for message passing
  Includes provision to create server and client
  Includes provision to create message
  Includes logging 
  Includes log reader(tracereader)
  Includes test server(master and test client(node1 and node2) 

I Hope you will like it...

Please share your comments or mailto : rishab99999@hotmail.com

I have provided a shell that has been written in google's famous java-scripte engine called V8.

1. Prerequisite:
  Linux or windows (no tested in windows)
  Pthread
  Google v8 libs
  python

2. Building:
  I have tested this in following platform : Ubuntu - x86
  You will need google v8 libs to build. For your reference I have provided my existing v8 libs in the following path:
      modules/control/lib

  Similary you will need to build v8 libs for your platform and replace my libs with your v8 libs.

  You can follow google's v8 site to see how to build the v8 libs
  
  Once you have copied your v8 libs to the project workspace.
  
  Execute the script (This will clean build everything)
  ./build.sh
  
  To build only framework execute
  ./framework.py
  
  To build only modules execute
  ./modules.py
  
  
  Once you have build the workspace you will see a folder called bin created in it:
  it includes following binaries:
  control         This is the shell creted using google v8 
  master          master is the test server created
  node1           node1 is the test client created 
  node2           node2 is the test client created
  tracereader     tracereader is the program which shows you the logs as you perform something
  
3. How to use:
  
  Go to bin folder that has been created
  
  Type:
  ./control
  
  You will see something like this
  >>
  
  
  NOTE: All the command will be treated as function starting with lowercase.
  
  For help type
  help()

4. Examples
  
  1. To spawn a new process, in this case let say master program
     type the following command in the control process shell
  
     spawn("master", "0" "9090")
     
  2. To Send a message to a process type to let say master program
     type the following command in the conrol process shell
     
     send_msg("master", "0", "9090", "This is a test message")
  
  3. To kill a process let say master program which has the pid 4482
     
     kill("4484", "-9")
     
5. Logging
   
   All the activities that you will perform will be logged in.
   You can use the tracereader program that is present in the bin directory to read logs at runtime.
   It will show you the updates when you will perform some activity.
   
  
  
  
  

