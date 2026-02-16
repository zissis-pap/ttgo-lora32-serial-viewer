const uint8_t bluetooth_greeting[] ="\
WELCOME TO ESP SERIAL VIEWER\n\
Software version: 0.19.0\n------------------------------------------------------------\n\n\
Enter 'esp help' to see all available options";

const uint8_t help_message[] = "\
ESP SERIAL VIEWER MANUAL\n\n\
------ ESP EXECUTION COMMANDS ------\n\n\
* esp read: enables reading the uart\n\
* esp stop: stops reading the uart\n\
* esp list: lists the contents of the SD\n\
* esp save <file>: starts reading and saves to the specified file\n\ 
* esp open <file>: opens the specified file\n\
* esp delt <file>: deletes the specified file\n\n\
------ ESP SETTING COMMANDS ------\n\n\
* esp set baud: opens UART configuration wizard";