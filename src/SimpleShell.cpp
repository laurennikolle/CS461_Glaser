// Lauren Glaser
// PS4
// CS461

#include <iostream>
#include <vector>
#include <string>
#include <unistd.h>
#include <sys/wait.h>
#include <fcntl.h>
#include <cstdlib>

using namespace std;

// Creates the SimpleShell class
class SimpleShell {
// These are the three functions: Execute, Parse, and Run
public:
    void execute(const vector<string>& argv, const string& inputFile, const string& outputFile, int inputFd = STDIN_FILENO, int outputFd = STDOUT_FILENO);
    void parse(const string& line, vector<string>& tokens, const string& delimiter);
    void run();
};

// Creates the function for SimpleShell to execute
void SimpleShell::execute(const vector<string>& argv, const string& inputFile, const string& outputFile, int inputFd, int outputFd) {
    // Checks to make sure the argument is not empty
    if (argv.empty()) {
        cerr << "Error: No command to execute" << endl;
        return;
    }

    // For the argument cd
    if (argv[0] == "cd") {
        if (argv.size() < 2) {
            cerr << "cd: missing argument" << endl;
        } else {
            if (chdir(argv[1].c_str()) != 0) {
                perror("cd failed");
            }
        }
        return;
    }

    int status;
    pid_t child;
    child = fork();

    if (child > 0) {
        waitpid(child, &status, 0);
    } else if (child == 0) {
        if (inputFd != STDIN_FILENO) {
            dup2(inputFd, STDIN_FILENO);
            close(inputFd);
        }

        if (outputFd != STDOUT_FILENO) {
            dup2(outputFd, STDOUT_FILENO);
            close(outputFd);
        }

        if (!inputFile.empty()) {
            int fd = open(inputFile.c_str(), O_RDONLY);
            if (fd < 0) {
                perror("open input file failed");
                _exit(1);
            }
            dup2(fd, STDIN_FILENO);
            close(fd);
        }

        if (!outputFile.empty()) {
            int fd = open(outputFile.c_str(), O_WRONLY | O_CREAT | O_TRUNC, 0644);
            if (fd < 0) {
                perror("open output file failed");
                _exit(1);
            }
            dup2(fd, STDOUT_FILENO);
            close(fd);
        }

        vector<const char*> args;
        for (const auto& arg : argv) {
            args.push_back(arg.c_str());
        }
        args.push_back(nullptr);

        if (execvp(args[0], const_cast<char* const*>(args.data())) == -1) {
            perror("execvp failed");
            _exit(1);
        }
    } else {
        perror("fork failed");
        exit(1);
    }
}

// Function for the SimpleShell to parse
void SimpleShell::parse(const string& line, vector<string>& tokens, const string& delimiter) {
    size_t start = 0;
    size_t end = 0;

    // While loop to loop through the text
    while ((end = line.find(delimiter, start)) != string::npos) {
        if (end != start) {
            tokens.push_back(line.substr(start, end - start));
        }
        start = end + delimiter.length();
    }

    if (start < line.length()) {
        tokens.push_back(line.substr(start));
    }
}

void SimpleShell::run() {
    while (true) {
        string line;
        vector<string> tokens;
        vector<vector<string>> commands;
        string inputFile;
        string outputFile;

        cout << "(" << getpid() << ") % ";

        if (!getline(cin, line)) {
            break;
        }

        size_t pipePos = line.find("|");

        while (pipePos != string::npos) {
            vector<string> commandTokens;
            parse(line.substr(0, pipePos), commandTokens, " ");
            commands.push_back(commandTokens);
            line = line.substr(pipePos + 1);
            pipePos = line.find("|");
        }

        vector<string> commandTokens;
        parse(line, commandTokens, " ");
        commands.push_back(commandTokens);

        int fd[2];
        int inputFd = STDIN_FILENO;

        for (size_t i = 0; i < commands.size(); ++i) {
            if (i < commands.size() - 1) {
                pipe(fd);
            }

            execute(commands[i], inputFile, outputFile, inputFd, (i == commands.size() - 1) ? STDOUT_FILENO : fd[1]);

            if (i < commands.size() - 1) {
                close(fd[1]);
                inputFd = fd[0];
            }
        }
    }
}

// Main method function declaration
int main() {
    // Creates a simple shell
    SimpleShell shell;
    // Runs the shell
    shell.run();
    return 0;
}