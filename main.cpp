#define WIN32_LEAN_AND_MEAN
#include <Windows.h>

#include <cstdio>
#include <cstdlib>

int main()
{
    int returnCode = 0;

    HANDLE hComPort = CreateFile(
        "\\\\.\\COM6",
        (GENERIC_READ | GENERIC_WRITE),
        0,
        NULL,
        OPEN_EXISTING,
        0,
        NULL);

    if (hComPort != INVALID_HANDLE_VALUE)
    {
        DCB serialParams = {};
        serialParams.DCBlength = sizeof(serialParams);

        BOOL result = GetCommState(hComPort, &serialParams);
        if (result)
        {
            serialParams.BaudRate = CBR_9600;
            serialParams.ByteSize = 8;
            serialParams.StopBits = ONESTOPBIT;
            serialParams.Parity = NOPARITY;

            result = SetCommState(hComPort, &serialParams);
        }

        if (result)
        {
            result = SetCommMask(hComPort, EV_RXCHAR);
        }

        int counter = 0;

        while (result)
        {
            const char txCharValue = 'A' + (counter % 26);

            DWORD bytesWritten = 0;
            if (result)
            {
                result = WriteFile(hComPort, &txCharValue, 1, &bytesWritten, NULL);
            }

            if (result)
            {
                DWORD eventMask = 0;
                result = WaitCommEvent(hComPort, &eventMask, NULL);

                if (result)
                {
                    result = (eventMask == EV_RXCHAR);
                }
            }

            char rxCharValue = 0;
            DWORD bytesRead = 0;
            if (result)
            {
                result = ReadFile(hComPort, &rxCharValue, 1, &bytesRead, NULL);
            }

            if (result)
            {
                printf("Sent: \'%c\', Received: \'%c\'\n", txCharValue, rxCharValue);
            }
            else
            {
                printf("Failed to echo serial data\n");
                returnCode = -1;
            }

            ++counter;

            //getchar();
        }

        CloseHandle(hComPort);
    }
    else
    {
        returnCode = -1;
    }

    return returnCode;
}