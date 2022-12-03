#include <algorithm>
#include <fstream>
#include <iostream>
#include <limits.h>
#include <string>
#include <unistd.h>
#include <vector>

#define MAX_IGNORE_FILE_SIZE (100 * 1024 * 1024) // 100MB

using namespace std;

// TODO find .git

bool find_gitignore(string cwd, string &gitignore_path) {
    if (!cwd.empty() && cwd.back() != '/') {
        cwd += '/';
    }

    while (!cwd.empty()) {
        gitignore_path = cwd + ".gitignore";

        cout << "Checking " << gitignore_path << endl;
        if (access(gitignore_path.c_str(), W_OK) != -1) {
            return true;
        }

        cwd.pop_back();
        while (!cwd.empty() && cwd.back() != '/') {
            cwd.pop_back();
        }
    }
    return false;
}

size_t file_size(const string &path) {
    ifstream file(path, ifstream::ate | ifstream::binary);

    if (!file.is_open()) {
        return 0;
    }

    return file.tellg();
}

// Remove leading and trailing whitespaces.
string &trim(string &s) {
    if (s.empty()) {
        return s;
    }

    while (!s.empty() && isspace(s.back())) {
        s.pop_back();
    }

    for (size_t i = 0; i < s.size(); i++) {
        if (!isspace(s[i])) {
            s.erase(0, i);
            break;
        }
    }

    return s;
}

// Read .gitignore file and return a vector of all patterns.
vector<string> read_gitignore(string gitignore_path) {
    vector<string> ignore_patterns;
    ifstream gitignore_file(gitignore_path);
    string line;
    while (getline(gitignore_file, line)) {
        trim(line);
        if (line.empty() || line[0] == '#') {
            continue;
        }
        ignore_patterns.push_back(line);
    }
    return ignore_patterns;
}

// Check if the pattern is already in the ignore_file.
bool is_ignored(const vector<string> &ignore_patterns, const string &pattern) {
    return find(ignore_patterns.begin(), ignore_patterns.end(), pattern) !=
           ignore_patterns.end();
}

int main(int argc, char **argv) {
    char cwd[PATH_MAX];
    if (getcwd(cwd, PATH_MAX) == NULL) {
        perror("getcwd() error");
        return 1;
    }

    // TODO: open the .gitignore file only once
    // TODO: ignore all executables under the current directory
    //   like: ignore -e
    // TODO: add '*' automatically if something like ".txt"
    // TODO: add "-r"/"--row" option, keep everything unchanged
    // TODO: do not add a newline before appending the patterns
    // TODO: add -d option to delete a pattern
    // TODO: add -l option to list all patterns
    // TODO: add -f option to specify the .gitignore file

    cout << "Current working directory is: " << cwd << endl;
    string gitignore_path;
    if (find_gitignore(cwd, gitignore_path)) {
        cout << "\nFound .gitignore at " << gitignore_path << endl;

        if (file_size(gitignore_path) > MAX_IGNORE_FILE_SIZE) {
            cerr << "The .gitignore file found is too large, it seems not a "
                    "'gitignore' file!"
                 << endl;
            return 1;
        }

        vector<string> ignore_patterns = read_gitignore(gitignore_path);

        ofstream gitignore(gitignore_path, ios::app);
        if (!gitignore.is_open()) {
            cerr << "Failed to open .gitignore" << endl;
            return 1;
        }

        cout << "Writing to .gitignore" << endl;

        bool first = true;

        for (int i = 1; i < argc; ++i) {
            if (!is_ignored(ignore_patterns, argv[i])) {
                if (first) {
                    gitignore << "\n";
                    first = false;
                }
                gitignore << argv[i] << "\n";
            } else {
                cout << "Already ignored: " << argv[i] << endl;
            }
        }

        gitignore.close();
    } else {
        cerr << "No .gitignore found" << endl;
        return 1;
    }

    return 0;
}
