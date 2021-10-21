# keylogger
- By
  - Vighnesh Sivaraman
  - Hamza Alwani

# Introduction
  This is a keylogger application to log all keys punched in from the keyboard and it will write it into a text file
  [data.txt](data.txt).
  This application is designed in C++ for the windows operating system using Visual studio code with the help of the
  windows.h headers.
  The Application was created to study about the functionality of:
  - Windows Registry keys.
  - Using the Windows `windows.h` C++ header API.
  - Understanding the functionality of the `keylogger` application.
  
# Working of the application
  The application works as a background task which is only visible from the task manager and minimises itself in the
  background monitoring all keyboard inputs and logging them in the previously mentioned data file.
  
  It also adds itself to the HKEY_CURRENT_USER `Software\Microsoft\Windows\CurrentVersion\Run` registry keys value to
  run itself on startup. 