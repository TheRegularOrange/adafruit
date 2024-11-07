class MacroProcessor {
    String[] macroNames = new String[10];
    String[][] macroDefinitions = new String[10][10];
    String[][] argumentTable = new String[10][5];
    int[] macroLineCount = new int[10];
    int macroCount = 0;

    public void processPass1(String[] sourceCode) {
        boolean insideMacro = false;
        int lineIndex = 0;

        for (String line : sourceCode) {
            String[] tokens = line.trim().split(" ");

            if (tokens[0].equals("MACRO")) {
                insideMacro = true;
                lineIndex = 0;
                continue;
            } else if (tokens[0].equals("MEND")) {
                insideMacro = false;
                macroCount++;
                continue;
            }

            if (insideMacro) {
                if (lineIndex == 0) {  // First line is the macro name
                    macroNames[macroCount] = tokens[0];
                    for (int i = 1; i < tokens.length; i++) {
                        argumentTable[macroCount][i - 1] = tokens[i];  // Store arguments
                    }
                }
                macroDefinitions[macroCount][lineIndex++] = line;
                macroLineCount[macroCount] = lineIndex;
            }
        }
    }

    public void processPass2(String[] sourceCode) {
        for (String line : sourceCode) {
            String[] tokens = line.trim().split(" ");
            if (tokens[0].equals("MACRO") || tokens[0].equals("MEND")) continue;

            boolean isMacroCall = false;
            for (int i = 0; i < macroCount; i++) {
                if (tokens[0].equals(macroNames[i])) {
                    isMacroCall = true;
                    expandMacro(i, tokens);
                    break;
                }
            }

            if (!isMacroCall) System.out.println(line);
        }
    }

    private void expandMacro(int macroIndex, String[] args) {
        for (int i = 1; i < macroLineCount[macroIndex]; i++) {
            String expandedLine = macroDefinitions[macroIndex][i];
            for (int j = 0; j < args.length - 1; j++) {
                expandedLine = expandedLine.replace(argumentTable[macroIndex][j], args[j + 1]);
            }
            System.out.println(expandedLine);
        }
    }

    public void displayTables() {
        System.out.println("Macro Name Table:");
        for (int i = 0; i < macroCount; i++) {
            System.out.println(macroNames[i]);
        }

        System.out.println("\nMacro Definitions:");
        for (int i = 0; i < macroCount; i++) {
            for (int j = 0; j < macroLineCount[i]; j++) {
                System.out.println(macroDefinitions[i][j]);
            }
        }

        System.out.println("\nArgument Table:");
        for (int i = 0; i < macroCount; i++) {
            for (String arg : argumentTable[i]) {
                if (arg != null) System.out.print(arg + " ");
            }
            System.out.println();
        }
    }

    public static void main(String[] args) {
        String[] sourceCode = {
            "MACRO", "INCR &ARG1", "ADD &ARG1, ONE", "MEND",
            "MACRO", "DECR &ARG1", "SUB &ARG1, ONE", "MEND",
            "START", "INCR A", "DECR B", "END"
        };

        MacroProcessor processor = new MacroProcessor();
        processor.processPass1(sourceCode);
        processor.displayTables();
        System.out.println("\nExpanded Source Code:");
        processor.processPass2(sourceCode);
    }
}
