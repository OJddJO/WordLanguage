"use strict";
var __createBinding = (this && this.__createBinding) || (Object.create ? (function(o, m, k, k2) {
    if (k2 === undefined) k2 = k;
    var desc = Object.getOwnPropertyDescriptor(m, k);
    if (!desc || ("get" in desc ? !m.__esModule : desc.writable || desc.configurable)) {
      desc = { enumerable: true, get: function() { return m[k]; } };
    }
    Object.defineProperty(o, k2, desc);
}) : (function(o, m, k, k2) {
    if (k2 === undefined) k2 = k;
    o[k2] = m[k];
}));
var __setModuleDefault = (this && this.__setModuleDefault) || (Object.create ? (function(o, v) {
    Object.defineProperty(o, "default", { enumerable: true, value: v });
}) : function(o, v) {
    o["default"] = v;
});
var __importStar = (this && this.__importStar) || function (mod) {
    if (mod && mod.__esModule) return mod;
    var result = {};
    if (mod != null) for (var k in mod) if (k !== "default" && Object.prototype.hasOwnProperty.call(mod, k)) __createBinding(result, mod, k);
    __setModuleDefault(result, mod);
    return result;
};
Object.defineProperty(exports, "__esModule", { value: true });
exports.activate = activate;
const vscode = __importStar(require("vscode"));
const tokenTypes = new Map();
const tokenModifiers = new Map();
const legend = (function () {
    const tokenTypesLegend = [
        'comment', 'string', 'keyword', 'number', 'regexp', 'operator', 'namespace',
        'type', 'struct', 'class', 'interface', 'enum', 'typeParameter', 'function',
        'method', 'decorator', 'macro', 'variable', 'parameter', 'property', 'label'
    ];
    tokenTypesLegend.forEach((tokenType, index) => tokenTypes.set(tokenType, index));
    const tokenModifiersLegend = [
        'declaration', 'documentation', 'readonly', 'static', 'abstract', 'deprecated',
        'modification', 'async'
    ];
    tokenModifiersLegend.forEach((tokenModifier, index) => tokenModifiers.set(tokenModifier, index));
    return new vscode.SemanticTokensLegend(tokenTypesLegend, tokenModifiersLegend);
})();
const provider = {
    provideDocumentSemanticTokens(document) {
        // analyze the document and return semantic tokens
        const tokensBuilder = new vscode.SemanticTokensBuilder(legend);
        const text = document.getText();
        const lines = text.split("\n");
        const vars = [];
        const functions = [];
        let funcArgs = [];
        for (let lineIndex = 0; lineIndex < lines.length; lineIndex++) {
            const line = lines[lineIndex];
            const words = line.split(/\s+/); // splits by any whitespace
            const wordIndexes = [];
            let currentIndex = 0;
            for (const word of words) { // get indexes of each words
                const wordIndex = line.indexOf(word, currentIndex);
                wordIndexes.push(wordIndex);
                currentIndex = wordIndex + word.length;
            }
            let isString = false; // if evaluating string
            for (let wordIndex = 0; wordIndex < words.length; wordIndex++) {
                if (!isString && words[wordIndex].startsWith('#')) { //comment
                    break;
                }
                if (words[wordIndex].startsWith('"')) { //if the word starts with " then it's a string
                    tokensBuilder.push(lineIndex, wordIndexes[wordIndex], words[wordIndex].length, getTokenType("string"));
                    isString = true;
                }
                else {
                    if (/\bdef\b/.test(words[wordIndex])) { //function def
                        wordIndex += 2; //skip function return type
                        functions.push(words[wordIndex]);
                        tokensBuilder.push(lineIndex, wordIndexes[wordIndex], words[wordIndex].length, getTokenType("function"), getTokenModifier("declaration"));
                        if (line.length > 3) {
                            wordIndex++; //skip with keyword
                            for (wordIndex += 2; wordIndex < words.length; wordIndex += 2) { //args
                                funcArgs.push(words[wordIndex]);
                                tokensBuilder.push(lineIndex, wordIndexes[wordIndex], words[wordIndex].length, getTokenType("parameter"), getTokenModifier("declaration"));
                            }
                        }
                        continue;
                    }
                    else if (/\benddef\b/.test(words[wordIndex])) { //enddef -> reset funcArgs
                        funcArgs = [];
                    }
                    else if (/\b(bool|int|float|str|list)\b/.test(words[wordIndex])) { //var def
                        wordIndex++;
                        vars.push(words[wordIndex]);
                        tokensBuilder.push(lineIndex, wordIndexes[wordIndex], words[wordIndex].length, getTokenType("variable"), getTokenModifier("declaration"));
                    }
                    else if (funcArgs.includes(words[wordIndex])) { //function args
                        tokensBuilder.push(lineIndex, wordIndexes[wordIndex], words[wordIndex].length, getTokenType("parameter"));
                    }
                    else if (vars.includes(words[wordIndex])) { //var
                        tokensBuilder.push(lineIndex, wordIndexes[wordIndex], words[wordIndex].length, getTokenType("variable"));
                    }
                    else if (functions.includes(words[wordIndex])) { //functions
                        tokensBuilder.push(lineIndex, wordIndexes[wordIndex], words[wordIndex].length, getTokenType("function"));
                    }
                }
                if (words[wordIndex].endsWith('"')) {
                    isString = false;
                }
            }
        }
        return tokensBuilder.build();
    }
};
function getTokenType(tokenType) {
    let result = 0;
    if (tokenTypes.has(tokenType)) {
        result = tokenTypes.get(tokenType);
    }
    return result;
}
function getTokenModifier(tokenModifier) {
    let result = 0;
    if (tokenModifiers.has(tokenModifier)) {
        result = tokenModifiers.get(tokenModifier);
    }
    return result;
}
const selector = { language: 'w', scheme: 'file' }; // register for all WordLang documents from the local file system
function activate(context) {
    context.subscriptions.push(vscode.languages.registerDocumentSemanticTokensProvider(selector, provider, legend));
}
//# sourceMappingURL=extension.js.map