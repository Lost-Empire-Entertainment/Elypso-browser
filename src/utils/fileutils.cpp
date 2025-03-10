//Copyright(C) 2025 Lost Empire Entertainment
//This program comes with ABSOLUTELY NO WARRANTY.
//This is free software, and you are welcome to redistribute it under certain conditions.
//Read LICENSE.md for more information.

#ifdef _WIN32
#include <Windows.h>
#elif __linux__
#include <unistd.h>
#include <sys/wait.h>
#include <cstring>
#endif
#include <iostream>

//browser
#include "fileutils.hpp"

using std::runtime_error;
using std::exception;
using std::wstring;
using std::wcout;
using std::cout;
using std::filesystem::exists;
using std::filesystem::directory_iterator;
using std::filesystem::copy_options;
using std::to_string;

namespace Utils
{
    string File::GetOutputFromBatFile(const char* file)
    {
#ifdef _WIN32
        char buffer[128];
        string result = "";
        string command = "\"" + string(file) + "\"";
        FILE* pipe = _popen(command.c_str(), "r");

        if (!pipe) throw runtime_error("popen() failed!");

        try
        {
            while (fgets(buffer, sizeof(buffer), pipe) != nullptr)
            {
                result += buffer;
            }
        }
        catch (...)
        {
            _pclose(pipe);
            throw;
        }

        _pclose(pipe);

        return result;
#elif __linux__
        char buffer[128];
        string result = "";
        string command = "\"" + string(file) + "\"";
        FILE* pipe = popen(command.c_str(), "r");

        if (!pipe) throw runtime_error("popen() failed!");

        try
        {
            while (fgets(buffer, sizeof(buffer), pipe) != nullptr)
            {
                result += buffer;
            }
        }
        catch (...)
        {
            pclose(pipe);
            throw;
        }

        pclose(pipe);

        return result;
#endif
    }

    int File::RunBatFile(const string& file, bool runSeparate, BatType type)
    {
        string command = runSeparate == true
            ? "start cmd /c \"" + file + "\""
            : "\"" + file + "\"";

        if (type == BatType::compile)
        {
            string param1 = " build";

            command += param1;
        }

        return system(command.c_str());
    }

    void File::RunApplication(const string& parentFolderPath, const string& exePath, const string& commands)
    {
#ifdef _WIN32
        wstring wParentFolderPath(parentFolderPath.begin(), parentFolderPath.end());
        wstring wExePath(exePath.begin(), exePath.end());
        wstring wCommands(commands.begin(), commands.end());

        //initialize structures for process creation
        STARTUPINFOW si;
        PROCESS_INFORMATION pi;
        ZeroMemory(&si, sizeof(si));
        ZeroMemory(&pi, sizeof(pi));
        si.cb = sizeof(si);

        // Create the new process
        if (!CreateProcessW
        (
            wExePath.c_str(),          //path to the executable
            const_cast<LPWSTR>(wCommands.c_str()), //command line arguments
            nullptr,                   //process handle not inheritable
            nullptr,                   //thread handle not inheritable
            FALSE,                     //handle inheritance
            0,                         //creation flags
            nullptr,                   //use parent's environment block
            wParentFolderPath.c_str(), //use parent's starting directory
            &si,                       //pointer to STARTUPINFO structure
            &pi                        //pointer to PROCESS_INFORMATION structure
        ))
        {
            //retrieve the error code and print a descriptive error message
            LPVOID lpMsgBuf = nullptr;
            DWORD dw = GetLastError();
            FormatMessageW(
                FORMAT_MESSAGE_ALLOCATE_BUFFER
                | FORMAT_MESSAGE_FROM_SYSTEM
                | FORMAT_MESSAGE_IGNORE_INSERTS,
                nullptr,
                dw,
                MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT),
                (LPWSTR)&lpMsgBuf, 0, nullptr);
            wcout << L"Error: " << reinterpret_cast<LPCWSTR>(lpMsgBuf) << L"\n\n";
            LocalFree(lpMsgBuf);
        }

        //close process and thread handles
        CloseHandle(pi.hProcess);
        CloseHandle(pi.hThread);
#elif __linux__
        //change working directory to parentFolderPath
        if (chdir(parentFolderPath.c_str()) != 0)
        {
            perror("Error changing directory");
            return;
        }

        //parse the commands into arguments
        vector<string> args;
        size_t pos = 0, found;
        while ((found = commands.find(' ', pos)) != string::npos)
        {
            args.push_back(commands.substr(pos, found - pos));
            pos = found + 1;
        }
        args.push_back(commands.substr(pos));

        //prepare arguments for execvp
        vector<char*> execArgs;
        execArgs.push_back(const_cast<char*>(exePath.c_str()));
        for (auto& arg : args)
        {
            execArgs.push_back(const_cast<char*>(arg.c_str()));
        }
        execArgs.push_back(nullptr);

        pid_t pid = fork();
        if (pid == -1)
        {
            perror("Error during fork");
            return;
        }

        if (pid == 0)
        {
            //child process: execute the program
            execvp(execArgs[0], execArgs.data());
            perror("Error during execvp");
            exit(EXIT_FAILURE); //exit if execvp fails
        }
        else
        {
            //parent process: wait for the child to finish
            int status;
            if (waitpid(pid, &status, 0) == -1)
            {
                perror("Error during waitpid");
            }
            else if (WIFEXITED(status))
            {
                cout << "Child exited with status: " << WEXITSTATUS(status) << "\n";
            }
            else
            {
                cout << "Child did not exit normally\n";
            }
        }
#endif
    }

    void File::MoveOrRenameFileOrFolder(const path& sourcePath, const path& destinationPath, const bool isRenaming)
    {
        if (!exists(sourcePath))
        {
            cout << "Error: Source path " << sourcePath.string() << " does not exist!\n\n";
            return;
        }
        if (exists(destinationPath))
        {
            string outputType = isRenaming ? "Cannot rename source '" : "Cannot move source '";
            cout << outputType << sourcePath.string() << "' to destination '" << destinationPath.string() << "' because destination already exists!\n\n";
            return;
        }

        try
        {
            rename(sourcePath, destinationPath);

            string outputType = isRenaming ? "Renamed '" : "Moved '";
            cout << outputType << sourcePath.string() << "' to '" << destinationPath.string() << "'.\n\n";
        }
        catch (const exception& e)
        {
            cout << "Exception in File::MoveOrRenameFileOrFolder: " << string(e.what()) << ".\n\n";
        }
    }

    void File::CopyFileOrFolder(const path& sourcePath, const path& destinationPath)
    {
        if (!exists(sourcePath))
        {
            cout << "Error: Source path '" << sourcePath.string() << "' does not exist!\n\n";
            return;
        }

        try
        {
            if (is_directory(sourcePath))
            {
                copy(sourcePath, destinationPath, copy_options::recursive | copy_options::overwrite_existing);

                cout << "Copied folder '" << sourcePath.string() << "' to '" << destinationPath.string() << "'.\n\n";
            }
            else if (is_regular_file(sourcePath))
            {
                copy_file(sourcePath, destinationPath, copy_options::overwrite_existing);

                cout << "Copied file '" << sourcePath.string() << "' to '" << destinationPath.string() << "'.\n\n";
            }
        }
        catch (const exception& e)
        {
            cout << "Exception in File::CopyFileOrFolder: " << string(e.what()) << ".\n\n";
        }
    }

    void File::DeleteFileOrfolder(const path& sourcePath)
    {
        if (!exists(sourcePath))
        {
            cout << "Error: Cannot delete file or folder '" << sourcePath.string() << "' because it does not exist!\n\n";
            return;
        }

        try
        {
            if (is_regular_file(sourcePath)) remove(sourcePath);
            else if (is_directory(sourcePath))
            {
                remove_all(sourcePath);
            }

            cout << "Deleted '" << sourcePath.string() << "'.\n\n";
        }
        catch (const exception& e)
        {
            cout << "Exception in File::DeleteFileOrFolder: " << string(e.what()) << ".\n\n";
        }
    }

    void File::CreateNewFolder(const path& folderPath)
    {
        if (exists(folderPath))
        {
            cout << "Error: " << folderPath.string() << " already exists!\n\n";

            return;
        }

        if (is_regular_file(folderPath))
        {
            cout << "Error: " << folderPath.string() << " must be a folder!\n\n";

            return;
        }

        try
        {
            create_directory(folderPath);

            cout << "Created new folder at '" << folderPath.string() << "'.\n\n";
        }
        catch (const exception& e)
        {
            cout << "Exception in File::CreateNewFolder: " << string(e.what()) << ".\n\n";
        }
    }

    string File::AddIndex(
        const path& parentFolderPath,
        const string& fileName,
        const string& extension)
    {
        string newFilePath = (path(parentFolderPath) / (fileName + extension)).string();

        if (exists(newFilePath))
        {
            //extract the actual base name (before the last '(')
            string baseName = fileName;
            size_t openParentPos = fileName.find_last_of('(');
            if (openParentPos != string::npos)
            {
                baseName = fileName.substr(0, openParentPos - 1);
            }

            //find highest available index for this exact base name
            int highestNumber = 1;
            for (const auto& entry : directory_iterator(parentFolderPath))
            {
                string entryName = path(entry).stem().string();

                //extract the base name of the existing entry
                string entryBaseName = entryName;
                size_t entryOpenParentPos = entryName.find_last_of('(');
                if (entryOpenParentPos != string::npos)
                {
                    entryBaseName = entryName.substr(0, entryOpenParentPos - 1);
                }

                //only compare entries with the exact same base name
                if (entryBaseName == baseName)
                {
                    string entryValue = GetValueBetweenParentheses(entryName);
                    if (!entryValue.empty() && all_of(entryValue.begin(), entryValue.end(), ::isdigit))
                    {
                        int entryConvertedValue = stoi(entryValue);
                        if (entryConvertedValue >= highestNumber)
                        {
                            highestNumber = entryConvertedValue + 1;
                        }
                    }
                }
            }

            //remove existing index (if any) before appending new one
            string cleanedFileName = fileName;
            size_t cleanOpenParentPos = fileName.find_last_of('(');
            size_t cleanCloseParentPos = fileName.find_last_of(')');

            if (cleanOpenParentPos != string::npos && cleanCloseParentPos != string::npos && cleanCloseParentPos > cleanOpenParentPos)
            {
                string potentialNumber = fileName.substr(cleanOpenParentPos + 1, cleanCloseParentPos - cleanOpenParentPos - 1);
                if (all_of(potentialNumber.begin(), potentialNumber.end(), ::isdigit))
                {
                    cleanedFileName = fileName.substr(0, cleanOpenParentPos - 1);
                }
            }

            //ensure cleaned filename is not empty
            if (cleanedFileName.empty()) cleanedFileName = fileName;

            //create new indexed filename
            string newFileName = cleanedFileName + " (" + to_string(highestNumber) + ")" + extension;
            newFilePath = (path(parentFolderPath) / newFileName).string();
        }

        return newFilePath;
    }
    string File::GetValueBetweenParentheses(const string& input)
    {
        size_t start = input.find('(');
        size_t end = input.find(')', start);

        if (start != string::npos
            && end != string::npos
            && end > start)
        {
            return input.substr(start + 1, end - start - 1);
        }

        return "";
    }
}