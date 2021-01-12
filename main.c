#include <windows.h>

HANDLE setEnv(const char* sframeExecutable) {
	SECURITY_ATTRIBUTES securityAttrib;
	HANDLE event;
	char environVarValue[128];

	// Get SFrame executable name in ptr variable (ex: "SFrame.exe")
	const char *ptr = sframeExecutable + lstrlen(sframeExecutable);

	while(*ptr != '\\' && ptr > sframeExecutable) ptr--;
	if(*ptr == '\\')
		ptr++;

	securityAttrib.bInheritHandle = TRUE;
	securityAttrib.lpSecurityDescriptor = NULL;
	securityAttrib.nLength = sizeof(securityAttrib);

	event = CreateEvent(&securityAttrib, FALSE, FALSE, NULL);

	//set "%s_PARENT"="Launcher.exe"
	//set "%s_RUNNER"=<event> handle in decimal

	SetEnvironmentVariable("sframe.exe_PARENT", "Launcher.exe");

	wsprintf(environVarValue, "%I64u", (unsigned long long)event);
	SetEnvironmentVariable("sframe.exe_RUNNER", environVarValue);

	return event;
}

int APIENTRY WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow)
{
	char** argv =  __argv;
	int argc =  __argc;
	STARTUPINFO si;
	PROCESS_INFORMATION pi;
	char commandLine[1024];
	int i;
	HANDLE event;

	// arguments not used
	(void)hInstance;
	(void)hPrevInstance;
	(void)lpCmdLine;
	(void)nCmdShow;

	if(argc < 2) {
		MessageBox(NULL, "Error: not enough arguments\nUsage: RappelzCmdLauncher.exe SFrame.exe ...", "RappelzCmdLauncher ~ glandu2", MB_OK | MB_ICONERROR);
		return -1;
	}

	ZeroMemory(&si, sizeof(STARTUPINFO));
	si.cb = sizeof(STARTUPINFO);
	ZeroMemory(&pi, sizeof(PROCESS_INFORMATION));

	event = setEnv(argv[1]);

	//Construct SFrame command line
	commandLine[0] = 0;
	for(i=1; i < argc; i++) {
		wsprintf(commandLine, "%s \"%s\"", commandLine, argv[i]);
	}

	if(!CreateProcess(argv[1], commandLine+1, NULL, NULL, TRUE, 0, NULL, NULL, &si, &pi))
		return GetLastError();

	CloseHandle(si.hStdError);
	CloseHandle(si.hStdInput);
	CloseHandle(si.hStdOutput);
	CloseHandle(pi.hProcess);
	CloseHandle(pi.hThread);

	//Wait for SFrame.exe to signal the event before exiting (else the event might be destroyed before SFrame could test its validity via _RUNNER env var)
	WaitForSingleObject(event, 120*1000); //timeout of 2 minutes to avoid hung processes

	return 0;
}
