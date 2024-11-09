// Lauren Glaser
// PS3
// CS461

#include <iostream>
#include <vector>
#include <string>
#include <unistd.h>
#include <sys/wait.h>
#include <fcntl.h>
#include <cstdlib>

using namespace std;

// This will execute, parse, and run the loop.
class SimpleShell {
public:
    void execute(const vector<string>& argv, const string& inputFile, const string& outputFile);
    void parse(const string& line, vector<string>& tokens, const string& delimiter);
    void run();
};

void SimpleShell::execute(const vector<string>& argv, const string& inputFile, const string& outputFile) {
    if (argv.empty()) {
        cerr << "Error: No command to execute" << endl;
        return;
    }

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

void SimpleShell::parse(const string& line, vector<string>& tokens, const string& delimiter) {
    size_t start = 0;
    size_t end = 0;

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
        string inputFile;
        string outputFile;

        cout << "(" << getpid() << ") % ";

        if (!getline(cin, line)) {
            break;
        }

        parse(line, tokens, " ");

        vector<string> command;
        for (size_t i = 0; i < tokens.size(); ++i) {
            if (tokens[i] == "<") {
                if (i + 1 < tokens.size()) {
                    inputFile = tokens[i + 1];
                    ++i;
                }
            } else if (tokens[i] == ">") {
                if (i + 1 < tokens.size()) {
                    outputFile = tokens[i + 1];
                    ++i;
                }
            } else {
                command.push_back(tokens[i]);
            }
        }

        if (command.empty()) {
            continue;
        }

        execute(command, inputFile, outputFile);
    }
}

int main() {
    SimpleShell shell;
    shell.run();
    return 0;
}
