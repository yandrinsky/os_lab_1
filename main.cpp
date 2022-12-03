#include <iostream>
#include <fileapi.h>
#include <windows.h>
#include <timezoneapi.h>

using namespace std;

bool menu();

int getDisksAmount();

string *getSystemDisksList();

void logSystemDisks();

void logSystemDisksDialog();

string convertApiDriveType(int);

void logDiskInfo(string *);

void logDiskInfoDialog();

void dispatcher(int);

void createDirectoryDialog();

void removeDirectoryDialog();

void createOrRemoveDirectoryDialog();

void createFileDialog();

void copyFileDialog();

void moveFileDialog();

void convertAndLogApiFileAttribute(int);

int convertUserFileAttributeToApi(int);

void logFileAttributesDialog();

void logPossibleFileAttributes();

void setFileAttributesDialog();

void logSystemLocOfFile(FILETIME);

void logFileInformationByHandleDialog();

void setFileTimeDialog();

void continueOnKeyEnter();

//константы описывающие системные флаги диска
DWORD ConstSystemFlag[26] =
        {
                FILE_CASE_SENSITIVE_SEARCH,
                FILE_CASE_PRESERVED_NAMES,
                FILE_UNICODE_ON_DISK,
                FILE_PERSISTENT_ACLS,
                FILE_FILE_COMPRESSION,
                FILE_VOLUME_QUOTAS,
                FILE_SUPPORTS_SPARSE_FILES,
                FILE_SUPPORTS_REPARSE_POINTS,
                FILE_SUPPORTS_REMOTE_STORAGE,
                FILE_VOLUME_IS_COMPRESSED,
                FILE_SUPPORTS_OBJECT_IDS,
                FILE_SUPPORTS_ENCRYPTION,
                FILE_NAMED_STREAMS,
                FILE_READ_ONLY_VOLUME,
                FILE_SEQUENTIAL_WRITE_ONCE,
                FILE_SUPPORTS_TRANSACTIONS,
                FILE_SUPPORTS_HARD_LINKS,
                FILE_SUPPORTS_EXTENDED_ATTRIBUTES,
                FILE_SUPPORTS_OPEN_BY_FILE_ID,
                FILE_SUPPORTS_USN_JOURNAL,
                FILE_SUPPORTS_INTEGRITY_STREAMS,
                512,
                67108864,
                134217728,
                268435456,
                536870912,
        };

//типы системных флагов диска
string SystemFlags[26] =
        {
                "FILE_CASE_SENSITIVE_SEARCH - The specified volume supports case-sensitive file names.",
                "FILE_CASE_PRESERVED_NAMES - The specified volume supports preserved case of file names when it places a name on disk. ",
                "FILE_UNICODE_ON_DISK - The specified volume supports Unicode in file names as they appear on disk. ",
                "FILE_FILE_PERSISTENT_ACLS - The specified volume preserves and enforces access control lists (ACL).\n   For example, the NTFS file system preserves and enforces ACLs, and the FAT file system does not. ",
                "FILE_FILE_COMPRESSION - The specified volume supports file-based compression. ",
                "FILE_VOLUME_QUOTAS - The specified volume supports disk quotas. ",
                "FILE_SUPPORTS_SPARSE_FILES - The specified volume supports sparse files. ",
                "FILE_SUPPORTS_REPARSE_POINTS - The specified volume supports reparse points.",
                "FILE_SUPPORTS_REMOTE_STORAGE - no description",
                "FILE_VOLUME_IS_COMPRESSED - The specified volume is a compressed volume, for example, a DoubleSpace volume. ",
                "FILE_SUPPORTS_OBJECT_IDS - The specified volume supports object identifiers.",
                "FILE_SUPPORTS_ENCRYPTION - The specified volume supports the Encrypted File System (EFS). For more information, see File Encryption. ",
                "FILE_NAMED_STREAMS - The specified volume supports named streams. ",
                "FILE_READ_ONLY_VOLUME - The specified volume is read-only. ",
                "FILE_SEQUENTIAL_WRITE_ONCE - The specified volume supports a single sequential write. ",
                "FILE_SUPPORTS_TRANSACTIONS - The specified volume supports transactions. For more information, see About KTM. ",
                "FILE_SUPPORTS_HARD_LINKS - The specified volume supports hard links. For more information, see Hard Links and Junctions. ",
                "FILE_SUPPORTS_EXTENDED_ATTRIBUTES - The specified volume supports extended attributes.\n         An extended attribute is a piece of application-specific metadata that an application can associate with a file and is not part of the file's data. ",
                "FILE_SUPPORTS_OPEN_BY_FILE_ID - The file system supports open by FileID. For more information, see FILE_ID_BOTH_DIR_INFO. ",
                "FILE_SUPPORTS_USN_JOURNAL - The specified volume supports update sequence number (USN) journals. For more information, see Change Journal Records. ",
                "FILE_SUPPORTS_INTEGRITY_STREAMS - no description",
                "FILE_SUPPORTS_GHOSTING - no description",
                "FILE_SUPPORTS_POSIX_UNLINK_RENAME - no description",
                "FILE_SUPPORTS_BLOCK_REFCOUNTING - The specified volume supports sharing logical clusters between files on the same volume.",
                "FILE_SUPPORTS_SPARSE_VDL - no description",
                "FILE_DAX_VOLUME - The specified volume is a direct access (DAX) volume.",
        };

void continueOnKeyEnter() {
    char mainPage;

    cout << endl << "Enter any key to continue: ";
    cin >> mainPage;
}

int getDisksAmount() {
    DWORD dwMyDisks = GetLogicalDrives();
    int counter = 1;
    int disksAmount = 0;
    bool flag = true;

    while (flag) {
        if (dwMyDisks >= counter) {
            counter = counter * 2;
        } else {
            counter /= 2;
            disksAmount += 1;
            dwMyDisks = dwMyDisks - counter;
            counter = 1;
        }

        if (dwMyDisks == 0) {
            flag = false;
        }
    }

    return disksAmount;
}

string *getSystemDisksList() {
    string allDisk[] = {"A:\\", "B:\\", "C:\\", "D:\\", "E:\\",
                        "F:\\", "G:\\", "H:\\", "I:\\", "J:\\", "K:\\",
                        "L:\\","M:\\", "N:\\", "O:\\", "P:\\", "Q:\\",
                        "R:\\", "S:\\", "T:\\", "U:\\", "V:\\", "W:\\",
                        "X:\\","Y:\\", "Z:\\"};

    int numbersDisk = getDisksAmount();
    string *disksInSystem = new string[numbersDisk];
    DWORD dwMyDisks = GetLogicalDrives();

    int counter = 1;
    int degree = 0;
    int foundCounter = 0;
    bool flag = true;

    while (flag) {
        if (dwMyDisks >= counter) {
            counter = counter * 2;
            degree = degree + 1;
        } else {
            counter = counter / 2;
            degree = degree - 1;

            disksInSystem[numbersDisk - 1 - foundCounter] = allDisk[degree];
            foundCounter += 1;
            dwMyDisks = dwMyDisks - counter;
            counter = 1;
            degree = 0;
        }

        if (dwMyDisks == 0) {
            flag = false;
        }
    }

    return disksInSystem;
}

void logSystemDisks() {
    string *disks = getSystemDisksList();
    int disksAmount = getDisksAmount();

    for (int i = 0; i < disksAmount; i++) {
        cout << i << ". " << disks[i] << endl;
    }
}

void logSystemDisksDialog() {
    system("cls");
    cout << endl << "There are disks on your computer: " << endl;

    logSystemDisks();

    continueOnKeyEnter();
}

string convertApiDriveType(int type) {
    switch (type) {
        case 0:
            return "The drive type cannot be determined.";
        case 1:
            return "The root path is invalid; for example, there is no volume mounted at the specified path.";
        case 2:
            return "The drive has removable media; for example, a floppy drive, thumb drive, or flash card reader.";
        case 3:
            return "The drive has fixed media; for example, a hard disk drive or flash drive.";
        case 4:
            return "The drive is a remote (network) drive.";
        case 5:
            return "The drive is a CD-ROM drive.";
        case 6:
            return "The drive is a RAM disk.";
        default:
            return "Error: drive type cannot be converted.";
    }
}

void logDiskInfo(string *disk = new string[1]) {
    char NameBuffer[MAX_PATH];
    char SysNameBuffer[MAX_PATH];
    DWORD VSNumber;
    DWORD MCLength;
    DWORD FileSF;
    DWORD SectCluster;
    DWORD ByteSec;
    DWORD NumFreeCluster;
    DWORD TotalNumCluster;

    LPCSTR Disk = disk[0].c_str();

    GetVolumeInformationA(
            Disk,
            NameBuffer, sizeof(NameBuffer),
            &VSNumber, &MCLength,
            &FileSF,SysNameBuffer,sizeof(SysNameBuffer));

    GetDiskFreeSpaceA(Disk, &SectCluster,
                      &ByteSec,&NumFreeCluster, &TotalNumCluster);

    cout << convertApiDriveType(GetDriveTypeA(Disk)) << endl;

    cout << endl << "Volume Name Buffer: " << NameBuffer << endl;
    cout << "Volume Serial Number: " << VSNumber << endl;
    cout << "Maximum Component Length: " << MCLength << endl;
    cout << "File System Flags: " << FileSF << endl << endl;

    for (int i = 0; i < 26; i++) {
        if (ConstSystemFlag[i] & FileSF) {
            cout << ConstSystemFlag[i] << "  " << SystemFlags[i] << endl;
        }
    }

    cout << endl << "File System Name Buffer: " << SysNameBuffer << endl << endl;
    cout << "Sectors Per Cluster: " << SectCluster << endl;
    cout << "Bytes Per Sector: " << ByteSec << endl;
    cout << "Number Of Free Clusters: " << NumFreeCluster << endl;
    cout << "Total Number Of Clusters: " << TotalNumCluster << endl;

    int FreeSpace = NumFreeCluster / 1024 * ByteSec / 1024 * SectCluster / 1024;
    int TotalSpace = TotalNumCluster / 1024 * ByteSec / 1024 * SectCluster / 1024;

    cout << "Free space in disk: " << FreeSpace << " gigabytes" << endl;
    cout << "Total space in disk: " << TotalSpace << " gigabytes" << endl;
}

void createOrRemoveDirectoryDialog() {
    system("cls");

    int userChoice;


    cout << "1. Create directory" << endl;
    cout << "2. Remove directory" << endl;
    cout << "0. Go to main page" << endl << endl;
    cin >> userChoice;

    if (!userChoice) {
        menu();
    } else if (userChoice == 1) {
        createDirectoryDialog();
    } else if (userChoice == 2) {
        removeDirectoryDialog();
    } else {
        cout << "Bad input!" << endl;

        continueOnKeyEnter();
    }
}

void createDirectoryDialog() {
    system("cls");
    cout << "Create new directory:" << endl;

    string *userPath = new string[1];

    cout << "Path for new directory: ";
    cin >> userPath[0];

    LPCSTR DirectPath = userPath[0].c_str();
    BOOL isCreated = CreateDirectoryA(DirectPath, NULL);

    if (isCreated) {
        cout << "The directory was created successfully!" << endl;
    } else {
        cout << "An error occurred while creating the directory!" << endl;
    }

    continueOnKeyEnter();
}

void removeDirectoryDialog() {
    system("cls");
    cout << "Remove directory: " << endl;

    string *userPath = new string[1];

    cout << "Path of remove a directory: ";
    cin >> userPath[0];

    LPCSTR DirectPath = userPath[0].c_str();
    BOOL isDeleted = RemoveDirectoryA(DirectPath);

    if (isDeleted) {
        cout << "The directory was removed" << endl;
    } else {
        cout << "An error occurred while removing the directory" << endl;
    }

    continueOnKeyEnter();
}

void createFileDialog() {
    system("cls");
    cout << "Create new file:" << endl;

    string *userPath = new string[1];

    cout << "File path: ";
    cin >> userPath[0];

    LPCSTR DirectPath = userPath[0].c_str();

    CreateFileA(DirectPath,
                GENERIC_READ | GENERIC_WRITE,
                FILE_SHARE_DELETE,
                NULL,
                CREATE_ALWAYS,
                FILE_ATTRIBUTE_NORMAL,
                NULL);

    continueOnKeyEnter();
}

void copyFileDialog() {
    system("cls");
    cout << "Copy file:" << endl;

    string *userPath = new string[2];

    cout << "Pld file path: ";
    cin >> userPath[0];

    LPCSTR directPath = userPath[0].c_str();

    cout << endl << "New file path: ";
    cin >> userPath[1];

    LPCSTR newPath = userPath[1].c_str();
    BOOL isCopied = CopyFileA(directPath, newPath, FALSE);

    if (isCopied) {
        cout << endl << "File was successfully copied!" << endl;
    } else {
        cout << "An error occurred while copy the file!";
    }

    continueOnKeyEnter();
}

void moveFileDialog() {
    system("cls");

    cout << "Move file:" << endl;

    string *userPath = new string[2];

    cout << "Old file path: ";
    cin >> userPath[0];

    LPCSTR DirectPath = userPath[0].c_str();

    cout << endl << "New file path: ";
    cin >> userPath[1];

    LPCSTR fileNameToMove = userPath[1].c_str();
    BOOL isMoved = MoveFileExA(DirectPath, fileNameToMove, MOVEFILE_COPY_ALLOWED);

    if (isMoved) {
        cout << endl << "File was successfully moved!" << endl;
    } else {
        cout << "Such file already exist!";
    }

    continueOnKeyEnter();
}


void logDiskInfoDialog() {
    int userChoice;

    system("cls");
    cout << "Select disk: " << endl << endl;

    logSystemDisks();

    cout << getDisksAmount() << ". " << "To the main page" << endl;
    cin >> userChoice;

    bool isChoiceCorrect = userChoice < getDisksAmount() && userChoice > -1;

    if (isChoiceCorrect) {
        system("cls");

        string userChoiceDisk[] = {getSystemDisksList()[userChoice]};
        cout << endl << userChoiceDisk[0] << endl;

        logDiskInfo(userChoiceDisk);
    } else if (userChoice == getDisksAmount()) {
        menu();
    } else {
        cout << "wrong input!" << endl;
    }

    continueOnKeyEnter();
}

void convertAndLogApiFileAttribute(int type) {
    switch (type) {
        case 1:
            cout << "FILE_ATTRIBUTE_READONLY" << endl << endl;
            cout << " A file that is read-only. Applications can read the file, but cannot write to it or delete it."
                 << endl;
            break;
        case 2:
            cout << "FILE_ATTRIBUTE_HIDDEN" << endl << endl;
            cout << "The file or directory is hidden. It is not included in an ordinary directory listing." << endl;
            break;
            break;
        case 4:
            cout << "FILE_ATTRIBUTE_SYSTEM" << endl << endl;
            cout << "A file or directory that the operating system uses a part of, or uses exclusively." << endl;
            break;
        case 16:
            cout << "FILE_ATTRIBUTE_DIRECTORY" << endl << endl;
            cout << "The handle that identifies a directory." << endl;
            break;
        case 32:
            cout << "FILE_ATTRIBUTE_ARCHIVE" << endl << endl;
            cout << "A file or directory that is an archive file or directory." << endl;
            break;
        case 64:
            cout << "FILE_ATTRIBUTE_DEVICE" << endl << endl;
            cout << "This value is reserved for system use." << endl;
            break;
        case 128:
            cout << "FILE_ATTRIBUTE_NORMAL" << endl << endl;
            cout << "A file that does not have other attributes set. This attribute is valid only when used alone."
                 << endl;
            break;
        case 256:
            cout << "FILE_ATTRIBUTE_TEMPORARY" << endl << endl;
            cout << "A file that is being used for temporary storage." << endl;
            break;
        case 512:
            cout << "FILE_ATTRIBUTE_SPARSE_FILE" << endl << endl;
            cout << "A file that is a sparse file." << endl;
        case 1024:
            cout << "FILE_ATTRIBUTE_REPARSE_POINT" << endl << endl;
            cout << "A file or directory that has an associated reparse point, or a file that is a symbolic link."
                 << endl;
            break;
        case 2048:
            cout << "FILE_ATTRIBUTE_COMPRESSED" << endl << endl;
            cout << "A file or directory that is compressed. For a file, all of the data in the file is compressed."
                 << endl;
            cout << "For a directory, compression is the default for newly created files and subdirectories." << endl;
            break;
        case 4096:
            cout << "FILE_ATTRIBUTE_OFFLINE" << endl << endl;
            cout << "The data of a file is not available immediately." << endl;
            cout << "This attribute indicates that the file data is physically moved to offline storage." << endl;
            break;
        case 8192:
            cout << "FILE_ATTRIBUTE_NOT_CONTENT_INDEXED" << endl << endl;
            cout << "The file or directory is not to be indexed by the content indexing service." << endl;
            break;
        case 16384:
            cout << "FILE_ATTRIBUTE_ENCRYPTED" << endl << endl;
            cout << "A file or directory that is encrypted. For a file, all data streams in the file are encrypted."
                 << endl;
            cout << "For a directory, encryption is the default for newly created files and subdirectories." << endl;
            break;
        case 32768:
            cout << "FILE_ATTRIBUTE_INTEGRITY_STREAM" << endl << endl;
            cout << "The directory or user data stream is configured with integrity (only supported on ReFS volumes)."
                 << endl;
            cout << "It is not included in an ordinary directory listing." << endl;
            break;
        case 65536:
            cout << "FILE_ATTRIBUTE_VIRTUAL" << endl << endl;
            cout << "This value is reserved for system use." << endl;
            break;
        case 131072:
            cout << "FILE_ATTRIBUTE_NO_SCRUB_DATA" << endl << endl;
            cout << "The user data stream not to be read by the background data integrity scanner (AKA scrubber)."
                 << endl;
            break;
        case 262144:
            cout << "FILE_ATTRIBUTE_RECALL_ON_OPEN" << endl << endl;
            cout << "The file or directory has no physical representation on the local system." << endl;
            break;
        case 524288:
            cout << "FILE_ATTRIBUTE_PINNED" << endl << endl;
            cout << "This attribute indicates user intent that the file or directory should be kept fully present locally"
                    << endl;
            cout << "even when not being actively accessed." << endl;
            break;
        case 1048576:
            cout << "FILE_ATTRIBUTE_UNPINNED" << endl << endl;
            cout << "This attribute indicates that the file or directory should not be kept fully present locally"
                 << endl;
            cout << "except when being actively accessed." << endl;
            break;
        case 4194304:
            cout << "FILE_ATTRIBUTE_RECALL_ON_DATA_ACCESS" << endl << endl;
            cout << "The file or directory is not fully present locally." << endl;
            break;
        default:
            cout << "File attribute cannot be converted" << endl;
            break;
    }
}

void logPossibleFileAttributes() {
    cout << endl << "Information about attributes:" << endl;
    cout << "1. " << "FILE_ATTRIBUTE_READONLY" << endl;
    cout << " A file that is read-only. Applications can read the file, but cannot write to it or delete it." << endl
         << endl;

    cout << "2. " << "FILE_ATTRIBUTE_HIDDEN" << endl;
    cout << "The file or directory is hidden. It is not included in an ordinary directory listing." << endl << endl;

    cout << "3. " << "FILE_ATTRIBUTE_SYSTEM" << endl;
    cout << "A file or directory that the operating system uses a part of, or uses exclusively." << endl << endl;

    cout << endl << "4. " << "FILE_ATTRIBUTE_ARCHIVE" << endl;
    cout << "A file or directory that is an archive file or directory." << endl << endl;

    cout << "5. " << "FILE_ATTRIBUTE_NORMAL" << endl;
    cout << "A file that does not have other attributes set. This attribute is valid only when used alone." << endl
         << endl;

    cout << "6. " << "FILE_ATTRIBUTE_TEMPORARY" << endl;
    cout << "A file that is being used for temporary storage." << endl << endl;

    cout << "7. " << "FILE_ATTRIBUTE_OFFLINE" << endl;
    cout << "The data of a file is not available immediately." << endl;
    cout << "This attribute indicates that the file data is physically moved to offline storage." << endl << endl;

    cout << "8. " << "FILE_ATTRIBUTE_NOT_CONTENT_INDEXED" << endl;
    cout << "The file or directory is not to be indexed by the content indexing service." << endl << endl;
}

int convertUserFileAttributeToApi(int type) {
    int key;
    
    switch (type) {
        case 1:
            key = 1;
            break;
        case 2:
            key = 2;
            break;
        case 3:
            key = 4;
            break;
        case 4:
            key = 32;
            break;
        case 5:
            key = 128;
            break;
        case 6:
            key = 256;
            break;
        case 7:
            key = 4096;
            break;
        case 8:
            key = 8192;
            break;
        default:
            break;
    }

    return key;
}

void logFileAttributesDialog() {
    string *userPath = new string[1];

    system("cls");
    cout << "Get file attributes:" << endl;
    cout << "Path: ";
    cin >> userPath[0];

    LPCSTR DirectPath = userPath[0].c_str();

    cout << endl << "Information about file attributes:" << endl;
    cout << endl;

    convertAndLogApiFileAttribute(GetFileAttributesA(DirectPath));

    continueOnKeyEnter();
}

void setFileAttributesDialog() {
    string *userPath = new string[1];
    int chooseParam;

    system("cls");
    cout << "Set file attributes:" << endl;
    cout << "File path: ";
    cin >> userPath[0];

    LPCSTR DirectPath = userPath[0].c_str();
    logPossibleFileAttributes();

    cout << "Choose attribute:" << endl;
    cin >> chooseParam;
    
    DWORD checkSet = SetFileAttributesA(DirectPath, convertUserFileAttributeToApi(chooseParam));

    if (checkSet) {
        cout << endl << "Attributes were successfully set!" << endl;
    } else {
        cout << "An error occurred!";
    }

    continueOnKeyEnter();
}

void logSystemLocOfFile(FILETIME fileTime) {
    SYSTEMTIME lpSystemTime;
    SYSTEMTIME LocalSystemTime;
    FileTimeToSystemTime(&fileTime, &lpSystemTime);
    SystemTimeToTzSpecificLocalTime(NULL, &lpSystemTime, &LocalSystemTime);

    cout << endl;
    cout << "       Year: " << LocalSystemTime.wYear << endl;
    cout << "       Month: " << LocalSystemTime.wMonth << endl;
    cout << "       Day of week: " << LocalSystemTime.wDayOfWeek << endl;
    cout << "       Day: " << LocalSystemTime.wDay << endl;
    cout << "       Hour: " << LocalSystemTime.wHour << endl;
    cout << "       Minute: " << LocalSystemTime.wMinute << endl;
    cout << "       Second: " << LocalSystemTime.wSecond << endl;
    cout << "       Millisecond: " << LocalSystemTime.wMilliseconds << endl;
    cout << endl;
}

FILETIME convertSysToFile(SYSTEMTIME SysTime) {
    FILETIME FileTime;
    BOOL time = SystemTimeToFileTime(&SysTime, &FileTime);
    cout << time;

    return FileTime;
}

void logFileInformationByHandleDialog() {
    string *userPath = new string[1];

    system("cls");
    cout << "Get file information by handle:" << endl;
    cout << "File path: ";
    cin >> userPath[0];

    LPCSTR DirectPath = userPath[0].c_str();
    HANDLE file = CreateFileA(
            DirectPath,
            GENERIC_READ | GENERIC_WRITE,
            FILE_SHARE_DELETE,
            NULL,
            OPEN_EXISTING,
            FILE_ATTRIBUTE_NORMAL,
            NULL);
    BY_HANDLE_FILE_INFORMATION lpFileInfo;
    BOOL isGotInfo = GetFileInformationByHandle(file, &lpFileInfo);

    if (isGotInfo) {
        cout << endl << "Information by handle: " << endl;
        cout << "FileAttributes: " << lpFileInfo.dwFileAttributes << endl;
        cout << "CreationTime: " << endl;

        logSystemLocOfFile(lpFileInfo.ftCreationTime);

        cout << "LastAccessTime: " << endl;

        logSystemLocOfFile(lpFileInfo.ftLastAccessTime);

        cout << "LastWriteTime: " << endl;

        logSystemLocOfFile(lpFileInfo.ftLastWriteTime);

        cout << "VolumeSerialNumber: " << lpFileInfo.dwVolumeSerialNumber << endl;
        cout << "FileSizeHigh: " << lpFileInfo.nFileSizeHigh << endl;
        cout << "FileSizeLow: " << lpFileInfo.nFileSizeLow << endl;
        cout << "NumberOfLinks: " << lpFileInfo.nNumberOfLinks << endl;
        cout << "FileIndexHigh: " << lpFileInfo.nFileIndexHigh << endl;
        cout << "FileIndexLow: " << lpFileInfo.nFileIndexLow << endl;
    } else {
        cout << "An error occurred!";
        cout << GetLastError();
    }

    CloseHandle(file);

    continueOnKeyEnter();
}

void setFileTimeDialog() {
    SYSTEMTIME LocalCreateTime;
    SYSTEMTIME LocalAccessTime;
    SYSTEMTIME LocalWriteTime;
    SYSTEMTIME NewCreateTime;
    SYSTEMTIME NewAccessTime;
    SYSTEMTIME NewWriteTime;

    string *userPath = new string[1];

    cout << "The path: ";
    cin >> userPath[0];

    LPCSTR DirectPath = userPath[0].c_str();
    HANDLE file = CreateFileA(DirectPath, GENERIC_READ | GENERIC_WRITE,
                              FILE_SHARE_DELETE, NULL, OPEN_EXISTING,
                              FILE_ATTRIBUTE_NORMAL, NULL);

    cout << endl
         << "Enter creation time (day, month, year, hour, minute, second, millisecond). Separate with space."
         << endl;
    cin >> LocalCreateTime.wDay >> LocalCreateTime.wMonth >> LocalCreateTime.wYear >> LocalCreateTime.wHour
        >> LocalCreateTime.wMinute >> LocalCreateTime.wSecond >> LocalCreateTime.wMilliseconds;

    BOOL timeCreation = TzSpecificLocalTimeToSystemTime(NULL, &LocalCreateTime, &NewCreateTime);
    FILETIME FileCreateTime = convertSysToFile(NewCreateTime);

    cout << endl
         << "Enter access time (day, month, year, hour, minute, second, millisecond). Separate with space."
         << endl;
    cin >> LocalAccessTime.wDay >> LocalAccessTime.wMonth >> LocalAccessTime.wYear >> LocalAccessTime.wHour
        >> LocalAccessTime.wMinute >> LocalAccessTime.wSecond >> LocalAccessTime.wMilliseconds;

    BOOL timeAccess = TzSpecificLocalTimeToSystemTime(NULL, &LocalAccessTime, &NewAccessTime);
    FILETIME FileAccessTime = convertSysToFile(NewAccessTime);

    cout << endl
         << "Enter write time (day, month, year, hour, minute, second, millisecond). Separate with space."
         << endl;
    cin >> LocalWriteTime.wDay >> LocalWriteTime.wMonth >> LocalWriteTime.wYear >> LocalWriteTime.wHour
        >> LocalWriteTime.wMinute >> LocalWriteTime.wSecond >> LocalWriteTime.wMilliseconds;

    BOOL timeWrite = TzSpecificLocalTimeToSystemTime(NULL, &LocalWriteTime, &NewWriteTime);
    FILETIME FileWriteTime = convertSysToFile(NewWriteTime);

    BOOL finalSetFileTime = SetFileTime(file, &FileCreateTime, &FileAccessTime, &FileWriteTime);
    cout << timeCreation << timeAccess << timeWrite << finalSetFileTime;

    continueOnKeyEnter();
}

void dispatcher(int choiceUser) {
    system("cls");

    switch (choiceUser) {
        case 1:
            logSystemDisksDialog();
            break;
        case 2:
            logDiskInfoDialog();
            break;
        case 3:
            createOrRemoveDirectoryDialog();
            break;
        case 4:
            createFileDialog();
            break;
        case 5:
            copyFileDialog();
            break;
        case 6:
            moveFileDialog();
            break;
        case 7:
            logFileAttributesDialog();
            break;
        case 8:
            setFileAttributesDialog();
            break;
        case 9:
            logFileInformationByHandleDialog();
            break;
        case 10:
            setFileTimeDialog();
            break;
        default:
            break;
    }
}

bool menu() {
    int choiceUser;

    system("cls");

    cout << endl << "Select an action:" << endl << endl;
    cout << "1.  -  Disks" << endl;
    cout << "2.  -  Disks information" << endl;
    cout << "3.  -  Create or delete a directory" << endl;
    cout << "4.  -  Create a new file" << endl;
    cout << "5.  -  Copy file" << endl;
    cout << "6.  -  Move file" << endl;
    cout << "7.  -  Get file attributes" << endl;
    cout << "8.  -  Set file attributes" << endl;
    cout << "9.  -  Get File Information by handle" << endl;
    cout << "10. -  Set File Time" << endl;
    cout << "0.  -  Exit" << endl << endl;

    cin >> choiceUser;

    if (!choiceUser) {
        return false;
    }

    dispatcher(choiceUser);
    menu();

    return true;
}

int main() {
    menu();
}
