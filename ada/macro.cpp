#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <string>
#include <memory>
#include <unordered_map>

using namespace std;

// Argument list for macros
struct ArgumentList {
    int index;
    string arg;
};

// Definition table entry
struct DefinitionTableEntry {
    int index;
    string definition;
    vector<shared_ptr<ArgumentList>> args;  // Using smart pointers
    bool isMacroName;  // Flag to identify macro name entries
};

// Macro name table
struct NameTableEntry {
    int index;
    string name;
    shared_ptr<DefinitionTableEntry> dtEntry;  // Using smart pointer
    vector<string> formalParameters;  // Store formal parameters
};

class MacroProcessor {
private:
    vector<shared_ptr<DefinitionTableEntry>> definitionTable;
    vector<NameTableEntry> nameTable;
    vector<ArgumentList> argumentList;
    int alIndex;
    
    // Helper function to tokenize a line
    vector<string> tokenizeLine(const string& line) {
        vector<string> tokens;
        istringstream iss(line);
        string token;
        while (iss >> token) {
            tokens.push_back(token);
        }
        return tokens;
    }
    
    // Find argument by name
    shared_ptr<ArgumentList> findArgument(const string& arg) {
        for (auto& argument : argumentList) {
            if (argument.arg == arg) {
                return make_shared<ArgumentList>(argument);
            }
        }
        return nullptr;
    }
    
    // Find macro definition by name
    shared_ptr<DefinitionTableEntry> findDefinitionEntry(const string& name) {
        for (const auto& entry : nameTable) {
            if (entry.name == name) {
                return entry.dtEntry;
            }
        }
        return nullptr;
    }
    
    // Replace macro parameters with arguments
    string replaceMacroParameters(const string& line, 
                                const vector<string>& formalParams,
                                const vector<string>& actualParams) {
        string result = line;
        for (size_t i = 0; i < formalParams.size() && i < actualParams.size(); ++i) {
            size_t pos = 0;
            while ((pos = result.find(formalParams[i], pos)) != string::npos) {
                result.replace(pos, formalParams[i].length(), actualParams[i]);
                pos += actualParams[i].length();
            }
        }
        return result;
    }

public:
    MacroProcessor() : alIndex(1) {}
    
    void pass1(ifstream& file) {
        string line;
        while (getline(file, line)) {
            if (line.find("MACRO") != string::npos) {
                vector<string> tokens = tokenizeLine(line);
                if (tokens.size() < 2) continue;  // Invalid macro definition
                
                // Process macro name and parameters
                string macroName = tokens[0];
                vector<string> formalParams;
                for (size_t i = 2; i < tokens.size(); ++i) {
                    if (tokens[i] != "MACRO") {
                        formalParams.push_back(tokens[i]);
                    }
                }
                
                // Create name table entry
                NameTableEntry ntEntry;
                ntEntry.index = nameTable.size() + 1;
                ntEntry.name = macroName;
                ntEntry.formalParameters = formalParams;
                
                // Create definition table entry for macro name
                auto dtEntry = make_shared<DefinitionTableEntry>();
                dtEntry->index = definitionTable.size() + 1;
                dtEntry->definition = macroName;
                dtEntry->isMacroName = true;
                definitionTable.push_back(dtEntry);
                ntEntry.dtEntry = dtEntry;
                nameTable.push_back(ntEntry);
                
                cout << "\nMACRO " << macroName << " Detected...\n";
                
                // Process macro body
                while (getline(file, line) && line != "MEND") {
                    auto bodyEntry = make_shared<DefinitionTableEntry>();
                    bodyEntry->index = definitionTable.size() + 1;
                    bodyEntry->isMacroName = false;
                    
                    tokens = tokenizeLine(line);
                    if (!tokens.empty()) {
                        bodyEntry->definition = tokens[0];
                        for (size_t i = 1; i < tokens.size(); ++i) {
                            auto arg = findArgument(tokens[i]);
                            if (!arg) {
                                ArgumentList newArg{alIndex++, tokens[i]};
                                argumentList.push_back(newArg);
                                arg = make_shared<ArgumentList>(newArg);
                            }
                            bodyEntry->args.push_back(arg);
                        }
                        definitionTable.push_back(bodyEntry);
                    }
                }
                // Add MEND entry
                auto mendEntry = make_shared<DefinitionTableEntry>();
                mendEntry->index = definitionTable.size() + 1;
                mendEntry->definition = "MEND";
                mendEntry->isMacroName = false;
                definitionTable.push_back(mendEntry);
            }
        }
        cout << "\nPass 1 Complete!\n";
    }
    
    void pass2(ifstream& file) {
        ofstream outFile("output.txt");
        if (!outFile.is_open()) {
            cerr << "Failed to open output file!\n";
            return;
        }
        
        string line;
        while (getline(file, line)) {
            vector<string> tokens = tokenizeLine(line);
            if (tokens.empty()) continue;
            
            auto macroEntry = findDefinitionEntry(tokens[0]);
            if (macroEntry && macroEntry->isMacroName) {
                // Get formal parameters from name table
                vector<string> formalParams;
                for (const auto& entry : nameTable) {
                    if (entry.name == tokens[0]) {
                        formalParams = entry.formalParameters;
                        break;
                    }
                }
                
                // Collect actual parameters
                vector<string> actualParams;
                for (size_t i = 1; i < tokens.size(); ++i) {
                    actualParams.push_back(tokens[i]);
                }
                
                // Process macro body
                bool inMacro = true;
                size_t defIndex = macroEntry->index;
                while (inMacro && defIndex < definitionTable.size()) {
                    auto currentEntry = definitionTable[defIndex];
                    if (currentEntry->definition == "MEND") {
                        inMacro = false;
                    } else if (!currentEntry->isMacroName) {
                        string expandedLine = currentEntry->definition;
                        for (const auto& arg : currentEntry->args) {
                            expandedLine += " " + arg->arg;
                        }
                        expandedLine = replaceMacroParameters(expandedLine, formalParams, actualParams);
                        outFile << expandedLine << "\n";
                    }
                    defIndex++;
                }
            } else {
                // Not a macro, write line as is
                outFile << line << "\n";
            }
        }
        cout << "\nPass 2 Complete!\n";
    }
    
    void printTables() const {
        cout << "\nDefinition Table:\n";
        for (const auto& entry : definitionTable) {
            cout << entry->index << "  " << entry->definition;
            for (const auto& arg : entry->args) {
                cout << " " << arg->arg;
            }
            cout << "\n";
        }
        
        cout << "\nName Table:\n";
        for (const auto& entry : nameTable) {
            cout << entry.index << "  " << entry.name << " Parameters: ";
            for (const auto& param : entry.formalParameters) {
                cout << param << " ";
            }
            cout << "\n";
        }
        
        cout << "\nArgument List:\n";
        for (const auto& arg : argumentList) {
            cout << arg.index << "  " << arg.arg << "\n";
        }
    }
};

int main() {
    ifstream file("input.asm");
    if (!file.is_open()) {
        cerr << "Failed to open the assembly file!\n";
        return 1;
    }

    MacroProcessor processor;
    
    cout << "Pass 1 in progress...\n";
    processor.pass1(file);
    
    file.clear();
    file.seekg(0);  // Rewind file for pass2
    
    cout << "Pass 2 in progress...\n";
    processor.pass2(file);
    
    processor.printTables();
    
    file.close();
    return 0;
}