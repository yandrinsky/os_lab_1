#include<iostream>
#include<windows.h>
#include<fileapi.h>
#include<string>

using std::cout;
using std::cin;
using std::endl;
using std::string;

void createFile(string, string, int, int);

void calculationValues(HANDLE, HANDLE, int, string, string, int);

void message(string, string, int, DWORD, DWORD, DWORD, DWORD64, DWORD64, int);

void asynchronousFileCopy(DWORD64, HANDLE, HANDLE, char *, char *, DWORD, DWORD64, DWORD, DWORD, string, string, int);

void informationInput();

bool menu();

int callback = 0;
DWORD counterInfo = 0;

VOID CALLBACK FileIOCompletionRoutine(DWORD dwErrorCode, DWORD dwNumberOfBytesTransfered, LPOVERLAPPED lpOverlapped) {
    callback++;
}

void createFile(string input, string output, int factorCluster, int numberOfThreads) {
    HANDLE handleInput, handleOutput;

    handleInput = CreateFileA(
            input.c_str(),
            GENERIC_READ,
            FILE_SHARE_READ,
            NULL,
            OPEN_EXISTING,
            FILE_FLAG_NO_BUFFERING | FILE_FLAG_OVERLAPPED,
            NULL
    );

    handleOutput = CreateFileA(
            output.c_str(),
            GENERIC_WRITE,
            FILE_SHARE_WRITE,
            NULL,
            CREATE_ALWAYS,
            FILE_FLAG_NO_BUFFERING | FILE_FLAG_OVERLAPPED,
            NULL
    );

    calculationValues(handleInput, handleOutput, factorCluster, input, output, numberOfThreads);
}

void calculationValues(
        HANDLE handleInput,
        HANDLE handleOutputint,
        int factorCluster,
        string input,
        string output,
        int numberOfThreads
) {
    char *readingBytes, *pointerToBeginning;
    DWORD block;
    DWORD64 sizeFile, sizeFileOnDisk;
    DWORD low, high, clusterSize = 4096;
    block = factorCluster * clusterSize;
    low = GetFileSize(handleInput, &high);
    sizeFile = ((DWORD64) high << 32) + low;
    sizeFileOnDisk = 0;

    while (sizeFileOnDisk < sizeFile) {
        sizeFileOnDisk += block;
    }

    readingBytes = new char[sizeFileOnDisk + 1];
    pointerToBeginning = readingBytes;

    asynchronousFileCopy(
            sizeFileOnDisk,
            handleInput,
            handleOutputint,
            readingBytes,
            pointerToBeginning,
            block,
            sizeFile,
            high,
            low,
            input,
            output,
            numberOfThreads
    );
}

void message(
        string input,
        string output,
        int numberOfThreads,
        DWORD block,
        DWORD high,
        DWORD low,
        DWORD64 sizeFile,
        DWORD64 sizeFileOnDisk,
        int copyTime
) {
    system("cls");

    char proceed;

    cout << endl << endl << "Summary information after copying the file:" << endl << endl;
    cout << "Path to original file: " << input << endl;
    cout << "Path to copied file: " << output << endl;
    cout << "Number of threads: " << numberOfThreads << endl;
    cout << "block size: " << block << endl;
    cout << "File size low: " << low << endl;
    cout << "File size high: " << high << endl;
    cout << "File size: " << sizeFile << endl;
    cout << "File size on disk: " << sizeFileOnDisk << endl;
    cout << "Copy time: " << copyTime << " ms/" << copyTime / 1000 << " sec" << endl;
    cout << "Enter any key: ";

    cin >> proceed;
}


void readFileOverlapped(
        HANDLE firstHandle,
        DWORD64 fileSize,
        DWORD blockSize,
        int operations,
        OVERLAPPED *overlappeds,
        char **buffer
) {
    DWORD64 writtenBytes = 0;
    DWORD64 highWriten = 0;
    int operations_counter = 0;
    DWORD counterInfoOld = counterInfo;

    for (int i = 0; i < operations && fileSize > counterInfo; i++) {
        operations_counter++;
        ReadFileEx(firstHandle, buffer[i], blockSize, &overlappeds[i], FileIOCompletionRoutine);
        counterInfo += blockSize;
    }

    while (callback < operations_counter) {
        SleepEx(-1, true);
    }

    for (int i = 0; i < operations; i++) {
        writtenBytes = ((DWORD64) overlappeds[i].Offset) + (DWORD64) (blockSize * operations) +
                       ((DWORD64) overlappeds[i].OffsetHigh << 32);
        highWriten = writtenBytes;
        overlappeds[i].OffsetHigh = (highWriten >> 32);

        if (overlappeds[i].OffsetHigh == 0) {
            overlappeds[i].Offset = writtenBytes;
        } else {
            overlappeds[i].Offset = writtenBytes - ((DWORD64) overlappeds[i].OffsetHigh << 32);
        }
    }

    callback = 0;
    counterInfo = counterInfoOld;
}

void writeFileOverlapped(
        HANDLE secondHandle,
        DWORD64 fileSize,
        DWORD blockSize,
        int operations,
        OVERLAPPED *overlappeds,
        char **buffer
) {
    int operations_counter = 0;
    DWORD64 writtenBytes = 0, highWriten = 0;

    for (int i = 0; i < operations && fileSize > counterInfo; i++) {
        operations_counter++;
        WriteFileEx(secondHandle, buffer[i], blockSize, &overlappeds[i], FileIOCompletionRoutine);
        counterInfo += blockSize;
        if (fileSize < (DWORD64) 10) {
            cout << endl << i << endl << fileSize;
        }
    }

    while (callback < operations_counter) {
        SleepEx(-1, true);
    }

    for (int i = 0; i < operations; i++) {
        writtenBytes = ((DWORD64) overlappeds[i].Offset) + (DWORD64) (blockSize * operations) +
                       ((DWORD64) overlappeds[i].OffsetHigh << 32);
        highWriten = writtenBytes;
        overlappeds[i].OffsetHigh = (highWriten >> 32);
        if (overlappeds[i].OffsetHigh == 0) {
            overlappeds[i].Offset = writtenBytes;
        } else {
            overlappeds[i].Offset = writtenBytes - ((DWORD64) overlappeds[i].OffsetHigh << 32);
        }
    }

    callback = 0;
}

void asynchronousFileCopy(
        DWORD64 sizeFileOnDisk,
        HANDLE handleInput,
        HANDLE handleOutputint,
        char *readingBytes,
        char *pointerToBeginning,
        DWORD blockSize,
        DWORD64 sizeFile,
        DWORD high,
        DWORD low,
        string input,
        string output,
        int numberOfThreads
) {
    DWORD64 curSize;
    int test, test_old;

    char **buffer = new char *[numberOfThreads];
    OVERLAPPED *overForRead = new OVERLAPPED[numberOfThreads], *overForWrite = new OVERLAPPED[numberOfThreads];

    for (int i = 0; i < numberOfThreads; i++) {
        buffer[i] = new char[blockSize];
        overForRead[i].OffsetHigh = overForWrite[i].OffsetHigh = ((DWORD64) (i * blockSize) >> 32);

        if (overForWrite[i].OffsetHigh == 0) {
            overForRead[i].Offset = overForWrite[i].Offset = (DWORD64) (i * blockSize);
        } else {
            overForRead[i].Offset = overForWrite[i].Offset =
                    ((DWORD64) (i * blockSize)) - ((DWORD64) overForRead[i].OffsetHigh << 32);
        }
        overForRead[i].hEvent = overForWrite[i].hEvent = NULL;
    }

    curSize = 0;
    int start = GetTickCount();

    do {
        readFileOverlapped(handleInput, sizeFile, blockSize, numberOfThreads, overForRead, buffer);
        writeFileOverlapped(handleOutputint, sizeFile, blockSize, numberOfThreads, overForWrite, buffer);
        curSize += (DWORD64) (blockSize * numberOfThreads);
        test_old = test;
        test = curSize / (sizeFileOnDisk / 100);
        if (test != test_old) cout << endl << test << "%";
    } while (curSize < sizeFileOnDisk);

    int finish = GetTickCount();
    counterInfo = 0;

    long long_high = (long) high;
    SetFilePointer(handleOutputint, sizeFile, &long_high, FILE_BEGIN);
    SetEndOfFile(handleOutputint);
    CloseHandle(handleInput);
    CloseHandle(handleOutputint);
    message(input, output, numberOfThreads, blockSize, high, low, sizeFile, sizeFileOnDisk, finish - start);
}

void informationInput() {
    int factorCluster, numberOfThreads;
    string input, output;
    cout << endl << "Enter file path: ";
    cin >> input;
    cout << endl << "Enter copy file: path";
    cin >> output;
    cout << endl << "Enter number of threads: ";
    cin >> numberOfThreads;
    cout << endl << "Enter the value by which the cluster size will be multiplied: ";
    cin >> factorCluster;
    system("cls");
    cout << endl << "Copying started...";
    createFile(input, output, factorCluster, numberOfThreads);
}

bool menu() {
    int choice;

    system("cls");
    cout << endl << endl << "Options:" << endl;
    cout << "1 - Start asynchronous file copy" << endl;
    cout << "0 - Exit" << endl << endl;
    cout << "Enter: ";
    cin >> choice;

    if (choice) {
        informationInput();
        menu()
    };

    return false;

}


int main() {
    menu();

    return 0;
}
