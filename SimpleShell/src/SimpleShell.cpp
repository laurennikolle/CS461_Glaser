// Lauren Glaser
// PS 02
// CS 461

#include <iostream>
#include <vector>
#include <string>
#include <unistd.h>
#include <sys/wait.h>
#include <cstdlib>

using namespace std;

// This will execute, parse and run the loop. [Lauren Glaser 10/27/2024]
class SimpleShell {
public:
    void execute(const vector<string>& argv);
    void parse(const string& line, vector<string>& tokens, const string& delimiter);
    void run();
};

void SimpleShell::execute(const vector<string>& argv) {
    // This will handle changing directories for the test command: cd /usr [Lauren Glaser 10/27/2024]
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

    // Spawning a child process
    child = fork();

    // Parent process portion
    if (child > 0) {
        cout << "(" << getpid() << ") : I am a parent process waiting..." << endl;
        waitpid(child, &status, 0); // Wait for child process to finish
        cout << "Waiting complete" << endl;
    }
    // Child process portion
    else if (child == 0) {
        cout << "I am a child executing a new environment" << endl;

        // Ensure there's at least one argument
        if (argv.empty()) {
            cerr << "Error: No command to execute" << endl;
            _exit(1);
        }

        // Prepare arguments for exec (must end with nullptr)
        vector<const char*> args;
        for (const auto& arg : argv) {
            args.push_back(arg.c_str());
        }
        args.push_back(nullptr);

        // Execute the command
        if (execvp(args[0], const_cast<char* const*>(args.data())) == -1) {
            perror("execvp failed"); // Print error message
            _exit(1);
        }
    }
    else {
        perror("fork failed"); // Error handling if fork fails
        exit(1);
    }
}

void SimpleShell::parse(const string& line, vector<string>& tokens, const string& delimiter) {
    size_t start = 0;
    size_t end = 0;

    while ((end = line.find(delimiter, start)) != string::npos) {
        if (end != start) { // Ignore empty tokens
            tokens.push_back(line.substr(start, end - start));
        }
        start = end + delimiter.length();
    }

    if (start < line.length()) { // Add the last token
        tokens.push_back(line.substr(start));
    }
}

void SimpleShell::run() {
    while (true) {
        string line;
        vector<string> tokens;

        // Print the prompt
        cout << "(" << getpid() << ") % ";

        // Get input from the keyboard
        if (!getline(cin, line)) {
            break; // Exit the shell if input fails
        }

        // Parse the input into tokens
        parse(line, tokens, " ");

        if (tokens.empty()) {
            continue; // Skip empty input
        }

        // Execute the user command
        execute(tokens);
    }
}

int main() {
    SimpleShell shell;
    shell.run(); // Start the shell loop
    return 0;
}